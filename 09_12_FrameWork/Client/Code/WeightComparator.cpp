#include "pch.h"
#include "WeightComparator.h"
#include "Stone.h"
#include "StonePedestal.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CWeightComparator::CWeightComparator(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_iImageID(0)
{
}

CWeightComparator::~CWeightComparator()
{
}

HRESULT CWeightComparator::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 30.f, 6.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_iImageID = 0;
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Weight_Comparator_empty.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Weight_Comparator_Anim02.png", &m_vecTexture[1]);

	return S_OK;
}

_int CWeightComparator::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CWeightComparator::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWeightComparator::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWeightComparator::Set_PedestalPos(float _fX, float _fZ)
{
	static_cast<Engine::CTransform*>(m_vecPedestal[0]->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX - 22.5f, .05f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[1]->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX - 7.5f, .05f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[2]->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX + 7.5f, .05f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[3]->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX + 22.5f, .05f, _fZ);

	static_cast<Engine::CTransform*>(m_vecPedestal[0]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX - 22.5f, 6.f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[1]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX - 7.5f, 6.f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[2]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + 7.5f, 6.f, _fZ);
	static_cast<Engine::CTransform*>(m_vecPedestal[3]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + 22.5f, 6.f, _fZ);

	static_cast<Engine::CTransform*>(m_vecStone[0]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX - 22.5f, 4.f, _fZ - 17.f);
	static_cast<Engine::CTransform*>(m_vecStone[1]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX - 7.5f, 3.f, _fZ - 1000.f);
	static_cast<Engine::CTransform*>(m_vecStone[2]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + 7.5f, 3.f, _fZ - 1000.f);
	static_cast<Engine::CTransform*>(m_vecStone[3]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + 22.5f, 3.f, _fZ - 1000.f);
}

void CWeightComparator::Puzzle_Clear()
{
	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(4);
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock(true, _vec3{ 0.f, -3.f, 0.f });
		static_cast<CStoneBlockHole*>(m_vecStoneBlocksHoles[i])->Set_ImageID(1);
	}
}

HRESULT CWeightComparator::Add_Component()
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

CWeightComparator* CWeightComparator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeightComparator* pCrystal = new CWeightComparator(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CWeightComparator::Free()
{
	Engine::CGameObject::Free();
}

void CWeightComparator::Match_Puzzle()
{
	for (int i = 0; i < m_vecPedestal.size(); ++i)
	{
		int j = static_cast<CStone*>(static_cast<CStonePedestal*>(m_vecPedestal[i])->Get_Stone())->Get_StoneID();
		if (j != i)
			return;
	}

	Puzzle_Clear();
}
