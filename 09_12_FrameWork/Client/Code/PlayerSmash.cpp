#include "pch.h"
#include "PlayerSmash.h"
#include "Player.h"

PlayerSmash* PlayerSmash::m_instance = nullptr;

void PlayerSmash::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_SMASH);

    if (!m_pStateController)
        SetComponent();

    m_CAnimComp = dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp();
    m_iStateCount = 0.f;
}

void PlayerSmash::Update(const _float& fTimeDelta)
{
    if (m_iStateCount == 0 && m_CAnimComp->GetCurrentFrame() >= 4)
    {
        m_iStateCount++;
        m_CAnimComp->SetAnimationPause(true);
    }
    if (Engine::GetKeyUp(CONTROL_KEY::PLY_SMASHKEY))
    {
        m_CAnimComp->SetAnimationPause(false);
        dynamic_cast<CDynamicCamera*>(
            dynamic_cast<CPlayer*>(m_CGameObject)->GetCamera()
            )->ShakeMove(0.3f);
    }

    if (dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
    {
        
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }
        
}

void PlayerSmash::Exit()
{

}