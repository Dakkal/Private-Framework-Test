#pragma once

#include "Base.h"
#include "Camera.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Manager)
	
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT		 Reserve_Manager(_uint iLevelIndex);
	HRESULT		 Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT		 Add_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& strPrototypeTag, void* pArg);
	HRESULT		 Direct_Add_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, class CGameObject* pObject);
	class CGameObject* Clone_GameObject(const wstring& strPrototypeTag, void* pArg);
	void		 Enter_Objects(_uint iLevelIndex, const _uint& iLayerIndex);
	void		 PriorityTick(_float fTimeDelta);
	void		 Tick(_float fTimeDelta);
	void		 LateTick(_float fTimeDelta);
	void		 Exit_Objects(_uint iLevelIndex, const _uint& iLayerIndex);
	void		 Clear(_uint iLevelIndex);

	CGameObject* Find_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& ObjName, _uint iIndex);
	CGameObject* Last_GameObject(_uint iLevelIndex, const _uint& iLayerIndex);
	HRESULT		 Delete_GameObject(_uint iLevelIndex, const _uint& iLayerIndex, const wstring& ObjName, _uint iIndex);
	HRESULT		 Delete_Layer(_uint iLevelIndex, const _uint& iLayerIndex);
	HRESULT		 Delete_NonActive_Objects(_uint iLevelIndex, const _uint& iLayerIndex);

	CGameObject* Get_Player(_uint iLevelIndex);

	const auto*		Get_Prototypes()	{ return &m_mapPrototypes; }
	list<class CGameObject*>*		Get_LayerList(_uint iLevelIndex, const _uint& iLayerIndex);

private:
	map<const wstring, class CGameObject*>	m_mapPrototypes;

private:
	_uint									m_iLevelIndex = { 0 };
	typedef map<const _uint, class CLayer*> LAYERS;
	LAYERS* m_pLayers = { nullptr };


private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const _uint& iLayerIndex);


public:
	virtual void Free() override;

};

END