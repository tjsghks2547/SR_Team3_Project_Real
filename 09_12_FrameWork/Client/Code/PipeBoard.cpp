#include "pch.h"
#include "PipeBoard.h"
#include "Pipe.h"
#include "BoardCursor.h"
#include "Export_Utility.h"

CPipeBoard::CPipeBoard(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_bIsInteracting(false)
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
	m_pTransformCom->Set_Pos(-50.f, 0.02f, 50.f);
	m_pPipeBoardCom->m_iCurPipeCount = 0;
	m_pPipeBoardCom->m_bIsPipeSpawned = false;
	m_pPipeBoardCom->m_iGridSizeX = 7;
	m_pPipeBoardCom->m_iGridSizeZ = 7;
	m_pPipeBoardCom->m_vecBoardGrid.resize(49);
	
    return S_OK;
}

_int CPipeBoard::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);

	if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 9) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
		vBasePos = { -50.f, 0.02f, 50.f };

		pGameObject = CBoardCursor::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"PipeCursor", pGameObject);
		m_pPipeBoardCom->m_pCursor = pGameObject;
		m_pPipeBoardCom->m_iCursorID = 25;
		
		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe0", pGameObject);
		vPos = { vBasePos.x - 24.9f, vBasePos.y + 0.05f, vBasePos.z + 25.f };		
		vRot = { 0.f, 0.f, -180.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[8] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe1", pGameObject);
		vPos = { vBasePos.x, vBasePos.y + 0.05f, vBasePos.z + 25.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 1, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[10] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe2", pGameObject);
		vPos = { vBasePos.x + 24.9f, vBasePos.y + 0.05f, vBasePos.z + 25.f };
		vRot = { 0.f, 0.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_DOWN, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[12] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe3", pGameObject);
		vPos = { vBasePos.x, vBasePos.y + 0.05f, vBasePos.z - 12.5f};
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_UP, FLOW_RIGHT, FLOW_END, true, 0, vPos, vRot);
		dynamic_cast<CPipeCom*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = true;
		m_pPipeBoardCom->m_pStatingPipe = pGameObject;
		m_pPipeBoardCom->m_vecBoardGrid[31] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe4", pGameObject);
		vPos = { vBasePos.x - 24.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 180.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_UP, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[36] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe5", pGameObject);
		vPos = { vBasePos.x + 24.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 180.f,  -180.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_UP, FLOW_END, FLOW_END, FLOW_END, true, 2, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[40] = pGameObject;

		// Moveable Blocks
		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe6", pGameObject);
		vPos = { vBasePos.x - 36.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_RIGHT, FLOW_UP, FLOW_END, false, 3, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[35] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe7", pGameObject);
		vPos = { vBasePos.x + 36.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 0.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_DOWN, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[41] = pGameObject;

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe8", pGameObject);
		vPos = { vBasePos.x + 24.9f, vBasePos.y + 0.05f, vBasePos.z - 37.5f };
		vRot = { 0.f, -90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_RIGHT, FLOW_DOWN, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[47] = pGameObject;

		m_pPipeBoardCom->m_iCurPipeCount = 8;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 10) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe9", pGameObject);
		vPos = { vBasePos.x + 24.9f, vBasePos.y + 0.05f, vBasePos.z - 37.5f };
		vRot = { 0.f, -90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_RIGHT, FLOW_DOWN, FLOW_END, FLOW_END, false, 5, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[47] = pGameObject;

		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 11) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe10", pGameObject);
		vPos = { vBasePos.x - 36.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_RIGHT, FLOW_UP, FLOW_END, false, 3, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[35] = pGameObject;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	else if (m_pPipeBoardCom->m_bIsPipeSpawned == false && m_pPipeBoardCom->m_iCurPipeCount < 12) {
		map<const _tchar*, CLayer*> pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();
		NULL_CHECK_RETURN(pMapLayer[L"Layer_GameLogic"], E_FAIL);
		Engine::CGameObject* pGameObject = nullptr;
		_vec3 vBasePos, vPos, vRot;
		m_pTransformCom->Get_Info(INFO_POS, &vBasePos);

		pGameObject = CPipe::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pMapLayer[L"Layer_GameLogic"]->Add_GameObject(L"Pipe11", pGameObject);
		vPos = { vBasePos.x - 36.9f, vBasePos.y + 0.05f, vBasePos.z - 25.f };
		vRot = { 0.f, 90.f, 0.f };
		dynamic_cast<CPipe*>(pGameObject)->Initialize_Pipe_Option(FLOW_LEFT, FLOW_RIGHT, FLOW_UP, FLOW_END, false, 3, vPos, vRot);
		m_pPipeBoardCom->m_vecBoardGrid[35] = pGameObject;
		m_pPipeBoardCom->m_bIsPipeSpawned = true;
	}
	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

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
	//m_pGraphicDev->SetTexture();
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

	pComponent = m_pPipeBoardCom = dynamic_cast<CPipeBoardCom*>(Engine::Clone_Proto(L"Proto_PipeBoard"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_PipeBoard", pComponent });

	return S_OK;
}

void CPipeBoard::On_Interacted()
{
	_vec3 vBasePos;
	m_pTransformCom->Get_Info(INFO_POS, &vBasePos);
	
	static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, vBasePos.y + 0.06f, vBasePos.z);
	static_cast<CBoardCursor*>(m_pPipeBoardCom->m_pCursor)->Set_Active(true);
	m_bIsInteracting = true;
}

void CPipeBoard::On_Exit()
{
	static_cast<CBoardCursor*>(m_pPipeBoardCom->m_pCursor)->Set_Active(false);
	m_pPipeBoardCom->m_iCursorID = 25;
	m_bIsInteracting = false;
}

void CPipeBoard::Reset_Connected()
{
	for (auto& iter : m_pPipeBoardCom->m_vecBoardGrid) {
		if (iter == nullptr || iter == m_pPipeBoardCom->m_pStatingPipe)
			continue;
		static_cast<CPipeCom*>(iter->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = false;
	}	
	
	if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
		static_cast<CPipeCom*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsConnected = false;	
}

void CPipeBoard::Check_Connected(CGameObject* _pSender, _int _eID)
{	 
	if (_eID % 7 == 0 || _eID % 7 == 6)
		return;

	_int iGridX = m_pPipeBoardCom->m_iGridSizeX;
	_int iGridZ = m_pPipeBoardCom->m_iGridSizeZ;

	CPipeCom* obj = static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID]->Get_Component(ID_DYNAMIC, L"Com_Pipe"));

	if (_pSender != m_pPipeBoardCom->m_pStatingPipe && obj->m_bIsFixed == true) {
		obj->m_bIsConnected = true;
		return;
	}
	
	for (auto& iter : obj->m_FlowList)
	{
		switch (iter)
		{
		case FLOW_UP:
			if (_eID - iGridX >= 0 && m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX] != nullptr	&& m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX] != _pSender) {
				if(static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID - iGridX]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_DOWN))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID - iGridX);
			}
			break;
		case FLOW_DOWN:
			if (_eID + iGridX <= iGridX * iGridZ &&	m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX] != nullptr && m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX] != _pSender) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID + iGridX]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_UP))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID + iGridX);
			}
			break;
		case FLOW_LEFT:
			if (_eID - 1 >= 0 && m_pPipeBoardCom->m_vecBoardGrid[_eID - 1] != nullptr && m_pPipeBoardCom->m_vecBoardGrid[_eID - 1] != _pSender ) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID - 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_RIGHT))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID - 1);
			}
			break;
		case FLOW_RIGHT:
			if (_eID + 1 <= iGridX * iGridZ && m_pPipeBoardCom->m_vecBoardGrid[_eID + 1] != nullptr	&& m_pPipeBoardCom->m_vecBoardGrid[_eID + 1] != _pSender) {
				if (static_cast<CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[_eID + 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->Is_PipeType_Exist(FLOW_LEFT))
					Check_Connected(m_pPipeBoardCom->m_vecBoardGrid[_eID], _eID + 1);
			}
			break;
		case FLOW_END:
			break;
		default:
			break;
		}
	}

	obj->m_bIsConnected = true;
}

void CPipeBoard::Key_Input(const _float& fTimeDelta)
{
	//if (Engine::GetKeyUp(DIK_2))
	//{
	//	m_pPipeBoardCom->m_iCurPipeCount++;
	//	m_pPipeBoardCom->m_bIsPipeSpawned = false;
	//}

	//if (Engine::GetKeyUp(DIK_1)) {
	//	if (m_bIsInteracting)		
	//		On_Exit();
	//	else			
	//		On_Interacted();
	//}

	if (!m_bIsInteracting)
		return;	

	if (Engine::GetKeyUp(DIK_UP))
	{
		if (m_pPipeBoardCom->m_iCursorID - m_pPipeBoardCom->m_iGridSizeZ <= 0)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.51f, vBasePos.z + 12.4f);
		m_pPipeBoardCom->m_iCursorID -= m_pPipeBoardCom->m_iGridSizeZ;

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.5f, vBasePos.z + 12.4f);
	}

	if (Engine::GetKeyUp(DIK_DOWN))
	{	
		if (m_pPipeBoardCom->m_iCursorID + m_pPipeBoardCom->m_iGridSizeZ > (m_pPipeBoardCom->m_iGridSizeX * m_pPipeBoardCom->m_iGridSizeZ))
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.51f, vBasePos.z - 12.4f);
		m_pPipeBoardCom->m_iCursorID += m_pPipeBoardCom->m_iGridSizeZ;

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x, 0.5f, vBasePos.z - 12.4f);
	}

	if (Engine::GetKeyUp(DIK_LEFT))
	{
		if ((m_pPipeBoardCom->m_iCursorID % m_pPipeBoardCom->m_iGridSizeX) == 1)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 12.f, 0.51f, vBasePos.z);
		m_pPipeBoardCom->m_iCursorID--;

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr) {			
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x - 12.f, 0.5f, vBasePos.z);
		}		
	}

	if (Engine::GetKeyUp(DIK_RIGHT))
	{
		if ((m_pPipeBoardCom->m_iCursorID % m_pPipeBoardCom->m_iGridSizeX) == 0)
			return;

		_vec3 vBasePos;
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vBasePos);
		static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pCursor->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x + 12.f, 0.51f, vBasePos.z);
		m_pPipeBoardCom->m_iCursorID++;

		if (m_pPipeBoardCom->m_pPickedPipe != nullptr)
			static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vBasePos.x + 12.f, 0.5f, vBasePos.z);
	}

	if (Engine::GetKeyUp(DIK_S))
	{
		if (m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] != nullptr && m_pPipeBoardCom->m_pPickedPipe == nullptr &&
			static_cast<Engine::CPipeCom*>(m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1]->Get_Component(ID_DYNAMIC, L"Com_Pipe"))->m_bIsFixed == false) {
			m_pPipeBoardCom->m_pPickedPipe = m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1];
			m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] = nullptr;

			Reset_Connected();
			Check_Connected(m_pPipeBoardCom->m_pStatingPipe, 31);			
		}
		else {
			if (m_pPipeBoardCom->m_pPickedPipe == nullptr || m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] != nullptr)
				return;			

			_vec3 vPos;			
			m_pPipeBoardCom->m_vecBoardGrid[m_pPipeBoardCom->m_iCursorID - 1] = m_pPipeBoardCom->m_pPickedPipe;
			CTransform* tr = static_cast<Engine::CTransform*>(m_pPipeBoardCom->m_pPickedPipe->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			tr->Get_Info(INFO_POS, &vPos);
			tr->Set_Pos(vPos.x, 0.5f, vPos.z);
			m_pPipeBoardCom->m_pPickedPipe = nullptr;			

			Reset_Connected();
			Check_Connected(m_pPipeBoardCom->m_pStatingPipe, 31);	
		}		
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