#pragma once

class AnimatedObject;
class BoneMask;

using ClipPair = vector<pair<string, float>>;

class State;

struct Transition {
	float	m_fTime = 0;
	float	m_fDelay = 0;
	bool	m_bInterruptedCancel = false;
	State*	m_pTargetState = NULL;

	Transition(State* pState) :m_pTargetState(pState) {}
};

class State {
public:
	State(const char* animClipName) 
		:m_strBaseAnimClip(animClipName), m_fTime(0) {}

public:
	virtual void Update(AnimatedObject* pObj, float fTimeElapsed) {}
	virtual void Input(AnimatedObject* pObj, UCHAR* pKeyBuffer) {}
	virtual ClipPair GetAnimateClipPair() { 
		return ClipPair();
	}

public:
	virtual void Interrupted() {
		if (m_pCurTransition && m_pCurTransition->m_bInterruptedCancel) {
			delete m_pCurTransition;
			m_pCurTransition = NULL;
		}
	}
	void StartTransition(Transition* t) { m_pCurTransition = t; }

public:
	float		m_fTime;
protected:
	vector<Transition*> m_pvTransition;
	Transition* m_pCurTransition = NULL;
	string		m_strBaseAnimClip;
};

class StateLayer {
public:
	void Update(AnimatedObject* pObj, float fTimeElapsed);
	void Input(AnimatedObject* pObj, UCHAR* pKeyBuffer);
	void Animate(AnimatedObject* pObj);
	void Interrupted();
	void ChangeStateTo(State* pState);

public:
	float		m_fWeight = 1.0f;
	BoneMask*	m_pMask = NULL;

private:
	State*		m_pState;
};

/*
	Humanoid State
	\Base Layer
	\Action Layer

	Base Layer
	- Idle
	- (Walk)
	- (Run)
	- Jump
	- RunJump

	Action Layer
	- Stand
	- Aim
	- Shoot
	- Reload
	- Skill(not yet)

	Interrupt Layer(not yet)
	- Damage
	- Stuck
	- Stun
	- Down
	- Die

	AnimClip�� ���� State�� �ƹ� �͵� Blend ���� �ʾƾ� �ϴµ�
	�װ� ��� ó���� ������?
	���ڿ� ""���� ���ϴ°� ���ٴϱ� ���� bool������ �ϳ� �ּ�
	m_bIsAnimBlend ���� �ع����� �͵� ������ ��
*/

/*
* Idle�� Walk, Run�� AnimClip�� ������
* BaseAnimClip�� Idle
* ��ȯ�� ��, Ű�������� ���ڱ� ���ؼ� �ȵǹǷ� �ð��� ���޹޾ƾ� ��.
* ��� ������ �����ϰ� ���� �ӷ¸� �ٸ�.
*/
class StateHumanoidIdle : public State {
public:
	StateHumanoidIdle();

public:
	virtual void Update(AnimatedObject* pObj, float fTimeElapsed);
	virtual void Input(AnimatedObject* pObj, UCHAR* pKeyBuffer);
	virtual ClipPair GetAnimateClipPair();
};

//class StateHumanoidStand : public State {
//public:
//	StateHumanoidStand();
//
//public:
//	virtual void Update(AnimatedObject* pObj, float fTimeElapsed);
//	virtual void Input(AnimatedObject* pObj, UCHAR* pKeyBuffer);
//	virtual ClipPair GetAnimateClipPair();
//};
//
class StateHumanoidAim : public State {
public:
	StateHumanoidAim();

public:
	virtual void Update(AnimatedObject* pObj, float fTimeElapsed);
	virtual void Input(AnimatedObject* pObj, UCHAR* pKeyBuffer);
	virtual ClipPair GetAnimateClipPair();
};

//class HumanoidObject;
//class BoneMask;
//
//class State {
//public:
//	virtual void EnterState() {}
//	virtual void LeaveState() {}
//
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void Input(UCHAR* pKeyBuffer) = 0;
//
//protected:
//	bool m_bEnable = false;
//};
//
//class HumanoidState : public State {
//public:
//	HumanoidState(HumanoidObject* pObject)
//		:m_pObject(pObject)
//	{}
//
//	virtual void EnterState() { m_fTime = 0; m_bEnable = true; }
//	virtual void LeaveState() { m_bEnable = false; }
//
//	virtual void MakeAnimTransform() = 0;
//	virtual void BlendAnimTransform() = 0;
//	void AddSubState(const char* strStateName);
//	void AddAction(const char* strStateName);
//	void DisableSubState(const char* strStateName);
//	bool IsEnable() { return m_bEnable; }
//	void SetEnable(const bool bEnable) { m_bEnable = bEnable; }
//protected:
//	HumanoidObject* m_pObject = NULL;
//	BoneMask* m_pBoneMask = NULL;
//	float m_fTime = 0;
//};
//
//class HumanoidBaseIdle : public HumanoidState {
//public:
//	HumanoidBaseIdle(HumanoidObject* pObject)
//		:HumanoidState(pObject)
//	{}
//
//	virtual void Update(float fTimeElapsed);
//	virtual void Input(UCHAR* pKeyBuffer);
//	virtual void MakeAnimTransform();
//	virtual void BlendAnimTransform() {}
//private:
//	XMFLOAT3 m_xmf3InputDir = XMFLOAT3(0,0,0);
//	float m_fDragFactor = 1.0f;
//	bool m_bPrevAiming = false;
//	bool m_bCurrAiming = false;
//};
//
//class HumanoidSubMoving : public HumanoidState {
//public:
//	HumanoidSubMoving(HumanoidObject* pObject);
//
//	virtual void Update(float fTimeElapsed);
//	virtual void Input(UCHAR* pKeyBuffer) {}
//	virtual void MakeAnimTransform() {}
//	virtual void BlendAnimTransform();
//};
//
//class HumanoidSubAiming : public HumanoidState {
//public:
//	HumanoidSubAiming(HumanoidObject* pObject);
//
//	virtual void Update(float fTimeElapsed);
//	virtual void Input(UCHAR* pKeyBuffer);
//	virtual void MakeAnimTransform() {}
//	virtual void BlendAnimTransform();
//};