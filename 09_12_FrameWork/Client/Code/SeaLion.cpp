#include "pch.h"
#include "SeaLion.h"

CSeaLion::CSeaLion(LPDIRECT3DDEVICE9 pGraphicDev)
    :CStoreNPC(pGraphicDev)
{
}

CSeaLion::~CSeaLion()
{
}

HRESULT CSeaLion::Ready_GameObject()
{
    CStoreNPC::Ready_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/SeaLion.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"SeaLion", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7);

    m_tInfo.pName = L"꼬치 바다사자";
    m_tInfo.pContent = L"생선꼬치 맜있어요. 사주세요.";

    return S_OK;
}

void CSeaLion::LateReady_GameObject()
{
    CStoreNPC::LateReady_GameObject();
}

_int CSeaLion::Update_GameObject(const _float& fTimeDelta)
{

    CStoreNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CSeaLion::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSeaLion::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"SeaLion", true);
    m_pAnimatorCom->render();
    m_pColliderCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShopTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();
}

void CSeaLion::OnCollision(CGameObject* _pOther)
{
}

void CSeaLion::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSeaLion::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CSeaLion::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FishShop"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TextureFishShop", pComponent });

    pComponent = m_pShopTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFishShop", pComponent });
    m_pShopTransformCom->m_vScale = { 30.f, 30.f, 30.f };
    m_pShopTransformCom->Set_Pos(300.f, 30.f, 798.f);

    //-여기까지 텍스쳐-

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
    m_pTransformCom->Set_Pos(300.f, 25.f, 800.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    return S_OK;
}

CSeaLion* CSeaLion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSeaLion* pNPC = new CSeaLion(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CSeaLion Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CSeaLion::Free()
{
    Engine::CGameObject::Free();
}
