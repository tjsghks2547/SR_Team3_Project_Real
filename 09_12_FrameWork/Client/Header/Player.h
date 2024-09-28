#pragma once
#include "GameObject.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerDash.h"
#include "PlayerSwing.h"
#include "PlayerLift.h"
#include "PlayerLiftMove.h"
#include "PlayerPush.h"
#include "Export_Utility.h"

//0920
#include "ExploreHat.h"
#include "PartyHat.h"
#include "SmallFruit.h"
#include "MiddleFruit.h"
#include "Mohican.h"
#include "InvenUI.h"
#include "QuickSlot.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCamera;
class CCalculator;
class CStateController;

//0923 선환 충돌 테스트
class CCollider;

END

struct PLAYERINFO
{
	_int iCurrHP;
	_int iMaxHP;

	// 플레이어 장비/인벤토리 관련
	_int m_iCurrCoin;
	_int m_iEquippedHat_IndexNumber;
	// 
};

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	//0913 진짜진짜 임시
	struct PLAYERHP
	{
		_int	iCurHP;
		_int	iMaxHP;
	};

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

public:
	CAnimation* GetAnimationComp() { return m_pAnimationCom; }

	PLAYERSTATE		GetPlayerState() { return m_ePlayerState; }
	void			SetPlayerState(PLAYERSTATE _ePlayerState)
	{
		m_ePlayerState = _ePlayerState;
		m_pAnimationCom->SetAnimFrame(m_ePlayerState, m_bIsDiagonal);
	}

	_bool			GetSwingTrigger() { return m_bSwingTrigger; }
	void			SetSwingTrigger(_bool bSwing) { m_bSwingTrigger = bSwing; }
	_bool			GetPushTrigger() { return m_bPushTrigger; }
	void			SetPushTrigger(_bool bPush) { m_bPushTrigger = bPush; }
	CGameObject* GetCollideObj() { return m_CCollideObj; }
	void			SetCollideObj(CGameObject* _obj) { m_CCollideObj = _obj; }

	_vec3			GetPlayerDirVector() { return m_vPlayerDir; }
	void			SetPlayerDirection();

	CCamera* GetCamera() { return m_pCamera; }
	void			SetCamera(CCamera* _camera) { m_pCamera = _camera; }

	float			GetMoveSpeed() { return m_fMoveSpeed; }
	void			SetMoveSpeed(float _fSpeed) { m_fMoveSpeed = _fSpeed; }
	//0913 임시 코드
	_int			GetPlayerCoin() { return m_iPlayerCoin; }
	PLAYERHP		GetPlayerHP() { return m_tPlayerHP; }
	_bool			GetPlayerInven() { return m_bInven; }
	//0924 임시 추가 코드
	void			SetPlayerCurHP(_int _SetHP)
	{
		m_tPlayerHP.iCurHP += _SetHP;
		if (m_tPlayerHP.iCurHP > m_tPlayerHP.iMaxHP)
			m_tPlayerHP.iCurHP = m_tPlayerHP.iMaxHP;
	}

	CItem* hat;
private:
	HRESULT			Add_Component();
	void			Key_Input(const _float& fTimeDelta);
	_vec3			Piking_OnTerrain();
	void			Print_PlayerState();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CAnimation* m_pAnimationCom;
	Engine::CCamera* m_pCamera;
	Engine::CCalculator* m_pCCalculatorCom;
	Engine::CStateController* m_pStateControlCom;

	//바운드 박스 테스트 
	Engine::CCollider* m_pBoundBox;

	PLAYERSTATE m_ePlayerState;
	//OBJ_DIRECTION m_ePlayerDir;
	int	  m_iPlayerDir;
	bool m_bIsDiagonal;

	// 현재 프레임 - 이전 프레임 하여 플레이어가 이동한 방향을 알아냄
	_vec3 m_vPlayerCurrPos, m_vPlayerPrevPos;
	_vec3 m_vPlayerDir;

	_bool m_bSwingTrigger;
	_bool m_bPushTrigger;
	CGameObject* m_CCollideObj;

	float m_fMoveSpeed;

	//0913 임시 코드
	_bool		m_bInven;
	CInvenUI* m_pInven;
	CQuickSlot* m_pQuickSlot;

	_int		m_iPlayerCoin;
	PLAYERHP	m_tPlayerHP;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

