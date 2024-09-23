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
    if (Engine::GetKeyDown(DIK_UP) ||
        Engine::GetKeyDown(DIK_DOWN) ||
        Engine::GetKeyDown(DIK_LEFT) ||
        Engine::GetKeyDown(DIK_RIGHT))
    {
        m_pStateController->ChangeState(PlayerMove::GetInstance(), m_CGameObject);
    }
}


void PlayerIdle::Exit()
{
}

