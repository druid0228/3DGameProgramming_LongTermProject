#include "stdafx.h"
#include "TransformComponent.h"
#include "Object.h"


TransformComponent::TransformComponent(Object* pObject)
	: Component(pObject)
	, m_xmf4x4Local(Matrix4x4::Identity())
{
}
TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetLocalTransform(const XMMATRIX& xmmtxTransform)
{
	XMStoreFloat4x4(&m_xmf4x4Local, xmmtxTransform);
}

void TransformComponent::SetPosition(const XMFLOAT3& xmf3Position)
{
	m_xmf4x4Local._41 = xmf3Position.x;
	m_xmf4x4Local._42 = xmf3Position.y;
	m_xmf4x4Local._43 = xmf3Position.z;
}
void TransformComponent::SetPosition(const float& fX, const float& fY, const float& fZ)
{
	m_xmf4x4Local._41 = fX;
	m_xmf4x4Local._42 = fY;
	m_xmf4x4Local._43 = fZ;
}
void TransformComponent::Translate(const XMFLOAT3& xmf3Val)
{
	m_xmf4x4Local._41 += xmf3Val.x;
	m_xmf4x4Local._42 += xmf3Val.y;
	m_xmf4x4Local._43 += xmf3Val.z;
}
void TransformComponent::Translate(const float& fX, const float& fY, const float& fZ)
{
	m_xmf4x4Local._41 += fX;
	m_xmf4x4Local._42 += fY;
	m_xmf4x4Local._43 += fZ;
}
void TransformComponent::RotateXYZDegree(const XMFLOAT3& xmf3Val)
{
	XMMATRIX xmmtxRotate 
		= XMMatrixRotationQuaternion(
			XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(-xmf3Val.x),
				XMConvertToRadians(-xmf3Val.y),
				XMConvertToRadians(-xmf3Val.z)
			)
		);
	XMFLOAT3 xmf3Right	= XMFLOAT3(m_xmf4x4Local._11, m_xmf4x4Local._12, m_xmf4x4Local._13);
	XMFLOAT3 xmf3Up		= XMFLOAT3(m_xmf4x4Local._21, m_xmf4x4Local._22, m_xmf4x4Local._23);
	XMFLOAT3 xmf3Look	= XMFLOAT3(m_xmf4x4Local._31, m_xmf4x4Local._32, m_xmf4x4Local._33);

	xmf3Look	= Vector3::TransformNormal(xmf3Look, xmmtxRotate);
	xmf3Right	= Vector3::TransformNormal(xmf3Right, xmmtxRotate);

	xmf3Look	= Vector3::Normalize(xmf3Look);
	xmf3Right	= Vector3::CrossProduct(xmf3Up, xmf3Look, true);
	xmf3Up		= Vector3::CrossProduct(xmf3Look, xmf3Right, true);

	m_xmf4x4Local._11 = xmf3Right.x;	m_xmf4x4Local._12 = xmf3Right.y;	m_xmf4x4Local._13 = xmf3Right.z;
	m_xmf4x4Local._21 = xmf3Up.x;		m_xmf4x4Local._22 = xmf3Up.y;		m_xmf4x4Local._23 = xmf3Up.z;
	m_xmf4x4Local._31 = xmf3Look.x;		m_xmf4x4Local._32 = xmf3Look.y;		m_xmf4x4Local._33 = xmf3Look.z;
}
void TransformComponent::RotateXYZDegree(const float& fX, const float& fY, const float& fZ)
{
	XMMATRIX xmmtxRotate
		= XMMatrixRotationQuaternion(
			XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(-fX),
				XMConvertToRadians(-fY),
				XMConvertToRadians(-fZ)
			)
		);

	XMFLOAT3 xmf3Right	= XMFLOAT3(m_xmf4x4Local._11, m_xmf4x4Local._12, m_xmf4x4Local._13);
	XMFLOAT3 xmf3Up		= XMFLOAT3(m_xmf4x4Local._21, m_xmf4x4Local._22, m_xmf4x4Local._23);
	XMFLOAT3 xmf3Look	= XMFLOAT3(m_xmf4x4Local._31, m_xmf4x4Local._32, m_xmf4x4Local._33);

	xmf3Look	= Vector3::TransformNormal(xmf3Look, xmmtxRotate);
	xmf3Right	= Vector3::TransformNormal(xmf3Right, xmmtxRotate);

	xmf3Look	= Vector3::Normalize(xmf3Look);
	xmf3Right	= Vector3::CrossProduct(xmf3Up, xmf3Look, true);
	xmf3Up		= Vector3::CrossProduct(xmf3Look, xmf3Right, true);

	m_xmf4x4Local._11 = xmf3Right.x;	m_xmf4x4Local._12 = xmf3Right.y;	m_xmf4x4Local._13 = xmf3Right.z;
	m_xmf4x4Local._21 = xmf3Up.x;		m_xmf4x4Local._22 = xmf3Up.y;		m_xmf4x4Local._23 = xmf3Up.z;
	m_xmf4x4Local._31 = xmf3Look.x;		m_xmf4x4Local._32 = xmf3Look.y;		m_xmf4x4Local._33 = xmf3Look.z;
}

XMMATRIX TransformComponent::GetLocalTransform()
{
	return XMLoadFloat4x4(&m_xmf4x4Local);
}
XMMATRIX TransformComponent::GetWorldTransform()
{
	if (nullptr != m_pObject->m_pParent) {
		TransformComponent* l_pParentTransform = m_pObject->m_pParent->FindComponent<TransformComponent>();
		return XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4Local), l_pParentTransform->GetWorldTransform());
	}
	else return XMLoadFloat4x4(&m_xmf4x4Local);
}
XMFLOAT3 const TransformComponent::GetLookVector()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4Local._31, m_xmf4x4Local._32, m_xmf4x4Local._33));
}
XMFLOAT3 const TransformComponent::GetUpVector()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4Local._21, m_xmf4x4Local._22, m_xmf4x4Local._23));
}
XMFLOAT3 const TransformComponent::GetRightVector()
{
	return Vector3::Normalize(XMFLOAT3(m_xmf4x4Local._11, m_xmf4x4Local._12, m_xmf4x4Local._13));
}
XMFLOAT3 const TransformComponent::GetPosition()
{
	return XMFLOAT3(m_xmf4x4Local._41, m_xmf4x4Local._42, m_xmf4x4Local._43);
}

XMFLOAT4 const TransformComponent::GetRotationQuaternion()
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMQuaternionRotationMatrix(GetLocalTransform()));
	return result;
}