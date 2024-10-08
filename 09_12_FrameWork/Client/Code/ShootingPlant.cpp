#include "pch.h"
#include "ShootingPlant.h"
#include "PlantOrb.h"
#include "Export_Utility.h"

CShootingPlant::CShootingPlant(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(0), m_isFire(false), m_fFireRate(1), m_bIsRight(false)
{
}

CShootingPlant::~CShootingPlant()
{
}

HRESULT CShootingPlant::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 150.f, 20.f, 150.f };
	m_pTexTransformCom->m_vScale = { 15.f, 15.f, 15.f };
	m_pTexTransformCom->Rotation(ROT_X, 30.f * 3.14159265359f / 180.f);
	m_eTag = TAG_NONE;
	m_vecTexture.resize(6);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Blue_Static.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Yellow_Static.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Yellow_Static.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_RedBlue_Static.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Yellow_Static.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_RedBlue_Static.png", &m_vecTexture[5]);
	
	return S_OK;
}

void CShootingPlant::LateReady_GameObject()
{

}

void CShootingPlant::OnCollision(CGameObject* _pOther)
{
}

void CShootingPlant::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		m_isFire = true;
	}
}

void CShootingPlant::OnCollisionExit(CGameObject* _pOther)	
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		m_isFire = false;
		m_fFireRate = 0.f;
	}
}

_int CShootingPlant::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_isFire) {
		m_fFireRate += fTimeDelta;

		if (m_fFireRate >= 0.2f) {
			Shot();
			m_fFireRate = 0;
		}
	}

	for (auto& iter : m_vecBullet)	
		iter->Update_GameObject(fTimeDelta);	

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CShootingPlant::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_vecBullet)
		iter->LateUpdate_GameObject(fTimeDelta);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CShootingPlant::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (auto& iter : m_vecBullet)
		iter->Render_GameObject();
}

void CShootingPlant::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 20.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, 15.f, _fZ);	
}

void CShootingPlant::Set_Dir(_bool _bIsRight)
{
	if (_bIsRight) {
		m_pTexTransformCom->Rotation(ROT_Y, 180.f * 3.14159265359f / 180.f);
		m_pTexTransformCom->Rotation(ROT_X, -60.f * 3.14159265359f / 180.f);
		m_bIsRight = true;
	}
}

HRESULT CShootingPlant::Add_Component()
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

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CShootingPlant* CShootingPlant::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShootingPlant* pCrystal = new CShootingPlant(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	return pCrystal;
}

void CShootingPlant::Shot()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	for (int i = 0; i < m_vecBullet.size(); ++i)
	{
		if (m_vecBullet[i]->Is_Active())
			continue;				

		float tempX =  m_bIsRight ? 1.f : -1.f;		
		static_cast<CPlantOrb*>(m_vecBullet[i])->Init_Pos(vPos.x, vPos.z, tempX);
		m_vecBullet[i]->Set_Active(true);
		return;
	}

	Create_Bullet();
}

void CShootingPlant::Create_Bullet()
{
	_vec3 vPos;
	m_pTexTransformCom->Get_Info(INFO_POS, &vPos);	
	float tempX = m_bIsRight ? 1.f : -1.f;	
	CGameObject* pOb = CPlantOrb::Create(m_pGraphicDev);
	static_cast<CPlantOrb*>(pOb)->Init_Pos(vPos.x + tempX, vPos.z, tempX);
	static_cast<CPlantOrb*>(pOb)->Set_Speed(m_bIsRight ? 35.f : -35.f);
	static_cast<CPlantOrb*>(pOb)->Set_ImageID(m_iImageID);
	pOb->Set_Active(true);

	m_vecBullet.push_back(pOb);
}

void CShootingPlant::Free()
{
	Engine::CGameObject::Free();
}