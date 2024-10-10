#include "pch.h"
#include "Grass.h"
#include "Export_Utility.h"
#include "ResMgr.h"	

CGrass::CGrass(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CGrass::~CGrass()
{
}

HRESULT CGrass::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);	

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Object/Grass_Effect.png", &m_pTex);
	
	IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[3];		

	m_pAnimatorCom->CreateAnimation(L"Grass_Move", m_pTex, _vec2(0.f, 0.f), _vec2(171.f, 256.f), _vec2(171.f, 0.f), 0.1f, 4);
	m_pAnimatorCom->CreateAnimation(L"Grass_Stop", m_pTex, _vec2(0.f, 0.f), _vec2(171.f, 256.f), _vec2(171.f, 0.f), 0.1f, 0);
	m_pAnimatorCom2->CreateAnimation(L"Grass_Effect", pEffectTexture1, _vec2(0.f, 0.f), _vec2(512.f, 512.f), _vec2(512.f, 0.f), 0.08f, 7);


	m_pAnimatorCom->Play(L"Grass_Stop", true);	

	m_pTransformCom->m_vScale = { 20.f,20.f,30.f };	
	m_pTransformCom->Set_Pos(200.f, 35.f, 650.f);

	return S_OK;
}

_int CGrass::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CGrass::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	if(m_pAnimatorCom->GetAnimation()->IsFinish())
	{
		m_pAnimatorCom->GetAnimation()->SetFrame(0);
		//m_pAnimatorCom2->GetAnimation()->SetFrame(0);
		m_pAnimatorCom->Play(L"Grass_Stop", true);
	}

	
}

void CGrass::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_pTex);	


	m_pAnimatorCom->render();
	//m_pAnimatorCom2->render();
	//m_pBoundBox->Render_Buffer();		

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGrass::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation_Buffer", pComponent });

	//pComponent = m_pAnimatorCom2 = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation_Buffer2", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));	
	NULL_CHECK_RETURN(pComponent, E_FAIL);	
	m_pBoundBox->SetGameObjectPtr(this);	
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });	

	return S_OK;
}

void CGrass::Key_Input(const _float& fTimeDelta)
{
}

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGrass* pGrass = new CGrass(pGraphicDev);

	if (FAILED(pGrass->Ready_GameObject()))
	{
		Safe_Release(pGrass);
		MSG_BOX("pTestObject Create Failed");
		return nullptr;
	}

	return pGrass;
}

void CGrass::OnCollisionEnter(CGameObject* _pOther)
{
	m_pAnimatorCom->Play(L"Grass_Move", false);		

	//m_pAnimatorCom2->Play(L"Grass_Effect", false);
}

void CGrass::Free()
{
	Engine::CGameObject::Free();	
}
