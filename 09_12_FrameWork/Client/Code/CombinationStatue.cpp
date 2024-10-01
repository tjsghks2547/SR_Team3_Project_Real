#include "pch.h"
#include "CombinationStatue.h"
#include "CombinationPuzzle.h"
#include "Export_Utility.h"

CCombinationStatue::CCombinationStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCombinationStatue::~CCombinationStatue()
{
}

HRESULT CCombinationStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 3.f, 4.f, 3.f };
	m_iImageID = 0;
	m_vecTexture.resize(3);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Ear.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Eye.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Mouth.png", &m_vecTexture[2]);

	return S_OK;
}

_int CCombinationStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCombinationStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCombinationStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCombinationStatue::On_Collision()
{
	m_iImageID++;
	m_iImageID %= 3;
	static_cast<CCombinationPuzzle*>(m_pGroup)->Check_Matched();
}

HRESULT CCombinationStatue::Add_Component()
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

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CCombinationStatue* CCombinationStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCombinationStatue* pCombinationStatue = new CCombinationStatue(pGraphicDev);

	if (FAILED(pCombinationStatue->Ready_GameObject()))
	{
		Safe_Release(pCombinationStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCombinationStatue);
	return pCombinationStatue;
}

void CCombinationStatue::Free()
{
	Engine::CGameObject::Free();
}
