#include "pch.h"
#include "Object.h"
#include "Export_Utility.h"
#include "ImGuiManger.h"

CObject::CObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CObject::~CObject()
{
}

HRESULT CObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//여기서 텍스처 다 넣자 
	
	m_vecTexture.resize(100);
	//Wall 넣은곳
	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/wall.png", &m_vecTexture[0]);
	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Tutorial_Super.png", &m_vecTexture[1]);

	return S_OK;
}



_int CObject::Update_GameObject(const _float& fTimeDelta)
{

	Add_RenderGroup(RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	


	if (m_strTextureKey == L"Wall") 
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[0]);

	}

	if (m_strTextureKey == L"Super")
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[1]);

	}



	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObject::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

HRESULT CObject::SetUp_Material()
{
	return S_OK;
}

void CObject::Key_Input(const _float& fTimeDelta)
{
}

CObject* CObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObject* pObject = new CObject(pGraphicDev);

	if(FAILED(pObject->Ready_GameObject()))
	{
		Safe_Release(pObject);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pObject;
}

void CObject::Free()
{
	Engine::CGameObject::Free();
}
