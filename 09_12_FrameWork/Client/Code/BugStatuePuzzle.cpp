#include "pch.h"
#include "BugStatuePuzzle.h"
#include "BugStatue.h"
#include "StoneBlock.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CBugStatuePuzzle::CBugStatuePuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBugStatuePuzzle::~CBugStatuePuzzle()
{
}

HRESULT CBugStatuePuzzle::Ready_GameObject()
{
	m_iStatueCount = 0;
	return S_OK;
}

_int CBugStatuePuzzle::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CBugStatuePuzzle::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CBugStatuePuzzle::Render_GameObject()
{
}

CBugStatuePuzzle* CBugStatuePuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBugStatuePuzzle* pStatuePuzzle = new CBugStatuePuzzle(pGraphicDev);

	if (FAILED(pStatuePuzzle->Ready_GameObject()))
	{
		Safe_Release(pStatuePuzzle);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStatuePuzzle);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStatuePuzzle);

	return pStatuePuzzle;
}

void CBugStatuePuzzle::Check_Matched()
{
	for (int i = 0; i < m_iStatueCount; ++i)
	{
		if (static_cast<CBugStatue*>(m_vecStatues[i])->Get_ImageID() != 1)
			return;
		else
			static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(2);
	}

	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock();
		static_cast<CStoneBlockHole*>(m_vecStoneBlocksHoles[i])->Set_ImageID(1);
	}
}

void CBugStatuePuzzle::Key_Input(const _float& fTimeDelta)
{
	//if (Engine::GetKeyUp(DIK_1))
	//{		
	//	static_cast<CBugStatue*>(m_vecStatues[0])->Active_StoneBlock();
	//}

	//if (Engine::GetKeyUp(DIK_2))
	//{
	//	static_cast<CBugStatue*>(m_vecStatues[1])->Active_StoneBlock();
	//}

	//if (Engine::GetKeyUp(DIK_3))
	//{
	//	static_cast<CBugStatue*>(m_vecStatues[2])->Active_StoneBlock();
	//}
}

void CBugStatuePuzzle::Free()
{
	Engine::CGameObject::Free();
}