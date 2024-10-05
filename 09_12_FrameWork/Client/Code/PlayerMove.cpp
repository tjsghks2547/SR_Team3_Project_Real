#include "pch.h"
#include "PlayerMove.h"
#include "Player.h"

PlayerMove* PlayerMove::m_instance = nullptr;

void PlayerMove::Enter()
{
    if (!m_pStateController)
        SetComponent();

    dynamic_cast<CPlayer*>(m_CGameObject)->SetPlayerState(PLAYERSTATE::PLY_MOVE);

    m_fMoveSpeed = 50.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);
    float fItemMoveSpeed = (dynamic_cast<CPlayer*>(m_CGameObject))->GetItemMoveSpeed();
    m_fMoveSpeed += fItemMoveSpeed;

}

void PlayerMove::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(CONTROL_KEY::PLY_UPKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_DOWNKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_LEFTKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_RIGHTKEY))
    {
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_DASHKEY))
        m_pStateController->ChangeState(PlayerDash::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_SWINGKEY))
        m_pStateController->ChangeState(PlayerSwing::GetInstance(), m_CGameObject);

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_SMASHKEY))
    {
        m_pStateController->ChangeState(PlayerSmash::GetInstance(), m_CGameObject);
    }

    if (Engine::GetKeyDown(CONTROL_KEY::PLY_ROLLKEY))
    {
        m_pStateController->ChangeState(PlayerRolling::GetInstance(), m_CGameObject);
    }

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
    vPlayerDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

    m_colliderObj = dynamic_cast<CPlayer*>(m_CGameObject)->GetInteractingObj();

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_UPKEY))
    {
        if (m_colliderObj != nullptr)
        {
            if ((dynamic_cast<CPlayer*>(m_CGameObject))->IsPlayerDiagonal())
            {
                if (vPlayerDir.z > 0)
                    return;
            }

            if (m_colliderObj->IncludingType(OBJ_TYPE::PUSH_ABLE))
            {
                m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            }

        }

        else
        {
            m_fMoveSpeed = 50.f;
        }


        _vec3 moveDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() || moveDir.z < 0)
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vLook, &vLook), fTimeDelta, m_fMoveSpeed);
        }
    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_DOWNKEY))
    {
        if (m_colliderObj != nullptr)
        {
            if ((dynamic_cast<CPlayer*>(m_CGameObject))->IsPlayerDiagonal())
            {
                if (vPlayerDir.z < 0)
                    return;
            }

            if (m_colliderObj->IncludingType(OBJ_TYPE::PUSH_ABLE))
            {
                m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            }

        }

        else
        {
            m_fMoveSpeed = 50.f;
        }

        _vec3 moveDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() || moveDir.z > 0)
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
        }
    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_LEFTKEY))
    {
        if (m_colliderObj != nullptr)
        {
            if ((dynamic_cast<CPlayer*>(m_CGameObject))->IsPlayerDiagonal())
            {
                if (vPlayerDir.x < 0)
                    return;
            }

            if (m_colliderObj->IncludingType(OBJ_TYPE::PUSH_ABLE))
            {
                m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            }

        }

        else
        {
            m_fMoveSpeed = 50.f;
        }


        _vec3 moveDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() || moveDir.x > 0)
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
        }
    }

    if (Engine::GetKeyPress(CONTROL_KEY::PLY_RIGHTKEY))
    {
        if (m_colliderObj != nullptr)
        {
            if ((dynamic_cast<CPlayer*>(m_CGameObject))->IsPlayerDiagonal())
            {
                if (vPlayerDir.x > 0)
                    return;
            }

            if (m_colliderObj->GetObjectType() == OBJ_TYPE::PUSH_ABLE)
            {
                m_pStateController->ChangeState(PlayerPush::GetInstance(), m_CGameObject);
            }

        }

        else
        {
            m_fMoveSpeed = 50.f;
        }

        _vec3 moveDir = (dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector2();

        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPassAble() || moveDir.x < 0)
        {
            m_pTransformCom->Move_Pos(D3DXVec3Normalize(
                &vRight, &vRight), fTimeDelta, m_fMoveSpeed);
        }
    }
}