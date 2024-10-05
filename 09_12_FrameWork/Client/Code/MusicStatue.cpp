#include "pch.h"
#include "MusicStatue.h"
#include "FirePit.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CMusicStatue::CMusicStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iCurNote(0), m_bIsClear(false)
{
}

CMusicStatue::~CMusicStatue()
{
}

HRESULT CMusicStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 4.5f, 4.f, 0.f };
	m_vecKeyOrder.push_back(0);
	m_vecKeyOrder.push_back(1);
	m_vecKeyOrder.push_back(2);
	m_vecKeyOrder.push_back(3);
	m_vecKeyOrder.push_back(4);
	m_vecKeyOrder.push_back(5);
	return S_OK;
}

_int CMusicStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);		

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMusicStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);	
}

void CMusicStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMusicStatue::Play_Music()
{
	
}

void CMusicStatue::Match_Note(_int _iNote)
{
	if (m_bIsClear)
		return;

	if (m_vecKeyOrder[m_iCurNote] != _iNote) {
		Reset_FirePit();
		return;
	}

	static_cast<CFirePit*>(m_vecFirePits[m_iCurNote])->Set_ImageID(1);
	++m_iCurNote;

	if (m_iCurNote >= m_vecFirePits.size()) {
		Clear_Puzzle();
		return;
	}
}

void CMusicStatue::Reset_FirePit()
{
	for (auto& iter : m_vecFirePits)
		static_cast<CFirePit*>(iter)->Set_ImageID(0);

	m_iCurNote = 0;
}

void CMusicStatue::Clear_Puzzle()
{
	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(4);
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock(true, _vec3{ 0.f, -3.f, 0.f });		
		static_cast<CStoneBlockHole*>(m_vecStoneBlocksHoles[i])->Set_ImageID(1);
	}

	m_bIsClear = true;
}

HRESULT CMusicStatue::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MusicStatue"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CMusicStatue* CMusicStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMusicStatue* pMusicStatue = new CMusicStatue(pGraphicDev);

	if (FAILED(pMusicStatue->Ready_GameObject()))
	{
		Safe_Release(pMusicStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pMusicStatue;
}

void CMusicStatue::Free()
{
	Engine::CGameObject::Free();
}
