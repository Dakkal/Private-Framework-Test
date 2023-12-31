#pragma once
#include "VIBuffer.h"
#include "BinModel.h"

BEGIN(Engine)

class CBinMesh final : public CVIBuffer
{
private:
	CBinMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBinMesh(class CGameObject* pOwner, const CBinMesh& rhs);
	virtual ~CBinMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(CBinModel::TYPE eModelType, SAVE_MESHINFO_STATIC tStaticMesh, _matrix PivotMatrix);
	virtual HRESULT Initialize_Prototype(CBinModel::TYPE eModelType, SAVE_MESHINFO_DYNAMIC tDynamicMesh, _matrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT	Bind_BoneMatrices(class CShader* pShader, const vector<class CBinBone*>& Bones, const char* pConstantName, _matrix PivotMatrix);

public:
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }
	string  Get_MeshName() const { return m_strName; }
	_bool	Get_RenderState() const { return m_bIsRender; }
	XMFLOAT4X4 Get_OffsetMatrix(const _int& BoneIndex) const { return m_OffsetMatrices[BoneIndex]; }
public:
	void	Set_RenderState(_bool IsRender) { m_bIsRender = IsRender; }

private:
	string				m_strName = "";
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 };
	vector<_int>		m_Bones;
	vector<XMFLOAT4X4>	m_OffsetMatrices;

	_bool				m_bIsRender = { true };

public:
	static CBinMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBinModel::TYPE eModelType, const SAVE_MESHINFO_STATIC tStaicMesh, _matrix PivotMatrix);
	static CBinMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBinModel::TYPE eModelType, const SAVE_MESHINFO_DYNAMIC tDynamicMesh, _matrix PivotMatrix);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END