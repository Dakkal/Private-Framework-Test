#include "pch.h"
#include "..\Public\Enemy_GreatSword.h"

#include "GameInstance.h"
#include "PartObject.h"
#include "StateMachine.h"
#include "State_Idle_GreatSword.h"
#include "State_Walk_GreatSword.h"
#include "State_Hit_Great_Sword.h"
#include "State_Attack_GreatSword.h"
#include "State_Avoid_GreatSword.h"
#include "State_Dead_GreatSword.h"
#include "State_Parry_GreatSword.h"

#include "Collider.h"
#include "Bounding_Sphere.h"


CEnemy_GreatSword::CEnemy_GreatSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLandObject(pDevice, pContext)
{

}

CEnemy_GreatSword::CEnemy_GreatSword(const CEnemy_GreatSword& rhs)
	: CLandObject(rhs)
{

}

HRESULT CEnemy_GreatSword::Initialize_Prototype(const wstring& strProtoTag)
{
	__super::Initialize_Prototype(strProtoTag);

	m_eObjType = OBJECT_TYPE::MONSTER;
	m_strObjectName = TEXT("EnemyGreatSword");

	return S_OK;
}

HRESULT CEnemy_GreatSword::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fDissolveDuraton = 4.f;

	return S_OK;
}

void CEnemy_GreatSword::Enter_Object()
{
	if (nullptr != m_pCurNavigationCom)
		m_pCurNavigationCom->IsIn(m_pTransformCom->Get_State(CTransform::STATE_POS));
}

void CEnemy_GreatSword::Tick(_float fTimeDelta)
{
	Out_Player(fTimeDelta);
	Look_Player(fTimeDelta);

	m_pStateMachineCom->Tick(fTimeDelta);

	if (true == m_IsDead)
	{
		if (false == m_bDissolveSound)
		{

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			pGameInstance->PlaySoundFile(TEXT("SlowMotion_01.ogg"), CHANNELID::CHANNEL_4, 0.25f);

			RELEASE_INSTANCE(CGameInstance);

			m_bDissolveSound = true;
		}
		
		m_fDissolveTime += fTimeDelta;

		if (m_fDissolveDuraton < m_fDissolveTime)
			m_IsActive = false;
	}
	/* 작동이 멈추면 리턴, 데드 스테이트 틱에서 꺼지니까 여기가 맞을거다 */
	if (false == m_IsActive)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (auto& iter : m_Parts)
	{
		if (nullptr != iter.second)
			iter.second->Tick(fTimeDelta);
	}

	if(false == g_EditMode)
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	RELEASE_INSTANCE(CGameInstance)
}

void CEnemy_GreatSword::LateTick(_float fTimeDelta)
{
	if (false == m_IsActive)
		return;

	if (true == m_IsHit) m_IsHit = false;

	m_pStateMachineCom->LateTick(fTimeDelta);

	for (auto& iter : m_Parts)
	{
		if (nullptr != iter.second)
			iter.second->LateTick(fTimeDelta);
	}

	if (false == g_EditMode)
		m_pColliderCom->LateUpdate();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (true == pGameInstance->IsIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POS), 2.f))
	{
#ifdef _DEBUG
		if (false == g_EditMode)
			m_pRendererCom->Add_Debug(m_pColliderCom);

		if (nullptr != m_pCurNavigationCom)
			m_pRendererCom->Add_Debug(m_pCurNavigationCom);
#endif
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RG_NONBLEND, this);

		m_IsCull = true;
	}
	else
		m_IsCull = false;

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CEnemy_GreatSword::Render()
{
	return S_OK;
}

void CEnemy_GreatSword::OnCollision_Enter(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PLAYER:
		Set_PlayerTransform();
		m_bIsOutPlayer = false;
		m_fReleaseTimeAcc = 0.f;
		break;
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
	case OBJECT_TYPE::PART:
		OnCollision_Part_Enter(_pColObj, fTimeDelta);
		break;
	}
}

void CEnemy_GreatSword::OnCollision_Stay(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PLAYER:
		break;
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
		/* For. Parts */
	case OBJECT_TYPE::PART:
		OnCollision_Part_Stay(_pColObj, fTimeDelta);
		break;
	}
}

void CEnemy_GreatSword::OnCollision_Exit(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PLAYER:
		m_bIsOutPlayer = true;
		break;
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
		/* For. Parts */
	case OBJECT_TYPE::PART:
		OnCollision_Part_Exit(_pColObj, fTimeDelta);
		break;
	}
}

void CEnemy_GreatSword::OnCollision_Part_Enter(CGameObject* _pColObj, _float fTimeDelta)
{
	CGameObject* pPartOwner = dynamic_cast<CPartObject*>(_pColObj)->Get_PartOwner();
	OBJECT_TYPE eOwnerType = pPartOwner->Get_ObjectType();
	CGameObject::PARTS ePart = dynamic_cast<CPartObject*>(_pColObj)->Get_Part_Index();

	switch (eOwnerType)
	{
	case OBJECT_TYPE::PLAYER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		}
	}
	break;
	case OBJECT_TYPE::PORP:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	case OBJECT_TYPE::MONSTER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	}
}

void CEnemy_GreatSword::OnCollision_Part_Stay(CGameObject* _pColObj, _float fTimeDelta)
{
	CGameObject* pPartOwner = dynamic_cast<CPartObject*>(_pColObj)->Get_PartOwner();
	OBJECT_TYPE eOwnerType = pPartOwner->Get_ObjectType();
	CGameObject::PARTS ePart = dynamic_cast<CPartObject*>(_pColObj)->Get_Part_Index();

	switch (eOwnerType)
	{
	case OBJECT_TYPE::PLAYER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		}
	}
	break;
	case OBJECT_TYPE::PORP:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	case OBJECT_TYPE::MONSTER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	}
}

void CEnemy_GreatSword::OnCollision_Part_Exit(CGameObject* _pColObj, _float fTimeDelta)
{
	CGameObject* pPartOwner = dynamic_cast<CPartObject*>(_pColObj)->Get_PartOwner();
	OBJECT_TYPE eOwnerType = pPartOwner->Get_ObjectType();
	CGameObject::PARTS ePart = dynamic_cast<CPartObject*>(_pColObj)->Get_Part_Index();

	switch (eOwnerType)
	{
	case OBJECT_TYPE::PLAYER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		}
	}
	break;
	case OBJECT_TYPE::PORP:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	case OBJECT_TYPE::MONSTER:
	{
		switch (ePart)
		{
		case Engine::CGameObject::BODY:
			break;
		case Engine::CGameObject::WEAPON_R:
			break;
		case Engine::CGameObject::WEAPON_L:
			break;
		case Engine::CGameObject::HITBOX:
			break;
		default:
			break;
		}
	}
	break;
	}
}

_float CEnemy_GreatSword::Get_PlayerDistance()
{
	if (nullptr == m_pPlayerTransform)
		return 99.f;

	_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POS);
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);

	_float fLength = _vector(vPlayerPos - vPos).Length();

	return fLength;
}

void CEnemy_GreatSword::Out_Player(_float fTimeDelta)
{
	if (false == m_bIsOutPlayer)
		return;

	m_fReleaseTimeAcc += fTimeDelta;
	if (5.f <= m_fReleaseTimeAcc)
	{
		m_pPlayerTransform = nullptr;
		m_fReleaseTimeAcc = 0.f;
		m_bIsOutPlayer = false;
		m_bIsLookPlayer = false;
	}
}

void CEnemy_GreatSword::Look_Player(_float fTimeDelta)
{
	if (nullptr == m_pPlayerTransform || false == m_bIsLookPlayer)
		return;

	_vector vDir = m_pPlayerTransform->Get_State(CTransform::STATE_POS) - m_pTransformCom->Get_State(CTransform::CTransform::STATE_POS);
	_float fDist = vDir.Length();

	if (50 >= fDist)
	{
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vDir.Normalize();
		vDir.y = 0.f;

		_float fRotMax = 20.f;
		_float fRotNormal = 15.f;

		_vector RotDir;

		if (3.f < acosf(vDir.Dot(vLook)))
		{
			RotDir = _vector::Lerp(vLook, vDir, fRotMax * fTimeDelta);
		}
		else
		{
			RotDir = _vector::Lerp(vLook, vDir, fRotNormal * fTimeDelta);
		}
		m_pTransformCom->Set_Look(RotDir);
	}

}

void CEnemy_GreatSword::Set_PlayerTransform()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	auto pObj = pGameInstance->Last_GameObject(LEVEL_GAMEPLAY, LAYER_PLAYER);
	if (nullptr == pObj)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	m_pPlayerTransform = dynamic_cast<CTransform*>(pObj->Get_Component(TEXT("Com_Transform")));

	RELEASE_INSTANCE(CGameInstance)
}

HRESULT CEnemy_GreatSword::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotRadianPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	if (true == g_EditMode)
	{
		/* Com_StateMachine*/
		if (FAILED(__super::Add_Component(LEVEL_EDIT, TEXT("Prototype_Component_StateMachine"),
			TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachineCom)))
			return E_FAIL;
	}
	else
	{
		/* Com_StateMachine*/
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_StateMachine"),
			TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachineCom)))
			return E_FAIL;

		/* Com_Navigation */
		CNavigation* pNavigation = { nullptr };
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Church_Navigation"),
			TEXT("Com_Navigation1"), (CComponent**)&pNavigation)))
			return E_FAIL;
		m_NavigationComs.push_back(pNavigation);

		m_pCurNavigationCom = pNavigation;

		/* Com_Collider_Sphere */
		CBounding_Sphere::BOUNDING_SPHERE_DESC	SphereDesc = {};
		SphereDesc.fRadius = 5.f;
		SphereDesc.vCenter = _float3(0.f, 0.01f, 0.f);
		SphereDesc.vCollideColor = _vector(1.f, 0.f, 0.f, 1.f);
		SphereDesc.vColor = _vector(1.f, 1.f, 0.f, 1.f);
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &SphereDesc)))
			return E_FAIL;
	}

	

	return S_OK;
}

HRESULT CEnemy_GreatSword::Ready_Parts()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	/* For.Part_Body */
	CPartObject::PART_DESC			PartDesc_Body;
	PartDesc_Body.pOwner = this;
	PartDesc_Body.ePart = PARTS::BODY;
	PartDesc_Body.pParentTransform = m_pTransformCom;

	pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Enemy_GreatSword_Body"), &PartDesc_Body);
	if (nullptr == pParts)
		return E_FAIL;
	m_Parts.emplace(CGameObject::PARTS::BODY, pParts);

	if (false == g_EditMode)
	{
		/* For.Part_Weapon */
		CPartObject::PART_DESC			PartDesc_Weapon;
		PartDesc_Weapon.pOwner = this;
		PartDesc_Weapon.ePart = PARTS::WEAPON_R;
		PartDesc_Weapon.pParentTransform = m_pTransformCom;
		PartDesc_Weapon.pSocketBone = dynamic_cast<CPartObject*>(m_Parts[PARTS::BODY])->Get_SocketBonePtr("weapon_r");
		PartDesc_Weapon.SocketPivot = dynamic_cast<CPartObject*>(m_Parts[PARTS::BODY])->Get_SocketPivotMatrix();

		pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Enemy_GreatSword_Weapon"), &PartDesc_Weapon);
		if (nullptr == pParts)
			return E_FAIL;
		m_Parts.emplace(CGameObject::PARTS::WEAPON_R, pParts);

		CPartObject::PART_DESC			PartDesc_HitBox;
		PartDesc_HitBox.pOwner = this;
		PartDesc_HitBox.ePart = PARTS::HITBOX;
		PartDesc_HitBox.pParentTransform = m_pTransformCom;

		pParts = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Enemy_GreatSword_HitBox"), &PartDesc_HitBox);
		if (nullptr == pParts)
			return E_FAIL;
		m_Parts.emplace(CGameObject::PARTS::HITBOX, pParts);
	}

	


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEnemy_GreatSword::Ready_State()
{
	CState* pState = CState_Idle_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::IDLE);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::IDLE, pState);

	pState = CState_Walk_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::WALK);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::WALK, pState);

	pState = CState_Hit_Great_Sword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::HIT);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::HIT, pState);

	pState = CState_Attack_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::ATTACK);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::ATTACK, pState);

	pState = CState_Avoid_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::AVOID);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::AVOID, pState);

	pState = CState_Dead_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::DEAD);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::DEAD, pState);

	pState = CState_Parry_GreatSword::Create(m_pDevice, m_pContext, m_pStateMachineCom, STATE::PARRY);
	if (nullptr == pState)
		return E_FAIL;
	m_pStateMachineCom->Add_State(STATE::PARRY, pState);


	m_pStateMachineCom->Set_State(STATE::IDLE);

	return S_OK;
}

CEnemy_GreatSword* CEnemy_GreatSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strProtoTag)
{
	CEnemy_GreatSword* pInstance = new CEnemy_GreatSword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strProtoTag)))
	{
		MSG_BOX("Failed to Created : CEnemy_GreatSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEnemy_GreatSword::Clone(void* pArg)
{
	__super::Clone(pArg);

	CEnemy_GreatSword* pInstance = new CEnemy_GreatSword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEnemy_GreatSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEnemy_GreatSword::Free()
{
	__super::Free();

	for (auto& iter : m_Parts)
		Safe_Release(iter.second);
	m_Parts.clear();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pStateMachineCom);
}