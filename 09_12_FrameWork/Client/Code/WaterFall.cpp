#include "pch.h"
#include "WaterFall.h"
#include "Export_Utility.h"
#include "ResMgr.h"

CWaterFall::CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CWaterFall::~CWaterFall()
{
}

HRESULT CWaterFall::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[1];	

	m_pAnimatorCom->CreateAnimation(L"Water_Fall", pEffectTexture1, _vec2(0.f, 0.f), _vec2(1024.f, 1024.f), _vec2(1024.f, 0.f), 0.2f, 7);
	
	m_pAnimatorCom->Play(L"Water_Fall",true);

	m_pTransformCom->Set_Pos(500.f, 100.f, 1000.f);
	m_pTransformCom->m_vScale = { 1000.f, 300.f,10.f };

	return S_OK;	
}

_int CWaterFall::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);		
	return Engine::CGameObject::Update_GameObject(fTimeDelta);		
}

void CWaterFall::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);	
}

void CWaterFall::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	


	m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetEffectTexture()[1]);
	m_pAnimatorCom->render();	

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
}

HRESULT CWaterFall::Add_Component()
{
	CComponent* pComponent = NULL;	

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);	
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

HRESULT CWaterFall::SetUp_Material()
{
	return E_NOTIMPL;
}

void CWaterFall::Key_Input(const _float& fTimeDelta)
{
}

CWaterFall* CWaterFall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFall* pWaterFall = new CWaterFall(pGraphicDev);

	if (FAILED(pWaterFall->Ready_GameObject()))
	{
		Safe_Release(pWaterFall);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pWaterFall;
}

void CWaterFall::Free()
{
	Engine::CGameObject::Free();
}
