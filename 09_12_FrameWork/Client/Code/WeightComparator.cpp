#include "pch.h"
#include "StonePedestal.h"
#include "WeightComparator.h"
#include "Stone.h"
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
	m_pTransformCom->m_vScale = { 120.f, 25.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Weight_Comparator_empty.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Weight_Comparator_Clear.png", &m_vecTexture[1]);

	return S_OK;
}

_int CWeightComparator::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	for (auto& iter: m_vecPedestals)	
		iter->Update_GameObject(fTimeDelta);

	for (auto& iter : m_vecStones)
		iter->Update_GameObject(fTimeDelta);
	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CWeightComparator::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_vecPedestals)
		iter->LateUpdate_GameObject(fTimeDelta);

	for (auto& iter : m_vecStones)
		iter->LateUpdate_GameObject(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWeightComparator::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	for (auto& iter : m_vecPedestals)
		iter->Render_GameObject();

	for (auto& iter : m_vecStones)
		iter->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWeightComparator::Init(_float _fX, _float _fZ)
{
	int iOffsetX = -90;
	m_pTransformCom->Set_Pos(_fX, .1f, _fZ);

	for (size_t i = 0; i < 4; i++)
	{
		CGameObject* pGameObject = CStonePedestal::Create(m_pGraphicDev);
		static_cast<CStonePedestal*>(pGameObject)->Init(_fX + iOffsetX, _fZ);
		static_cast<CStonePedestal*>(pGameObject)->Set_Group(this);
		m_vecPedestals.push_back(pGameObject);

		pGameObject = CStone::Create(m_pGraphicDev);
		static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + iOffsetX, 15.f, _fZ - 50.f);
		static_cast<CStone*>(pGameObject)->Set_StoneID(i);
		m_vecStones.push_back(pGameObject);

		iOffsetX += 60.f;
	}
}

void CWeightComparator::Puzzle_Clear()
{
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
	for (int i = 0; i < m_vecPedestals.size() - 1; ++i)
	{
		int iNext = i+1;
		int iTemp = static_cast<CStonePedestal*>(m_vecPedestals[i])->Get_StoneID();
		int iSour = static_cast<CStonePedestal*>(m_vecPedestals[i + 1])->Get_StoneID();

		if (iTemp == -1 || iSour == -1)
			return;

		if (iTemp > iSour)
			return;
	}
	
	m_iImageID = 1;
}
