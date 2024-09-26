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
	m_pTransformCom->m_vScale = { 3.f, 4.f, 0.f };
	m_bIs = false;
	return S_OK;
}

_int CMonkeyStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);
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

void CMonkeyStatue::Active_StoneBlock()
{
	for (int i = 0; i < m_vecStoneBlocks.size(); i++)
	{
		if (m_bIs) {
			static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock(true, _vec3{0.f, 3.f, 0.f});
			static_cast<CStoneBlockHole*>(m_vecStoneBlocksHoles[i])->Set_ImageID(0);
		}
		else {
			static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock(true, _vec3{ 0.f, -3.f, 0.f });
			static_cast<CStoneBlockHole*>(m_vecStoneBlocksHoles[i])->Set_ImageID(1);
		}
	}

	m_bIs = m_bIs ? false : true;
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

	return pMonkeyStatue;
}

void CMonkeyStatue::Key_Input(const _float& fTimeDelta)
{
	if (Engine::GetKeyUp(DIK_4))
	{
		//Active_StoneBlock();			
	}
}

void CMonkeyStatue::Free()
{
	Engine::CGameObject::Free();
}
