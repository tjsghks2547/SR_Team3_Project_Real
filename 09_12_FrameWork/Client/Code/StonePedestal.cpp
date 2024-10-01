#include "pch.h"
#include "StonePedestal.h"
#include "Export_Utility.h"

CStonePedestal::CStonePedestal(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CStonePedestal::~CStonePedestal()
{
}

HRESULT CStonePedestal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTexTransformCom->m_vScale = { 6.f, 6.f, 6.f };
	m_pTransformCom->m_vScale = { 6.f, 6.f, 6.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	return S_OK;
}

_int CStonePedestal::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CStonePedestal::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStonePedestal::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStonePedestal::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePedestal"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CStonePedestal* CStonePedestal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStonePedestal* pStonePedestal = new CStonePedestal(pGraphicDev);

	if (FAILED(pStonePedestal->Ready_GameObject()))
	{
		Safe_Release(pStonePedestal);
		MSG_BOX("pStonePedestal Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStonePedestal);
	return pStonePedestal;
}

void CStonePedestal::Free()
{
	Engine::CGameObject::Free();
}