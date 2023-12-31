#include "pch.h"
#include "Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Edit_Terrain.h"
#include "ToolCamera.h"
#include "PlayerCamera.h"
#include "SkyDome.h"

#include "Church.h"
#include "ChurchGrillesFloor.h"
#include "ChurchWindowGriles.h"
#include "ChurchLeftDoor.h"
#include "ChurchRightDoor.h"
#include "ChurchSideDoor.h"
#include "Stair.h"
#include "BossRoom.h"
#include "Sub_Building.h"
#include "Sub_Door.h"
#include "DeadTree.h"
#include "Left_BossDoor.h"
#include "Left_Shaft.h"
#include "Right_Shaft.h"
#include "Right_BossDoor.h"
#include "Wall.h"
#include "WindowWall.h"
#include "WindowWall2.h"

#include "Player.h"
#include "Body_Player.h"
#include "Weapon_Player_Saber.h"
#include "Weapon_Player_Dagger.h"
#include "HitBox_Player.h"
#include "Seq_Camera_Player.h"

#include "Boss_Urd.h"
#include "Body_Boss_Urd.h"
#include "Weapon_Boss_Urd.h"
#include "HitBox_Boss_Urd.h"
#include "WeaponGara_Boss_Urd.h"
#include "Seq_Camera_Urd.h"

#include "Enemy_GreatSword.h"
#include "Body_GreatSword.h"
#include "HitBox_GreatSword.h"
#include "Weapon_GreatSword.h"

#include "Enemy_Halberd.h"
#include "Body_Halberd.h"
#include "Weapon_Halberd.h"
#include "HitBox_Halberd.h"

#include "Enemy_Shield.h"
#include "Body_Shield.h"
#include "HitBox_Shield.h"
#include "Weapon_Shield.h"
#include "Weapon_Sword.h"

#include "Enemy_TwinSword.h"
#include "Body_TwinSword.h"
#include "HitBox_TwinSword.h"
#include "Weapon_TwinSword.h"
#include "Weapon_TwinSword2.h"

#include "Npc_Kid.h"
#include "Body_Npc_Kid.h"

#include "ParrySpark.h"

#include "BinModel.h"

#include "Navigation.h"
#include "StateMachine.h"
#include "Collider.h"
#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Rect_Instance.h"

_bool	g_EditMode = false;

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

	/* For.Object */
	m_strLoading = TEXT("오브젝트를 생성 중 입니다.");
	Loading_Object();


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Edit()
{
	g_EditMode = true;

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	Loading_Texture();

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	Loading_Mesh();

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");
	Loading_Shader();

	/* For.Etc_Component */
	m_strLoading = TEXT("나머지 컴포넌트들을 로딩 중 입니다.");
	Loading_EtcComponent();

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

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Logo/GameLogo2.png"), 1))))
			return E_FAIL;

		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Prototype_Component_Texture_Dissolve */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Dissolve"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Dissolve/Dissolve.png"), 1))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Dissolve */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ParryFlare"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ParryFlare/ParryFlare.dds"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Key"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialog/TexUI_PuzzleIcon_Key02.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Back"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialog/TexUI_DialogueReplyWindowBackground.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Line"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Dialog/TexUI_PauseMenuBackground_05.png"), 1))))
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
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SkyDome"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/SkyDome/Sky.dat")))))
			return E_FAIL;

		/* For.PlayerMesh */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Player/Body/Player.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Weapon_Saber"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Player/Weapon/Weapon_Saber/Weapon_Player_Saber.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Weapon_Dagger"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Player/Weapon/Weapon_Dagger/Weapon_Player_Dagger.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.UrdMesh */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Urd_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Boss/Body/Boss_Urd.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Urd_Weapon"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Boss/Weapon/Boss_Weapon.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.GreatSwordMesh */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_GreatSword_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_GreatSword/Body/Enemy_GreatSword.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_GreatSword_Weapon"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Enemy_GreatSword/Weapon/GreatSword.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.Halberd */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_Halberd_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_Halberd/Body/Enemy_Halberd.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_Halberd_Weapon"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Enemy_Halberd/Weapon/Halberd.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.Shield & Sword */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_Shield_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_Shield/Enemy_Shield.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_TwinSword_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_TwinSword/Enemy_TwinSword.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_Shield_Weapon"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Enemy_Soldier/Shield/Shield.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Enemy_Sword_Weapon"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Enemy_Soldier/Sword/Sword.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Npc_Kid_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Kiddo/Kiddo.dat"), ModelInitMatrix))))
			return E_FAIL;


		/* For.PropMesh */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Church"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Church/Church.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BossRoom"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/BossRoom/BossRoom.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ChurchWindowGriles"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchWindowGriles/ChurchWindowGriles.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ChurchLeftDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchLeftDoor/ChurchLeftDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ChurchRightDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchRightDoor/ChurchRightDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ChurchSideDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchSideDoor/ChurchSideDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stair"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Stair/Stair.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sub_Door"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Sub_Door/Sub_Door.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sub_Building"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Sub_Building/SubBuilding.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Left_BossDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Left_BossDoor/LeftBossDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Left_Shaft"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Left_Shaft/LeftShaft.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Right_BossDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Right_BossDoor/RightBossDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Right_Shaft"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Right_Shaft/RightShaft.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DeadTree"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/DeadTree/DeadTree.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_WindowWall"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/WindowWall/WindowWall.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_WindowWall2"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/WindowWall2/WindowWall2.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Wall/Wall.dat"), ModelInitMatrix))))
			return E_FAIL;

		break;
	case Client::LEVEL_EDIT:
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_VIBuffer_Edit_Terrain"),
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Church"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Church/Church.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_BossRoom"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/BossRoom/BossRoom.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_ChurchWindowGriles"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchWindowGriles/ChurchWindowGriles.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_ChurchLeftDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchLeftDoor/ChurchLeftDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_ChurchRightDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchRightDoor/ChurchRightDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_ChurchSideDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/ChurchSideDoor/ChurchSideDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Stair"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Stair/Stair.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Sub_Building"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Sub_Building/SubBuilding.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Sub_Door"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Sub_Door/Sub_Door.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Left_BossDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Left_BossDoor/LeftBossDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Left_Shaft"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Left_Shaft/LeftShaft.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Right_BossDoor"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Right_BossDoor/RightBossDoor.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Right_Shaft"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Right_Shaft/RightShaft.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_DeadTree"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/DeadTree/DeadTree.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.GreatSwordMesh */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Enemy_GreatSword_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_GreatSword/Body/Enemy_GreatSword.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.Halberd */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Enemy_Halberd_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_Halberd/Body/Enemy_Halberd.dat"), ModelInitMatrix))))
			return E_FAIL;

		/* For.Shield & Sword */
		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Enemy_Shield_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_Shield/Enemy_Shield.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Enemy_TwinSword_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Enemy_TwinSword/Enemy_TwinSword.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_WindowWall"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/WindowWall/WindowWall.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_WindowWall2"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/WindowWall2/WindowWall2.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Wall"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_NONANIM, TEXT("../Bin/Resources/Models/Static/Props/Wall/Wall.dat"), ModelInitMatrix))))
			return E_FAIL;

		ModelInitMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Model_Npc_Kid_Body"),
			CBinModel::Create(m_pDevice, m_pContext, CBinModel::TYPE_ANIM, TEXT("../Bin/Resources/Models/Dynamic/Kiddo/Kiddo.dat"), ModelInitMatrix))))
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

		/* For.Proto_VtxMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::tElements, VTXMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Point_Instance */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Point_Instance"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Point_Instance.hlsl"), VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Point_Instance */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Rect_Instance"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Rect_Instance.hlsl"), VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements))))
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

		/* For.Proto_VtxAnimMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::tElements, VTXANIMMESH::iNumElements))))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CLoader::Loading_EtcComponent()
{
	if (m_eNextLevel >= LEVEL_END)
		return E_FAIL;

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_GAMEPLAY:
		/* For.Prototype_Component_Collider_AABB */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
			return E_FAIL;

		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Frustrum"),
			CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_FRUSTUM))))
			return E_FAIL;

		/* For.Prototype_Component_Navigation */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Church_Navigation"),
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/Church.dat")))))
			return E_FAIL;

		/* For.Prototype_Component_Navigation */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Stair_Navigation"),
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/Stair.dat")))))
			return E_FAIL;

		/* For.Prototype_Component_Navigation */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_BossRoom_Navigation"),
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/BossRoom.dat")))))
			return E_FAIL;

		/* For.Prototype_Component_Navigation */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_SubBuilding_Navigation"),
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/SubBuilding.dat")))))
			return E_FAIL;

		/* For.Prototype_Component_StateMachine*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_StateMachine"),
			CStateMachine::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_Point_Instance */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instance"),
			CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instance"),
			CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext))))
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
		/* For.Prototype_Component_StateMachine*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_EDIT, TEXT("Prototype_Component_StateMachine"),
			CStateMachine::Create(m_pDevice, m_pContext))))
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
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerCamera"), CPlayerCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* For.Sky */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkyDome"), CSkyDome::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_SkyDome")))))
			return E_FAIL;

		/* For.LandingObject */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"), CPlayer::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Body"), CBody_Player::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Weapon_Saber"), CWeapon_Player_Saber::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player_Weapon_Saber")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Weapon_Dagger"), CWeapon_Player_Dagger::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player_Weapon_Dagger")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_HitBox"), CHitBox_Player::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player_HitBox")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_SeqCamera"), CSeq_Camera_Player::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Player_SeqCamera")))))
			return E_FAIL;

		/* 우르드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd"), CBoss_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd_Body"), CBody_Boss_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd_Weapon"), CWeapon_Boss_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd_WeaponGara"), CWeaponGara_Boss_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd_WeaponGara")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd_HitBox"), CHitBox_Boss_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd_HitBox")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Urd_SeqCamera"), CSeq_Camera_Urd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Boss_Urd_SeqCamera")))))
			return E_FAIL;

		/* 키드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_Kid"), CNpc_Kid::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Npc_Kid")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_Kid_Body"), CBody_Npc_Kid::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Npc_Kid_Body")))))
			return E_FAIL;

		/* 그소 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword"), CEnemy_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword_Body"), CBody_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword_Weapon"), CWeapon_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword_HitBox"), CHitBox_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword_HitBox")))))
			return E_FAIL;

		/* 할버드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd"), CEnemy_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd_Body"), CBody_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd_Weapon"), CWeapon_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd_HitBox"), CHitBox_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd_HitBox")))))
			return E_FAIL;

		/* 쉴드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield"), CEnemy_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield_Body"), CBody_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield_Weapon"), CWeapon_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Sword_Weapon"), CWeapon_Sword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Sword_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield_HitBox"), CHitBox_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield_HitBox")))))
			return E_FAIL;

		/* 트윈소드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword"), CEnemy_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword_Body"), CBody_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword_Weapon"), CWeapon_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword2_Weapon"), CWeapon_TwinSword2::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword2_Weapon")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword_HitBox"), CHitBox_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword_HitBox")))))
			return E_FAIL;

		/* For.LandObject */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Church"), CChurch::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Church")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossRoom"), CBossRoom::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_BossRoom")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair"), CStair::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Stair")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sub_Building"), CSub_Building::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Sub_Building")))))
			return E_FAIL;

		/* For.DecoObject */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchWindowGriles"), CChurchWindowGriles::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchWindowGriles")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchLeftDoor"), CChurchLeftDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchLeftDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchRightDoor"), CChurchRightDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchRightDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchSideDoor"), CChurchSideDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchSideDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sub_Door"), CSub_Door::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Sub_Door")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Left_BossDoor"), CLeft_BossDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Left_BossDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Left_Shaft"), CLeft_Shaft::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Left_Shaft")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Right_BossDoor"), CRight_BossDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Right_BossDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Right_Shaft"), CRight_Shaft::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Right_Shaft")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DeadTree"), CDeadTree::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_DeadTree")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"), CWall::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Wall")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowWall"), CWindowWall::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_WindowWall")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowWall2"), CWindowWall2::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_WindowWall2")))))
			return E_FAIL;

		/* For.Effect */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ParrySpark"), CParrySpark::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ParrySpark")))))
			return E_FAIL;

		break;
	case Client::LEVEL_EDIT:
		/* For.Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Edit_Terrain"), CEdit_Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		/* For.Camera*/
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ToolCamera"), CToolCamera::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Church"), CChurch::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Church")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossRoom"), CBossRoom::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_BossRoom")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchWindowGriles"), CChurchWindowGriles::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchWindowGriles")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchLeftDoor"), CChurchLeftDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchLeftDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchRightDoor"), CChurchRightDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchRightDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChurchSideDoor"), CChurchSideDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_ChurchSideDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stair"), CStair::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Stair")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sub_Building"), CSub_Building::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Sub_Building")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sub_Door"), CSub_Door::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Sub_Door")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Left_BossDoor"), CLeft_BossDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Left_BossDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Left_Shaft"), CLeft_Shaft::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Left_Shaft")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Right_BossDoor"), CRight_BossDoor::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Right_BossDoor")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Right_Shaft"), CRight_Shaft::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Right_Shaft")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DeadTree"), CDeadTree::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_DeadTree")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Wall"), CWall::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Wall")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowWall"), CWindowWall::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_WindowWall")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WindowWall2"), CWindowWall2::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_WindowWall2")))))
			return E_FAIL;

		/* 그소 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword"), CEnemy_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_GreatSword_Body"), CBody_GreatSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_GreatSword_Body")))))
			return E_FAIL;

		/* 할버드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd"), CEnemy_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Halberd_Body"), CBody_Halberd::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Halberd_Body")))))
			return E_FAIL;

		/* 쉴드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield"), CEnemy_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_Shield_Body"), CBody_Shield::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_Shield_Body")))))
			return E_FAIL;

		/* 트윈소드 */
		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword"), CEnemy_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Enemy_TwinSword_Body"), CBody_TwinSword::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Enemy_TwinSword_Body")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_Kid"), CNpc_Kid::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Npc_Kid")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_Kid_Body"), CBody_Npc_Kid::Create(m_pDevice, m_pContext, TEXT("Prototype_GameObject_Npc_Kid_Body")))))
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
