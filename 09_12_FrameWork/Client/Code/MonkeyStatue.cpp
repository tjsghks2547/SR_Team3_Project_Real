#include "pch.h"
#include "MonkeyStatue.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CMonkeyStatue::CMonkeyStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMonkeyStatue::~CMonkeyStatue()
{
}

HRESULT CMonkeyStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 18.f, 22.f, 18.f };
	m_bIs = false;
	return S_OK;
}

_int CMonkeyStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMonkeyStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonkeyStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMonkeyStatue::OnCollision(CGameObject* _pOther)
{
}

void CMonkeyStatue::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		Active_StoneBlock();
	}
}

void CMonkeyStatue::OnCollisionExit(CGameObject* _pOther)
{
}

void CMonkeyStatue::Active_StoneBlock()
{
	for (int i = 0; i < m_vecStoneBlocks.size(); i++)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock();		
	}
}

HRESULT CMonkeyStatue::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonkeyStatue"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CMonkeyStatue* CMonkeyStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonkeyStatue* pMonkeyStatue = new CMonkeyStatue(pGraphicDev);

	if (FAILED(pMonkeyStatue->Ready_GameObject()))
	{
		Safe_Release(pMonkeyStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pMonkeyStatue);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pMonkeyStatue);
	return pMonkeyStatue;
}

void CMonkeyStatue::Free()
{
	Engine::CGameObject::Free();
}
