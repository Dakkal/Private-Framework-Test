#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode,
		_uint iWinCX, _uint iWinCY, _Inout_ ID3D11Device** ppDeviceOut,
		_Inout_ ID3D11DeviceContext** ppDeviceContextOut);

	HRESULT Clear_BackBuffer_View(_vector vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
	IDXGISwapChain* m_pSwapChain = nullptr;

	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;




private:
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END