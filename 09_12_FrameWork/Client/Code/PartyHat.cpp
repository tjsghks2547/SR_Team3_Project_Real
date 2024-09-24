#include "pch.h"
#include "PartyHat.h"

CPartyHat::CPartyHat(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CPartyHat::~CPartyHat()
{
}

HRESULT CPartyHat::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		PARTY,
		 L"파티 모자",
		 L"파티하기 좋은 모자. 파티닷! 끼 - 얏 - 하 - !",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPartyHat::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);

}

void CPartyHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CPartyHat::Render_GameObject()
{
	//const Engine::_matrix* matTemp = m_pTransformCom->Get_WorldMatrix();
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, matTemp);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CPartyHat::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Party Hat
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PartyHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePartyHat", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformPartyHat", pComponent });
	m_pTransformCom->m_vScale = { 40.f, 40.f, 1.f };

	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.f };

	return S_OK;
}

CPartyHat* CPartyHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPartyHat* pItem = new CPartyHat(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("PartyHat Create Failed");
		return nullptr;
	}

	return pItem;
}
