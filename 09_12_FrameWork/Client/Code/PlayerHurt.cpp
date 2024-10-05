#include "pch.h"
#include "PlayerHurt.h"
#include "Player.h"

PlayerHurt* PlayerHurt::m_instance = nullptr;

void PlayerHurt::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->DisableDiagonal();
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_HURT);

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerCurHP(-1);
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetInvincible();

    m_fFixedTimer = 0.f;
}

void PlayerHurt::Update(const _float& fTimeDelta)
{
    m_fFixedTimer += fTimeDelta;
    if(m_fFixedTimer >= 0.1f &&
        dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

}


void PlayerHurt::Exit()
{
}

