#pragma once
#include "Base.h"
#include "FbxExporter.h"

BEGIN(Engine)

class CBinAnimation final : public CBase
{
private:
	CBinAnimation();
	CBinAnimation(const CBinAnimation& rhs);
	virtual ~CBinAnimation() = default;

public:
	HRESULT		Initialize(const class CBinModel* pModel, const SAVE_ANIM_INFO tAnimDesc);
	void		Update_TransformationMatrix(vector<class CBinBone*>& Bones, _float fTimeDelta);
	void		Reset();

public:
	void Set_Loop(_bool isLoop) { m_isLoop = isLoop; }

private:
	_float						m_fDuration = { 0.f };
	_float						m_fTickPerSecond = { 0.f };
	_float						m_fTrackPosition = { 0.f };

	_bool						m_isFinished = { false };
	_bool						m_isLoop = { false };

	string						m_strName = "";
	_uint						m_iNumChannels = { 0 };
	vector<class CBinChannel*>		m_Channels;
	vector<_uint>				m_iCurKeyFrames;

public:
	static CBinAnimation* Create(const class CBinModel* pModel, const SAVE_ANIM_INFO tAnimDesc);
	CBinAnimation* Clone();
	virtual void Free() override;
};

END