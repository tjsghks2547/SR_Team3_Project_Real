#include "pch.h"
#include "UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CUI::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPFRAME]->Get_WorldMatrix());
	m_pTextureCom[HPFRAME]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBAR]->Get_WorldMatrix());
	m_pTextureCom[HPBAR]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBACK]->Get_WorldMatrix());
	m_pTextureCom[HPBACK]->Set_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CUI::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//HPFRAME
	pComponent = m_pTextureCom[HPFRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureFrame", pComponent });

	pComponent = m_pTransformCom[HPFRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFrame", pComponent });
	m_pTransformCom[HPFRAME]->m_vInfo[INFO_POS] = { -450.f, 310.f, 0.1f }; // UI 위치긴한디 상수로 넣어도 됨?ㅇㅅㅇ
	m_pTransformCom[HPFRAME]->m_vScale = { 174.f, 30.f, 0.f }; // 해당 UI의 스케일

	//HPBAR
	pComponent = m_pTextureCom[HPBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBar", pComponent });

	pComponent = m_pTransformCom[HPBAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBar", pComponent });
	m_pTransformCom[HPBAR]->m_vInfo[INFO_POS] = { -420.f, 310.f, 0.2f };
	m_pTransformCom[HPBAR]->m_vScale = { 130.5f, 17.f, 0.f };

	//HPBACK
	pComponent = m_pTextureCom[HPBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBarBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBack", pComponent });

	pComponent = m_pTransformCom[HPBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBack", pComponent });
	m_pTransformCom[HPBACK]->m_vInfo[INFO_POS] = { -420.f, 310.f, 0.3f };
	m_pTransformCom[HPBACK]->m_vScale = { 134.f, 19.f, 0.f };

	return S_OK;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI* pUI = new CUI(pGraphicDev);

	if (FAILED(pUI->Ready_GameObject()))
	{
		Safe_Release(pUI);
		MSG_BOX("UI Create Failed");
		return nullptr;
	}

	return pUI;
}

void CUI::Free()
{
	Engine::CGameObject::Free();
}
