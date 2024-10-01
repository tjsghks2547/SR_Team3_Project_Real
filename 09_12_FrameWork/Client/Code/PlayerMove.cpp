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

    Key_Input(fTimeDelta);
}

void PlayerMove::Exit()
{

}

void PlayerMove::Key_Input(const _float& fTimeDelta)
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