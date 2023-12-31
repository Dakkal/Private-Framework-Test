#include "pch.h"
#include "GameInstance.h"
#include "PartObject.h"
#include "Player.h"
#include "StateMachine.h"
#include "State_Parry.h"
#include "Input_Device.h"
#include "State_Walk.h"

#include "Player.h"

CState_Parry::CState_Parry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
	: CState(pDevice, pContext, pOwner, eState)
{
}

HRESULT CState_Parry::Initialize()
{
	__super::Initialize();

	return S_OK;
}

STATE CState_Parry::Tick(const _float& fTimeDelta)
{
	STATE eState = m_eState;

	if (true == m_pRealOwner->Is_Hit())
	{
		if (true == dynamic_cast<CPlayer*>(m_pRealOwner)->Is_Parry())
		{
			return STATE::PARRY_SUCCESS;
		}
		else
			return STATE::HIT;
	}
		

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bIdle && m_pOwnerBodyPart->Is_AnimOverKeyFrame(120))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::IDLE;
	}
	else if (true == m_bAttack && false == m_bIdle && false == m_IsKeepParry && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(35))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::ATTACK;
	}
	else if (true == m_bAvoid && false == m_bIdle && false == m_IsKeepParry && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(35))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::AVOID;
	}
	else if (true == m_bWalk && false == m_bIdle && false == m_IsKeepParry && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(35))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::WALK;
	}
	else if (true == m_bParry2 && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(30))
	{
		m_pOwnerBodyPart->Set_AnimationIndex(false, 45, 4.f);
		m_IsKeepParry = false;
	}
	else if (true == m_bParryRe && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(30))
	{
		m_pOwnerBodyPart->Set_AnimationIndex(false, 44, 4.f, false, 5);
		m_IsKeepParry = false;
	}
	else
		eState = m_eState;


	if (2 <= m_pOwnerBodyPart->Get_CurKeyFrameNumb() &&
		25 >= m_pOwnerBodyPart->Get_CurKeyFrameNumb())
	{
		dynamic_cast<CPlayer*>(m_pRealOwner)->Set_Parry(true);
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pRealOwner)->Set_Parry(false);
	}

	RELEASE_INSTANCE(CGameInstance);

	return eState;
}

STATE CState_Parry::LateTick(const _float& fTimeDelta)
{
	STATE eState = m_eState;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIMouseState(CInput_Device::MOUSEKEY_STATE::LBUTTON) & 0x80)
	{
		m_bAttack = true;
		m_bAvoid = false;
		m_bWalk = false;
		m_bIdle = false;
		m_IsKeepParry = false;
	}
	else if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		m_bAttack = false;
		m_bAvoid = true;
		m_bWalk = false;
		m_bIdle = false;
		m_IsKeepParry = false;
	}
	else if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80 ||
		pGameInstance->Get_DIKeyState(DIK_D) & 0x80 ||
		pGameInstance->Get_DIKeyState(DIK_S) & 0x80 ||
		pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_bAttack = false;
		m_bAvoid = false;
		m_bWalk = true;
		m_bIdle = false;
		m_IsKeepParry = false;
	}
	else
	{
		m_bAttack = false;
		m_bAvoid = false;
		m_bWalk = false;
		m_bIdle = true;
		m_IsKeepParry = false;
	}

	if (true == m_bParry1 && 44 == m_pOwnerBodyPart->Get_AnimationIndex() &&
		pGameInstance->Get_DIKeyState(DIK_F) & 0x80 ||
		true == m_bParryRe && 44 == m_pOwnerBodyPart->Get_AnimationIndex() &&
		pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
	{
		pGameInstance->PlaySoundFile(TEXT("WhooshDagger02.ogg"), CHANNELID::CHANNEL_2, 1.f);

		m_bParry2 = true;
		m_bParry1 = false;
		m_bParryRe = false;
		m_IsKeepParry = true;
		m_bIdle = false;
	}
	else if (true == m_bParry2 && 45 == m_pOwnerBodyPart->Get_AnimationIndex() &&
		pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
	{
		pGameInstance->PlaySoundFile(TEXT("WhooshDagger01.ogg"), CHANNELID::CHANNEL_2, 1.f);

		m_bParry2 = false;
		m_bParryRe = true;
		m_IsKeepParry = true;
		m_bIdle = false;
	}



	RELEASE_INSTANCE(CGameInstance);

	return eState;
}

void CState_Parry::Enter_State()
{
	m_pRealOwner->Set_Move(false);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->PlaySoundFile(TEXT("WhooshDagger01.ogg"), CHANNELID::CHANNEL_2, 1.f);

	RELEASE_INSTANCE(CGameInstance);

	m_pOwnerBodyPart->Set_AnimationIndex(false, 44, 4.f);
	m_bParry1 = true;
}

void CState_Parry::Reset_State()
{
	dynamic_cast<CPlayer*>(m_pRealOwner)->Set_Parry(false);


	m_bAttack = false;
	m_bAvoid = false;
	m_bWalk = false;
	m_bIdle = true;
	m_IsKeepParry = false;
	m_bParry1 = false;
	m_bParry2 = false;
	m_bParryRe = false;
}

STATE CState_Parry::Key_Input(const _float& fTimeDelta)
{
	return STATE();
}


CState_Parry* CState_Parry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
{
	CState_Parry* pInstance = new CState_Parry(pDevice, pContext, pOwner, eState);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CState_Parry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Parry::Free()
{
	__super::Free();
}
