#include "pch.h"
#include "RhinoEffect.h"
#include "ResMgr.h" 
#include "Export_Utility.h"

CRhinoEffect::CRhinoEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , name(L"")
    , m_bEffect(false)
{
}

CRhinoEffect::~CRhinoEffect()
{
}

HRESULT CRhinoEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    //D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/Effect30.png", &m_pTex);
    m_pTex = CResMgr::GetInstance()->GetEffectTexture()[30];
    m_fViewZ = 0.1f;
    m_pAnimatorCom->CreateAnimation(L"RhinoDust", m_pTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 128.f), 0.5f, 3);

    return S_OK;
}

_int CRhinoEffect::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_TRANSLUCNET, this);
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CRhinoEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRhinoEffect::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"RhinoDust", false);

    m_pAnimatorCom->render();	

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


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

        auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(name.c_str()));

        if (iter != pMap.end())
        {
            pMap.erase(iter);
        }
    }


}

HRESULT CRhinoEffect::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 50.f, 50.f, 10.f };

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Attack_Area"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });



    return S_OK;
}

CRhinoEffect* CRhinoEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRhinoEffect* pRhinoEffect = new CRhinoEffect(pGraphicDev);

    if (FAILED(pRhinoEffect->Ready_GameObject()))
    {
        Safe_Release(pRhinoEffect);
        MSG_BOX("RhinoEffect Create Failed");
        return nullptr;
    }

    return pRhinoEffect;

}

void CRhinoEffect::Free()
{
    Engine::CGameObject::Free();
}

