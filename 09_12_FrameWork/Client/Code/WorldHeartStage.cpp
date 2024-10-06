#include "pch.h"
#include "WorldHeartStage.h"
#include "Export_Utility.h" 
#include "MoonForestStage.h"


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


    if (GetAsyncKeyState('B') & 0x8000)
    {
        Engine::CScene* pStage3 = CJungleForestStage::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pStage3, -1); 
    
        FAILED_CHECK_RETURN(Engine::Set_Scene(pStage3), E_FAIL);    
        //pStage3->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용   
    
        return 0;   
    }


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
	HANDLE hFile = CreateFile(L"../Map/WorldHeartStageReal.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        if (hFile != INVALID_HANDLE_VALUE) {

            Engine::CLayer* pLayer = CLayer::Create();

            DWORD bytesRead = 1;

            while (bytesRead > 0)
            {
                ObjectData* objData = new ObjectData;

                if (!ReadFile(hFile, objData, sizeof(ObjectData), &bytesRead, NULL))
                {
                    MSG_BOX("읽기 오류");
                };

                if (bytesRead == 0)
                {
                    break;
                }
                
                
                int a = 4;
                
                Engine::CGameObject* pGameObject = nullptr;
                pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
                if (pGameObject == nullptr)
                {
                    MSG_BOX("CObject nullptr Error");
                }
                
                pGameObject->SetObjectKey(objData->objectName);
                pGameObject->SetTextureKey(objData->textureKey);
                
                
                map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
                pMapLayer[L"Layer_GameLogic"]->Add_GameObject(objData->objectName, pGameObject);
                
                CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", objData->objectName, L"Com_Transform"));
                
                
                pTransform->m_vScale = { objData->scale.x,objData->scale.y,objData->scale.z };
                pTransform->Rotation(ROT_X, objData->rotation.x * 3.14f / 180.f);
                pTransform->Rotation(ROT_Y, objData->rotation.y * 3.14f / 180.f);
                pTransform->Rotation(ROT_Z, objData->rotation.z * 3.14f / 180.f);
                pTransform->Set_Pos(objData->worldMatrix._41, objData->worldMatrix._42, objData->worldMatrix._43);
                
                
                pTransform->Rotation_x = objData->rotation.x;
                pTransform->Rotation_y = objData->rotation.y;
                pTransform->Rotation_z = objData->rotation.z;
                
                
                
                pTransform->Scale_x = objData->scale.x;
                pTransform->Scale_y = objData->scale.y;
                pTransform->Scale_z = objData->scale.z;



            }
            CloseHandle(hFile);
        }
    }


}

HRESULT CWorldHearStage::Ready_LightInfo()
{
    return S_OK;
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

#pragma region Main
    pGameObject = CWorldHeartMap::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WorldHeartMap", pGameObject), E_FAIL);

    pGameObject = CPlayer::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PLAYER, pGameObject);
#pragma endregion
   
#pragma region MinJi
    pGameObject = CBranch::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    _vec3 ItemPos = { 300.f, 20.f, 700.f };
    dynamic_cast<CBranch*>(pGameObject)->Set_DropItem(ItemPos);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Item_Branch", pGameObject), E_FAIL);
    CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pGameObject);
#pragma endregion

#pragma region Crystal Puzzle
    _float fOffsetX = 3000;
    _float fOffsetZ = 1000;

    Engine::CGameObject* pCGameObject = nullptr;
    pCGameObject = CCrystalPuzzle::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pCGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrystalPuzzle", pCGameObject), E_FAIL);
    CCrystalPuzzle* pCrystalPuzzle = static_cast<CCrystalPuzzle*>(pCGameObject);

    pGameObject = CCrystal::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_00", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX, 18.f, fOffsetZ);
    pCrystalPuzzle->Add_Crystal(pGameObject);

    pGameObject = CPressBlock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_00", pGameObject), E_FAIL);
    static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX, fOffsetZ - 35.f);
    pCrystalPuzzle->Add_PressBlock(pGameObject);

    pGameObject = CCrystal::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_01", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 35.f, 18.f, fOffsetZ);
    static_cast<CCrystal*>(pGameObject)->Set_ImageID(3);
    pCrystalPuzzle->Add_Crystal(pGameObject);

    pGameObject = CPressBlock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_01", pGameObject), E_FAIL);
    static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 4, fOffsetX + 35.f, fOffsetZ - 35.f);
    pCrystalPuzzle->Add_PressBlock(pGameObject);

    pGameObject = CCrystal::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crystal_02", pGameObject), E_FAIL);
    static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(fOffsetX + 70.f, 18.f, fOffsetZ);
    pCrystalPuzzle->Add_Crystal(pGameObject);

    pGameObject = CPressBlock::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PressBlock_02", pGameObject), E_FAIL);
    static_cast<CPressBlock*>(pGameObject)->Init(pCrystalPuzzle, 2, fOffsetX + 70.f, fOffsetZ - 35.f);
    pCrystalPuzzle->Add_PressBlock(pGameObject);

#pragma endregion

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

    pGameObject = CInvenUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Inven_UI", pGameObject), E_FAIL);

    pGameObject = CQuickSlot::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"QuickSlot_UI", pGameObject), E_FAIL);

    pGameObject = CQuestUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Quest_UI", pGameObject), E_FAIL);

    pGameObject = CPowerUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Power_UI", pGameObject), E_FAIL);

    pGameObject = CSpeedUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Speed_UI", pGameObject), E_FAIL);

    pGameObject = CStoreUI::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Store_UI", pGameObject), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

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
