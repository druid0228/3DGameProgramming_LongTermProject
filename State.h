#pragma once

class HumanoidObject;
class BoneMask;

class HumanoidState {
public:
	HumanoidState(HumanoidObject* pObject)
		:m_pObject(pObject)
	{}

	virtual void EnterState() { m_fTime = 0; m_bEnable = true; }
	virtual void LeaveState() { m_bEnable = false; }

	virtual void Update(float fTimeElapsed) = 0;
	virtual void Input(UCHAR* pKeyBuffer) = 0;
	virtual void MakeAnimTransform() = 0;
	virtual void BlendAnimTransform() = 0;
	void AddSubState(const char* strStateName);
	void AddAction(const char* strStateName);
	void DisableSubState(const char* strStateName);
	bool IsEnable() { return m_bEnable; }
	void SetEnable(const bool bEnable) { m_bEnable = bEnable; }
protected:
	HumanoidObject* m_pObject = NULL;
	BoneMask* m_pBoneMask = NULL;
	float m_fTime = 0;
	bool m_bEnable = false;
};

class HumanoidBaseIdle : public HumanoidState {
public:
	HumanoidBaseIdle(HumanoidObject* pObject)
		:HumanoidState(pObject)
	{}

	virtual void Update(float fTimeElapsed);
	virtual void Input(UCHAR* pKeyBuffer);
	virtual void MakeAnimTransform();
	virtual void BlendAnimTransform() {}
private:
	XMFLOAT3 m_xmf3InputDir = XMFLOAT3(0,0,0);
	float m_fDragFactor = 1.0f;
	bool m_bPrevAiming = false;
	bool m_bCurrAiming = false;
};

class HumanoidSubMoving : public HumanoidState {
public:
	HumanoidSubMoving(HumanoidObject* pObject);

	virtual void Update(float fTimeElapsed);
	virtual void Input(UCHAR* pKeyBuffer) {}
	virtual void MakeAnimTransform() {}
	virtual void BlendAnimTransform();
};

class HumanoidSubAiming : public HumanoidState {
public:
	HumanoidSubAiming(HumanoidObject* pObject);

	virtual void Update(float fTimeElapsed);
	virtual void Input(UCHAR* pKeyBuffer);
	virtual void MakeAnimTransform() {}
	virtual void BlendAnimTransform();
};




//
//class Transaction;
//class HumanoidObject;
//
//constexpr float walkSpd = 1.5;
//constexpr float runSpd = 8;



//
//class State {
//public:
//	State(const char* strName, Object* obj) :m_strStateName(strName), m_pObject(obj) {}
//
//	void AddTransation(Transaction* transation) { m_vecTransactions.push_back(transation); }
//	bool IsSatisfyTransaction(const char* strStateName);
//	void ChangeStateTo(const char* strStateName);
//
//	virtual void EnterState() {}
//	virtual void LeaveState() {}
//public:
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void Input(UCHAR* pKeyBuffer) = 0;
//	virtual vector<pair<string, float>> GetAnimClipNameList() = 0;
//
//public:
//	string m_strStateName;
//
//protected:
//	vector<Transaction*>	m_vecTransactions;
//	Object*					m_pObject;
//};
//
//class HumanoidState_Idle : public State {
//public:
//	HumanoidState_Idle(const char* strName, Object* obj) :State(strName, obj) {}
//
//	virtual void Update(float fTimeElapsed) {};
//	virtual void Input(UCHAR* pKeyBuffer) {
//		if (IsKeyDown(pKeyBuffer, KeyCode::_W) || 
//			IsKeyDown(pKeyBuffer, KeyCode::_A) || 
//			IsKeyDown(pKeyBuffer, KeyCode::_S) || 
//			IsKeyDown(pKeyBuffer, KeyCode::_D))
//			if (IsSatisfyTransaction("HumanoidState_Moving")) { 
//				ChangeStateTo("HumanoidState_Moving"); 
//			}
//	}
//	virtual vector<pair<string, float>> GetAnimClipNameList() {
//		vector<pair<string, float>> result;
//		result.push_back(pair<string, float>("Humanoid_Idle", 1));
//		return result;
//	}
//};
//
//class HumanoidState_Moving : public State {
//public:
//	HumanoidState_Moving(const char* strName, Object* obj) 
//		:State(strName, obj) 
//		, m_xmf3InputDir(XMFLOAT3(0, 0, 0))
//	{}
//
//	virtual void EnterState() { m_bKeyDown = true; m_xmf3InputDir = XMFLOAT3(0, 0, 0); }
//	virtual void Update(float fTimeElapsed);
//	virtual void Input(UCHAR* pKeyBuffer);
//	virtual vector<pair<string, float>> GetAnimClipNameList();
//public:
//	bool m_bKeyDown		= false;
//	bool m_bShiftDown	= false;
//	float m_fDragFactor = 1.0f;
//	XMFLOAT3 m_xmf3InputDir;
//};
//

/*
	State, SubState, Action

	- State: Ű�Է� ó��, �⺻���� Animation ���
	- SubState: Ư�� ���� ����, State Animation�� Overwrite
	- Action: ���� Animation�� Overwrite

	Mask Struct�� �־�� ��.
	BoneMask::eUpperBody
	BoneMask::eLowerBody
	�⺻������ Bone�� Idx�� ���� �ִ� �迭
	���⼱ Pair�� ó���ؾ� �ϳ�?
	Mask�� ������ �� ����: �� �� Bone�� ���� ��. (���ٽ� ó��)

	�ִϸ��̼��� BaseState���� ���� �ִϸ��̼��� ���.	m_AnimCtrl->CalcAnimTransformFromBaseState(vecPairClipWeight, baseStateTime);
	�� ����, SubState���� ���� �ִϸ��̼ǿ� Blend.	m_AnimCtrl->BlendToPrevAnimTransform(vecPairClipWeight, time, Mask);
	�߰��� Action�� �ִٸ� ���� ������� �߰��� Blend.	m_AnimCtrl->BlendToPrevAnimTransform(vecPairClipWeight, time, Mask);

	State�� �� ������ �� �ϳ��� ����.
	State�� ��ȯ�� Object�� ���¸� ���� State���� �Ǵ�.	if(IsPossibleToChangeStatdTo(strStateName)) ChangeStateTo(strStateName);
	SubState�� �߰� �� �ð� ������ State���� �Ǵ�.			if(IsPossibleToAddSubState(strSubStateName))AddSubState(strSubStateName);
	Action�� �߰� �� �ð� ������ State���� �Ǵ�.			if(IsPossibleToAddAction(strActionName))	AddAction(strActionName);

	SubState�� ���� ������ �� ������ ���� �� �� ����.
	������ �̹� �����ϴ� SubState�� �߰��� ������ ���� ����.
	�̹� �����ϴ� SubState�� �߰��� ������ �ش� SubState�� �ð��� ������. �Ǵ� �ƹ� �͵� ���� ����. (���ʿ� SubState�� �����Ǵ� ���� State�ϱ�)

	Action ���� SubState�� ����.

	HumanoidObject�� ������ ���� ������ ����.
	m_CurrState, m_vecCurrSubState, m_vecCurrAction;

	State->GetPairClipWeight()�� Clip�̶� Weight Pair�� �ѱ��� ���� �׳� Update()���� ó���ع����ų� Render()���� ó���ع����ų� �� ��.
	�׷��� SubState, Action������ �ϰ��� �ְ� ó���� �� ����.

*/

//class BoneMask;
//class BaseState;
//extern unordered_map<string, BaseState*> g_uomStates;
//
//class BaseState {
//public:
//	enum class baseState {
//		eIDLE = 0,
//		eJUMP,
//		eDIE
//	};
//	enum class subState {
//		eMOVING = 0,
//		eAIMING,
//		eINAIR
//	};
//	enum class action {
//		eSHOOTING = 0
//	};
//public:
//	BaseState() = default;
//
//
//	virtual void EnterState() {}
//	virtual void LeaveState() {}
//
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
//	virtual void InputProcess(UCHAR* pKeyBuffer) = 0;
//
//private:
//	virtual void ChangeStateTo(baseState type) = 0;
//	virtual void AddSubState(subState type) = 0;
//	virtual void AddAction(action type) = 0;
//
//protected:
//	float m_fTime = 0;
//	BoneMask* m_pBoneMask = NULL;
//};
//
//class HumanoidBaseState : public BaseState {
//public:
//	HumanoidBaseState(HumanoidObject* pObject);
//
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
//	virtual void InputProcess(UCHAR* pKeyBuffer) = 0;
//
//private:
//	virtual void ChangeStateTo(baseState type) = 0;
//	virtual void AddSubState(subState type) = 0;
//	virtual void AddAction(action type) = 0;
//
//protected:
//	HumanoidObject* m_pObject = NULL;
//};
//class HumanoidSubState : public HumanoidBaseState {
//
//public:
//	HumanoidSubState() = delete;
//	HumanoidSubState(HumanoidObject* pObject) :HumanoidBaseState(pObject) {}
//
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
//	virtual void InputProcess(UCHAR* pKeyBuffer) = 0;
//
//public:
//
//};
//class HumanoidAction : public HumanoidBaseState {
//
//public:
//	HumanoidAction() = delete;
//	HumanoidAction(HumanoidObject* pObject) :HumanoidBaseState(pObject) {}
//
//	virtual void Update(float fTimeElapsed) = 0;
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) = 0;
//	virtual void InputProcess(UCHAR* pKeyBuffer) {};
//
//public:
//
//};
//
//class HumanoidBaseIdle : public HumanoidBaseState {
//public:
//	HumanoidBaseIdle(HumanoidObject* pObject);
//
//	virtual void Update(float fTimeElapsed);
//	virtual void InputProcess(UCHAR* pKeyBuffer);
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
//
//private:
//	virtual void ChangeStateTo(baseState type);
//	virtual void AddSubState(subState type);
//	virtual void AddAction(action type);
//private:
//	XMFLOAT3 m_xmf3InputDirection = XMFLOAT3(0, 0, 0);
//	float m_fDragFactor = 1.0f;
//	
//};
//class HumanoidBaseJump : public HumanoidBaseState {
//public:
//	HumanoidBaseJump(HumanoidObject* pObject);
//
//
//	virtual void Update(float fTimeElapsed);
//	virtual void InputProcess(UCHAR* pKeyBuffer);
//	virtual void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
//protected:
//	virtual void ChangeStateTo(baseState type);
//	virtual void AddSubState(subState type);
//	virtual void AddAction(action type);
//
//};
//class HumanoidSubAiming : public HumanoidSubState {
//public:
//
//};
//class HumanoidSubMoving : public HumanoidSubState {
//public:
//
//};
//class HumanoidSubInAir : public HumanoidSubState {
//public:
//
//};
//class HumanoidActShooting : public HumanoidAction {
//public:
//
//};
//class HumanoidActDamage : public HumanoidAction {
//public:
//
//};