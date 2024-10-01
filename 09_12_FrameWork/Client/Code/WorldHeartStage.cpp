#include "pch.h"
#include "WorldHeartStage.h"
#include "Export_Utility.h" 


CWorldHearStage::CWorldHearStage(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CScene(pGraphicDev)
{


}

CWorldHearStage::~CWorldHearStage()
{



}

HRESULT CWorldHearStage::Ready_Scene()
{
    FAILED_CHECK_RETURN(Ready_Layer_Environmnet(L"Layer_Environment"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}

_int CWorldHearStage::Update_Scene(const _float& fTimeDelta)
{
    _int  iExit = Engine::CScene::Update_Scene(fTimeDelta);



    return iExit;
}

void CWorldHearStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CWorldHearStage::Render_Scene()
{
}

void CWorldHearStage::init()
{
	Engine::CLayer* pLayer = CLayer::Create();

	DWORD bytesRead = 1;
	HANDLE hFile = CreateFile(L"../Map/WorldHeartStage.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		while (bytesRead > 0)
		{
			//wstring ObjectName = L"";
			//WCHAR* buffer = new WCHAR[256];
			WCHAR buffer[256] = { 0 };
			bytesRead = 100;

			//오브젝트키 (오브젝트 이름)
			if (!ReadFile(hFile, buffer, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}

			wstring* pObjectName = new wstring;
			*pObjectName = buffer;



			WCHAR buffer2[256] = { 0 };
			bytesRead = 100;

			//텍스처키 (텍스처 이름)
			if (!ReadFile(hFile, buffer2, 100, &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}

			wstring* pTextureWstring = new wstring;
			*pTextureWstring = buffer2;


			D3DXMATRIX worldmatrix;
			bytesRead = 0;

			if (!ReadFile(hFile, worldmatrix, sizeof(D3DXMATRIX), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ WORLDMAXTIRX");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Rotation_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Rotation_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}

			D3DXVECTOR3 Sclae_vec3;
			bytesRead = 0;

			if (!ReadFile(hFile, Sclae_vec3, sizeof(D3DXVECTOR3), &bytesRead, NULL))
			{
				MSG_BOX("FAILED TO READ D3DXVECTOR3 INFO");
				CloseHandle(hFile);
			}


			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
			if (pGameObject == nullptr)
			{
				MSG_BOX("CObject nullptr Error");
			}



			pGameObject->SetTextureKey((*pTextureWstring).c_str());

			//  지금 신이 변경이 안돼서 startScene을 가져와서 문제 발생 



			map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
			pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*pObjectName).c_str(), pGameObject);

			CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::MONSTER, pGameObject);

			// 왜 못찾는거지?;;; 아 시발 아직도 신이 안바뀐거였음  이거 찾는 기준이 현재신기준이였음.
			CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", (*pObjectName).c_str(), L"Com_Transform"));
			//pTransform->ForGetWorldMaxtrix() = worldmatrix;

			////회전값만 이제 넣어주면 됨 ( 크기 -> 자전 -> 이동 ) 순서로  아 시발 병신같이 월드매트릭스를 넣어줫네;;
			// 월드매트릭스에 넣기 전의 크기값을 넣어줘야하네 
			pTransform->m_vScale = { Sclae_vec3.x,Sclae_vec3.y,Sclae_vec3.z };
			pTransform->Rotation(ROT_X, Rotation_vec3.x * 3.14f / 180.f);
			pTransform->Rotation(ROT_Y, Rotation_vec3.y * 3.14f / 180.f);
			pTransform->Rotation(ROT_Z, Rotation_vec3.z * 3.14f / 180.f);
			pTransform->Set_Pos(worldmatrix._41, worldmatrix._42, worldmatrix._43);

			//pTransform->m_vAngle = vec3;
			//D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
		}
		CloseHandle(hFile);
	}



}

HRESULT CWorldHearStage::Ready_LightInfo()
{
    return E_NOTIMPL;
}

HRESULT CWorldHearStage::Ready_Layer_Environmnet(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    _vec3 Eye = { 0.f, 0.f, 0.f };
    _vec3 At = { 0.f, 1.f, 1.f };
    _vec3 Up = { 0.f, 1.f, 0.f };

    pGameObject = CDynamicCamera::Create(m_pGraphicDev, &Eye, &At, &Up);

    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);


    //GameObject = CSkyBox::Create(m_pGraphicDev);          
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CWorldHearStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{

    Engine::CLayer* pLayer = CLayer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;


    pGameObject = CWorldHeartMap::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WorldHeartMap", pGameObject), E_FAIL);


    pGameObject = CPlayer::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CWorldHearStage::Ready_Layer_UI(const _tchar* pLayerTag)
{
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

    pGameObject = CQuickSlot::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuickSlot_UI", pGameObject), E_FAIL);
    m_mapLayer.insert({ pLayerTag, pLayer });

    pGameObject = CQuestUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quest_UI", pGameObject), E_FAIL);
    m_mapLayer.insert({ pLayerTag, pLayer });
    return S_OK;


    return S_OK;
}

CWorldHearStage* CWorldHearStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWorldHearStage* pStage2 = new CWorldHearStage(pGraphicDev);

    return pStage2;
}

void CWorldHearStage::Free()
{
    Engine::CScene::Free();
}
