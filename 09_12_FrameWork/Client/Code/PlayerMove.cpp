#include "pch.h"
#include "PlayerMove.h"
#include "Player.h"

PlayerMove* PlayerMove::m_instance = nullptr;

void PlayerMove::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_MOVE);

    if (!m_pStateController)
        SetComponent();

    m_fMoveSpeed = 50.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);
}

void PlayerMove::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(DIK_UP) &&
        !Engine::GetKeyPress(DIK_DOWN) &&
        !Engine::GetKeyPress(DIK_LEFT) &&
        !Engine::GetKeyPress(DIK_RIGHT))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyPress(DIK_LSHIFT))
        m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(DIK_A))
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);



    Key_Input(fTimeDelta);
}

void PlayerMove::Exit()
{

}

void PlayerMove::Key_Input(const _float& fTimeDelta)
{

    _vec3  vLook;
    _vec3  vRight;
    _vec3  vPlayerDir;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector();

    if (Engine::GetKeyPress(DIK_UP))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPushTrigger() &&
            vPlayerDir.x == 0 &&
            vPlayerDir.z == 1)
        {
            m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            return;
        }


        m_pTransformCom->Move_Pos(
            D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_DOWN))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPushTrigger() &&
            vPlayerDir.x == 0 &&
            vPlayerDir.z == -1)
        {
            m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            return;
        }

        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_LEFT))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPushTrigger() &&
            vPlayerDir.x == -1 &&
            vPlayerDir.z == 0)
        {
            m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            return;
        }

        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
    }


    if (Engine::GetKeyPress(DIK_RIGHT))
    {
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPushTrigger() &&
            vPlayerDir.x == 1 &&
            vPlayerDir.z == 0)
        {
            m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            return;
        }
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, m_fMoveSpeed);
    }
}