#include "Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayeTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayeTag));

	if (iter == m_mapLayer.end())
		return nullptr; 


	return iter->second->Get_Component(eID,pObjTag,pComponentTag);
}

CGameObject* CScene::Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag)
{
	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayeTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pLayeTag, pObjTag);
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

void CScene::LateReady_Scene()
{
	for (auto& pLayer : m_mapLayer)
	{
		pLayer.second->LateReady_Layer();
	}
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
	_int  iResult(0);

	for (auto& pLayer : m_mapLayer)
	{
		iResult = pLayer.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult; 
	}

	return iResult;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_mapLayer)
	{
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	}
}

//void CScene::Render_Scene()
//{
//	for (auto& pLayer : m_mapLayer)
//	{
//		pLayer.second->Render_Layer();
//	}
//}

void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
