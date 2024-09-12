#include "pch.h"
#include "Player.h"
#include "Define.h"
#include "Export_System.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_fMoveSpeed(0.f)
    , m_ePlayerState(PLAYERSTATE::PLY_END)
{

}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fMoveSpeed = 50.f;

    // m_pTransformCom->m_vScale = { 10.f,10.f,10.f };
    // m_pTransformCom->Rotation(ROT_X, 90.f * 3.141592f / 180.f);

    m_pStateControlCom->ChangeState(PlayerIdle::GetInstance(), this);
    return S_OK;
}

void CPlayer::Start_GameObject()
{

}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    //Key_Input(fTimeDelta);

    //카메라 부분넣기


    Add_RenderGroup(RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    //일단 플레이어의 좌표가 필요 

    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);




    //플레이어가 밟고 있는 발판이 필요
    //CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
    //NULL_CHECK(pTerrainTex);
    //
    //_float fY =  m_pCCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainTex->Get_VtxPos(), VTXCNTX, VTXCNTZ);
    //
    //m_pTransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);





    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();

    Print_PlayerState();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    //m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

HRESULT CPlayer::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pCCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

    pComponent = m_pStateControlCom = dynamic_cast<CStateController*>(Engine::Clone_Proto(L"Proto_State"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_State", pComponent });


}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

    _vec3  vLook;
    _vec3  vRight;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

    if (GetAsyncKeyState(VK_UP))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_DOWN))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_LEFT))
    {
        // m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        //  m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, m_fMoveSpeed);
    }


    //if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
    //{
    //    _vec3	vPickPos = Piking_OnTerrain();
    //        
    //    _vec3	vPos;
    //    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    //
    //    _vec3	vDir = vPickPos - vPos;
    //
    //    m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), fTimeDelta, 15.f);
    //}


    ////마우스 좌클릭 했을때 발생하는 이벤트
    //if(Engine::Get_DIMouseState(DIM_LB) & 0x80)
    //{

    //    //Screen space 에서의 점 
    //    POINT mousepos; 
    //    GetCursorPos(&mousepos);

    //    float a = mousepos.x;
    //    float b = mousepos.y;


    //    //Screen space의 점을 view스페이스로 전환 
    //   /* D3DVIEWPORT9 ViewPort; 
    //    m_pGraphicDev->GetViewport(&ViewPort);*/
    //    
    //    D3DMATRIX matProj;

    //    m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

        //D3DXMatrixInverse(&)

        //D3DVIEWPORT9 ViewPort;
        //m_pGraphicDev->GetViewport(&ViewPort);

        //D3DXIntersectTri()


        //
        //_matrix matCamWorld, m_matView;
        //m_pGraphicDev->GetTransform(D3DTS_VIEW, &m_matView);
        //
        //D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
        //
        //_vec3 vCamLook = { matCamWorld._21,matCamWorld._22,matCamWorld._23 };
        //
       // m_pTransformCom->Move_Pos(&vCamLook, fTimeDelta, 10.f);

        //_long dwMouseMove(0);



        //m_pTransformCom->Move_Pos()


   /*
    if(dwMouseState == Engine::Get_DIKeyState(DIM_LB))
    {
        dwMouseXMove = Engine::Get_DIMouseMove(DIMS_X);
        dwMouseYMove = Engine::Get_DIMouseMove(DIMS_Y);

        cout << "dwMouseXMove" << endl;
    }*/


    //dwMouseXMove = Engine::Get_DIMouseMove(DIMS_X);
    //dwMouseYMove = Engine::Get_DIMouseMove(DIMS_Y);




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
    _tchar buf[32];
    _vec2 position = { 600,100 };

    switch (m_ePlayerState)
    {
    case Engine::PLY_IDLE:
        lstrcpy(buf, L"현재 상태 : IDLE");
        break;
    case Engine::PLY_WALK:
        lstrcpy(buf, L"현재 상태 : WALK");
        break;
    case Engine::PLY_END:
        break;
    default:
        break;
    }

    Engine::Render_Font(
        L"Font_Default",
        buf,
        &position,
        D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
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
    PlayerWalk::DestroyInstance();
    Engine::CGameObject::Free();
}
