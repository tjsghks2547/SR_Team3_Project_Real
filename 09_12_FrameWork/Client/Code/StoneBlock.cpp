#include "pch.h"
#include "StoneBlock.h"
#include "Export_Utility.h"

CStoneBlock::CStoneBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsMove(false)
{
}

CStoneBlock::~CStoneBlock()
{
}

HRESULT CStoneBlock::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 3.f, 3.f, 0.f };	
	m_iImageID = 0;
	m_vecTexture.resize(5);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce_Clear.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_OceanStoneBlockOnce.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_OceanStoneBlockOnce_Clear.png", &m_vecTexture[4]);
	return S_OK;
}

_int CStoneBlock::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bIsMove) {
		_vec3 vPos, vMovePos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_fTargetPos.x = vPos.x;
		m_fTargetPos.z = vPos.z;
		vMovePos = m_fTargetPos - vPos;

		m_pTransformCom->Move_Pos(&vMovePos, fTimeDelta, 4.f);
	}

	return iExit;
}

void CStoneBlock::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoneBlock::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStoneBlock::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StoneBlock"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	
	return S_OK;
}

CStoneBlock* CStoneBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStoneBlock* pStoneBlock = new CStoneBlock(pGraphicDev);

	if (FAILED(pStoneBlock->Ready_GameObject()))
	{
		Safe_Release(pStoneBlock);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pStoneBlock;
}

void CStoneBlock::Free()
{
	Engine::CGameObject::Free();
}