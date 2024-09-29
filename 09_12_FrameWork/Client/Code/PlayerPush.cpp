#include "pch.h"
#include "PlayerPush.h"
#include "Player.h"

PlayerPush* PlayerPush::m_instance = nullptr;

void PlayerPush::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_PUSH);

    if (!m_pStateController)
        SetComponent();

    m_fMoveSpeed = 0.5f;
}

void PlayerPush::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(DIK_UP) &&
        !Engine::GetKeyPress(DIK_DOWN) &&
        !Engine::GetKeyPress(DIK_LEFT) &&
        !Engine::GetKeyPress(DIK_RIGHT))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }
}


void PlayerPush::Exit()
{
}

void PlayerPush::Key_Input(const _float& fTimeDelta)
{

    _vec3  vLook;
    _vec3  vRight;
    _vec3  vPlayerDir;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector();

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