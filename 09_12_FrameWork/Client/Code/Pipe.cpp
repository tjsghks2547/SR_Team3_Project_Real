#include "pch.h"
#include "Pipe.h"
#include "Export_Utility.h"

CPipe::CPipe(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPipe::~CPipe()
{
}

HRESULT CPipe::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 6.f, 6.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);
	m_pTransformCom->Rotation(ROT_Y, 90.f * 3.14f / 180.f);
	m_pTransformCom->Set_Pos(0.f, 0.02f, -12.2f);
	return S_OK;
}

_int CPipe::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPipe::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPipe::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPipe::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });
		
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CPipe* CPipe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPipe* pPipe = new CPipe(pGraphicDev);

	if (FAILED(pPipe->Ready_GameObject()))
	{
		Safe_Release(pPipe);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pPipe;
}

void CPipe::Free()
{
}