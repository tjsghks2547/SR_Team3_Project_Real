#include "pch.h"
#include "ShootingPlant.h"
#include "PlantOrb.h"
#include "Export_Utility.h"

CShootingPlant::CShootingPlant(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_fShootTimer(0), m_fShootTimerMax(1), m_bIsShooting(false)
{
}

CShootingPlant::~CShootingPlant()
{
}

HRESULT CShootingPlant::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTexTransformCom->m_vScale = { 6.f, 6.f, 3.f };
	m_pTransformCom->m_vScale = { 50.f, 10.f, 50.f };	
	m_iImageID = 0;	
	m_vecTexture.resize(4);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Blue_Static.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Yellow_Static.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_Yellow_Static.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PlantCannon_RedBlue_Static.png", &m_vecTexture[3]);

	return S_OK;
}

_int CShootingPlant::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	

	if (m_bIsShooting) {
		Shoot(fTimeDelta);
	}
		
	for (auto& iter : m_vecBullets)
		iter->Update_GameObject(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CShootingPlant::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_vecBullets)
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

	for (auto& iter : m_vecBullets)
		iter->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CShootingPlant::OnCollision(CGameObject* _pOther)
{
}

void CShootingPlant::OnCollisionEnter(CGameObject* _pOther)
{
	if(_pOther->Get_Tag() == OBJ_PLAYER)
		m_bIsShooting = true;
}

void CShootingPlant::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == OBJ_PLAYER)
		m_bIsShooting = false;
}

void CShootingPlant::Init_Pos(_float _fX, _float _fZ)
{
	m_pTexTransformCom->Set_Pos(_fX, 4.f, _fZ);
	m_pTransformCom->Set_Pos(_fX, 0.f, _fZ);
}

void CShootingPlant::Shoot(const _float& fTimeDelta)
{
	m_fShootTimer += fTimeDelta;

	if (m_fShootTimer >= m_fShootTimerMax) {
		m_fShootTimer = 0;


		for (int i = 0; i < m_vecBullets.size(); ++i)
		{
			if (m_vecBullets[i]->Is_Active() == false) {				
				_vec3 vBasePos;
				m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
				static_cast<Engine::CTransform*>(m_vecBullets[i]->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 7.f, 4.3f, vBasePos.z);				
				m_vecBullets[i]->Set_Active(true);
				
				return;	
			}
		}

		Create_Bullet();
	}
}	

void CShootingPlant::Create_Bullet()
{	
	Engine::CGameObject* pGameObject = nullptr;
	_vec3 vBasePos;
	m_pTransformCom->Get_Info(INFO_POS, &vBasePos);	
	pGameObject = CPlantOrb::Create(m_pGraphicDev);	
	static_cast<Engine::CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 7.f, 4.3f, vBasePos.z);
	static_cast<CPlantOrb*>(pGameObject)->Set_Speed(-20.f);
	static_cast<CPlantOrb*>(pGameObject)->Set_ImageID(m_iImageID);

	m_vecBullets.push_back(pGameObject);
}

HRESULT CShootingPlant::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

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
	CShootingPlant* pStone = new CShootingPlant(pGraphicDev);

	if (FAILED(pStone->Ready_GameObject()))
	{
		Safe_Release(pStone);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStone);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStone);
	return pStone;
}

void CShootingPlant::Free()
{
	Engine::CGameObject::Free();
}
