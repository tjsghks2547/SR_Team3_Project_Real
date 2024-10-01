#include "pch.h"
#include "Catapult.h"
#include "Stone.h"
#include "Export_Utility.h"

CCatapult::CCatapult(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pReadyStone(nullptr)
{
}

CCatapult::~CCatapult()
{
}

HRESULT CCatapult::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 6.f, 6.f, 6.f };
	m_pTexTransformCom->m_vScale = { 6.f, 6.f, 6.f };
	m_pBucketTexTransformCom->m_vScale = { 5.f, 5.f, 0.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_pBucketTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	return S_OK;
}

_int CCatapult::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_pReadyStone != nullptr) {
		_vec3 temp;
		m_pTransformCom->Get_Info(INFO_POS, &temp);
		static_cast<Engine::CTransform*>(m_pReadyStone->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(temp.x, 6.f, temp.z - 2.f);
	}

	return iExit;
}

void CCatapult::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCatapult::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pBucketTexTransformCom->Get_WorldMatrix());

	m_pBucketTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	
	//m_pBoundBox->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCatapult::OnCollision(CGameObject* _pOther)
{
}

void CCatapult::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == OBJ_STONE) {
		if (m_pReadyStone != nullptr)
			return;
		m_pReadyStone = _pOther;
	}
	else if (_pOther->Get_Tag() == OBJ_PLAYER) {
		if (m_pReadyStone == nullptr)
			return;

		static_cast<CStone*>(m_pReadyStone)->Launch();
		m_pReadyStone = nullptr;
	}
}

void CCatapult::OnCollisionExit(CGameObject* _pOther)
{

}

void CCatapult::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 3.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, 0.3f, _fZ);
	m_pBucketTexTransformCom->Set_Pos(_fX, 2.f, _fZ);
}

HRESULT CCatapult::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Catapult"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pBucketTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CatapultBucket"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_BucketTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBucketTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_BucketTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CCatapult* CCatapult::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCatapult* pCrystal = new CCatapult(pGraphicDev);

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

void CCatapult::Free()
{
	Engine::CGameObject::Free();
}