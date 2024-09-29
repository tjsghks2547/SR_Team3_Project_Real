#include "pch.h"
#include "PlayerLift.h"
#include "Player.h"

PlayerLift* PlayerLift::m_instance = nullptr;

void PlayerLift::Enter()
{
    if (!m_pStateController)
        SetComponent();

    m_CAnimComp = dynamic_cast<CPlayer*>(m_CGameObject)->GetAnimationComp();
    timeElapsed = 0.f;
    switch (m_iStateCount)
    {
    case 0:
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTSTART);

        colObj = dynamic_cast<CPlayer*>(m_CGameObject)->GetCollideObj();
        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Get_Info(INFO_POS, &vColPos);

        break;
    case 1:
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTIDLE);
        break;
    case 2:
        m_CAnimComp->SetAnimationPlaying();
        dynamic_cast<CTransform*>(
            (dynamic_cast<CPlayer*>(m_CGameObject))->Get_Component(
                ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vDownPos);
        
        (dynamic_cast<CPlayer*>(m_CGameObject))->SetPlayerState(
            PLAYERSTATE::PLY_LIFTEND);
        break;
    }

    
}

void PlayerLift::Update(const _float& fTimeDelta)
{
    _vec3 vCurObjPos;
    _vec3 vPlayerHeadPos;
    _vec3 currentPosition;
    
    switch (m_iStateCount)
    {
    case 0:
        if (m_CAnimComp->IsAnimationEnd())
        {
            // 무조건 ChangeState보다 먼저 할 것
            // 그래야 ChangeState()안에서 PlayerLift 다음 행동으로 진입함
            m_iStateCount++;
            m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
            return;
        }

        dynamic_cast<CTransform*>(
            dynamic_cast<CPlayer*>(m_CGameObject)->Get_Component(
                ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPlayerHeadPos);
        vPlayerHeadPos.y += 20;

        currentPosition = vColPos; 
        MoveAlongBezierCurve(fTimeDelta, currentPosition, vColPos, vPlayerHeadPos);

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Set_Pos(currentPosition);

        break;
    case 1:
        if (Engine::GetKeyPress(DIK_UP) ||
            Engine::GetKeyPress(DIK_DOWN) ||
            Engine::GetKeyPress(DIK_LEFT) ||
            Engine::GetKeyPress(DIK_RIGHT))
        {
            PlayerLiftMove::GetInstance()->SetColObj(colObj);
            m_pStateController->ChangeState(PlayerLiftMove::GetInstance(), m_CGameObject);
            
        }

        if (Engine::GetKeyDown(DIK_S))
        {
            m_iStateCount++;
            m_pStateController->ChangeState(PlayerLift::GetInstance(), m_CGameObject);
            
        }
        
        
        break;
    case 2:
        if (m_CAnimComp->IsAnimationEnd())
        {
            m_iStateCount = 0;
            m_pStateController->ChangeState(PlayerIdle::GetInstance(), m_CGameObject);
            
        }
            
        if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector().z == 1)
            vDownPos.z += 30;
        else if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector().z == -1)
            vDownPos.z -= 30;
        else
        {
            if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector().x == 1)
                vDownPos.x += 50;
            else if ((dynamic_cast<CPlayer*>(m_CGameObject))->GetPlayerDirVector().x == -1)
                vDownPos.x -= 50;
        }

        dynamic_cast<CTransform*>(
            dynamic_cast<CPlayer*>(m_CGameObject)->Get_Component(
                ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPlayerHeadPos);
        vPlayerHeadPos.y += 20;

        currentPosition = vPlayerHeadPos;
        MoveAlongBezierCurve(fTimeDelta, currentPosition, vPlayerHeadPos, vDownPos);

        dynamic_cast<CTransform*>(
            colObj->Get_Component(ID_DYNAMIC, L"Com_Transform")
            )->Set_Pos(currentPosition);

        break;
    }
    
}


void PlayerLift::Exit()
{
}

void PlayerLift::Key_Input(const _float& fTimeDelta)
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
}

void PlayerLift::MoveAlongBezierCurve(
    float fTimeDelta, _vec3& currentPosition, const _vec3& A, const _vec3& B)
{
    //float timeElapsed = 0.0f;
    const float duration = 1.0f;

    timeElapsed += fTimeDelta;
    float t = timeElapsed / duration;
    if (t > 1.0f)
    {
        t = 1.0f;
    }


    _vec3 controlPoint1 = _vec3(A.x, B.y, (A.z + B.z) / 2.0f );
    currentPosition = QuadraticBezier(A, controlPoint1, B, t);
}
