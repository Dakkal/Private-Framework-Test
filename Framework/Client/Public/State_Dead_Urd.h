#pragma once
#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_Dead_Urd final : public CState
{
private:
	CState_Dead_Urd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState);
	virtual ~CState_Dead_Urd() = default;

public:
	virtual HRESULT			Initialize();
	virtual STATE			Tick(const _float& fTimeDelta);
	virtual STATE			LateTick(const _float& fTimeDelta);

	virtual void			Enter_State();
	virtual void			Reset_State();

	virtual STATE			Key_Input(const _float& fTimeDelta);

private:
	class CBinBone*			m_pPlayerAnimBone{ nullptr };
	_bool					m_bExcuted = { false };

public:
	static CState_Dead_Urd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState);
	virtual void	Free() override;
};

END


