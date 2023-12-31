#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT Enter_Level();
	virtual HRESULT PriorityTick(_float fTimeDelta);
	virtual HRESULT Tick(_float fTimeDelta);
	virtual HRESULT LateTick(_float fTimeDelta);
	virtual HRESULT Exit_Level();

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

public:
	virtual void Free() override;

};

END