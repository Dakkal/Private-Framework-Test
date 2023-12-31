#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_iCloneIndex(rhs.m_iCloneIndex)
	, m_strObjectName(rhs.m_strObjectName)
	, m_strProtoTag(rhs.m_strProtoTag)
	, m_eObjType(rhs.m_eObjType)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype(const wstring& strProtoTag)
{
	m_strProtoTag = strProtoTag;

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_IsCloned = true;
	m_iIndex = m_iCloneIndex;

	return S_OK;
}

void CGameObject::Enter_Object()
{
}

void CGameObject::PriorityTick(_float fTimeDelta)
{
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

void CGameObject::Exit_Object()
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& strPrototypeRag, const wstring& strComponentTag, _Inout_ CComponent** ppOut, void* pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, strPrototypeRag, this, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;
	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& strComponentTag)
{
	auto iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

CGameObject* CGameObject::Clone(void* pArg)
{
 	m_iCloneIndex++;

	return nullptr;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
