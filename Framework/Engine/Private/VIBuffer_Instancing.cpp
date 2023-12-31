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

	/* 정점버퍼와 인덱스 버퍼를 만드낟. */
	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);

	// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
	m_tBufferDesc.ByteWidth = m_iStrideInstance * m_iNumInstance;
	m_tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 동적버퍼로 할당 */
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

	RELEASE_INSTANCE(CGameInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float		fScaleX = pGameInstance->Random_Float(InstanceDesc.vScaleMin.x, InstanceDesc.vScaleMax.x);
		_float		fScaleY = pGameInstance->Random_Float(InstanceDesc.vScaleMin.y, InstanceDesc.vScaleMax.y);
		_float		fScaleZ = pGameInstance->Random_Float(InstanceDesc.vScaleMin.z, InstanceDesc.vScaleMax.z);

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

	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, m_iNumVBs, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pIsLifeOver);
	Safe_Delete_Array(m_pTimeAccs);
	Safe_Delete_Array(m_pLifeTimes);
	Safe_Delete_Array(m_pSpeeds);
	Safe_Delete_Array(m_pVertices);

	Safe_Release(m_pVBInstance);
}
