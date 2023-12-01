#include "..\Public\VIBuffer_Instancing.h"
#include "GameInstance.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Instancing::CVIBuffer_Instancing(CGameObject* pOwner, const CVIBuffer_Instancing& rhs)
	: CVIBuffer(pOwner, rhs)
	, m_iStrideInstance(rhs.m_iStrideInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iNumIndicesPerInstance(rhs.m_iNumIndicesPerInstance)
	, m_pVertices(rhs.m_pVertices)
	, m_pSpeeds(rhs.m_pSpeeds)
	, m_pLifeTimes(rhs.m_pLifeTimes)
	, m_pTimeAccs(rhs.m_pTimeAccs)
	, m_iLifeCnt(rhs.m_iLifeCnt)
	, m_pIsLifeOver(rhs.m_pIsLifeOver)
{

}

HRESULT CVIBuffer_Instancing::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	INSTANCE_DESC InstanceDesc;
	memmove(&InstanceDesc, pArg, sizeof(InstanceDesc));


	m_iStrideInstance = sizeof(VTXINSTANCE);

	/* �������ۿ� �ε��� ���۸� ���峮. */
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	// m_BufferDesc.ByteWidth = �����ϳ��� ũ��(Byte) * ������ ����;
	m_tBufferDesc.ByteWidth = m_iStrideInstance * m_iNumInstance;
	m_tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* �������۷� �Ҵ��Ѵ�. (Lock, unLock ȣ�� �Ұ�)*/
	m_tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_tBufferDesc.MiscFlags = 0;
	m_tBufferDesc.StructureByteStride = m_iStrideInstance;

	m_pVertices = new VTXINSTANCE[m_iNumInstance];
	ZeroMemory(m_pVertices, sizeof(VTXINSTANCE) * m_iNumInstance);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_iLifeCnt = InstanceDesc.iNumInstance;
	m_pSpeeds = new _float[InstanceDesc.iNumInstance];
	m_pLifeTimes = new _float[InstanceDesc.iNumInstance];
	m_pTimeAccs = new _float[InstanceDesc.iNumInstance];
	m_pIsLifeOver = new _bool[InstanceDesc.iNumInstance];


	for (size_t i = 0; i < InstanceDesc.iNumInstance; i++)
	{
		m_pIsLifeOver[i] = false;
		m_pSpeeds[i] = pGameInstance->Random_Float(InstanceDesc.fSpeedMin, InstanceDesc.fSpeedMax);;
		m_pLifeTimes[i] = pGameInstance->Random_Float(InstanceDesc.fLifeTimeMin, InstanceDesc.fLifeTimeMax);
		m_pTimeAccs[i] = 0.f;
	}


	/* ���� ���� �ʱ�ȭ.  */
	RELEASE_INSTANCE(CGameInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float		fScaleX = pGameInstance->Random_Float(InstanceDesc.vScaleMin.x, InstanceDesc.vScaleMax.x);
		_float		fScaleY = pGameInstance->Random_Float(InstanceDesc.vScaleMin.y, InstanceDesc.vScaleMax.y);
		_float		fScaleZ = pGameInstance->Random_Float(InstanceDesc.vScaleMin.z, InstanceDesc.vScaleMax.z);

		_matrix matRot = XMMatrixRotationAxis(_vector(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(45.f));

		m_pVertices[i].vRight = _vector(fScaleX, 0.f, 0.f, 0.f);
		m_pVertices[i].vUp = _vector(0.f, fScaleY, 0.f, 0.f);
		m_pVertices[i].vLook = _vector(0.f, 0.f, fScaleZ, 0.f);


		m_pVertices[i].vTranslation = _vector(
			InstanceDesc.vCenter.x + pGameInstance->Random_Float(InstanceDesc.vRange.x * -InstanceDesc.fRangeOffset, InstanceDesc.vRange.x * InstanceDesc.fRangeOffset),
			InstanceDesc.vCenter.y + pGameInstance->Random_Float(InstanceDesc.vRange.y * -InstanceDesc.fRangeOffset, InstanceDesc.vRange.y * InstanceDesc.fRangeOffset),
			InstanceDesc.vCenter.z + pGameInstance->Random_Float(InstanceDesc.vRange.z * -InstanceDesc.fRangeOffset, InstanceDesc.vRange.z * InstanceDesc.fRangeOffset),
			1.f);
	}

	ZeroMemory(&m_tInitialData, sizeof m_tInitialData);
	m_tInitialData.pSysMem = m_pVertices;


	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{


	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint			iStrides[] = {
		m_iStride,
		m_iStrideInstance,

	};

	_uint			iOffsets[] = {
		0,
		0,
	};

	/* ���ؽ� ���۵��� �Ҵ��Ѵ�. */
	/* �׸���� �������� + ���º�ȯ�� �������� */
	m_pContext->IASetVertexBuffers(0, m_iNumVBs, pVertexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� �Ҵ��Ѵ�. */
	/* �׸����� �ϴ� �ν��Ͻ��� ������ŭ Ȯ��Ǿ��ִ� �ε��� ����. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* �ش� �������� � ������� �׸�����. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* �ε����� ����Ű�� ������ Ȱ���Ͽ� �׸���. */
	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (false == m_IsCloned)
	{
		Safe_Delete_Array(m_pIsLifeOver);
		Safe_Delete_Array(m_pTimeAccs);
		Safe_Delete_Array(m_pLifeTimes);
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pVertices);
	}

	Safe_Release(m_pVBInstance);
}
