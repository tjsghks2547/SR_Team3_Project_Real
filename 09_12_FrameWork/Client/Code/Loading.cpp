#include "pch.h"
#include "Loading.h"
#include "Export_Utility.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	,m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_eID = eLoading;

	//  스레드를 만드는 함수 _beginthreadx 
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
	
	return S_OK;
}

_uint CLoading::Loading_Stage()
{

	lstrcpy(m_szLoading, L"Etc Loading................");

	//ETC
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_State", Engine::CStateController::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoard", Engine::CPipeBoardCom::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pipe", Engine::CPipeCom::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoundBox", Engine::CBoundBox::Create(m_pGraphicDev)), E_FAIL);


	lstrcpy(m_szLoading, L"Buffer Loading................");
	//BUFFER 

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TestMapTex", Engine::CStartMap::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MapTex", Engine::CMapTex::Create(m_pGraphicDev, 1000, 1000, 1.f)), E_FAIL);
	

	lstrcpy(m_szLoading, L"Texture Loading................");
	//TEXTURE

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player1.jpg", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", TEX_CUBE, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Tutorial_Map.png", TEX_NORMAL)), E_FAIL);
	//UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HeartHPFrame_UI2.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPBar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HPBar.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HPBarBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HPBarBack.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CoinUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Coin_UI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KeyBoardUI", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/KeyBoardUI.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenBackGround.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenDefault", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenDefault.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_InvenFrame", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/InvenFrame.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EquipIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/equipment_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ConsumIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Consumption_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_OtherIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Other_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_QuestIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Quest_Icon.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_IconButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/IconButton.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EquipOnOffButton", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/EquipOnOff.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TextBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/TextBox.png", TEX_NORMAL)), E_FAIL);
	//Item
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExploreHat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/Explore.png", TEX_NORMAL)), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoardTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePuzzleFloor2Line.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleThreeWay.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleConer.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeCursorTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_UI_Cursor_Green.png", TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonkeyStatue", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Cave.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StoneBlock", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_StoneBlock.png", TEX_NORMAL)), E_FAIL);

	//TEST 높이 맵 
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainHeightTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Height2.bmp", TEX_NORMAL)), E_FAIL);

	Loading_AnimData();


	lstrcpy(m_szLoading, L"Loading Complete!!");

	m_bFinish = true; 

	return 0; 
}

_uint CLoading::Loading_AnimData()
{
	const TCHAR* pGetPath = L"../Bin/Resource/Texture/Ogu_Move_Motion/Animation.dat";
	HANDLE hFile = CreateFile(pGetPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD	dwByte(0);
	DWORD	dwStrByte(0);

	TCHAR* pObjectName;
	TCHAR* pImgFilePath;
	TCHAR* pImgFileName;

	TCHAR* pAnimationName;

	int imageSizeX, imageSizeY;

	list<TCHAR*> imgPathList;
	vector<_vec2> animFrameCount;
	vector<vector<int>*> animFramePlayList;

	while (true)
	{
		// Player
		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
		pObjectName = new TCHAR[dwStrByte / 2];
		ReadFile(hFile, pObjectName, dwStrByte, &dwByte, nullptr);

		int animCount = 0;
		ReadFile(hFile, &animCount, sizeof(int), &dwByte, nullptr);

		for (int i = 0; i < animCount; i++)
		{
			// png 주소
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			pImgFileName = new TCHAR[dwStrByte / 2];
			ReadFile(hFile, pImgFileName, dwStrByte, &dwByte, nullptr);

			ReadFile(hFile, &imageSizeX, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &imageSizeY, sizeof(int), &dwByte, nullptr);

			// Idle
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			pAnimationName = new TCHAR[dwStrByte / 2];
			ReadFile(hFile, pAnimationName, dwStrByte, &dwByte, nullptr);

			// 가로세로 이미지 개수
			int frameCountX, frameCountY;
			ReadFile(hFile, &frameCountX, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &frameCountY, sizeof(int), &dwByte, nullptr);

			// 5방향
			vector<int>* framePlayList = new vector<int>[5];
			for (int j = 0; j < 5; j++)
			{
				int frameCount = 0;
				ReadFile(hFile, &frameCount, sizeof(int), &dwByte, nullptr);

				int readFrame = 0;
				for (int k = 0; k < frameCount; k++)
				{
					ReadFile(hFile, &readFrame, sizeof(int), &dwByte, nullptr);
					framePlayList[j].push_back(readFrame);
				}
			}

			pImgFilePath = new TCHAR[256];
			lstrcpy(pImgFilePath, L"../Bin/Resource/Texture/Ogu_Move_Motion/");
			lstrcat(pImgFilePath, pImgFileName);
			imgPathList.push_back(pImgFilePath);
			animFrameCount.push_back(_vec2(frameCountX, frameCountY));
			animFramePlayList.push_back(framePlayList);
		}

		TCHAR* pProtoName = new TCHAR[128];
		lstrcpy(pProtoName, L"Proto_");
		lstrcat(pProtoName, pObjectName); // Proto_Player

		///////////////////////////////////////////////////////////////////////

		TCHAR* pProtoTextureName = new TCHAR[128];
		lstrcpy(pProtoTextureName, pProtoName);
		lstrcat(pProtoTextureName, L"Texture"); // Proto_PlayerTexture

		FAILED_CHECK_RETURN(Engine::Ready_Proto(pProtoTextureName,
			Engine::CTexture::Create(m_pGraphicDev, imgPathList)), E_FAIL);

		////////////////////////////////////////////////////////////////////////

		TCHAR* pProtoAnimName = new TCHAR[128];
		lstrcpy(pProtoAnimName, pProtoName);
		lstrcat(pProtoAnimName, L"Anim");      // Proto_PlayerAnim

		FAILED_CHECK_RETURN(Engine::Ready_Proto(pProtoAnimName,
			Engine::CAnimation::Create(m_pGraphicDev, animFrameCount, animFramePlayList)), E_FAIL);
		/*if (0 == dwByte)
		{
			delete[]pName;
			break;
		}*/
		break;
	}

	CloseHandle(hFile);
	return 0;
}

unsigned int __stdcall CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());
	
	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_Stage();
		break;

	case LOADING_BOSS:
		break;

	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag; 

}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pLoading = new CLoading(pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(eID)))
	{
		Safe_Release(pLoading);
		return nullptr;
	}

	return pLoading;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
