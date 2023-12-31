#include "PartObject.h"

#include "GameInstance.h"
#include "BinBone.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CPartObject::CPartObject(const CPartObject& rhs)
	: CGameObject(rhs)
{
}

CBinBone* CPartObject::Get_SocketBonePtr(const char* pBoneName)
{
	if (nullptr == m_pModelCom)
		return nullptr;

	return m_pModelCom->Get_BonePtr(pBoneName);
}

_matrix CPartObject::Get_SocketPivotMatrix()
{
	if (nullptr == m_pModelCom)
		return _matrix();

	return m_pModelCom->Get_PivotMatrix();
}

_bool CPartObject::Is_AnimCurKeyFrame(_uint iIndex)
{
	return m_pModelCom->Is_CurAnimKeyFrame(iIndex);
}

_bool CPartObject::Is_AnimOverKeyFrame(_uint iIndex)
{
	return m_pModelCom->Is_OverAnimKeyFrame(iIndex);
}

const _int& CPartObject::Get_AnimationIndex()
{
	return m_pModelCom->Get_CurAnimIndex();
}

_uint CPartObject::Get_CurKeyFrameNumb()
{
	return m_pModelCom->Get_CurKeyFrameNumb();
}

_bool CPartObject::IsAnimationEnd()
{
	return m_pModelCom->Is_CurAnimFinished();
}

const _bool& CPartObject::IsAnimChange()
{
	return m_pModelCom->Is_AnimChange();
}

void CPartObject::Set_AnimationIndex(_bool isLoop, _uint iAnimIndex, _float fAnimSpeed, _bool bWantReset, _uint iStartKeyFrame, _float fRootAnimDist, _float fChangeDuration)
{
	m_pModelCom->Set_Animation(isLoop, iAnimIndex, fAnimSpeed, bWantReset, iStartKeyFrame, fRootAnimDist, fChangeDuration);
}

void CPartObject::Set_First_AnimationIndex(_bool isLoop, _uint iAnimIndex, _float fAnimSpeed)
{
	m_pModelCom->First_Set_Animation(isLoop, iAnimIndex, fAnimSpeed);
}

void CPartObject::Set_CurAnimSpeed(_float fAnimSpeed)
{
	m_pModelCom->Set_CurAnimSpeed(fAnimSpeed);
}

_vector CPartObject::Get_Collide_Center()
{
	return m_pColliderCom->Get_ParentBouning()->Get_ColWorldPos();
}

HRESULT CPartObject::Initialize_Prototype(const wstring& strProtoTag)
{
	__super::Initialize_Prototype(strProtoTag);

	return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (nullptr != pArg)
	{
		PART_DESC* pPartDesc = (PART_DESC*)pArg;
		m_pOwner = pPartDesc->pOwner;
		m_ePart = pPartDesc->ePart;
		m_pParentTransform = pPartDesc->pParentTransform;
		Safe_AddRef(m_pParentTransform);
		m_pSocketBone = pPartDesc->pSocketBone;
		m_pSocketBoneforPivot = pPartDesc->pSocketBoneforPivot;
		Safe_AddRef(m_pSocketBone);
		Safe_AddRef(m_pSocketBoneforPivot);

		m_SocketPivotMatrix = pPartDesc->SocketPivot;
	}

	return S_OK;
}

void CPartObject::Tick(_float fTimeDelta)
{
}

void CPartObject::LateTick(_float fTimeDelta)
{
}

HRESULT CPartObject::Render()
{
	return S_OK;
}

HRESULT CPartObject::Compute_RenderMatrix(_matrix ChildMatrix)
{
	m_WorldMatrix = ChildMatrix * m_pParentTransform->Get_WorldMatrix();

	return S_OK;
}

CGameObject* CPartObject::Clone(void* pArg)
{
	__super::Clone(pArg);

	return nullptr;
}

void CPartObject::Free()
{
	__super::Free();

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pSocketBoneforPivot);
	Safe_Release(m_pParentTransform);
}