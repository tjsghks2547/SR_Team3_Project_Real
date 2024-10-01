#include "pch.h"
#include "FirePit.h"
#include "Export_Utility.h"

CFirePit::CFirePit(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFirePit::~CFirePit()
{
}

HRESULT CFirePit::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 3.f, 4.f, 3.f };
	m_iImageID = 0;
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_FirePlace.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_FirePit.png", &m_vecTexture[1]);
	return S_OK;
}

_int CFirePit::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFirePit::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFirePit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFirePit::Add_Component()
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

	return S_OK;
}

CFirePit* CFirePit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFirePit* pFirePit = new CFirePit(pGraphicDev);

	if (FAILED(pFirePit->Ready_GameObject()))
	{
		Safe_Release(pFirePit);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pFirePit);
	return pFirePit;
}

void CFirePit::Free()
{
	Engine::CGameObject::Free();
}
