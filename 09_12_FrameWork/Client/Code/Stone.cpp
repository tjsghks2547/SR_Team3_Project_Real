#include "pch.h"
#include "Stone.h"
#include "Export_Utility.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_bIsActive(true)
{
}

CStone::~CStone()
{
}

HRESULT CStone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };	
	m_eTag = OBJ_STONE;
	m_eObjType = PUSH_ABLE;
	m_bIsLaunched = false;
	return S_OK;
}

_int CStone::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);
	_vec3 position;
	m_pTransformCom->Get_Info(INFO_POS, &position);

	if (m_bIsLaunched) {
		const float gravity = -9.8f;
		// 속도 업데이트
		m_vVelocity.y += gravity * fTimeDelta * 7.f;
		// 위치 업데이트
		position += m_vVelocity * fTimeDelta * 7.f;

		if (position.y <= 15.0f) {
			m_bIsLaunched = false;
			m_pTransformCom->Set_Pos(position.x, 15.f, position.z);
		}
		else {
			m_pTransformCom->Set_Pos(position.x, position.y, position.z);
		}					
	}

	return iExit;
}

void CStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStone::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();	
	m_pBoundBox->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
	m_pGraphicDev->SetTexture(0, NULL); 
}

void CStone::OnCollision(CGameObject* _pOther)
{
}

void CStone::OnCollisionEnter(CGameObject* _pOther)
{
}

void CStone::OnCollisionExit(CGameObject* _pOther)	
{
}

void CStone::Launch()
{
	m_bIsLaunched = true;
	m_vVelocity = { 0, 25.0f, 20.0f };
}

HRESULT CStone::Add_Component()
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

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStone* pStone = new CStone(pGraphicDev);

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

void CStone::Free()
{
	Engine::CGameObject::Free();
}