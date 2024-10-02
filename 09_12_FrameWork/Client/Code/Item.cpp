#include "pch.h"
#include "Item.h"
#include "Player.h" //0924

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
    //m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    //NULL_CHECK_RETURN(m_pPlayer, 0);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItem::Render_GameObject()
{
}

void CItem::Render_QuickItem()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pQuickTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    if (m_tInfo.eType == CONSUM)
    {
        _vec2 vCountPos;

        vCountPos.x = m_pQuickTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 10;
        vCountPos.y = -(m_pQuickTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 10;

        wchar_t Division[32] = L"x";
        wchar_t ItemCount[32];

        swprintf(ItemCount, 32, L"%d", m_tInfo.iItemCount);

        wcscat_s(Division, 32, ItemCount);   // "x + °³¼ö"
        //Engine::Render_Font(L"Font_Ogu10", Division, &vCountPos, D3DXCOLOR(0.f, 0.1f, 0.1f, 1.f));
        Engine::Render_Font(L"Font_OguBold14", Division, &vCountPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
}

void CItem::Render_ItemView()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pViewTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();
}

HRESULT CItem::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pViewTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformItemView", pComponent });
    m_pViewTransformCom->m_vScale = { 100.f, 100.f, 1.f };
    m_pViewTransformCom->m_vInfo[INFO_POS] = { 430.f, 100.f, 0.1f };

    return S_OK;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CItem* pItem = new CItem(pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Item Create Failed");
        return nullptr;
    }

    return pItem;
}

void CItem::Free()
{
    Engine::CGameObject::Free();
}
