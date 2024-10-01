#include "pch.h"
#include "LightFlower.h"
#include "FlowerGlow.h"
#include "PlantBullet.h"
#include "Export_Utility.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_iImageID(1), m_pGlow(nullptr)
{
}

CLightFlower::~CLightFlower()
{
}

HRESULT CLightFlower::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 7.f, 11.f, 3.f };
	m_iImageID = 0;
	m_iComStoneID = 0;
	m_vecTexture.resize(4);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Blue.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Red.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_Yellow.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FlowerLightStandArea_BlueRed.png", &m_vecTexture[3]);

	return S_OK;
}

_int CLightFlower::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_pGlow != nullptr)
		m_pGlow->Update_GameObject(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CLightFlower::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_pGlow != nullptr)
		m_pGlow->LateUpdate_GameObject(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLightFlower::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_pGlow != nullptr)
		m_pGlow->Render_GameObject();
}

void CLightFlower::OnCollision(CGameObject* _pOther)
{
}

void CLightFlower::OnCollisionEnter(CGameObject* _pOther)
{
}

void CLightFlower::OnCollisionExit(CGameObject* _pOther)
{
}

void CLightFlower::Init_Pos(_float _fX, _float _fZ)
{	
	m_pTransformCom->Set_Pos(_fX, 13.1f, _fZ);

	if (m_pGlow == nullptr) {
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
		pGameObject = CFlowerGlow::Create(m_pGraphicDev);
		static_cast<CFlowerGlow*>(pGameObject)->Init_Pos(_fX, _fZ);
		static_cast<CFlowerGlow*>(pGameObject)->Set_ImageID(m_iImageID);

		m_pGlow = pGameObject;
	}
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

CLightFlower* CLightFlower::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightFlower* pStone = new CLightFlower(pGraphicDev);

	if (FAILED(pStone->Ready_GameObject()))
	{
		Safe_Release(pStone);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStone);
	return pStone;
}

void CLightFlower::Free()
{
	Engine::CGameObject::Free();
}

void CLightFlower::Match_Puzzle()
{
	if (m_pGlow == nullptr) {
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
		pGameObject = CFlowerGlow::Create(m_pGraphicDev);		
		static_cast<CFlowerGlow*>(pGameObject)->Init_Pos(vBasePos.x, vBasePos.z);
		static_cast<CFlowerGlow*>(pGameObject)->Set_ImageID(m_iImageID);

		m_pGlow = pGameObject;
	}
	else {		
		static_cast<CFlowerGlow*>(m_pGlow)->Set_ImageID(m_iImageID);
	}
	
}
