#include "pch.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "Level_Edit.h"
#include "Level_1.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevel)
{
	if (FAILED(Ready_Layer_Loading(LAYER_BACKGROUND)))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Loading::Tick(_float fTimeDelta)
{


	return S_OK;
}

HRESULT CLevel_Loading::LateTick(_float fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Key_Down(VK_RETURN))
	{
		if (true == m_pLoader->Get_Finished())
		{
			

			CLevel* pNewLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_1:
				pNewLevel = CLevel_1::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_EDIT:
				pNewLevel = CLevel_Edit::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return E_FAIL;

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				return E_FAIL;

			
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Loading(const _uint& iLayerIndex)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, iLayerIndex, TEXT("Prototype_GameObject_Loading"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_LOADING, iLayerIndex, TEXT("Prototype_GameObject_LoadingSymbol"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
