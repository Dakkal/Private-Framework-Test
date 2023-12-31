#include "pch.h"
#include "GameInstance.h"
#include "PartObject.h"
#include "Player.h"
#include "StateMachine.h"
#include "State_Lockon_ParrySuccess.h"
#include "Input_Device.h"
#include "Player.h"
#include "ParrySpark.h"

CState_Lockon_ParrySuccess::CState_Lockon_ParrySuccess(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
	: CState(pDevice, pContext, pOwner, eState)
{
}

HRESULT CState_Lockon_ParrySuccess::Initialize()
{
	__super::Initialize();

	m_bParryLeft = true;

	return S_OK;
}

STATE CState_Lockon_ParrySuccess::Tick(const _float& fTimeDelta)
{
	CGameObject* pTarget = dynamic_cast<CPlayer*>(m_pRealOwner)->Get_TargetEnemy();
	if (nullptr == pTarget)
		return STATE::IDLE;

	if (true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(10) && true == m_pRealOwner->Is_Hit())
		return STATE::HIT;

	CComponent* pCom = pTarget->Get_Component(TEXT("Com_Transform"));
	CTransform* pTargetTransform = dynamic_cast<CTransform*>(pCom);

	_vector vLook = m_pOwnerTransform->Get_State(CTransform::STATE_LOOK);
	_vector vDir = pTargetTransform->Get_State(CTransform::STATE_POS) - m_pOwnerTransform->Get_State(CTransform::STATE_POS);
	vDir.Normalize();
	vDir.y = 0.f;

	_float fRotMax = 66.f;
	_float fRotNormal = 20.f;

	_vector RotDir;

	if (3.f < acosf(vDir.Dot(vLook)))
	{
		RotDir = _vector::Lerp(vLook, vDir, fRotMax);
	}
	else
	{
		RotDir = _vector::Lerp(vLook, vDir, fRotNormal * fTimeDelta);
	}
	m_pOwnerTransform->Set_Look(RotDir);


	STATE eState = Key_Input(fTimeDelta);

	return eState;
}

STATE CState_Lockon_ParrySuccess::LateTick(const _float& fTimeDelta)
{
	STATE eState = m_eState;

	if (true == m_IsParry && true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(2))
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CEffectObject::EFFECT_DESC	effectdesc;
		effectdesc.pTargetTransform = m_pOwnerTransform;
		effectdesc.vTargetPos = m_pOwnerWeaponRPart->Get_Collide_Center();
		effectdesc.vRotFactorX = _float2(-15.f, 15.f);

		if (true == m_bParryLeftUp)
			effectdesc.vRotFactorZ = _float2(60.f, 80.f);

		else if (true == m_bParryLeftDown)
			effectdesc.vRotFactorZ = _float2(100.f, 120.f);	

		else if (true == m_bParryRightUp)
			effectdesc.vRotFactorZ = _float2(-60.f, -80.f);

		else if (true == m_bParryRightDown)
			effectdesc.vRotFactorZ = _float2(-100.f, -120.f);


		effectdesc.eEffectType = CEffectObject::TYPE::RECT;

		pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYER_EFFECT, TEXT("Prototype_GameObject_ParrySpark"), &effectdesc);

		RELEASE_INSTANCE(CGameInstance);

		m_IsParry = false;
	}

	

	return eState;
}

void CState_Lockon_ParrySuccess::Reset_State()
{
	m_IsParry = false;
	m_bParryLeftUp = false;
	m_bParryRightUp = false;
	m_bParryLeftDown = false;
	m_bParryRightDown = false;
}

void CState_Lockon_ParrySuccess::Enter_State()
{
	m_pRealOwner->Set_Move(false);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_bParryLeft)
	{
		m_bParryLeft = false;

		if (true == pGameInstance->Random_Coin(0.5f))
		{
			m_pOwnerBodyPart->Set_AnimationIndex(false, 111, 3.5f);
			m_bParryLeftDown = true;
		}
		else
		{
			m_pOwnerBodyPart->Set_AnimationIndex(false, 113, 3.5f);
			m_bParryLeftUp = true;
		}

		m_bParryRight = true;
	}
	else if (true == m_bParryRight)
	{
		m_bParryRight = false;

		if (true == pGameInstance->Random_Coin(0.5f))
		{
			m_pOwnerBodyPart->Set_AnimationIndex(false, 115, 3.5f);
			m_bParryRightDown = true;
		}
		else
		{
			m_pOwnerBodyPart->Set_AnimationIndex(false, 116, 3.5f);
			m_bParryRightUp = true;
		}

		m_bParryLeft = true;
	}

	m_IsParry = true;

	pGameInstance->PlaySoundFile(TEXT("ParrySuc_03_01.ogg"), CHANNELID::CHANNEL_2, 1.f);

	RELEASE_INSTANCE(CGameInstance);
}

STATE CState_Lockon_ParrySuccess::Key_Input(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(10))
	{
		if (pGameInstance->Get_DIKeyState(DIK_SPACE) & 0x80)
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::LOCK_AVOID;
		}
		if (pGameInstance->Get_DIMouseState(CInput_Device::MOUSEKEY_STATE::LBUTTON) & 0x80)
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::LOCK_ATTACK;
		}
		if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80 ||
			pGameInstance->Get_DIKeyState(DIK_D) & 0x80 ||
			pGameInstance->Get_DIKeyState(DIK_S) & 0x80 ||
			pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::LOCK_WALK;
		}
	}
	if (true == m_pOwnerBodyPart->Is_AnimOverKeyFrame(3))
	{
		if (pGameInstance->Get_DIKeyState(DIK_F) & 0x80)
		{
			RELEASE_INSTANCE(CGameInstance);
			return STATE::LOCK_PARRY;
		}
	}
	
	if(true == m_pOwnerBodyPart->IsAnimationEnd())
	{
		RELEASE_INSTANCE(CGameInstance);
		return STATE::LOCK_IDLE;
	}
	

	RELEASE_INSTANCE(CGameInstance);

	return m_eState;
}

CState_Lockon_ParrySuccess* CState_Lockon_ParrySuccess::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CStateMachine* pOwner, STATE eState)
{
	CState_Lockon_ParrySuccess* pInstance = new CState_Lockon_ParrySuccess(pDevice, pContext, pOwner, eState);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CState_Lockon_ParrySuccess");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Lockon_ParrySuccess::Free()
{
	__super::Free();
}