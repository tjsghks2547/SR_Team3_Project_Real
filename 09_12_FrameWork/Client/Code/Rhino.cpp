#include "pch.h"
#include "Rhino.h"

CRhino::CRhino(LPDIRECT3DDEVICE9 pGraphicDev)
    :CArenaMonster(pGraphicDev)
    , m_fTime(0), m_fSpeed(2.f), m_bStoneAppear(false)
{
    m_tInfo.pBossName = L"거대 사막뿔소";
    m_tInfo.iMaxHP = 100;
    m_tInfo.iCurHP = 100;
}

CRhino::~CRhino()
{
}

HRESULT CRhino::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    SetObjectType(OBJ_TYPE::DESTROY_ABLE);

    m_eState = RHINO_APPEAR;

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStomp.png", &m_pTex);

    m_pAnimatorCom->CreateAnimation(L"RhinoStompFront", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLB", m_pTex, _vec2(1024.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLeft", m_pTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompLT", m_pTex, _vec2(1024.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompBack", m_pTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRT", m_pTex, _vec2(1024.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRight", m_pTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStompRB", m_pTex, _vec2(1024.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->Play(L"RhinoStompFront", true);
    //m_pRunTex

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStun.png", &m_pStunTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunLB", m_pStunTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunRB", m_pStunTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunLT", m_pStunTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoStunRT", m_pStunTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 3);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoRun.png", &m_pRunTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunFront", m_pRunTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLB", m_pRunTex, _vec2(768.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLeft", m_pRunTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunLT", m_pRunTex, _vec2(768.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunBack", m_pRunTex, _vec2(0.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRT", m_pRunTex, _vec2(768.f, 512.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRight", m_pRunTex, _vec2(0.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);
    m_pAnimatorCom->CreateAnimation(L"RhinoRunRB", m_pRunTex, _vec2(768.f, 768.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.15f, 2);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoJump.png", &m_pAppearTex);
    m_pAnimatorCom->CreateAnimation(L"RhinoLand", m_pAppearTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.25f, 3);
    m_pAnimatorCom->CreateAnimation(L"RhinoJump", m_pAppearTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.25f, 3);

    m_pAnimatorCom->Play(L"RhinoLand", true); // 등장씬~

    return S_OK;
}

void CRhino::LateReady_GameObject()
{
    if (!m_bCreate)
        return;

    CArenaMonster::LateReady_GameObject();

    m_pBossHPBar = dynamic_cast<CBossHPBar*>(CBossHPBar::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pBossHPBar);
    m_pBossHPBar->Init_BossInfo(&m_tInfo);

    for (size_t i = 0; i < 10; i++)
    {
        m_pRhinoStone[i] = dynamic_cast<CRhinoStone*>(CRhinoStone::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(m_pRhinoStone[i]);
        CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, m_pRhinoStone[i]);
        m_pRhinoStone[i]->SetObjectKey(L"RhinoStone");
    }

    for (size_t i = 0; i < 10; i++)
    {
        m_pRhinoStone[i]->LateReady_GameObject();
    }
}

_int CRhino::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCreate)
        return 0;

    if (m_tInfo.iCurHP <= 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            if (m_pRhinoStone[i] != nullptr)
            {
                Safe_Release(m_pRhinoStone[i]); // 동적 할당된 객체 삭제
                m_pRhinoStone[i] = nullptr; // 포인터 초기화
            }
        }
        m_bCreate = false;
        return 0;
    }

    Add_RenderGroup(RENDER_ALPHA, this);

    m_pBossHPBar->Update_GameObject(fTimeDelta);

    RhinoState_Update(fTimeDelta); // 상태 업데이트 + 애니

    for (size_t i = 0; i < 10; i++)
    {
        m_pRhinoStone[i]->Update_GameObject(fTimeDelta);
    }

    _int iExit = CArenaMonster::Update_GameObject(fTimeDelta);

    return iExit;
}

void CRhino::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bCreate)
        return;

    if (m_tInfo.iCurHP <= 0)
    {
        return;
    }
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    for (size_t i = 0; i < 10; i++)
    {
        m_pRhinoStone[i]->LateUpdate_GameObject(fTimeDelta);
    }
}

void CRhino::Render_GameObject()
{
    if (m_tInfo.iCurHP <= 0)
    {
        return;
    }

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    LPDIRECT3DTEXTURE9 pCurrentTex = nullptr;

    switch (m_eState)
    {
    case RHINO_LAND:
    case RHINO_APPEAR:
    case RHINO_JUMP:
        pCurrentTex = m_pAppearTex;
        break;
    case RHINO_STOMPING:
        pCurrentTex = m_pTex;
        break;
    case RHINO_RUNNING:
        pCurrentTex = m_pRunTex;
        break;
    case RHINO_STUN:
        pCurrentTex = m_pStunTex;
        break;
    }

    if (pCurrentTex)
        m_pGraphicDev->SetTexture(0, pCurrentTex);

    m_pAnimatorCom->render();
    m_pColliderCom->Render_Buffer();
}

void CRhino::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() == L"Player")
    {
        m_pPlayer->SetPlayerCurHP(-1);
        m_pPlayer->IsInvincible();
    }

    //if (_pOther->GetObjectKey() == L"RhinoStone")
    //{
    //    m_eState = RHINO_STUN;
    //}

    return;
}

void CRhino::Stomping_Ani()
{
    //몬스터가 플레이어를 바라보는 방향 벡터가 있어야 함
    _vec3 vPlayerPos = m_pPlayer->GetPlayerPos();
    _vec3 vRhinoPos = m_pTransformCom->m_vInfo[INFO_POS];

    m_vLookDir = vPlayerPos - vRhinoPos;
    D3DXVec3Normalize(&m_vLookDir, &m_vLookDir); // 정규화

    float fMarginOfError = 0.1f;

    if (fabs(m_vLookDir.x) < fMarginOfError && m_vLookDir.z > 0)
    {
        // Back
        m_pAnimatorCom->Play(L"RhinoStompBack", true);
    }
    else if (fabs(m_vLookDir.x) < fMarginOfError && m_vLookDir.z < 0)
    {
        // Front
        m_pAnimatorCom->Play(L"RhinoStompFront", true);
    }
    else if (m_vLookDir.x > 0 && fabs(m_vLookDir.z) < fMarginOfError)
    {
        // Right
        m_pAnimatorCom->Play(L"RhinoStompRight", true);
    }
    else if (m_vLookDir.x < 0 && fabs(m_vLookDir.z) < fMarginOfError)
    {
        // Left
        m_pAnimatorCom->Play(L"RhinoStompLeft", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        // RT
        m_pAnimatorCom->Play(L"RhinoStompRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        // LT
        m_pAnimatorCom->Play(L"RhinoStompLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        // RB
        m_pAnimatorCom->Play(L"RhinoStompRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        // LB
        m_pAnimatorCom->Play(L"RhinoStompLB", true);
    }
}

void CRhino::RhinoState_Update(_float fTimeDelta)
{
    m_fTime += fTimeDelta;

    switch (m_eState)
    {
    case RHINO_APPEAR: // 1초 등장
        m_pAnimatorCom->Play(L"RhinoLand", false); // 착지

        //if (m_fTime >= 1.0f) //1초 기나면
        //{
        if (m_pAnimatorCom->GetAnimation()->IsFinish())
        {
            m_eState = RHINO_STOMPING; // 발구르기
            m_fTime = 0.0f;
        }
        break;

    case RHINO_STOMPING: // 3초 발구르기
        m_pTransformCom->m_vInfo[INFO_POS].y = 50.f;
        Stomping_Ani();

        if (m_fTime >= 3.0f) // 3초 지나면
        {
            m_eState = RHINO_RUNNING; // 들이박기
            m_fTime = 0.0f;
        }
        break;

    case RHINO_RUNNING: // 들이박기 3초
        m_pTransformCom->m_vInfo[INFO_POS].y = 55.f;
        Running_Ani();

        if (m_fTime >= 3.0f && m_pRhinoStone[9]->Get_Dead()) // 마지막 돌이 부서졌는지 확인
        {
            m_eState = RHINO_JUMP; // 부서졌다면 점프로 넘어가기
            m_fTime = 0.0f;

        }
        else if (m_fTime >= 3.0f && !m_pRhinoStone[9]->Get_Dead())
        {
            m_eState = RHINO_STOMPING; // 아니면 다시 발구르러 가기
            m_fTime = 0.0f;
        }
        break;

    case RHINO_JUMP: // 리노 점프해서 
        m_pAnimatorCom->Play(L"RhinoJump", true); // 점프(1회만 출력 / false)

        if (m_fTime >= 3.0f) //3초 뒤에 
        {
            m_eState = RHINO_LAND; // 착지
            m_fTime = 0.0f;
            m_bStoneAppear = false; // 새 생성 없던 일로ㅋ
            break;
        }

        if (m_bStoneAppear) // 이미 생성했으면. 
            break;

        for (size_t i = 0; i < 10; i++) // 스톤 타겟 잡는 애니 + 돌부서짐 + 하나만 남기 업데이트 
        {
            m_pRhinoStone[i]->Set_Appear();
        }
        m_bStoneAppear = true;

        break;

    case RHINO_LAND:
        m_pAnimatorCom->Play(L"RhinoLand", true); // 점프

        if (m_fTime >= 1.0f)
        {
            m_eState = RHINO_STOMPING; // 초기 자세로 대충 얘가 idle인듯
            m_fTime = 0.0f;
            for (size_t i = 0; i < 9; i++)
            {
                m_pRhinoStone[i]->Set_Dead(); // 나머지 9개 죽인 척 하기
            }
        }
        break;

    case RHINO_STUN:
        Stun_Ani();
        if (m_fTime >= 3.0f)
        {
            m_eState = RHINO_STOMPING;
            m_fTime = 0.0f;
        }

    }
}

void CRhino::Running_Ani()
{
    //m_pTransformCom->m_vInfo[INFO_POS] += (m_vLookDir * m_fSpeed);
    m_pTransformCom->m_vInfo[INFO_POS] += m_vLookDir * m_fSpeed;

    // 달리기 애니메이션 재생
    if (fabs(m_vLookDir.x) < 0.1f && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunBack", true);
    }
    else if (fabs(m_vLookDir.x) < 0.1f && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunFront", true);
    }
    else if (m_vLookDir.x > 0 && fabs(m_vLookDir.z) < 0.1f)
    {
        m_pAnimatorCom->Play(L"RhinoRunRight", true);
    }
    else if (m_vLookDir.x < 0 && fabs(m_vLookDir.z) < 0.1f)
    {
        m_pAnimatorCom->Play(L"RhinoRunLeft", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoRunLB", true);
    }
}

void CRhino::Stun_Ani()
{
    if (m_vLookDir.x > 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunRT", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z > 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunLT", true);
    }
    else if (m_vLookDir.x > 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunRB", true);
    }
    else if (m_vLookDir.x < 0 && m_vLookDir.z < 0)
    {
        m_pAnimatorCom->Play(L"RhinoStunLB", true);
    }
}


HRESULT CRhino::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 65.f, 65.f, 10.f };
    m_pTransformCom->Set_Pos(500.f, 50.f, 600.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    return S_OK;
}

CRhino* CRhino::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRhino* pMonster = new CRhino(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Rhino Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CRhino::Free()
{
    Engine::CGameObject::Free();
}
