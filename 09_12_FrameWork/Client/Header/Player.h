#pragma once
#include "GameObject.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "PlayerDash.h"
#include "PlayerSwing.h"
#include "PlayerLift.h"
#include "PlayerLiftMove.h"
#include "PlayerPush.h"
#include "PlayerDance.h"
#include "PlayerSmash.h"
#include "PlayerRolling.h"
#include "PlayerHurt.h"
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
// Engine_Enum에서 작동 안 하길래 여기에 작성
enum CONTROL_KEY
{
	PLY_LEFTKEY = DIK_LEFT,
	PLY_RIGHTKEY = DIK_RIGHT,
	PLY_UPKEY = DIK_UP,
	PLY_DOWNKEY = DIK_DOWN,
	PLY_DASHKEY = DIK_LSHIFT,
	PLY_SWINGKEY = DIK_A,
	PLY_LIFTKEY = DIK_S,
	PLY_ROLLKEY = DIK_D,
	PLY_DANCEKEY = DIK_Q,
	PLY_SMASHKEY = DIK_E,
};
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
	// 애니메이션 관련 /////////////////////////////////////////////////////////
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
	_vec3			GetPlayerDirVector2()
	{
		_vec3 returnValue = _vec3(0.f, 0.f, 0.f);
		if (m_iPlayerDir & OBJDIR_LEFT)
			returnValue.x = -1;
		else if (m_iPlayerDir & OBJDIR_RIGHT)
			returnValue.x = 1;
		else
			returnValue.x = 0;

		if (m_iPlayerDir & OBJDIR_BACK)
			returnValue.z = 1;
		else if (m_iPlayerDir & OBJDIR_FRONT)
			returnValue.z = -1;
		else
			returnValue.z = 0;

		return returnValue;

	}
	void			SetPlayerDirection();
	void			FixPlayerDir(int _fixDir)
	{
		m_iPlayerDir = _fixDir;
		DisableDiagonal();
		m_pAnimationCom->SetAnimDir(m_ePlayerState, m_iPlayerDir, false);
	}
	void			DisableDiagonal() { m_bIsDiagonal = false; }
	///////////////////////////////////////////////////////////////////////////

	// 플레이어 능력치 관련 //////////////////////////////////////////////////////
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
		else if (m_tPlayerHP.iCurHP < 0)
			m_tPlayerHP.iCurHP = 0;
	}
	void			SetPlayerMaxHP(_int _SetHP)
	{
		m_tPlayerHP.iMaxHP = _SetHP;
	}
	void			SetInvincible(_bool value = true) { m_bInvincible = value; }
	bool			IsInvincible() { return m_bInvincible; }
	void			DurationInvincible(const _float& fTimeDelta);
	////////////////////////////////////////////////////////////////////////////
	CCamera* GetCamera() { return m_pCamera; }
	void			SetCamera(CCamera* _camera) { m_pCamera = _camera; }
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

	///////////////////////////////////////////////////////
	float m_fMoveSpeed;
	_bool m_bInvincible;

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

