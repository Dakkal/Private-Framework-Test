#include "Level.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

HRESULT CLevel::Enter_Level()
{
	return S_OK;
}

HRESULT CLevel::PriorityTick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel::LateTick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel::Exit_Level()
{
	return S_OK;
}

void CLevel::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
