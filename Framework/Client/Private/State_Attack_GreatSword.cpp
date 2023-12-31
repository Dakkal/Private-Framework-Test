#include "pch.h"
#include "GameInstance.h"
#include "PartObject.h"
#include "Player.h"
#include "StateMachine.h"
#include "State_Attack_GreatSword.h"
#include "Transform.h"

#include "Enemy_GreatSword.h"

CState_Attack_GreatSword::CState_Attack_GreatSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
	: CState(pDevice, pContext, pOwner, eState)
{
}

HRESULT CState_Attack_GreatSword::Initialize()
{
	__super::Initialize();

	return S_OK;
}

STATE CState_Attack_GreatSword::Tick(const _float& fTimeDelta)
{
	STATE eState = m_eState;

	if (true == m_bRush)
		eState = Rush();

	if (true == m_bCombo1)
		eState = Combo_1();

	if (true == m_bCombo2)
		eState = Combo_2();

	if (true == m_bCombo3)
		eState = Combo_3();

	return eState;
}

STATE CState_Attack_GreatSword::LateTick(const _float& fTimeDelta)
{
	STATE eState = m_eState;

	return eState;
}

void CState_Attack_GreatSword::Reset_State()
{
	m_pOwnerBodyPart->Set_Anim_TargetPos(XMVectorZero());

	if (true == m_bRush) 
		m_bRush = false;

	if (true == m_bCombo1)
	{
		m_bCombo1 = false;
		m_bCombo1_1 = false;
		m_bCombo1_2 = false;
	}
	if (true == m_bCombo2)
	{
		m_bCombo2 = false;
		m_bCombo2_1 = false;
		m_bCombo2_2 = false;
	}
		
	if (true == m_bCombo3)
	{
		m_bCombo3 = false;
		m_bCombo3_1 = false;
		m_bCombo3_2 = false;
		m_bCombo3_3 = false;
	}
		
}

void CState_Attack_GreatSword::Enter_State()
{
	m_pRealOwner->Set_Move(true);

	dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Set_LookPlayer(true);

	_float fMinRushDist = 8.f; _float fMaxRushDist = 10.f;
	if (fMinRushDist <= dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Get_PlayerDistance() &&
		fMaxRushDist >= dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Get_PlayerDistance())
	{
		m_bRush = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 32, 1.2f);
		return;
	}
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_int iRandom = pGameInstance->Random_Int(0, 4);

	if (0 == iRandom || 1 == iRandom)
	{
		m_bCombo1 = true;
		m_bCombo1_1 = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 5, 1.2f);
	}
	else if (2 == iRandom || 3 == iRandom)
	{
		m_bCombo2 = true;
		m_bCombo2_1 = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 1, 1.2f);
	}
	else if (4 == iRandom)
	{
		m_bCombo3 = true;
		m_bCombo3_1 = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 3, 1.2f);
	}

	RELEASE_INSTANCE(CGameInstance);
}

STATE CState_Attack_GreatSword::Key_Input(const _float& fTimeDelta)
{
	return m_eState;
}

STATE CState_Attack_GreatSword::Rush()
{
	if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(15))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("FootStep_FullPlateA_03.ogg"), CHANNELID::CHANNEL_3, 1.f);

		RELEASE_INSTANCE(CGameInstance);
	}
	else if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(30))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("FootStep_FullPlateA_04.ogg"), CHANNELID::CHANNEL_3, 1.f);

		RELEASE_INSTANCE(CGameInstance);
	}

	if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(35))
	{
		CTransform* pTransform = dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Get_PlayerTransform();
		if(nullptr != pTransform)
			m_pOwnerBodyPart->Set_Anim_TargetPos(pTransform->Get_State(CTransform::STATE_POS));

		dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Set_LookPlayer(false);
		
		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(45))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("NM_V_GreatSword_RaidAttack1.ogg"), CHANNELID::CHANNEL_3, 1.f);

		RELEASE_INSTANCE(CGameInstance);
		
	}
	else if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(50))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("JumpAttackLanding_01.ogg"), CHANNELID::CHANNEL_4, 1.f);

		RELEASE_INSTANCE(CGameInstance);

	}
	else if (true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(60))
	{
		m_pRealOwner->Set_Attack(false);
	}

	if (true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(80))
	{
		if (true == m_pRealOwner->Is_Hit())
			return STATE::HIT;
	}
	if (true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(90))
	{
		return STATE::IDLE;
	}
	
	return m_eState;
}

STATE CState_Attack_GreatSword::Combo_1()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bCombo1_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(10))
	{
		pGameInstance->PlaySoundFile(TEXT("FullPlateAction_01_01.ogg"), CHANNELID::CHANNEL_3, 1.f);
	}
	else if (true == m_bCombo1_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(40))
	{
		pGameInstance->PlaySoundFile(TEXT("Sword_H_ATK_02.ogg"), CHANNELID::CHANNEL_4, 1.f);

		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_bCombo1_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(60))
	{
		m_pRealOwner->Set_Attack(false);
	}
	else if (true == m_bCombo1_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(80))
	{
		m_bCombo1_1 = false;
		m_bCombo1_2 = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 0, 1.2f);
	}

	if (true == m_bCombo1_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(20))
	{
		pGameInstance->PlaySoundFile(TEXT("FullPlateAction_01_02.ogg"), CHANNELID::CHANNEL_3, 1.f);
	}
	else if (true == m_bCombo1_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(40))
	{
		pGameInstance->PlaySoundFile(TEXT("Sword_H_ATK_03.ogg"), CHANNELID::CHANNEL_3, 1.f);
		pGameInstance->PlaySoundFile(TEXT("NM_V_GreatSword_ComboA01_2.ogg"), CHANNELID::CHANNEL_4, 1.f);
		
		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_bCombo1_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(60))
	{
		dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Set_LookPlayer(false);
		m_pRealOwner->Set_Attack(false);
	}

	if (true == m_bCombo1_2 && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(100))
	{
		if (true == m_pRealOwner->Is_Hit())
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::HIT;
		}
			
	}
	if (true == m_bCombo1_2 && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(120))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::IDLE;
	}

	RELEASE_INSTANCE(CGameInstance);

	return m_eState;
}

STATE CState_Attack_GreatSword::Combo_2()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bCombo2_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(10))
	{
		pGameInstance->PlaySoundFile(TEXT("Sword_H_ATK_02.ogg"), CHANNELID::CHANNEL_4, 1.f);
		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_bCombo2_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(40))
	{

		m_pRealOwner->Set_Attack(false);
	}
	else if (true == m_bCombo2_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(60))
	{
		m_bCombo2_1 = false;
		m_bCombo2_2 = true;
		m_pOwnerBodyPart->Set_AnimationIndex(false, 4, 1.2f, false, 15);
	}

	if (true == m_bCombo2_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(16))
	{
		pGameInstance->PlaySoundFile(TEXT("FullPlateAction_01_01.ogg"), CHANNELID::CHANNEL_3, 1.f);
	}
	else if (true == m_bCombo2_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(35))
	{
		pGameInstance->PlaySoundFile(TEXT("Sword_H_ATK_03.ogg"), CHANNELID::CHANNEL_3, 1.f);
		pGameInstance->PlaySoundFile(TEXT("NM_V_GreatSword_ComboB02-002.ogg"), CHANNELID::CHANNEL_4, 1.f);
		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_bCombo2_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(60))
	{
		dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Set_LookPlayer(false);
		m_pRealOwner->Set_Attack(false);
	}

	if (true == m_bCombo2_2 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(100))
	{
		if (true == m_pRealOwner->Is_Hit())
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::HIT;
		}
			
	}
	if (true == m_bCombo2_2 && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(120))
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::IDLE;
	}

	return m_eState;
}

STATE CState_Attack_GreatSword::Combo_3()
{

	if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(10))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("FullPlateAction_01_02.ogg"), CHANNELID::CHANNEL_3, 1.f);

		RELEASE_INSTANCE(CGameInstance);
	}
	else if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(30))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		pGameInstance->PlaySoundFile(TEXT("Sword_H_ATK_03.ogg"), CHANNELID::CHANNEL_4, 1.f);

		RELEASE_INSTANCE(CGameInstance);

		m_pRealOwner->Set_Attack(true);
	}
	else if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(40))
	{
		dynamic_cast<CEnemy_GreatSword*>(m_pRealOwner)->Set_LookPlayer(false);
	}
	else if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->Is_AnimCurKeyFrame(50))
	{
	
		m_pRealOwner->Set_Attack(false);
	}

	if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(80))
	{
		if (true == m_pRealOwner->Is_Hit())
			return STATE::HIT;
	}
	if (true == m_bCombo3_1 && true == m_pOwnerBodyPart->IsAnimationEnd())
	{
		return STATE::AVOID;
	}
	

	return m_eState;
}

CState_Attack_GreatSword* CState_Attack_GreatSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
{
	CState_Attack_GreatSword* pInstance = new CState_Attack_GreatSword(pDevice, pContext, pOwner, eState);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CState_Attack_GreatSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Attack_GreatSword::Free()
{
	__super::Free();
}
