#include "pch.h"
#include "../Header/InvenUI.h"

#include "Player.h"
#include "ItemSelector.h"

CInvenUI::CInvenUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_bCursorCreate(false), m_pItemSelector(nullptr)
{
}

CInvenUI::~CInvenUI()
{
}

HRESULT CInvenUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 인벤토리 간격
    m_InvenInterval = { 145.7f, 137.f };
    // 인벤토리 첫번째 칸 위치
    m_vInvenPos = { -459.5f, 39.5f, 0.1f };
    // 인벤토리 초기 선택 필터
    m_tItemFilter = CItem::EQUIP;

    //0913 레이트 레디로 보내야 함 일단 업데이트로 보내놨다
    //m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    //NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

    return S_OK;

}

_int CInvenUI::Update_GameObject(const _float& fTimeDelta)
{

    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer, 0);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_pPlayer->GetPlayerInven())
    {

        Engine::Add_RenderGroup(RENDER_UI, this);
        Key_Input(fTimeDelta);

        for (auto& pItem : m_ItemList[m_tItemFilter])
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
    }

    return iExit;
}

void CInvenUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    // 나중애 z비교 함수 만들기....;
    m_fViewZ = 0.9f;
}

void CInvenUI::Render_GameObject()
{
    if (m_pPlayer->GetPlayerInven())
    {
        _vec2 HpPosition(556.f, 15.f);

        Engine::Render_Font(L"Font_Ogu48", L"인벤토리", &HpPosition, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[BACKGROUND]->Get_WorldMatrix());
        m_pTextureCom[BACKGROUND]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[DEFAULT]->Get_WorldMatrix());
        m_pTextureCom[DEFAULT]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[FRAME]->Get_WorldMatrix());
        m_pTextureCom[FRAME]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        // 아이콘에 알파값이 들어가야 함 -> 나중에 해야지 ㅋㅋ
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[EQUIP]->Get_WorldMatrix());
        m_pTextureCom[EQUIP]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[CONSUM]->Get_WorldMatrix());
        m_pTextureCom[CONSUM]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[OTHER]->Get_WorldMatrix());
        m_pTextureCom[OTHER]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[QUEST]->Get_WorldMatrix());
        m_pTextureCom[QUEST]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[ICONBUTTON]->Get_WorldMatrix());
        m_pTextureCom[ICONBUTTON]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        // 인벤토리 아이템 출력

        for (auto& pItem : m_ItemList[m_tItemFilter])
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
}

HRESULT CInvenUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //Background
    pComponent = m_pTextureCom[BACKGROUND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenBack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenBack", pComponent });

    pComponent = m_pTransformCom[BACKGROUND] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenBack", pComponent });
    m_pTransformCom[BACKGROUND]->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.9f };
    m_pTransformCom[BACKGROUND]->m_vScale = { 640.f, 360.f, 1.f };

    //InvenDefault
    pComponent = m_pTextureCom[DEFAULT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenDefault"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenDefault", pComponent });

    pComponent = m_pTransformCom[DEFAULT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenDefault", pComponent });
    m_pTransformCom[DEFAULT]->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.2f };
    m_pTransformCom[DEFAULT]->m_vScale = { 640.f, 360.f, 1.f };

    //InvenFrame
    pComponent = m_pTextureCom[FRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenFrame"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenFrame", pComponent });

    pComponent = m_pTransformCom[FRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenFrame", pComponent });
    m_pTransformCom[FRAME]->m_vInfo[INFO_POS] = { -165.f, -100.f, 0.2f };
    m_pTransformCom[FRAME]->m_vScale = { 382.f, 222.f, 1.f };

    //InvenIcon-Equip
    pComponent = m_pTextureCom[EQUIP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EquipIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenFrame", pComponent });

    pComponent = m_pTransformCom[EQUIP] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformEquipIcon", pComponent });
    m_pTransformCom[EQUIP]->m_vInfo[INFO_POS] = { -470.f, 157.f, 0.2f };
    m_pTransformCom[EQUIP]->m_vScale = { 38.f, 38.f, 1.f };

    //InvenIcon-Consum
    pComponent = m_pTextureCom[CONSUM] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ConsumIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureConsumIcon", pComponent });

    pComponent = m_pTransformCom[CONSUM] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformConsumIcon", pComponent });
    m_pTransformCom[CONSUM]->m_vInfo[INFO_POS] = { -390.f, 160.f, 0.2f };
    m_pTransformCom[CONSUM]->m_vScale = { 40.f, 40.f, 1.f };

    //InvenIcon-Other
    pComponent = m_pTextureCom[OTHER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_OtherIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureOtherIcon", pComponent });

    pComponent = m_pTransformCom[OTHER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformOtherIcon", pComponent });
    m_pTransformCom[OTHER]->m_vInfo[INFO_POS] = { -310.f, 161.f, 0.2f };
    m_pTransformCom[OTHER]->m_vScale = { 40.f, 40.f, 1.f };

    //InvenIcon-Other
    pComponent = m_pTextureCom[QUEST] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_QuestIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureQuestIcon", pComponent });

    pComponent = m_pTransformCom[QUEST] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuestIcon", pComponent });
    m_pTransformCom[QUEST]->m_vInfo[INFO_POS] = { -230.f, 162.f, 0.2f };
    m_pTransformCom[QUEST]->m_vScale = { 40.f, 40.f, 1.f };

    //ICONBUTTON
    pComponent = m_pTextureCom[ICONBUTTON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_IconButton"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureIconButton", pComponent });

    pComponent = m_pTransformCom[ICONBUTTON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformIconButton", pComponent });
    m_pTransformCom[ICONBUTTON]->m_vInfo[INFO_POS] = { -351.f, 162.f, 0.2f };
    m_pTransformCom[ICONBUTTON]->m_vScale = { 190.f, 16.f, 1.f };

    return S_OK;
}

void CInvenUI::Key_Input(const _float& fTimeDelta)
{
    if (Engine::GetKeyDown(DIK_UP))
    {
    }
    if (Engine::GetKeyDown(DIK_DOWN))
    {
        if (m_bCursorCreate == false)
        {
            m_bCursorCreate = true;
            m_pItemSelector = dynamic_cast<CItemSelector*>(CItemSelector::Create(m_pGraphicDev));
        }
    }
    if (Engine::GetKeyDown(DIK_LEFT))
    {
    }
    if (Engine::GetKeyDown(DIK_RIGHT))
    {
    }
    if (Engine::GetKeyDown(DIK_F))
    {
        m_tItemFilter = CItem::CONSUM;
    }

}

CInvenUI* CInvenUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CInvenUI* pUI = new CInvenUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("InvenUI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CInvenUI::Free()
{
    Engine::CGameObject::Free();
}
