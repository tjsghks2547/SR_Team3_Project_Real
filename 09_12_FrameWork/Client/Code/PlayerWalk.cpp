#include "pch.h"
#include "PlayerWalk.h"
#include "Player.h"

PlayerWalk* PlayerWalk::m_instance = nullptr;

void PlayerWalk::Enter()
{
	(dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(PLAYERSTATE::PLY_WALK);
	if (!m_pStateController)
		SetComponent();

    m_fMoveSpeed = 51.f;
}

void PlayerWalk::Update(const _float& fTimeDelta)
{
    if (GetAsyncKeyState(VK_UP))
        m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
}

void PlayerWalk::Exit()
{
	
}

void PlayerWalk::Key_Input(const _float& fTimeDelta)
{

    _vec3  vLook;
    _vec3  vRight;

    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

    if (GetAsyncKeyState(VK_UP))
    {
        m_pTransformCom->Move_Pos(
            D3DXVec3Normalize(&vLook, &vLook), fTimeDelta, m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_DOWN))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vLook, &vLook), fTimeDelta, -m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_LEFT))
    {
        // m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, -m_fMoveSpeed);
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        //  m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(
            &vRight, &vRight), fTimeDelta, m_fMoveSpeed);
    }
}