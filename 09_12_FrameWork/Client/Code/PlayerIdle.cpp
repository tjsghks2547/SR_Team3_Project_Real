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
    if (Engine::GetKeyUp(DIK_UP))
    {
        m_pStateController->ChangeState(PlayerWalk::GetInstance(), m_CGameObject);
    }
}


void PlayerIdle::Exit()
{
}

