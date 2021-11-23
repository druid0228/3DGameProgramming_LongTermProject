#pragma once
#include "Component.h"

/*
	Transform Component is Component for Object Coordnation.
*/
class Transform : public Component
{
public:
	Transform() = delete;
	Transform(Object* pObject);
	~Transform();
	
	void SetPosition(const XMFLOAT3& xmf3Position);
	void SetPosition(const float& fX, const float& fY, const float& fZ);
	void Translate(const XMFLOAT3& xmf3Val);
	void Translate(const float& fX, const float& fY, const float& fZ);
	void RotateXYZDegree(const XMFLOAT3& xmf3Val);
	void RotateXYZDegree(const float& fX, const float& fY, const float& fZ);

	XMMATRIX GetLocalTransform();
	XMFLOAT3 const GetLookVector();
	XMFLOAT3 const GetPosition();

private:
	XMFLOAT4X4	m_xmf4x4Local;
};
