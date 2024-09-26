#include "pch.h"
#include "Player.h"
#include "Define.h"
#include "Export_System.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_CCamera(nullptr)
    , m_fMoveSpeed(0.f)
    , m_ePlayerState(PLAYERSTATE::PLY_END)
    , m_ePlayerDir(OBJ_DIRECTION::OBJDIR_FRONT)
    , m_iPlayerDir(OBJ_DIRECTION::OBJDIR_FRONT)
    , m_vPlayerDir()
    , m_vPlayerCurrPos()
    , m_vPlayerPrevPos()
    , m_bIsDiagonal(false)
    //0913 임시 졸라 많이 한줄한줄 잘썻죠?ㅋ_ㅋ
    , m_iPlayerCoin(10), m_bInven(false)
{
    //0913 얘까지^^ 멋지게 초기화~!!!
    ZeroMemory(&m_tPlayerHP, sizeof(PLAYERHP));
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    //0913 임시 코드
    m_tPlayerHP.iCurHP = 5;
    m_tPlayerHP.iMaxHP = 6;

    m_pTransformCom->m_vScale = { 20.f,20.f,20.f };
    m_pTransformCom->Set_Pos(200.f, 30.f, 700.f);

    m_pStateControlCom->ChangeState(PlayerIdle::GetInstance(), this);
    return S_OK;
}

void CPlayer::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();
    
    //0926 레이트레디
    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_pQuickSlot = dynamic_cast<CQuickSlot*>(Engine::Get_GameObject(L"Layer_UI", L"QuickSlot_UI"));
    NULL_CHECK_RETURN(m_pInven);
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerPrevPos);

    Key_Input(fTimeDelta);


    //카메라 부분넣기


    Add_RenderGroup(RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->Get_Info(INFO_POS, &m_vPlayerCurrPos);

    m_vPlayerDir = m_vPlayerCurrPos - m_vPlayerPrevPos;
    D3DXVec3Normalize(&m_vPlayerDir, &m_vPlayerDir);
    SetPlayerDirection();

    m_pAnimationCom->Update_Component(fTimeDelta);
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
    
    //9월 25일 충돌관련
    m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

void CPlayer::SetPlayerDirection(/*OBJ_DIRECTION _ePlayerDir*/)
{
    int num = 0;


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

    if (num != m_iPlayerDir)
    {
        m_iPlayerDir = num;
        m_pAnimationCom->SetAnimDir(m_ePlayerState, m_iPlayerDir, m_bIsDiagonal);
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
    }

    if (m_bInven)
        return;
    // 이 아래 추가 하삼 코드!!


    //0920 좆임시 아이템 추가
    if (Engine::GetKeyDown(DIK_L))
    {
        CItem* pItem = dynamic_cast<CExploreHat*>(CExploreHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CPartyHat*>(CPartyHat::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMohican*>(CMohican::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CSmallFruit*>(CSmallFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        m_pInven->Add_Item(pItem);

        pItem = dynamic_cast<CMiddleFruit*>(CMiddleFruit::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(pItem);
        m_pInven->Add_Item(pItem);
    }

    if (Engine::GetKeyDown(DIK_1))
    {
        m_pQuickSlot->Use_QuickItem(0);
    }
    else if (Engine::GetKeyDown(DIK_2))
    {
        m_pQuickSlot->Use_QuickItem(1);
    }
    else if (Engine::GetKeyDown(DIK_3))
    {
        m_pQuickSlot->Use_QuickItem(2);
    }
    else if (Engine::GetKeyDown(DIK_4))
    {
        m_pQuickSlot->Use_QuickItem(3);
    }

    if (Engine::GetKeyDown(DIK_K))
    {
        //0923 임시 대화상자
    }
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
    _tchar buf[128];
    _vec2 position = { 1000,100 };

    switch (m_ePlayerState)
    {
    case Engine::PLY_IDLE:
    case Engine::PLY_IDLEDIAGONAL:
        lstrcpy(buf, L"현재 상태 : IDLE");
        break;
    case Engine::PLY_MOVE:
    case Engine::PLY_MOVEDIAGONAL:
        lstrcpy(buf, L"현재 상태 : WALK");
        break;
    case Engine::PLY_END:
        break;
    default:
        break;
    }

    Engine::Render_Font(L"Font_Ogu24", buf, &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    position = { 1000,130 };
    switch (m_iPlayerDir)
    {
    case Engine::OBJDIR_FRONT:
        lstrcpy(buf, L"현재 방향 : 앞");
        break;
    case Engine::OBJDIR_BACK:
        lstrcpy(buf, L"현재 상태 : 뒤");
        break;
    case Engine::OBJDIR_LEFT:
        lstrcpy(buf, L"현재 상태 : 왼");
        break;
    case Engine::OBJDIR_RIGHT:
        lstrcpy(buf, L"현재 상태 : 오");
        break;
    case Engine::OBJDIR_LEFTBACK:
        lstrcpy(buf, L"현재 상태 : 왼뒤");
        break;
    case Engine::OBJDIR_LEFTFRONT:
        lstrcpy(buf, L"현재 상태 : 왼앞");
        break;
    case Engine::OBJDIR_RIGHTBACK:
        lstrcpy(buf, L"현재 상태 : 오뒤");
        break;
    case Engine::OBJDIR_RIGHTFRONT:
        lstrcpy(buf, L"현재 상태 : 오앞");
        break;
    default:
        lstrcpy(buf, L"현재 상태 :");
        break;
    }
    Engine::Render_Font(L"Font_Ogu24", buf, &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    position = { 1000,160 };
    lstrcpy(buf, m_pTextureCom->GetNowPath());
    Engine::Render_Font(L"Font_Ogu24", buf, &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
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
    Engine::CGameObject::Free();
}
