#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:
	typedef struct tagTerrain_Desc 
	{
		_int			iNumVerticesX;
		_int			iNumVerticesZ;
		_bool			bIsWireFrame;

	}TERRAIN_DESC;

private:
	CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, class CGameObject* pOwner);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring & strHeightMapFilePath);
	virtual HRESULT Initialize(void* pArg) override;

	TERRAIN_DESC	Get_TerrainDesc() const { return m_tTerrainDesc; }

private:
	TERRAIN_DESC	m_tTerrainDesc;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, class CGameObject* pOwner, const wstring & strHeightMapFilePath = TEXT(""));
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END