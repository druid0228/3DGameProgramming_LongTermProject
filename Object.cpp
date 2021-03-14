#include "stdafx.h"
#include "Object.h"
#include "Mesh.h"

Object::Object(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_xmf4x4World	= Matrix4x4::Identity();
	m_pMesh			= new Mesh(pd3dDevice, pd3dCommandList);

	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);

	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
}

void Object::Update(float fTimeElapsed)
{
}

void Object::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTSIGNATUREOBJECTIDX, m_d3dCbvGPUDescriptorHandle);
	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);
	memset(m_pCBMappedObjects, NULL, ncbElementBytes);
	XMStoreFloat4x4(&m_pCBMappedObjects->xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));

	m_pMesh->Render(pd3dCommandList);
}

void Object::SetCbvGpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE& d3dCbvGPUDescriptorStartHandle)
{
	m_d3dCbvGPUDescriptorHandle = d3dCbvGPUDescriptorStartHandle;
	d3dCbvGPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;
}

void Object::CreateCBV(ID3D12Device* pd3dDevice, D3D12_CPU_DESCRIPTOR_HANDLE& d3dCbvCPUDescriptorStartHandle)
{
	D3D12_GPU_VIRTUAL_ADDRESS		d3dGpuVirtualAddress;
	D3D12_CONSTANT_BUFFER_VIEW_DESC d3dCBVDesc;
	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);

	if (nullptr != m_pd3dCBResource) {
		m_pd3dCBResource->Map(0, NULL, (void**)&m_pCBMappedObjects);
		d3dGpuVirtualAddress = m_pd3dCBResource->GetGPUVirtualAddress();
		d3dCBVDesc.SizeInBytes = ncbElementBytes;
		d3dCBVDesc.BufferLocation = d3dGpuVirtualAddress;
		pd3dDevice->CreateConstantBufferView(&d3dCBVDesc, d3dCbvCPUDescriptorStartHandle);

		d3dCbvCPUDescriptorStartHandle.ptr += gnCbvSrvDescriptorIncrementSize;
	}
}

void Object::Move(const XMFLOAT3 xmf3Vector)
{
	m_xmf4x4World._41 += xmf3Vector.x;
	m_xmf4x4World._42 += xmf3Vector.y;
	m_xmf4x4World._43 += xmf3Vector.z;
}

void Object::Rotate(const XMFLOAT3 angle)
{
	XMMATRIX xmmtxRotate	= XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-angle.x), XMConvertToRadians(-angle.y), XMConvertToRadians(-angle.z)));
	XMFLOAT3 xmf3Right		= XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	XMFLOAT3 xmf3Up			= XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	XMFLOAT3 xmf3Look		= XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);

	xmf3Look	= Vector3::TransformNormal(xmf3Look, xmmtxRotate);
	xmf3Right	= Vector3::TransformNormal(xmf3Right, xmmtxRotate);

	xmf3Look	= Vector3::Normalize(xmf3Look);
	xmf3Right	= Vector3::CrossProduct(xmf3Up, xmf3Look, true);
	xmf3Up		= Vector3::CrossProduct(xmf3Look, xmf3Right, true);

	m_xmf4x4World._11 = xmf3Right.x;	m_xmf4x4World._12 = xmf3Right.y;	m_xmf4x4World._13 = xmf3Right.z;
	m_xmf4x4World._21 = xmf3Up.x;		m_xmf4x4World._22 = xmf3Up.y;		m_xmf4x4World._23 = xmf3Up.z;
	m_xmf4x4World._31 = xmf3Look.x;		m_xmf4x4World._32 = xmf3Look.y;		m_xmf4x4World._33 = xmf3Look.z;
}

DebugWindowObject::DebugWindowObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, bool bIsPass2Screen)
	: Object(pd3dDevice, pd3dCommandList)
{
	m_xmf4x4World = Matrix4x4::Identity();
	m_pDWMesh = new DebugWindowMesh(pd3dDevice, pd3dCommandList, bIsPass2Screen);

	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);

	m_pd3dCBResource = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes,
		D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);
}

void DebugWindowObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootDescriptorTable(ROOTSIGNATUREOBJECTIDX, m_d3dCbvGPUDescriptorHandle);
	UINT ncbElementBytes = ((sizeof(CB_OBJECT_INFO) + 255) & ~255);
	memset(m_pCBMappedObjects, NULL, ncbElementBytes);
	XMStoreFloat4x4(&m_pCBMappedObjects->xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));

	m_pDWMesh->Render(pd3dCommandList);
}