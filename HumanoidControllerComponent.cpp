#include "stdafx.h"
#include "HumanoidControllerComponent.h"
#include "Components.h"
#include "Object.h"

HumanoidControllerComponent::HumanoidControllerComponent(Object* pObject, Object* pWeapon)
	:Component(pObject)
	, m_pWeaponObject(pWeapon)
	, m_fTime(0)
	, m_xmf3Velocity(0,0,0)
	, m_fSpeed(1.0f)
	, m_fDragFactor(1.0f)
	, m_fAimProgress(0)
	, m_fTimeForAim(0.2f)
{
}

HumanoidControllerComponent::~HumanoidControllerComponent()
{
}

void HumanoidControllerComponent::Update(float fTimeElapsed)
{
	if (!m_bEnabled) return;

	InputManagerComponent* l_pInput = m_pObject->FindComponent<InputManagerComponent>();
	TransformComponent* l_transform = m_pObject->FindComponent<TransformComponent>();
	
	m_fTime += fTimeElapsed;

	// Movement Part
	{
		XMFLOAT2 xmf2MouseMovement = l_pInput->GetMouseMovement();
		
		// for test
		if (0 != xmf2MouseMovement.x) {

			l_transform->RotateXYZDegree(0, xmf2MouseMovement.x * -0.05f, 0);
		}
	}
	{
		XMFLOAT3 l_xmf3Direction = XMFLOAT3(0, 0, 0);
		
		if (l_pInput->IsKeyDown(KeyCode::_W)) { l_xmf3Direction.z += m_fSpeed; }
		if (l_pInput->IsKeyDown(KeyCode::_A)) { l_xmf3Direction.x -= m_fSpeed; }
		if (l_pInput->IsKeyDown(KeyCode::_S)) { l_xmf3Direction.z -= m_fSpeed; }
		if (l_pInput->IsKeyDown(KeyCode::_D)) { l_xmf3Direction.x += m_fSpeed; }

		float fPrevSpd = Vector3::Length(m_xmf3Velocity);
		float fSpdLimit = 1.5f;
		if (!Vector3::IsZero(l_xmf3Direction)) {
			m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::Multiply(fTimeElapsed * 2.8, Vector3::Normalize(l_xmf3Direction)));
			Clamp(m_xmf3Velocity.x, -fSpdLimit, fSpdLimit);
			Clamp(m_xmf3Velocity.z, -fSpdLimit, fSpdLimit);
		}

		if (0 == l_xmf3Direction.x) {
			if (m_xmf3Velocity.x > 0)	m_xmf3Velocity.x -= m_fDragFactor * fTimeElapsed;
			else						m_xmf3Velocity.x += m_fDragFactor * fTimeElapsed;
			if (m_fDragFactor * fTimeElapsed >= fabs(m_xmf3Velocity.x)) m_xmf3Velocity.x = 0;
		}
		if (0 == l_xmf3Direction.z) {
			if (m_xmf3Velocity.z > 0)	m_xmf3Velocity.z -= m_fDragFactor * fTimeElapsed;
			else						m_xmf3Velocity.z += m_fDragFactor * fTimeElapsed;
			if (m_fDragFactor * fTimeElapsed >= fabs(m_xmf3Velocity.z)) m_xmf3Velocity.z = 0;
		}

		XMFLOAT3 xmf3Velocity;
		XMFLOAT4 xmf4RotationQuaternion;
		XMVECTOR velocity, rotationQuaternion;
		velocity				= XMLoadFloat3(&m_xmf3Velocity);
		xmf4RotationQuaternion	= l_transform->GetRotationQuaternion(Space::world);
		rotationQuaternion		= XMLoadFloat4(&xmf4RotationQuaternion);
		velocity = XMVector3Rotate(velocity, rotationQuaternion);
		XMStoreFloat3(&xmf3Velocity, velocity);

		l_transform->Translate(Vector3::Multiply(fTimeElapsed, xmf3Velocity));
	}

	// Action Part
	{
		if (l_pInput->IsKeyDown(KeyCode::_LMB)) {
			if (m_fTimeForAim > m_fAimProgress) 
				m_fAimProgress += fTimeElapsed;
			else 
				m_pWeaponObject->FindComponent<WeaponControllerComponent>()->Fire();
		}
		else {
			m_fAimProgress -= fTimeElapsed;
			if (0 > m_fAimProgress) m_fAimProgress = 0;
		}
	}
}
