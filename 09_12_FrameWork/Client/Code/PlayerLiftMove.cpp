#include "pch.h"
#include "PlayerLiftMove.h"
#include "Player.h"

PlayerLiftMove* PlayerLiftMove::m_instance = nullptr;

void PlayerLiftMove::Enter()
{
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
        PLAYERSTATE::PLY_LIFTMOVE);


    if (!m_pStateController)
        SetComponent();

    m_CAnimComp = dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp();
    m_fMoveSpeed = 20.f;
    (dynamic_cast<CPlayer*>(m_CGameObject))->SetMoveSpeed(m_fMoveSpeed);
}

void PlayerLiftMove::Update(const _float& fTimeDelta)
{
    if (!Engine::GetKeyPress(CONTROL_KEY::PLY_UPKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_DOWNKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_LEFTKEY) &&
        !Engine::GetKeyPress(CONTROL_KEY::PLY_RIGHTKEY))
    {
        m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
    }

    Key_Input(fTimeDelta);

}


void PlayerLiftMove::Exit()
{
}

void PlayerLiftMove::Key_Input(const _float& fTimeDelta)
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

    _vec3 colObjPos;
    m_pTransformCom->Get_Info(INFO_POS, &colObjPos);
    colObjPos.y += 20;

    dynamic_cast<CTransform*>(
        colObj->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Set_Pos(colObjPos);
}