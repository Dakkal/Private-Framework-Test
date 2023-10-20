#pragma once
#include "Base.h"

BEGIN(Engine)

class CStateMachine;

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState);
	virtual ~CState() = default;

public:
	virtual HRESULT			Initialize() PURE;
	// ����Ÿ���� ��ȯ�Ͽ� ü���� �˻�;
	virtual STATE			Tick(const _float & fTimeDelta) PURE;
	virtual STATE			LateTick(const _float & fTimeDelta) PURE;
	virtual void			Reset_State() PURE;
	virtual void			Enter_State() PURE;

	virtual STATE			Key_Input(const _float & fTimeDelta) PURE;
	

	virtual	STATE			Get_State() { return m_eState; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	// � ���¸ӽ��� �ڽ��� ���ϰ� �ִ��� �˷��ִ� ����
	CStateMachine*			m_pStateOwner = { nullptr };
	// � �������� �����ִ� ����
	STATE					m_eState = { STATE::_END};
	// ó�������� 
	_bool					m_bEnter = { false };

public:
	virtual void	Free() override;
};

END

