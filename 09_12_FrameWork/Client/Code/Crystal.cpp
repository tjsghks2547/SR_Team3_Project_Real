#include "pch.h"
#include "Crystal.h"
#include "Export_Utility.h"

CCrystal::CCrystal(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1)
{
}

CCrystal::~CCrystal()
{
}

HRESULT CCrystal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 18.f, 18.f, 0.f };
	SetObjectType(OBJ_TYPE::NOTPASS_ABLE);
	m_vecTexture.resize(4);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Gray.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Yellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Green.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Red.png", &m_vecTexture[3]);	

	return S_OK;
}

_int CCrystal::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;
		
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCrystal::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCrystal::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCrystal::Add_Component()
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

CCrystal* CCrystal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrystal* pCrystal = new CCrystal(pGraphicDev);

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

void CCrystal::Free()
{
	Engine::CGameObject::Free();
}