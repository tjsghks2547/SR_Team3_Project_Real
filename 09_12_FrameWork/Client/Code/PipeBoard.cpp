#include "pch.h"
#include "PipeBoard.h"
#include "Pipe.h"
#include "Export_Utility.h"

CPipeBoard::CPipeBoard(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CPipeBoard::~CPipeBoard()
{
}

HRESULT CPipeBoard::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 50.f, 50.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);
	m_pTransformCom->Set_Pos(0.f, 0.01f, 0.f);

    return S_OK;
}

_int CPipeBoard::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);	

	//map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
	//NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
	//Engine::CGameObject* pGameObject = nullptr;
	//pGameObject = CPipeObj::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe1", pGameObject);

	return iExit;
}

void CPipeBoard::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPipeBoard::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPipeBoard::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeBoardTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CPipeBoard::Key_Input(const _float& fTimeDelta)
{
	if (GetAsyncKeyState('2'))
	{
		map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		//NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		pGameObject = CPipe::Create(m_pGraphicDev);
		//NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe1", pGameObject);

	}
}

CPipeBoard* CPipeBoard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPipeBoard* pPipeBoard = new CPipeBoard(pGraphicDev);

	if (FAILED(pPipeBoard->Ready_GameObject()))
	{
		Safe_Release(pPipeBoard);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pPipeBoard;
}

void CPipeBoard::Free()
{
	Engine::CGameObject::Free();
}