#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
    CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGameObject(const CGameObject& rhs);
    virtual ~CGameObject() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

protected:
    ID3D11Device*           m_pDevice = { nullptr };
    ID3D11DeviceContext*    m_pContext = { nullptr };

protected:
    map<const wstring, class CComponent*>   m_Components;



protected:
    HRESULT Add_Component(_uint iLevelIndex, const wstring & strPrototypeRag, const wstring & strComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
    class CComponent* Find_Component(const wstring & strComponentTag);


public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;

};

END