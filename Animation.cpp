#include "stdafx.h"
#include "Animation.h"
#include "Importer.h"

void AnimationManager::Initialize()
{
	m_uomAnimClip.clear();
}

void AnimationManager::AddAnimClip(const char* fileName, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	AnimClipDataImporter importer;
	AnimClip* animClip = new AnimClip();
	*animClip = importer.Load(fileName);

	m_uomAnimClip[fileName] = animClip;
}

AnimationController::AnimationController(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE& cbvCpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& cbvGpuHandle)
{
	m_fTime = 0;

	UINT ncbElementBytes = ((sizeof(CB_BONE_INFO) + 255) & ~255);

	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	D3D12_GPU_VIRTUAL_ADDRESS		d3dGpuVirtualAddress;
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;

	if (nullptr != m_pd3dCBResource) {
		m_pd3dCBResource->Map(0, NULL, (void**)&m_pCBMappedBones);
		d3dGpuVirtualAddress = m_pd3dCBResource->GetGPUVirtualAddress();
		d3dCBVDesc.SizeInBytes = ncbElementBytes;
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, cbvCpuHandle);

		cbvCpuHandle.ptr += gnCbvSrvDescriptorIncrementSize;
	}
	m_d3dCbvGPUDescriptorHandle = cbvGpuHandle;
	cbvGpuHandle.ptr += gnCbvSrvDescriptorIncrementSize;
}

void AnimationController::Update(float fTimeElapsed)
{
	m_fTime += fTimeElapsed;
}


//m_uomAnimClip[name].vecBone[i][0].xmf3Translation;
//m_uomAnimClip[name].vecBone[i][0].xmf4QuatRotation;
//XMMatrixRotationQuaternion(XMLoadFloat4(&m_uomAnimClip[name].vecBone[i][0].xmf4QuatRotation));
//XMMatrixTranslationFromVector(XMLoadFloat3(&m_uomAnimClip[name].vecBone[i][0].xmf3Translation));
//XMMatrixMultiply(XMMatrixRotationQuaternion(
//	XMLoadFloat4(&m_uomAnimClip[name].vecBone[i][0].xmf4QuatRotation)),
//	XMMatrixTranslationFromVector(XMLoadFloat3(&m_uomAnimClip[name].vecBone[i][0].xmf3Translation)));

void AnimationController::SetMatrix(ID3D12GraphicsCommandList* pd3dCommandList)
{
	AnimClip* temp = gAnimMng.GetAnimClip("animTest2");

	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTSIGNATURE_ANIMTRANSFORM, m_d3dCbvGPUDescriptorHandle);
	UINT ncbElementBytes = ((sizeof(CB_BONE_INFO) + 255) & ~255);
	memset(m_pCBMappedBones, NULL, ncbElementBytes);

	float fTime = m_fTime;
	while (fTime > temp->fClipLength) {
		fTime -= temp->fClipLength;
	}

	/*========================================================================
	* �� Bone���� InterpolationIndex�� ã�ƾ� ��
	========================================================================*/
	for (int iBone = 0; iBone < temp->vecBone.size(); iBone++) {
		/*========================================================================
		* ����, Ű�������� �ϳ��ۿ� ������ �� ���� �״�� �ִ´�.
		========================================================================*/
		if (temp->vecBone[iBone].size() <= 1) {
			XMStoreFloat4x4(&m_pCBMappedBones->xmf4x4Transform[iBone], 
				XMMatrixTranspose(
					XMMatrixMultiply(
						XMMatrixRotationQuaternion(XMLoadFloat4(&temp->vecBone[iBone][0].xmf4QuatRotation)), 
						XMMatrixTranslationFromVector(XMLoadFloat3(&temp->vecBone[iBone][0].xmf3Translation)))));
		}
		/*========================================================================
		* Ű�������� �� �� �̻��̸� index�� ã�ƾ� �Ѵ�.
		* 
		========================================================================*/
		else {
			Keyframe k0, k1, k2, k3, result;
			int iFrame = 0;

#ifdef DEBUG
			bool ok = false;
#endif
			for (; iFrame < temp->vecBone[iBone].size(); iFrame++) {
				/*========================================================================
				* �߰��� �ִ� ���:
				========================================================================*/
				if (fTime < temp->vecBone[iBone][iFrame].keyTime) {
					k2 = temp->vecBone[iBone][iFrame];

					if (iFrame + 1 == temp->vecBone[iBone].size())	k3 = temp->vecBone[iBone][iFrame];
					else											k3 = temp->vecBone[iBone][iFrame + 1];

					k1 = temp->vecBone[iBone][iFrame - 1];

					if (iFrame - 1 == 0)	k0 = temp->vecBone[iBone][iFrame - 1];
					else					k0 = temp->vecBone[iBone][iFrame - 2];

					float t = (fTime - k2.keyTime) / (k3.keyTime - k2.keyTime);

					InterpolateKeyframe(k0, k1, k2, k3, t, result);
#ifdef DEBUG
					ok = true;
#endif
					break;
				}
			}
			/*========================================================================
			* �� �����µ� ������. �� �ڿ� �ִ� ����.
			========================================================================*/
			if (iFrame == temp->vecBone[iBone].size()) {
				k1 = temp->vecBone[iBone][temp->vecBone[iBone].size() - 1];
				k2 = k1;
				k3 = k1;

				if (temp->vecBone[iBone].size() - 2 >= 0)	k0 = temp->vecBone[iBone][temp->vecBone[iBone].size() - 2];
				else										k0 = k1;

				float t = (fTime - k1.keyTime) / (temp->fClipLength - k1.keyTime);

				InterpolateKeyframe(k0, k1, k2, k3, t, result);
#ifdef DEBUG
				ok = true;
#endif
			}
#ifdef DEBUG
			if (!ok) assert(!"idx �ٽ� ã��~~");
#endif
			XMStoreFloat4x4(&m_pCBMappedBones->xmf4x4Transform[iBone],
				XMMatrixTranspose(
					XMMatrixMultiply(
						XMMatrixRotationQuaternion(XMLoadFloat4(&result.xmf4QuatRotation)),
						XMMatrixTranslationFromVector(XMLoadFloat3(&result.xmf3Translation)))));

		}
	}
}

void AnimationController::InterpolateKeyframe(Keyframe k0, Keyframe k1, Keyframe k2, Keyframe k3, float t, Keyframe& out)
{
	out.xmf4QuatRotation = Interpolate(k0.xmf4QuatRotation, k1.xmf4QuatRotation, k2.xmf4QuatRotation, k3.xmf4QuatRotation, t);
	out.xmf3Translation = Interpolate(k0.xmf3Translation, k1.xmf3Translation, k2.xmf3Translation, k3.xmf3Translation, t);
}

XMFLOAT4 AnimationController::Interpolate(XMFLOAT4 q0, XMFLOAT4 q1, XMFLOAT4 q2, XMFLOAT4 q3, float t)
{
	XMVECTOR resultV = XMVectorCatmullRom(XMLoadFloat4(&q0), XMLoadFloat4(&q1), XMLoadFloat4(&q2), XMLoadFloat4(&q3), t);
	XMFLOAT4 result;

	XMStoreFloat4(&result, resultV);

	return result;
}

XMFLOAT3 AnimationController::Interpolate(XMFLOAT3 q0, XMFLOAT3 q1, XMFLOAT3 q2, XMFLOAT3 q3, float t)
{
	XMVECTOR resultV = XMVectorCatmullRom(XMLoadFloat3(&q0), XMLoadFloat3(&q1), XMLoadFloat3(&q2), XMLoadFloat3(&q3), t);
	XMFLOAT3 result;

	XMStoreFloat3(&result, resultV);
	return result;
}
