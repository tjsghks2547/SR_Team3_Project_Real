#include "pch.h"
#include "ColorStone.h"
#include "Export_Utility.h"

CColorStone::CColorStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_pCombinedStone(nullptr)
{
}

CColorStone::~CColorStone()
{
}

HRESULT CColorStone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 12.f, 12.f, 12.f };
	m_eTag = OBJ_STONE;
	m_iImageID = 0;
	m_iComStoneID = 0;	
	m_eObjType = PUSH_ABLE;	
	m_vecTexture.resize(6);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableCyan.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableMagenta.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableYellow.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableBlue.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableGreen.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableRed.png", &m_vecTexture[5]);

	return S_OK;
}

_int CColorStone::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CColorStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CColorStone::Render_GameObject()
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

void CColorStone::OnCollision(CGameObject* _pOther)
{
}

void CColorStone::OnCollisionEnter(CGameObject* _pOther)
{
	if (m_bIsActive == false)
		return;

	if (_pOther->Get_Tag() == OBJ_STONE) {
		if (m_pCombinedStone != nullptr || _pOther->Is_Active() == false)
			return;		
		
		m_iComStoneID = static_cast<CColorStone*>(_pOther)->Get_ImageID();
		if (m_iComStoneID == m_iImageID || m_iComStoneID >= 3)
			return;
		else
			m_iImageID = m_iImageID + m_iComStoneID + 2;

		m_pCombinedStone = _pOther;	
		static_cast<CColorStone*>(_pOther)->Set_Active(false);
		m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(vPos.x, 15.1f, vPos.z);
	}

	//if (_pOther->Get_Tag() == OBJ_PLAYER) {

	//	if (m_pCombinedStone == nullptr)
	//		return;

	//	m_iImageID = m_iImageID - m_iComStoneID - 2;			
	//	m_pTransformCom->m_vScale = { 12.f, 12.f, 12.f };

	//	_vec3 temp;
	//	m_pTransformCom->Get_Info(INFO_POS, &temp);
	//	m_pTransformCom->Set_Pos(temp.x, temp.y, temp.z - 7.f);
	//	static_cast<Engine::CTransform*>(m_pCombinedStone->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(temp.x, temp.y, temp.z - 7.f);		
	//	static_cast<CColorStone*>(m_pCombinedStone)->Set_Active(true);
	//	m_pCombinedStone = nullptr;
	//}
}

void CColorStone::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CColorStone::Add_Component()
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

CColorStone* CColorStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColorStone* pStone = new CColorStone(pGraphicDev);

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

void CColorStone::Free()
{
	Engine::CGameObject::Free();
}
