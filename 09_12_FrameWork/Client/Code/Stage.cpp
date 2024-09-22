#include "pch.h"
#include "Stage.h"
#include "Export_Utility.h"



CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);	
	FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;

}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int  iExit = Engine::CScene::Update_Scene(fTimeDelta);

	return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{


}


HRESULT CStage::Ready_LightInfo()
{

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f,1.f,1.f,1.f };
	tLightInfo.Specular = { 1.f,1.f,1.f,1.f };
	tLightInfo.Ambient = { 1.f,1.f,1.f,1.f };
	tLightInfo.Direction = { 1.F, -1.f, 1.f };

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr; 

	_vec3 Eye = { 0.f, 10.f, -10.f };
	_vec3 At  = {0.f, 0.f, 1.f };
	_vec3 Up = { 0.f, 1.f, 0.f };

	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &Eye, &At, &Up);	

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);

	//pGameObject = CPipeBoard::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PipeBoard", pGameObject), E_FAIL);

	//pGameObject = CPipe::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pipe", pGameObject), E_FAIL);

	//pGameObject = CBoardCursor::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PipeCursor", pGameObject), E_FAIL);

	//여기다가 맵 오브젝트들 넣기 파일 읽어오는기능 


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	pGameObject = CMap::Create(m_pGraphicDev);	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);		
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	//pGameObject = CTestMap::Create(m_pGraphicDev);	
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StartMap", pGameObject), E_FAIL);	
	 
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	//

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);


	//pGameObject = CMonster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	

	//pGameObject = CSkyBox::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
	//0913
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDefaultUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Default_UI", pGameObject), E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	pGameObject = CInvenUI::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inven_UI", pGameObject), E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pStage = new CStage(pGraphicDev);

	if (FAILED(pStage->Ready_Scene()))
	{
		Safe_Release(pStage);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pStage;
}

void CStage::Free()
{
	Engine::CScene::Free();
}


void CStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();	
	

	HANDLE hFile = CreateFile(L"../Map/hi4.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		wstring	teststring = L"";
		WCHAR buffer[256] = {0};		
		DWORD bytesRead = 100;
		
		
		if (!ReadFile(hFile, buffer, 100, &bytesRead, NULL))	
		{
			MSG_BOX("FAILED TO READ TextrueKey");
			CloseHandle(hFile);
		}


		teststring = buffer; 

		
		int a = 4; 

		
		D3DXMATRIX worldmatrix;	
		bytesRead = 0;	
		
		if (!ReadFile(hFile, worldmatrix, sizeof(D3DXMATRIX), &bytesRead, NULL))	
		{
			MSG_BOX("FAILED TO READ WORLDMAXTIRX");	
			CloseHandle(hFile);	
		}
		
		
		Engine::CGameObject* pGameObject = nullptr;	
		
		pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());	
		if (pGameObject == nullptr)
		{
			MSG_BOX("CObject nullptr Error");
		}
	
		pGameObject->SetTextureKey(teststring.c_str());
	
		//  지금 신이 변경이 안돼서 startScene을 가져와서 문제 발생 
	
	
	
		map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();	
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(teststring.c_str(), pGameObject);
		
		
		// 왜 못찾는거지?;;; 아 시발 아직도 신이 안바뀐거였음  이거 찾는 기준이 현재신기준이였음.
		CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", teststring.c_str(), L"Com_Transform"));
		pTransform->ForGetWorldMaxtrix() = worldmatrix;
		
		pTransform->m_vInfo[INFO_POS] = { worldmatrix._41,worldmatrix._42,worldmatrix._43 };
		


		CloseHandle(hFile);
	}
}