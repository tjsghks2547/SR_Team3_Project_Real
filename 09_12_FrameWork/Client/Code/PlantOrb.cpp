#include "pch.h"
#include "PlantOrb.h"
#include "Player.h"
#include "Export_Utility.h"

CPlantOrb::CPlantOrb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_fActiveTime(0)
{
}

CPlantOrb::~CPlantOrb()
{
}

HRESULT CPlantOrb::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 3.f, 3.f, 3.f };
	m_iImageID = 0;
	m_eTag = OBJ_ENEMY;
	m_vecTexture.resize(4);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Blue.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Red.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_Yellow.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Plant_Bullet_BlueRed.png", &m_vecTexture[3]);

	return S_OK;
}

_int CPlantOrb::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);

	m_fActiveTime += fTimeDelta;

	if (m_fActiveTime > 2.f) {
		m_fActiveTime = 0;
		m_bIsActive = false;
		return 0;
	}

	_vec3  vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, m_fMoveSpeed * 2.f);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	return iExit;
}

void CPlantOrb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlantOrb::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	//m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlantOrb::OnCollision(CGameObject* _pOther)
{
}

void CPlantOrb::OnCollisionEnter(CGameObject* _pOther)
{
	if (!m_bIsActive)
		return;

	if (_pOther->Get_Tag() == OBJ_PLAYER) {
		static_cast<CPlayer*>(_pOther)->SetPlayerCurHP(-1);
		m_fActiveTime = 0;
		m_bIsActive = false;
	}
}

void CPlantOrb::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CPlantOrb::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StonePushable"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pFlowerBullet = dynamic_cast<CPlantBulletCom*>(Engine::Clone_Proto(L"Proto_PlantBulletCom"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_PlantBulletCom", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CPlantOrb* CPlantOrb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlantOrb* pStone = new CPlantOrb(pGraphicDev);

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

void CPlantOrb::Free()
{
	Engine::CGameObject::Free();
}
