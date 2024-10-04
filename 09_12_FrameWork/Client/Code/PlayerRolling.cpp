#include "pch.h"
#include "PlayerRolling.h"
#include "Player.h"

PlayerRolling* PlayerRolling::m_instance = nullptr;

void PlayerRolling::Enter()
{
    if (!m_pStateController)
        SetComponent();

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_ROLLING);



    m_fMoveSpeed = 150.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);

    (dynamic_cast<CPlayer*>(m_CGameObject))->SetInvincible();
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(true);
}

void PlayerRolling::Update(const _float& fTimeDelta)
{
    if (dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp()->IsAnimationEnd())
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);

    Key_Input(fTimeDelta);
}

void PlayerRolling::Exit()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->FixCurPlayerDir(false);
}

void PlayerRolling::Key_Input(const _float& fTimeDelta)
{
    _vec3  vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

    m_pTransformCom->Move_Pos(D3DXVec3Normalize(
        &vPlayerDir, &vPlayerDir), fTimeDelta, m_fMoveSpeed);
}