#include "pch.h"
#include "Player.h"
#include "Define.h"
#include "InvenUI.h"//1003

#include "Export_System.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pCamera(nullptr)
    , m_ePlayerState(PLAYERSTATE::PLY_END)
    , m_iPlayerDir(OBJ_DIRECTION::OBJDIR_FRONT)
    , m_vPlayerCurrPos()
    , m_vPlayerPrevPos()
    , m_bIsDiagonal(false)
    , m_bSwingTrigger(false)
    , m_objInteractionBox(nullptr)
    , m_objInteracting(nullptr)
    , m_fMoveSpeed(0.f)
    , m_bInvincible(false)
    , m_bPushTrigger(false)
    , m_bPassAble(true)
    // UI 관련 초기화
    , m_iPlayerCoin(10), m_bInven(false), m_bQuest(false)

{
    ZeroMemory(&m_tPlayerHP, sizeof(PLAYERHP));
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    //민지 임시 코드
    m_tPlayerHP.iCurHP = 5;
    m_tPlayerHP.iMaxHP = 6;
    m_eTag = TAG_PLAYER;
    m_pTransformCom->m_vScale = { 20.f,20.f,20.f };
    m_pTransformCom->Set_Pos(200.f, 30.f, 500.f);

    m_pStateControlCom->ChangeState(PlayerIdle::GetInstance(), this);
    return S_OK;
}

void CPlayer::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();

    m_pQuestUI = dynamic_cast<CQuestUI*>(Engine::Get_GameObject(L"Layer_UI", L"Quest_UI"));
    NULL_CHECK_RETURN(m_pQuestUI); // 아직 스테이지에 안만듬

    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_pQuickSlot = dynamic_cast<CQuickSlot*>(Engine::Get_GameObject(L"Layer_UI", L"QuickSlot_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_BuffArray[0] = dynamic_cast<CPowerUI*>(Engine::Get_GameObject(L"Layer_UI", L"Power_UI"));
    NULL_CHECK_RETURN(m_BuffArray[0]);

    m_BuffArray[1] = dynamic_cast<CSpeedUI*>(Engine::Get_GameObject(L"Layer_UI", L"Speed_UI"));
    NULL_CHECK_RETURN(m_BuffArray[1]);

    //hat = dynamic_cast<CExploreHat*>(CExploreHat::Create(m_pGraphicDev));
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven, 0);

    if (m_BuffArray[0]->Get_BuffTime() < m_BuffArray[1]->Get_BuffTime())
    {
        m_BuffArray[0]->Set_YPos(90.f);
        m_BuffArray[1]->Set_YPos(175.f);
    }
    else
    {
        m_BuffArray[1]->Set_YPos(90.f);
        m_BuffArray[0]->Set_YPos(175.f);
    }

    m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerPrevPos);

    Key_Input(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerCurrPos);
    //hat->Set_ItemPos(m_vPlayerCurrPos);
    SetPlayerDirection();

    m_pAnimationCom->Update_Component(fTimeDelta);

    //DurationInvincible(fTimeDelta);
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CPlayer::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    if (m_bIsDiagonal)
        m_pTextureCom->Set_Texture(m_ePlayerState + 1);
    else
        m_pTextureCom->Set_Texture(m_ePlayerState);

    //Print_PlayerState();

    m_pAnimationCom->Render_Buffer();
    //hat->Render_GameObject();

    //9월 25일 충돌관련
    m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{
    if (_pOther->IncludingType(OBJ_TYPE::NOTPASS_ABLE))
    {
        m_bPassAble = false;
    }
}

void CPlayer::OnCollisionExit(CGameObject* _pOther)
{
    if (_pOther->IncludingType(OBJ_TYPE::NOTPASS_ABLE))
    {
        m_bPassAble = true;
    }

}

void CPlayer::SetPlayerDirection()
{
    int num = 0;
    m_vPlayerDir = m_vPlayerCurrPos - m_vPlayerPrevPos;
    D3DXVec3Normalize(&m_vPlayerDir, &m_vPlayerDir);

    if (m_vPlayerDir.x > 0)
    {
        if (m_vPlayerDir.z > 0)
        {
            num = OBJ_DIRECTION::OBJDIR_RIGHT | OBJ_DIRECTION::OBJDIR_BACK;
            m_bIsDiagonal = true;
        }


        else if (m_vPlayerDir.z == 0)
        {
            num = OBJ_DIRECTION::OBJDIR_RIGHT;
            m_bIsDiagonal = false;
        }


        else if (m_vPlayerDir.z < 0)
        {
            num = OBJ_DIRECTION::OBJDIR_RIGHT | OBJ_DIRECTION::OBJDIR_FRONT;
            m_bIsDiagonal = true;
        }

    }

    else if (m_vPlayerDir.x == 0)
    {
        if (m_vPlayerDir.z > 0)
        {
            m_bIsDiagonal = false;
            num = OBJ_DIRECTION::OBJDIR_BACK;
        }

        else if (m_vPlayerDir.z == 0)
            num = m_iPlayerDir; //-> 해당방향 그대로 유지

        else if (m_vPlayerDir.z < 0)
        {
            m_bIsDiagonal = false;
            num = OBJ_DIRECTION::OBJDIR_FRONT;
        }

    }

    else if (m_vPlayerDir.x < 0)
    {
        if (m_vPlayerDir.z > 0)
        {
            num = OBJ_DIRECTION::OBJDIR_LEFT | OBJ_DIRECTION::OBJDIR_BACK;
            m_bIsDiagonal = true;
        }


        else if (m_vPlayerDir.z == 0)
        {
            num = OBJ_DIRECTION::OBJDIR_LEFT;
            m_bIsDiagonal = false;
        }

        else if (m_vPlayerDir.z < 0)
        {
            num = OBJ_DIRECTION::OBJDIR_LEFT | OBJ_DIRECTION::OBJDIR_FRONT;
            m_bIsDiagonal = true;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    if (num == m_iPlayerDir)
        return;

    // 방향이 이전과 다르면 애니메이션 갱신
    m_iPlayerDir = num;
    m_pAnimationCom->SetAnimDir(m_ePlayerState, m_iPlayerDir, m_bIsDiagonal);
}


void CPlayer::DurationInvincible(const _float& fTimeDelta)
{
    if (!m_bInvincible)
        return;

    static float fDurationTime = 0.f;
    fDurationTime += fTimeDelta;
    if (fDurationTime >= 3.f)
    {
        fDurationTime = 0.f;
        m_bInvincible = false;
    }
}

HRESULT CPlayer::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTexture"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    pComponent = m_pAnimationCom = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Proto_PlayerAnim"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pCCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

    pComponent = m_pStateControlCom = dynamic_cast<CStateController*>(Engine::Clone_Proto(L"Proto_State"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_State", pComponent });

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBoundBox->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    //0922
    if (Engine::GetKeyDown(DIK_I))
    {
        m_bInven = m_bInven ? false : true;
        if (m_bQuest)
            m_bQuest = false;
    }

    if (Engine::GetKeyDown(DIK_O))
    {
        m_bQuest = m_bQuest ? false : true;
        if (m_bInven)
            m_bInven = false;
    }

    if (m_bInven || m_bQuest)
        return;
    // 이 아래 추가 하삼 코드!!


    //0920 좆임시 아이템 추가
    if (Engine::GetKeyDown(DIK_L))
    {
        CItem* pItem = dynamic_cast<CExploreHat*>(CExploreHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPartyHat*>(CPartyHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMohican*>(CMohican::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CAppleHat*>(CAppleHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CEnkyHat*>(CEnkyHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPenguin*>(CPenguin::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPoopHat*>(CPoopHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallFruit*>(CSmallFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleFruit*>(CMiddleFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigFruit*>(CBigFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallHerb*>(CSmallHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleHerb*>(CMiddleHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigHerb*>(CBigHerb::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallRoot*>(CSmallRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleRoot*>(CMiddleRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CBigRoot*>(CBigRoot::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookSmallFish*>(CCookSmallFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookMiddleFish*>(CCookMiddleFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CCookBigFish*>(CCookBigFish::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);


        pItem = dynamic_cast<CBranch*>(CBranch::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CLeaf*>(CLeaf::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CNet*>(CNet::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CExpressTicket*>(CExpressTicket::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        pItem->LateReady_GameObject();
        m_pInven->Add_Item(pItem);

    }

    for (int i = 0; i < 4; i++)
    {
        if (Engine::GetKeyDown(DIK_1 + i))
            m_pQuickSlot->Use_QuickItem(i);
    }

    if (Engine::GetKeyDown(DIK_SPACE))
        m_bInvincible ^= TRUE;
}

_vec3 CPlayer::Piking_OnTerrain()
{
    CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
    NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

    CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", L"Terrain", L"Com_Transform"));
    NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

    return m_pCCalculatorCom->Picking_OnTerrian(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CPlayer::Print_PlayerState()
{

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayer* pPlayer = new CPlayer(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }

    return pPlayer;
}

void CPlayer::Free()
{
    PlayerIdle::DestroyInstance();
    PlayerMove::DestroyInstance();
    PlayerDash::DestroyInstance();
    PlayerPush::DestroyInstance();
    PlayerLift::DestroyInstance();
    PlayerLiftMove::DestroyInstance();
    PlayerDance::DestroyInstance();
    PlayerSmash::DestroyInstance();
    PlayerRolling::DestroyInstance();
    PlayerHurt::DestroyInstance();

    Engine::CGameObject::Free();
}
