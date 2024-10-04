#include "pch.h"
#include "LightFlower.h"
#include "FlowerGlow.h"
#include "Export_Utility.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev)
{
}

CLightFlower::~CLightFlower()
{
}

HRESULT CLightFlower::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 20.f, 30.f, 20.f };	

	return S_OK;
}

_int CLightFlower::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_pGlow != nullptr)
		m_pGlow->Update_GameObject(fTimeDelta);

	return iExit;
}

void CLightFlower::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);


	if (m_pGlow != nullptr)
		m_pGlow->LateUpdate_GameObject(fTimeDelta);

}

void CLightFlower::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_pGlow != nullptr)
		m_pGlow->Render_GameObject();
}

void CLightFlower::LateReady_GameObject()
{
	/*_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pGlow = CFlowerGlow::Create(m_pGraphicDev);
	m_pGlow->Set_Active(true);
	static_cast<CFlowerGlow*>(m_pGlow)->Set_ImageID(0);
	static_cast<Engine::CTransform*>(m_pGlow->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos.x, 0.05f ,vPos.z);*/

}

HRESULT CLightFlower::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LightFlower"));
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

void CLightFlower::Match_Puzzle()
{
}


CLightFlower* CLightFlower::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightFlower* pCrystal = new CLightFlower(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCrystal);
	return pCrystal;
}

void CLightFlower::Free()
{
	Engine::CGameObject::Free();
}