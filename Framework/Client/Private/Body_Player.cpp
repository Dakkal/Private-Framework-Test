#include "pch.h"
#include "..\Public\Body_Player.h"

#include "GameInstance.h"
#include "BinMesh.h"
#include "LandObject.h"
#include "Collider.h"
#include "Bounding_AABB.h"
#include "PartObject.h"
#include "PipeLine.h"
#include "Bounding.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext)
{

}

CBody_Player::CBody_Player(const CBody_Player& rhs)
	: CPartObject(rhs)
{

}

HRESULT CBody_Player::Initialize_Prototype(const wstring& strProtoTag)
{
	__super::Initialize_Prototype(strProtoTag);

	m_eObjType = OBJECT_TYPE::PART;
	m_strObjectName = TEXT("Player_Body");

	return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBody_Player::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	m_pModelCom->Set_OwnerPosToRootPos(m_pParentTransform, fTimeDelta, dynamic_cast<CLandObject*>(m_pOwner)->Get_CurNaviCom());
	dynamic_cast<CLandObject*>(m_pOwner)->Set_On_NaviMesh(m_pParentTransform);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix());

	/* 계산한뒤에 월드를 업데이트 */
	m_pColliderCom->Update(m_WorldMatrix);
}

void CBody_Player::LateTick(_float fTimeDelta)
{
	for (auto& pMesh : m_pModelCom->Get_Meshes())
	{
		string Name = pMesh->Get_MeshName();

		if (Name == "Player_Corvus.Raven")
			pMesh->Set_RenderState(false);
		if (Name == "Player_Corvus.Cloak")
			pMesh->Set_RenderState(false);
	}

	m_pColliderCom->LateUpdate();
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RG_BLEND, this);
}

HRESULT CBody_Player::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif

	return S_OK;
}

void CBody_Player::OnCollision_Enter(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
	case OBJECT_TYPE::BOSS:
		break;
	case OBJECT_TYPE::PART:
		if (PARTS::BODY == dynamic_cast<CPartObject*>(_pColObj)->Get_Part_Index())
		{
			CCollider* pCollider = dynamic_cast<CCollider*>(_pColObj->Get_Component(TEXT("Com_Collider")));

			_float3 vTargetCenter = dynamic_cast<CBounding_AABB*>(pCollider->Get_ParentBouning())->Get_Bouding()->Center;
			_float3 vPlayerCenter = dynamic_cast<CBounding_AABB*>(m_pColliderCom->Get_ParentBouning())->Get_Bouding()->Center;
			_float3 vFinalCenter = vPlayerCenter - vTargetCenter;

			_float3 vTargetExtents = dynamic_cast<CBounding_AABB*>(pCollider->Get_ParentBouning())->Get_Bouding()->Extents;
			_float3 vPlayerExtents = dynamic_cast<CBounding_AABB*>(m_pColliderCom->Get_ParentBouning())->Get_Bouding()->Extents;
			_float3 vFinalExtents = 0.5f * _float3(::fabs(vFinalCenter.x), ::fabs(vFinalCenter.y), ::fabs(vFinalCenter.z));

			if (vFinalExtents.x >= vFinalExtents.y && vFinalExtents.x >= vFinalExtents.z)
			{
				// 충돌이 X 축에서 발생.

				// TODO -> Left or Right
				if (vPlayerCenter.x > vTargetCenter.x)
				{
					_float vFinalExtents = fabs((vPlayerExtents.x + vTargetExtents.x)) - fabs(vFinalCenter.x);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE_POS);
					vPos.x += vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
				else
				{
					_float vFinalExtents = fabs((vPlayerExtents.x + vTargetExtents.x)) - fabs(vFinalCenter.x);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE::STATE_POS);
					vPos.x -= vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
			}
			else if (vFinalExtents.z >= vFinalExtents.y && vFinalExtents.z >= vFinalExtents.x)
			{
				// 충돌이 X 축에서 발생.

				// TODO -> Left or Right
				if (vPlayerCenter.z > vTargetCenter.z)
				{
					_float vFinalExtents = fabs((vPlayerExtents.z + vTargetExtents.z)) - fabs(vFinalCenter.z);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE_POS);
					vPos.z += vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
				else
				{
					_float vFinalExtents = fabs((vPlayerExtents.z + vTargetExtents.z)) - fabs(vFinalCenter.z);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE::STATE_POS);
					vPos.z -= vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
			}
		}
		break;
	default:
		break;
	}
}

void CBody_Player::OnCollision_Stay(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
	case OBJECT_TYPE::BOSS:
		break;
	case OBJECT_TYPE::PART:
		if (PARTS::BODY == dynamic_cast<CPartObject*>(_pColObj)->Get_Part_Index())
		{
			CCollider* pCollider = dynamic_cast<CCollider*>(_pColObj->Get_Component(TEXT("Com_Collider")));

			_float3 vTargetCenter = dynamic_cast<CBounding_AABB*>(pCollider->Get_ParentBouning())->Get_Bouding()->Center;
			_float3 vPlayerCenter = dynamic_cast<CBounding_AABB*>(m_pColliderCom->Get_ParentBouning())->Get_Bouding()->Center;
			_float3 vFinalCenter = vPlayerCenter - vTargetCenter;

			_float3 vTargetExtents = dynamic_cast<CBounding_AABB*>(pCollider->Get_ParentBouning())->Get_Bouding()->Extents;
			_float3 vPlayerExtents = dynamic_cast<CBounding_AABB*>(m_pColliderCom->Get_ParentBouning())->Get_Bouding()->Extents;
			_float3 vFinalExtents = 0.5f * _float3(::fabs(vFinalCenter.x), ::fabs(vFinalCenter.y), ::fabs(vFinalCenter.z));

			if (vFinalExtents.x >= vFinalExtents.y && vFinalExtents.x >= vFinalExtents.z)
			{
				// 충돌이 X 축에서 발생.

				// TODO -> Left or Right
				if (vPlayerCenter.x > vTargetCenter.x)
				{
					_float vFinalExtents = fabs((vPlayerExtents.x + vTargetExtents.x)) - fabs(vFinalCenter.x);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE_POS);
					vPos.x += vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
				else
				{
					_float vFinalExtents = fabs((vPlayerExtents.x + vTargetExtents.x)) - fabs(vFinalCenter.x);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE::STATE_POS);
					vPos.x -= vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
			}
			else if (vFinalExtents.z >= vFinalExtents.y && vFinalExtents.z >= vFinalExtents.x)
			{
				// 충돌이 X 축에서 발생.

				// TODO -> Left or Right
				if (vPlayerCenter.z > vTargetCenter.z)
				{
					_float vFinalExtents = fabs((vPlayerExtents.z + vTargetExtents.z)) - fabs(vFinalCenter.z);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE_POS);
					vPos.z += vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
				else
				{
					_float vFinalExtents = fabs((vPlayerExtents.z + vTargetExtents.z)) - fabs(vFinalCenter.z);

					Vec4 vPos = m_pParentTransform->Get_State(CTransform::STATE::STATE_POS);
					vPos.z -= vFinalExtents;

					m_pParentTransform->Set_State(CTransform::STATE::STATE_POS, vPos);
				}
			}
		}
		break;
	default:
		break;
	}
}

void CBody_Player::OnCollision_Exit(CGameObject* _pColObj, _float fTimeDelta)
{
	OBJECT_TYPE eObject = _pColObj->Get_ObjectType();

	switch (eObject)
	{
	case OBJECT_TYPE::PORP:
		break;
	case OBJECT_TYPE::MONSTER:
		break;
	case OBJECT_TYPE::BOSS:
		break;
	case OBJECT_TYPE::PART:
		break;
	default:
		break;
	}
}

HRESULT CBody_Player::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Body"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc;
	TransformDesc = m_pParentTransform->Get_TransformDesc();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Collider_AABB */
	CBounding_AABB::BOUNDING_AABB_DESC		AABBDesc = {};
	AABBDesc.vExtents = _float3(0.3f, 0.85f, 0.3f);
	AABBDesc.vCenter = _float3(0.0f, AABBDesc.vExtents.y + 0.01f, 0.f);
	AABBDesc.vCollideColor = _vector(1.f, 0.f, 0.f, 1.f);
	AABBDesc.vColor = _vector( 0.33f, 0.63f, 0.93f, 1.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), (CComponent**)&m_pColliderCom, &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_CamPosToShader(m_pShaderCom, "g_vCamPosition")))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	_uint		iPassIndex = 0;

	if (LIGHT_DESC::TYPE::DIRECTIONAL == pLightDesc->eLightType)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vLightDir, sizeof(_vector))))
			return E_FAIL;
		iPassIndex = 0;
	}
	else
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightPos", &pLightDesc->vLightPos, sizeof(_vector))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fLightRange", &pLightDesc->fLightRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 1;
	}


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_vector))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_vector))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_vector))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strProtoTag)
{
	CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strProtoTag)))
	{
		MSG_BOX("Failed to Created : CBody_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
	__super::Clone(pArg);

	CBody_Player* pInstance = new CBody_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Player::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
