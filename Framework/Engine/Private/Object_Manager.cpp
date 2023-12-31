#include "..\Public\Object_Manager.h"
#include "GameObject.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

HRESULT CObject_Manager::Reserve_Manager(_uint iLevelIndex)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iLevelIndex];

	m_iLevelIndex = iLevelIndex;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_mapPrototypes.insert({ strPrototypeTag, pPrototype });

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(iLayerIndex, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);


	return S_OK;
}

HRESULT CObject_Manager::Direct_Add_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, CGameObject* pObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pObject);

		m_pLayers[iLevelIndex].emplace(iLayerIndex, pLayer);
	}
	else
		pLayer->Add_GameObject(pObject);


	return S_OK;
}

CGameObject* CObject_Manager::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	CGameObject* pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

void CObject_Manager::Enter_Objects(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return;

	return iter->Enter_Layer();
}

void CObject_Manager::PriorityTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelIndex; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->PriorityTick(fTimeDelta);
		}
	}
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelIndex; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);
		}
	}

}

void CObject_Manager::LateTick(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iLevelIndex; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateTick(fTimeDelta);
		}
	}
}

void CObject_Manager::Exit_Objects(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return;

	return iter->Exit_Layer();
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();

}

CGameObject* CObject_Manager::Find_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& ObjName, _uint iIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return nullptr;

	return iter->Find_GameObject(ObjName, iIndex);
}

CGameObject* CObject_Manager::Last_GameObject(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return nullptr;

	
	return iter->Last_GameObject();
}

HRESULT CObject_Manager::Delete_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& ObjName, _uint iIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return E_FAIL;

	return iter->Delete_GameObject(ObjName, iIndex);
}

HRESULT CObject_Manager::Delete_Layer(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return E_FAIL;

	for (auto& mapiter : m_mapPrototypes)
	{
		mapiter.second->Reset_CloneIndex();
	}

	return iter->Delete_Layer();
}

HRESULT CObject_Manager::Delete_NonActive_Objects(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return E_FAIL;

	return iter->Delete_NonActive_Objects();
}

CGameObject* CObject_Manager::Get_Player(_uint iLevelIndex)
{
	CGameObject* pPlayer = Last_GameObject(iLevelIndex, LAYER_PLAYER);
	if (nullptr == pPlayer)
		return nullptr;

	return pPlayer;
}

list<class CGameObject*>* CObject_Manager::Get_LayerList(_uint iLevelIndex, const _uint& iLayerIndex)
{
	auto iter = Find_Layer(iLevelIndex, iLayerIndex);
	if (nullptr == iter)
		return nullptr;

	return iter->Get_LayerList();
}

CGameObject* CObject_Manager::Find_Prototype(const wstring& strPrototypeTag)
{
	auto iter = m_mapPrototypes.find(strPrototypeTag);

	if (iter == m_mapPrototypes.end())
		return nullptr;

	return iter->second;
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _uint& iLayerIndex)
{
	if (iLevelIndex >= m_iLevelIndex)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(iLayerIndex);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iLevelIndex; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_mapPrototypes)
		Safe_Release(Pair.second);

	m_mapPrototypes.clear();
}
