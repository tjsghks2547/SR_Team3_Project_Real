#include "pch.h"
#include "PlayerDash.h"
#include "Player.h"

PlayerDash* PlayerDash::m_instance = nullptr;

void PlayerDash::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_DASH);

    if (!m_pStateController)
        SetComponent();

    m_fMoveSpeed = 0.5f;
    m_fMoveDuration = 0.f;
    m_bZoomOutTrigger = false;
}

void PlayerDash::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(DIK_UP) &&
        !Engine::GetKeyPress(DIK_DOWN) &&
        !Engine::GetKeyPress(DIK_LEFT) &&
        !Engine::GetKeyPress(DIK_RIGHT))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }

    else if (Engine::GetKeyUp(DIK_LSHIFT))
    {
        m_pStateController->ChangeState(PlayerMove::GetInstance(), m_CGameObject);
    }
    Key_Input(fTimeDelta);

    if (m_fMoveDuration > 3.f && !m_bZoomOutTrigger)
    {
        dynamic_cast<CDynamicCamera*>(
            dynamic_cast<CPlayer*>(m_CGameObject)->GetCamera()
            )->ZoomTo(60.f, 2);

        m_bZoomOutTrigger = true;
    }


    else
        m_fMoveDuration += fTimeDelta;
}


void PlayerDash::Exit()
{
    m_fMoveDuration = 0.f;
    m_bZoomOutTrigger = false;
}

void PlayerDash::Key_Input(const _float& fTimeDelta)
{

    _vec3  vLook;
    _vec3  vRight;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);


    if (Engine::GetKeyPress(DIK_UP))
    {
        m_pTransformCom->Move_Pos(
            D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_DOWN))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_LEFT))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_RIGHT))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, m_fMoveSpeed);
    }
}