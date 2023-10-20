#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine : public CComponent
{
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);;
	CStateMachine(class CGameObject* pOwner, const CStateMachine& rhs);
	virtual ~CStateMachine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

	// ��������� ������Ʈ�� ��ȯ ����Ÿ���� �޾� ���Ͽ� ���°� ���ߴ��� �Ǵ���
	void			Tick_StateMachine(const _float& fTimeDelta);
	void			LateTick_StateMachine(const _float& fTimeDelta);
	void			Render_StateMachine();



public:
	// ���¸� ���� �� �������ִ� �Լ�(�ִϸ����ͱ��� ���� �ٲ���)
	void			Set_State(STATE eState);
	// ���¸� �������̳ʿ��ٰ� �߰����ִ� �Լ�
	HRESULT			Add_State(STATE eState, class CState* pState);

	const STATE& Get_CurState() { return m_eCurState; }

protected:
	map<STATE, CState*>		m_StateMap;
	class CState*			m_pCurState;
	STATE					m_eCurState;
	class CBinModel*		m_pOwnerModel;

public:
	static CStateMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;

};


END