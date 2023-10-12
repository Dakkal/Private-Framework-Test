#pragma once
#include "Base.h"

BEGIN(Engine)

class CCalculator final : public CBase
{
	DECLARE_SINGLETON(CCalculator)

private:
	CCalculator();
	virtual ~CCalculator() = default;

public:
	_vector		Picking_Terrain(RECT rc, POINT pt, class CTransform* pTransform,class CVIBuffer* pBuffer);
	_vector		Picking_Object(RECT rc, POINT pt, class CTransform* pTransform, class CVIBuffer* pBuffer);

public:
	virtual void Free() override;
};

END