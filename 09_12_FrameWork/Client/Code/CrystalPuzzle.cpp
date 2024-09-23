#include "pch.h"
#include "CrystalPuzzle.h"
#include "Export_Utility.h"

CCrystalPuzzle::CCrystalPuzzle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CCrystalPuzzle::~CCrystalPuzzle()
{
}

HRESULT CCrystalPuzzle::Ready_GameObject()
{
	return S_OK;
}

_int CCrystalPuzzle::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCrystalPuzzle::LateUpdate_GameObject(const _float& fTimeDeltsa)
{
}

void CCrystalPuzzle::Render_GameObject()
{
}

CCrystalPuzzle* CCrystalPuzzle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrystalPuzzle* pCrystal = new CCrystalPuzzle(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CCrystalPuzzle::Check_Matched()
{
	int i = 0;
	for (auto& iter : m_vecPressBlocks)
	{
		if (static_cast<CPressBlock*>(iter)->m_iImageID != static_cast<CCrystal*>(m_vecCrystals[i])->m_iImageID) {
			//MSG_BOX("NOT Matched!");
			return;
		}

		i++;
	}

	for (auto& iter : m_vecCrystals) {
		static_cast<CCrystal*>(iter)->Set_Active(false);
	}		
	for (auto& iter : m_vecPressBlocks) {
		static_cast<CPressBlock*>(iter)->m_bIsCleared = true;
		static_cast<CPressBlock*>(iter)->m_iImageID = 8;
	}

	//All Matched Function
	//MSG_BOX("All Matched!");
}

void CCrystalPuzzle::Key_Input(const _float& fTimeDelta)
{
	if (Engine::GetKeyUp(DIK_2))
	{		
		//for (auto& iter : m_vecCrystals)
		//	static_cast<CPressBlock*>(iter)->On_CollisionEnter();
	}
}

void CCrystalPuzzle::Free()
{
	Engine::CGameObject::Free();
}