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
    //m_pTextureCom[EQUIP].resize(EQUIP_END);
    //m_pTextureCom[CONSUM].resize(CONSUM_END);
    //m_pTextureCom[OTHER].resize(OTHER_END);
    //m_pTextureCom[QUEST].resize(QUEST_END);

    //m_pTransformCom[EQUIP].resize(EQUIP_END);
    //m_pTransformCom[CONSUM].resize(CONSUM_END);
    //m_pTransformCom[OTHER].resize(OTHER_END);
    //m_pTransformCom[QUEST].resize(QUEST_END);

//    ITEM_INFO   tInfo = { EQUIP,
//          L"../Bin/Resource/Texture/Item/Explore.png",
//          L"탐험 모자",
//          L"탐험할 때 필수로 챙기는 모자. \
//`           탐험가가 된 기분..!!! \
//            끼얏 -? 호!",
//              0 };
//    m_ItemArray[tInfo.tType].push_back(tInfo);

    /*tInfo = { EQUIP,
          L"../Bin/Resource/Texture/Item/Mini.png",
          L"파티 모자",
          L"응원하기 좋은 모자.",
          0 };
    m_ItemArray[tInfo.tType].push_back(tInfo);

    tInfo = { CONSUM,
          L"../Bin/Resource/Texture/Item/SmallFruit.png",
          L"건강 열매",
          L"3등급 열매. \
            작지만 맛있다.",
          100 };
    m_ItemArray[tInfo.tType].push_back(tInfo);

    tInfo = { CONSUM,
          L"../Bin/Resource/Texture/Item/MiddleFruit.png",
          L"든든 열매",
          L"2등급 열매. \
            상큼한 과즙이 입안에 퍼진다.",
          200 };
    m_ItemArray[tInfo.tType].push_back(tInfo);

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
