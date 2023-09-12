#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CImgui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)

private:
	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	HRESULT	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Tick(_float fTimeDelta);
	HRESULT LateTick(_float fTimeDelta);
	HRESULT Render();

public:
	/* ���⿡ ����� �Լ� ���� */

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };


public:
	virtual void Free() override;

};

END