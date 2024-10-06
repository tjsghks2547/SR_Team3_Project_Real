#include "pch.h"
#include "StoreUI.h"

#include "Player.h"
#include "ItemSelector.h"
CStoreUI::CStoreUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pItemSelector(nullptr)
{
}

CStoreUI::~CStoreUI()
{
}

HRESULT CStoreUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 인벤토리 간격
    m_InvenInterval = { 145.7f, 137.f };
    m_fViewZ = 0.9f;

    return S_OK;
}

void CStoreUI::LateReady_GameObject()
{
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);

    m_pItemSelector = dynamic_cast<CItemSelector*>(CItemSelector::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pItemSelector);
}

_int CStoreUI::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);
    Key_Input(fTimeDelta);

    for (auto& pItem : m_ItemList)
    {
        if (pItem == nullptr)
            continue;

        pItem->Update_GameObject(fTimeDelta);
        pItem->LateUpdate_GameObject(fTimeDelta);

    }

    if (m_pItemSelector)
    {
        m_pItemSelector->Update_GameObject(fTimeDelta);
        m_pItemSelector->LateUpdate_GameObject(fTimeDelta);
    }

    return iExit;
}

void CStoreUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    // 나중애 z비교 함수 만들기....;
    m_fViewZ = 0.9f;
}

void CStoreUI::Render_GameObject()
{
    _vec2 HpPosition(556.f, 15.f);

    Engine::Render_Font(L"Font_OguBold48", L"상점", &HpPosition, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();


    // 상점 아이템 출력
    for (auto& pItem : m_ItemList)
    {
        if (pItem == nullptr)
            continue;

        pItem->Render_GameObject();
    }
    if (m_pItemSelector)
    {
        m_pItemSelector->Render_GameObject();
    }

}

void CStoreUI::Add_Item(CItem* _Item)
{
    m_ItemList.push_back(_Item); // 해당 필터 벡터 뒤에 넣어줌
    Set_InvenPos(m_ItemList.size() - 1);
    _Item->Set_ItemPos(m_vInvenPos); //인벤 위치를 저장해둔 
}

HRESULT CStoreUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //Background
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StoreUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureStoreUI", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformStoreUI", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.9f };
    m_pTransformCom->m_vScale = { 640.f, 360.f, 1.f };

}

void CStoreUI::Key_Input(const _float& fTimeDelta)
{
    if (Engine::GetKeyDown(DIK_D))
    {
        m_ItemList.clear();
    }
}

CStoreUI* CStoreUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStoreUI* pUI = new CStoreUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("StoreUI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CStoreUI::Free()
{
    Engine::CGameObject::Free();
}
