#include "pch.h"
#include "NPC.h"

CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CNPC::~CNPC()
{
}

HRESULT CNPC::Ready_GameObject()
{
	return S_OK;
}

_int CNPC::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CNPC::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CNPC::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CNPC::Add_Component()
{
	return S_OK;
}

CNPC* CNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNPC* pNPC = new CNPC(pGraphicDev);

	if (FAILED(pNPC->Ready_GameObject()))
	{
		Safe_Release(pNPC);
		MSG_BOX("pNPC Create Failed");
		return nullptr;
	}

	return pNPC;
}

void CNPC::Free()
{
	Engine::CGameObject::Free();
}
