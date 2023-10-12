#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Edit_Terrain.h"
#include "ToolCamera.h"
#include "ChurchGrillesFloor.h"
#include "Player.h"
#include "Monster.h"

_uint APIENTRY ThreadEntry(void* pArg)
{
	CoInitializeEx(nullptr, 0);


	CLoader* pLoader = reinterpret_cast<CLoader*>(pArg);

	pLoader->Loading();

	return 0;
}


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;


	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case Client::LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	case Client::LEVEL_1:
		hr = Loading_For_Level_1();
		break;
	case Client::LEVEL_EDIT:
		hr = Loading_For_Level_Edit();
		break;
	default:
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(&m_Critical_Section);
		return -1;
	}

	LeaveCriticalSection(&m_Critical_Section);

	return 0;
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();
	
		
	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_1()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_2()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_3()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_4()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Edit()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Texture()
{
	if (m_eNextLevel >= LEVEL_END)
		return E_FAIL;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;
		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Prototype_Component_Texture_Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Terrain_Mask */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_Mask"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.bmp"), 1))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Brush */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
			return E_FAIL;
		break;
	case Client::LEVEL_1:
		break;
	case Client::LEVEL_2:
		break;
	case Client::LEVEL_3:
		break;
	case Client::LEVEL_4:
		break;
	case Client::LEVEL_EDIT:
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Mesh()
{
	if (m_eNextLevel >= LEVEL_END)
		return E_FAIL;

	_matrix	ModelInitMatrix = XMMatrixIdentity();

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Proto_Terrain */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
			return E_FAIL;
		ModelInitMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", ModelInitMatrix))))
			return E_FAIL;
		break;
	case Client::LEVEL_1:
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_1, TEXT("Prototype_Component_VIBuffer_Edit_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_1, TEXT("Prototype_Component_Model_ChurchGrillesFloor"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Props/ChurchGrillesFloor/ChurchGrillesFloor.fbx", ModelInitMatrix))))
			return E_FAIL;
		break;
	case Client::LEVEL_2:
		break;
	case Client::LEVEL_3:
		break;
	case Client::LEVEL_4:
		break;
	case Client::LEVEL_EDIT:
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_VIBuffer_Edit_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_ChurchGrillesFloor"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Props/ChurchGrillesFloor/ChurchGrillesFloor.fbx", ModelInitMatrix))))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Shader()
{
	if (m_eNextLevel >= LEVEL_END)
		return E_FAIL;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Proto_VtxPosNorTex */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosNorTex.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Proto_VtxAnimMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::tElements, VTXANIMMESH::iNumElements))))
			return E_FAIL;
		break;
	case Client::LEVEL_1:
		/* For.Proto_VtxPosNorTex */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_1, TEXT("Prototype_Component_Shader_VtxPosNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosNorTex.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElements))))
			return E_FAIL;
		/* For.Proto_VtxMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_1, TEXT("Prototype_Component_Shader_VtxMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::tElements, VTXMESH::iNumElements))))
			return E_FAIL;
		break;
	case Client::LEVEL_2:
		break;
	case Client::LEVEL_3:
		break;
	case Client::LEVEL_4:
		break;
	case Client::LEVEL_EDIT:
		/* For.Proto_VtxPosNorTex */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Shader_VtxPosNorTex"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosNorTex.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElements))))
			return E_FAIL;
		/* For.Proto_VtxMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Shader_VtxMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::tElements, VTXMESH::iNumElements))))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Object()
{
	if (m_eNextLevel >= LEVEL_END)
		return E_FAIL;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		/* For.BackGround */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"), CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Camera*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"), CToolCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"), CMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		break;
	case Client::LEVEL_1:
		/* For.Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Edit_Terrain"), CEdit_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Camera*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera"), CToolCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Props_ChurchGrillesFloor */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchGrillesFloor"), CChurchGrillesFloor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchGrillesFloor")))))
			return E_FAIL;
		break;
	case Client::LEVEL_2:
		break;
	case Client::LEVEL_3:
		break;
	case Client::LEVEL_4:
		break;
	case Client::LEVEL_EDIT:
		/* For.Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Edit_Terrain"), CEdit_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Camera*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ToolCamera"), CToolCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Props_ChurchGrillesFloor */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchGrillesFloor"), CChurchGrillesFloor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchGrillesFloor")))))
			return E_FAIL;
		break;
	default:
		break;
	}
	

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
