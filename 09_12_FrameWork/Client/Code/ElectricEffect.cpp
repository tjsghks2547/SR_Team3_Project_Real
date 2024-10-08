#include "pch.h"
#include "ElectricEffect.h"
#include "Export_Utility.h"

CElectricEffect::CElectricEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
    
}

CElectricEffect::~CElectricEffect()
{
}

HRESULT CElectricEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ElectriceelBoss/Sprite_ThunderVertical.png", &m_pTexture);

    m_pAnimatorCom->CreateAnimation(L"Lighting_Y", m_pTexture, _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.1f, 3);   
    m_pAnimatorCom->Play(L"Lighting_Y",false);

    m_pTransformCom->Set_Pos(500.f, 100.f, 500.f);
    m_pTransformCom->m_vScale = { 75.f, 100.f, 20.f }; 
    return S_OK;
}

_int CElectricEffect::Update_GameObject(const _float& fTimeDelta)
{


    Add_RenderGroup(RENDER_ALPHA, this);    
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
 
}

void CElectricEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CElectricEffect::Render_GameObject()
{

    if (m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        Engine::CGameObject::Free();
        map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
        //그니깐 레이어에서 삭제해줘야한다는것. 
        CLayer* pLayer = nullptr;

        for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
        {
            const _tchar* layerKey = iter->first;

            if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
            {
                pLayer = iter->second;
            }
        }
        map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();

        auto it = pMap.find(L"Electric_Effect");


        if (it != pMap.end())
        {
            pMap.erase(it);
        }


    }


    else
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

        m_pGraphicDev->SetTexture(0, m_pTexture);

        m_pAnimatorCom->render();

        //맵툴 작성시 끄기 
        //m_pBoundBox->Render_Buffer();
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }

}

HRESULT CElectricEffect::Add_Component()
{
    CComponent* pComponent = NULL;  

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBoundBox->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    return S_OK;
}

CElectricEffect* CElectricEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CElectricEffect* pElectricEffect = new CElectricEffect(pGraphicDev);    

    if (FAILED(pElectricEffect->Ready_GameObject()))
    {
        Safe_Release(pElectricEffect);
        MSG_BOX("pObject Create Failed");
        return nullptr;
    }

    return pElectricEffect; 
}

void CElectricEffect::Free()
{
    Engine::CGameObject::Free();
}

void CElectricEffect::OnCollision(CGameObject* _pOther)
{
    int a = 4;
}
