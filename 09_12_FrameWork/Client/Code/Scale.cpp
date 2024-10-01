#include "pch.h"
#include "Scale.h"
#include "Stone.h"
#include "StonePedestal.h"
#include "Export_Utility.h"

CScale::CScale(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_iImageID(0)
{
}

CScale::~CScale()
{
}

HRESULT CScale::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 15.f, 15.f, 6.f };
	m_pLeftCompTransformCom->m_vScale = { 7.f, 7.f, 0.f };
	m_pRightCompTransformCom->m_vScale = { 7.f, 7.f, 0.f };
	m_vecTexture.resize(3);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Balanced.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Left_Anim05.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Scale_Right_Anim05.png", &m_vecTexture[2]);	

	return S_OK;
}

_int CScale::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CScale::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CScale::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetTexture(0, NULL);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pLeftCompTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pLeftCompTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRightCompTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRightCompTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CScale::Init_Position(float _fX, float _fZ)
{
	m_pLeftCompTransformCom->Set_Pos(_fX - 15.f, 0.05f, _fZ - 9.f);
	m_pLeftCompTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	m_pRightCompTransformCom->Set_Pos(_fX + 15.f, 0.05f, _fZ - 9.f);
	m_pRightCompTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);

	static_cast<Engine::CTransform*>(m_pLeftPedestal->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX - 15.f, 0.05f, _fZ - 9.f);
	static_cast<Engine::CTransform*>(m_pLeftPedestal->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX - 15.f, 3.f, _fZ - 9.f);
	static_cast<Engine::CTransform*>(m_pRightPedestal->Get_Component(ID_DYNAMIC, L"Com_TexTransform"))->Set_Pos(_fX + 15.f, 0.05f, _fZ - 9.f);
	static_cast<Engine::CTransform*>(m_pRightPedestal->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(_fX + 15.f, 3.f, _fZ - 9.f);
}

HRESULT CScale::Add_Component()
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

	pComponent = m_pLeftCompTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePlaceFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_CLeftompTexture", pComponent });

	pComponent = m_pLeftCompTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_LeftCompTransform", pComponent });	

	pComponent = m_pRightCompTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePlaceFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_RightCompTexture", pComponent });

	pComponent = m_pRightCompTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_RightCompTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CScale* CScale::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScale* pCrystal = new CScale(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	return pCrystal;
}

void CScale::Free()
{
	Engine::CGameObject::Free();
}

void CScale::Match_Puzzle()
{
	Measure_Weight();
}

void CScale::Measure_Weight()
{
	int tempL = static_cast<CStone*>(static_cast<CStonePedestal*>(m_pLeftPedestal)->Get_Stone())->Get_StoneID();
	int tempR = static_cast<CStone*>(static_cast<CStonePedestal*>(m_pRightPedestal)->Get_Stone())->Get_StoneID();

	if (tempL == -1 || tempR == -1)
		return;

	m_iImageID = tempL > tempR ? 1 : 2;
}
