#include "../Public/BinChannel.h"
#include "BinBone.h"


CBinChannel::CBinChannel()
{
}

HRESULT CBinChannel::Initialize(const CBinModel* pModel, const SAVE_CHANNEL tChannel)
{
	m_strName = tChannel.strChannelName;

	m_iBoneIndex = tChannel.iBoneIndex;

	m_iNumKeyFrames = tChannel.iNumKeyFrame;

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME		KeyFrame;
		ZeroMemory(&KeyFrame, sizeof KeyFrame);

		KeyFrame.fTime = tChannel.vecKeyFrames[i].fTime;
		KeyFrame.vScale = tChannel.vecKeyFrames[i].vScale;
		KeyFrame.vRotation = tChannel.vecKeyFrames[i].vRotation;
		KeyFrame.vTranslation = tChannel.vecKeyFrames[i].vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}
	return S_OK;
}

void CBinChannel::Update_TransformationMatrix(_uint* pCurKeyFrame, vector<class CBinBone*>& Bones, _float fTrackPosition)
{
	if (0.f == fTrackPosition)
		*pCurKeyFrame = 0;

	_float3		vScale;
	_vector		vRotation;
	_vector		vTranslation;

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	if (fTrackPosition >= LastKeyFrame.fTime)
	{
		*pCurKeyFrame = m_KeyFrames.size() - 1;
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vTranslation = LastKeyFrame.vTranslation;
	}
	else
	{
		while (fTrackPosition >= m_KeyFrames[*pCurKeyFrame + 1].fTime)
			++* pCurKeyFrame;

		_float	fRatio = (fTrackPosition - m_KeyFrames[*pCurKeyFrame].fTime) / (m_KeyFrames[*pCurKeyFrame + 1].fTime - m_KeyFrames[*pCurKeyFrame].fTime);

		_vector vSourScale = _vector(m_KeyFrames[*pCurKeyFrame].vScale);
		_vector vDestScale = _vector(m_KeyFrames[*pCurKeyFrame + 1].vScale);
		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);

		_vector vSourRotation = _vector(m_KeyFrames[*pCurKeyFrame].vRotation);
		_vector vDestRotation = _vector(m_KeyFrames[*pCurKeyFrame + 1].vRotation);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);

		_vector vSourTranslation = m_KeyFrames[*pCurKeyFrame].vTranslation;
		_vector vDestTranslation = m_KeyFrames[*pCurKeyFrame + 1].vTranslation;
		vTranslation = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
	}

	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, _vector(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_Transform(TransformationMatrix);
}

CBinChannel* CBinChannel::Create(const CBinModel* pModel, const SAVE_CHANNEL tChannel)
{
	CBinChannel* pInstance = new CBinChannel();

	if (FAILED(pInstance->Initialize(pModel, tChannel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinChannel::Free()
{
	__super::Free();

	m_KeyFrames.clear();
}