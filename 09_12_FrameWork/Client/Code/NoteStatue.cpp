#include "pch.h"
#include "NoteStatue.h"
#include "MusicStatue.h"
#include "Export_Utility.h"

CNoteStatue::CNoteStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CNoteStatue::~CNoteStatue()
{
}

HRESULT CNoteStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 3.f, 4.f, 0.f };
	return S_OK;
}

_int CNoteStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CNoteStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CNoteStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CNoteStatue::Play_Note()
{
	static_cast<CMusicStatue*>(m_pGroup)->Match_Note(m_iNoteID);
}

HRESULT CNoteStatue::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_NoteStatue"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CNoteStatue* CNoteStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNoteStatue* pNoteStatue = new CNoteStatue(pGraphicDev);

	if (FAILED(pNoteStatue->Ready_GameObject()))
	{
		Safe_Release(pNoteStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pNoteStatue;
}

void CNoteStatue::Key_Input(const _float& fTimeDelta)
{
	if (Engine::GetKeyUp(DIK_1))
	{
		if (m_iNoteID == 0) {
			Play_Note();
		}
	}

	if (Engine::GetKeyUp(DIK_2))
	{
		if (m_iNoteID == 1) {
			Play_Note();
		}
	}

	if (Engine::GetKeyUp(DIK_3))
	{
		if (m_iNoteID == 2) {
			Play_Note();
		}
	}

	if (Engine::GetKeyUp(DIK_4))
	{
		if (m_iNoteID == 3) {
			Play_Note();
		}
	}

	if (Engine::GetKeyUp(DIK_5))
	{
		if (m_iNoteID == 4) {
			Play_Note();
		}
	}

	if (Engine::GetKeyUp(DIK_6))
	{
		if (m_iNoteID == 5) {
			Play_Note();
		}
	}
}

void CNoteStatue::Free()
{
	Engine::CGameObject::Free();
}
