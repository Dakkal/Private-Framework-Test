#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_FRUSTUM, TYPE_END };
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(class CGameObject* pOwner, const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);

public:
	void					Update(_matrix TransformMatrix);
	void					LateUpdate();
	_bool					IsCollision(CCollider * pTargetCollider);
	void					Set_Colli(_bool _IsColli);
	const TYPE&				Get_ColliderType() const { return m_eColliderType; }
	class CBounding*		Get_ParentBouning() const { return m_pBounding; }

	_bool			Is_Colli() const { return (0 < m_iNumbCol) ? true : false; }
	const _uint&	Get_ID() const { return m_iColID; }

	_vector					Get_ColWorldPos();

	void			OnCollision_Enter(CGameObject* _pColObj, _float fTimedelta);
	void			OnCollision_Stay(CGameObject* _pColObj, _float fTimedelta);
	void			OnCollision_Exit(CGameObject* _pColObj, _float fTimedelta);

#ifdef _DEBUG
public:
	virtual HRESULT Render();

private:
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect*		m_pEffect = { nullptr };
	ID3D11InputLayout*  m_pInputLayout = { nullptr };
	ID3D11DepthStencilState* m_pDSState = { nullptr };
#endif

private:
	TYPE			 m_eColliderType = { TYPE_END };
	class CBounding* m_pBounding = { nullptr };

	_uint			 m_iColID = { 0 };
	_uint			 m_iNumbCol = { 0 };

	static _uint		g_iNextID;

public:
	static CCollider* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eColliderType);
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg) override;
	virtual void Free() override;
};

END

