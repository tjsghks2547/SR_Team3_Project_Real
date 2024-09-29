#include "pch.h"
#include "PlayerIdle.h"
#include "Player.h"

PlayerIdle* PlayerIdle::m_instance = nullptr;

void PlayerIdle::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_IDLE);

    if (!m_pStateController)
        SetComponent();


}

void PlayerIdle::Update(const _float& fTimeDelta)
{
    if (Engine::GetKeyPress(DIK_UP) ||
        Engine::GetKeyPress(DIK_DOWN) ||
        Engine::GetKeyPress(DIK_LEFT) ||
        Engine::GetKeyPress(DIK_RIGHT))
    {
        if (Engine::GetKeyPress(DIK_LSHIFT))
            m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);
        else
            m_pStateController->ChangeState(PlayerMove::GetInstance(), m_CGameObject);


    }

    if (Engine::GetKeyPress(DIK_A))
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);

    if (Engine::GetKeyDown(DIK_S) &&
        (dynamic_cast<CPlayer*>(m_CGameObject))->GetCollideObj() != nullptr)
        m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);


}


void PlayerIdle::Exit()
{
}

