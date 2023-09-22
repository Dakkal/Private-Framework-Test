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
	HRESULT	Ready_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	HRESULT Tick(_float fTimeDelta);
	HRESULT LateTick(_float fTimeDelta);
	HRESULT Render();

	void ImGuiStyles();

public:
	/* ���⿡ ����� �Լ� ���� */
	void Menu();
	void ToolBox();



	_bool	Is_MouseClickedGUI();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

private:
	/* �ͷ��ο� ���� */
	_int			m_iNumVerticesX = { 0 };
	_int			m_iNumVerticesZ = { 0 };
	_bool			m_bIsCreateTerrain = { false };

	/* GUI�� ���� */
	_bool			m_bFrameReady = { false };

	string			m_strCurLevel = { "Level : " };
	_int			m_iCurLevel = {0};


private:
	/* Terrain�� ����*/

public:
	virtual void Free() override;

};

END