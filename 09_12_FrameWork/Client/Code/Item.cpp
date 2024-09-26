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
    /*

        tInfo = { CONSUM,
              L"../Bin/Resource/Texture/Item/BigFruit.png",
              L"튼튼 열매",
              L"1등급 열매. \
                크다. 비싸다.",
              500 };
        m_ItemArray[tInfo.tType].push_back(tInfo);

        tInfo = { OTHER,
              L"../Bin/Resource/Texture/Item/Leaf.png",
              L"나뭇잎",
              L"어디서든 쉽게 볼 수 있는 나뭇잎.",
              10 };
        m_ItemArray[tInfo.tType].push_back(tInfo);

        tInfo = { OTHER,
              L"../Bin/Resource/Texture/Item/Branch.png",
              L"나뭇가지",
              L"어디서든 쉽게 볼 수 있는 나뭇가지.",
              10 };
        m_ItemArray[tInfo.tType].push_back(tInfo);

        tInfo = { QUEST,
              L"../Bin/Resource/Texture/Item/Net.png",
              L"잠자리 채",
              L"아기 오구가 아끼는 잠자리 채.",
              0 };
        m_ItemArray[tInfo.tType].push_back(tInfo);

        tInfo = { QUEST,
              L"../Bin/Resource/Texture/Item/DarwTool.png",
              L"그림 도구",
              L"그림을 그릴 수 있는 도구.",
              0 };
        m_ItemArray[tInfo.tType].push_back(tInfo);*/

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

        wcscat_s(Division, 32, ItemCount);   // "x + 개수"
        //Engine::Render_Font(L"Font_Ogu10", Division, &vCountPos, D3DXCOLOR(0.f, 0.1f, 0.1f, 1.f));
        Engine::Render_Font(L"Font_OguBold14", Division, &vCountPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
}

HRESULT CItem::Add_Component()
{
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
