#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(class CGameObject* pOwner, const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

#ifdef _DEBUG
	virtual HRESULT Render() { return S_OK; }
#endif

public:
	CGameObject* Get_Owner() const { return m_pOwner; }

public:
	void			Set_Active(_bool bActive) { m_bActive = bActive; }
	const _bool&	Is_Active() const { return m_bActive; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameObject*		m_pOwner = { nullptr };

	_bool		m_IsCloned = { false };
	_bool		m_bActive = { true };

public:
	virtual CComponent* Clone(class CGameObject* pOwner, void* pArg) = 0;
	virtual void Free() override;
};

END

