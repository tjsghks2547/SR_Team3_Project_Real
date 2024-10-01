#include "pch.h"
#include "Net.h"

CNet::CNet(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CNet::~CNet()
{
}

HRESULT CNet::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { QUEST,
		NET,
		L"잠자리채",
		L"잠자리를 잡을 수 있는 채. 이외의 용도로 사용할 수 있을지도??ㅋ_ㅋ??",
		1, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CNet::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CNet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CNet::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

}

HRESULT CNet::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//SmallFruit
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Net"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureNet", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformNet", pComponent });
	m_pTransformCom->m_vScale = { 40.f, 40.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };
	////0925Quick
	//pComponent = m_pQuickTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuick", pComponent });


	//CountRC
	//pComponent = m_pCountRCTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CountRC"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Com_TextureCountRC", pComponent });

	//pComponent = m_pCountRCTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCountRC", pComponent });
	//m_pCountRCTransformCom->m_vScale = { 20.f, 17.f, 1.f };
	//m_pCountRCTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	return S_OK;
}

CNet* CNet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNet* pItem = new CNet(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("Net Create Failed");
		return nullptr;
	}

	return pItem;
}
