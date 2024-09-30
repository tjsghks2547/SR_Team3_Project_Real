#include "pch.h"
#include "PlayerIdle.h"
#include "Player.h"

PlayerIdle* PlayerIdle::m_instance = nullptr;

void PlayerIdle::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_IDLE);

    if (!m_pStateController)
        SetComponent();

    m_fDuration = 0.f;
    m_fKeyDownTimer = 0.f;
}

void PlayerIdle::Update(const _float& fTimeDelta)
{
    m_fDuration += fTimeDelta;
    if (m_fDuration >= 3.f)
    {
        PlayerDance::GetInstance()->SetDanceType(0);
        m_pStateController->ChangeState(PlayerDance::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_UPKEY) ||
        Engine::GetKeyPress(CONTROL_KEY::PLY_DOWNKEY) ||
        Engine::GetKeyPress(CONTROL_KEY::PLY_LEFTKEY) ||
        Engine::GetKeyPress(CONTROL_KEY::PLY_RIGHTKEY))
    {
        if (Engine::GetKeyPress(CONTROL_KEY::PLY_DASHKEY))
            m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);
        else
            m_pStateController->ChangeState(PlayerMove::GetInstance(), m_CGameObject);

    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_SWINGKEY))
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);

    if (Engine::GetKeyDown(CONTROL_KEY::PLY_LIFTKEY) &&
        (dynamic_cast<CPlayer*>(m_CGameObject))->GetCollideObj() != nullptr)
        m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_DANCEKEY))
    {
        m_fKeyDownTimer += fTimeDelta;
        if (m_fKeyDownTimer > 1.f)
        {
            PlayerDance::GetInstance()->SetDanceType(1);
            m_pStateController->ChangeState(PlayerDance::GetInstance(), m_CGameObject);
        }
    }

    if (Engine::GetKeyDown(CONTROL_KEY::PLY_SMASHKEY))
    {
        m_pStateController->ChangeState(PlayerSmash::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyDown(CONTROL_KEY::PLY_ROLLKEY))
    {
        m_pStateController->ChangeState(PlayerRolling::GetInstance(), m_CGameObject);
    }
}


void PlayerIdle::Exit()
{
    m_fDuration = 0.f;
    m_fKeyDownTimer = 0.f;
}

