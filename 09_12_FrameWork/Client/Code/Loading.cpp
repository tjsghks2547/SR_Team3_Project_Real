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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_StartMap", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map_22_1208x1032 #3240.png", TEX_NORMAL)), E_FAIL);
	// UI
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


	// Puzzle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoardTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePuzzleFloor2Line.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeBoardSlotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleSlot.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_MoonForest_MazePushPuzzleThreeWay.png", TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PipeCursorTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/puzzle/Sprite_UI_Cursor.png", TEX_NORMAL)), E_FAIL);

	//TEST 높이 맵 
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainHeightTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Height2.bmp", TEX_NORMAL)), E_FAIL);




	lstrcpy(m_szLoading, L"Loading Complete!!");

	m_bFinish = true; 

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
