#include "stdafx.h"
#include "Object.h"
#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "State.h"
#include "Animation.h"
#include "Mask.h"
#include "Components.h"


Object::Object()
{
}

Object::~Object()
{
	for_each(m_vecComponents.begin(), m_vecComponents.end(), [](Component* c) { delete c; });
}

void Object::CheckCollsion(const Object& other)
{
	vector<Component*> l_vecpMyCollider = FindComponentsByName("Collider");
	vector<Component*> l_vecpOtherCollider = other.FindComponentsByName("Collider");

	for (int i = 0; i < l_vecpMyCollider.size(); i++) 
		for (int j = 0; j < l_vecpOtherCollider.size(); j++) 
			dynamic_cast<ColliderComponent*>(l_vecpMyCollider[i])
			->CheckCollision(dynamic_cast<ColliderComponent*>(l_vecpOtherCollider[j]));
}
void Object::SolveConstraint()
{
}
void Object::Input(UCHAR* pKeyBuffer)
{
	for_each(m_vecComponents.begin(), m_vecComponents.end(), [pKeyBuffer](Component* c) { c->InputEvent(pKeyBuffer); });
}
void Object::Update(float fTimeElapsed)
{
	m_fTime += fTimeElapsed;
	for_each(m_vecComponents.begin(), m_vecComponents.end(), [fTimeElapsed](Component* c) { c->Update(fTimeElapsed); });
}
void Object::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for_each(m_vecComponents.begin(), m_vecComponents.end(), [pd3dCommandList](Component* c) { c->Render(pd3dCommandList); });
}
void Object::Move(const XMFLOAT3 xmf3Vector)
{
}
void Object::Rotate(const XMFLOAT3 angle)
{
}

//DebugWindowObject::DebugWindowObject(
//	ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
//	D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle,
//	D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle, 
//	bool bIsPass2Screen)
//	: Object(pd3dDevice, pd3dCommandList, d3dCbvCPUDescriptorStartHandle, d3dCbvGPUDescriptorStartHandle)
//{
//	m_xmf4x4Local = Matrix4x4::Identity();
//	m_pDWMesh = new DebugWindowMesh(pd3dDevice, pd3dCommandList, bIsPass2Screen);
//
//	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);
//
//	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
//		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//	CreateCBV(pd3dDevice, d3dCbvCPUDescriptorStartHandle);
//	SetCbvGpuHandle(d3dCbvGPUDescriptorStartHandle);
//}
//DebugWindowObject::DebugWindowObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
//	D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle, 
//	D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle, 
//	float width, float height)
//	: Object(pd3dDevice, pd3dCommandList, d3dCbvCPUDescriptorStartHandle, d3dCbvGPUDescriptorStartHandle)
//{
//	m_xmf4x4Local = Matrix4x4::Identity();
//	m_pDWMesh = new DebugWindowMesh(pd3dDevice, pd3dCommandList, width, height);
//
//	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);
//
//	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
//		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
//
//	CreateCBV(pd3dDevice, d3dCbvCPUDescriptorStartHandle);
//	SetCbvGpuHandle(d3dCbvGPUDescriptorStartHandle);
//}
//void DebugWindowObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTSIGNATURE_OBJECTS, m_d3dCbvGPUDescriptorHandle);
//	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);
//	memset(m_pCBMappedObjects, NULL, ncbElementBytes);
//	XMStoreFloat4x4(&m_pCBMappedObjects->xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Local)));
//
//	m_pDWMesh->Render(pd3dCommandList);
//}
//
//AnimatedObject::AnimatedObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle, D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle)
//	:Object(pd3dDevice, pd3dCommandList, d3dCbvCPUDescriptorStartHandle, d3dCbvGPUDescriptorStartHandle)
//{
//}
//XMMATRIX const AnimatedObject::GetBoneMatrix(int boneIdx)
//{
//	return XMLoadFloat4x4(&m_boneHierarchyInfo.toWorld[boneIdx] );
//}
//void AnimatedObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
//{
//	g_AnimUploader->SetAnimationTransform(pd3dCommandList, m_boneHierarchyInfo);
//	Object::Render(pd3dCommandList);
//}
//void AnimatedObject::Update(float fTimeElapsed)
//{
//	m_time += fTimeElapsed;
//
//	memset(&m_boneHierarchyInfo.local, NULL, sizeof(XMFLOAT4) * 64);	// 이거 안해주면
//
//	for (int i = 0; i < m_vecpStateLayer.size(); i++)
//		m_vecpStateLayer[i]->Update(fTimeElapsed);
//
//	for (int i = 0; i < m_vecpStateLayer.size(); i++)
//		m_vecpStateLayer[i]->Animate(this);
//
//	AnimationCalculate::MakeToWorldTransform(m_boneHierarchyInfo); 
//}
//void AnimatedObject::Input(UCHAR* pKeyBuffer)
//{
//	for (int i = 0; i < m_vecpStateLayer.size(); i++)
//		m_vecpStateLayer[i]->Input(pKeyBuffer);
//}
//
//HumanoidObject::HumanoidObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle, D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle)
//	:AnimatedObject(pd3dDevice, pd3dCommandList, d3dCbvCPUDescriptorStartHandle, d3dCbvGPUDescriptorStartHandle)
//{
//	m_boneHierarchyInfo = g_AnimMng.GetBoneHierarchyFromAnimClip("Humanoid_Idle");
//
//	StateLayer* MovementLayer = new StateLayer();
//	StateLayer* ActionLayer = new StateLayer();
//
//	MovementLayer->ChangeStateTo(new StateHumanoidIdle(this, MovementLayer));
//	ActionLayer->ChangeStateTo(new StateHumanoidStand(this, ActionLayer));
//	ActionLayer->m_pMask = new BoneMask(BoneMask::PreDefined::eUpperBody);
//
//	m_vecpStateLayer.push_back(MovementLayer);
//	m_vecpStateLayer.push_back(ActionLayer);
//}
//
Screen::Screen(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle, D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle, float width, float height)
{
	m_pScreenMesh = new Mesh(pd3dDevice, pd3dCommandList, width, height);

	UINT ncbElementBytes = ((sizeof(XMFLOAT4X4) + 255) & ~255);

	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	D3D12_GPU_VIRTUAL_ADDRESS		d3dGpuVirtualAddress;
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;

	if (nullptr != m_pd3dCBResource) {
		m_pd3dCBResource->Map(0, NULL, (void**)&m_pCBMappedTransform);
		d3dGpuVirtualAddress = m_pd3dCBResource->GetGPUVirtualAddress();
		d3dCBVDesc.SizeInBytes = ncbElementBytes;
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorStartHandle);

		d3dCbvCPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;
	}

	m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorStartHandle;
	d3dCbvGPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;
}

Screen::~Screen()
{
	if (m_pScreenMesh) delete m_pScreenMesh;
	if (m_pd3dCBResource) {
		m_pd3dCBResource->Unmap(0, nullptr);
		m_pd3dCBResource->Release();
	}
	if (m_pCBMappedTransform) delete m_pCBMappedTransform;
}

void Screen::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTSIGNATURE_OBJECTS, m_d3dCbvGPUDescriptorHandle);
	UINT ncbElementBytes = ((sizeof(XMFLOAT4X4) + 255) & ~255);
	memset(m_pCBMappedTransform, NULL, ncbElementBytes);
	XMStoreFloat4x4(m_pCBMappedTransform, XMMatrixTranspose(XMMatrixIdentity()));

	m_pScreenMesh->Render(pd3dCommandList);
}
