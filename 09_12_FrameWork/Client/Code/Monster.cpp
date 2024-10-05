#include "pch.h"
#include "Monster.h"
#include "Define.h"
#include "Export_System.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_vKnockBackDir(_vec3(0.f, 0.f, 0.f))
    , m_bKnockBackTrigger(false)
    , m_bInvincible(false)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };

    SetObjectType(OBJ_TYPE::DESTROY_ABLE);

    LoadTextureFromFile(m_pGraphicDev,
        "../Bin/Resource/Texture/puzzle/Sprite_StonePushable.png",
        &m_Texture);

    m_tMonsterHP.iCurHP = 3;
    m_tMonsterHP.iMaxHP = 3;

    return S_OK;
}

void CMonster::LateReady_GameObject()
{
    Engine::CGameObject::LateReady_GameObject();
    CTransform* PlayerTrasform = static_cast<Engine::CTransform*>(
        m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    _vec3 PlayerPos;
    PlayerTrasform->Get_Info(INFO_POS, &PlayerPos);
    PlayerPos.x += 70;
    m_pTransformCom->Set_Pos(PlayerPos);
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);

    if (m_tMonsterHP.iCurHP == 0)
    {
        return 0;
    }
    DurationInvincible(fTimeDelta);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    KnockBack(fTimeDelta, m_vKnockBackDir);
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonster::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetTexture(0, m_Texture);
    m_pBufferCom->Render_Buffer();
    if (!m_bInvincible)
        m_pBoundBox->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  // 이거 설정안해주면 안됨 전역적으로 장치세팅이 저장되기 때문에
    m_pGraphicDev->SetTexture(0, NULL);  // 이거 설정안해주면 그대로 텍스처 나옴 이것도 마찬가지로 전역적으로 장치세팅이 되므로
}

void CMonster::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
        return;

    m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
    if (m_CPlayer->GetSwingTrigger() && !m_bInvincible)
    {
        m_vKnockBackDir = m_CPlayer->GetPlayerDirVector2();
        m_bKnockBackTrigger = true;
        m_bInvincible = true;
        SetMonsterCurHP(-1);
    }
}

void CMonster::KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir)
{
    if (!m_bKnockBackTrigger)
        return;

    static float distance = 0.f;
    static float speed = 30.f;
    static float tickTime = 0.f;

    if (speed <= 10)
    {
        speed = 30.f;
        distance = 0.f;
        tickTime = 0.f;
        m_vKnockBackDir = { 0.f, 0.f, 0.f };
        m_bKnockBackTrigger = false;
        return;
    }

    tickTime += fTimeDelta;
    if (tickTime >= 0.5f)
    {
        tickTime = 0.f;
        speed /= 2.f;
    }

    CTransform* transform =
        dynamic_cast<CTransform*>(Get_Component(ID_DYNAMIC, L"Com_Transform"));
    transform->Move_Pos(&m_vKnockBackDir, fTimeDelta, speed);
}

void CMonster::DurationInvincible(const _float& fTimeDelta)
{
    if (!m_bInvincible)
        return;

    static float fDurationTime = 0.f;
    fDurationTime += fTimeDelta;
    if (fDurationTime >= 3.f)
    {
        fDurationTime = 0.f;
        m_bInvincible = false;
    }
}
HRESULT CMonster::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBoundBox->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonster* pMonster = new CMonster(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("Monster Create Failed");
        return nullptr;
    }

    return pMonster;
}

void CMonster::Free()
{
    Engine::CGameObject::Free();
}
