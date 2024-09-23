#pragma once
#include "GameObject.h"
#include "PlayerIdle.h"
#include "PlayerMove.h"
#include "Export_Utility.h"
BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CCamera;
class CCalculator;
class CStateController;

END

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

	PLAYERSTATE		GetPlayerState() { return m_ePlayerState; }
	void			SetPlayerState(PLAYERSTATE _ePlayerState)
	{
		m_ePlayerState = _ePlayerState;
		/*if (m_bIsDiagonal)
			m_ePlayerState = static_cast<PLAYERSTATE>(m_ePlayerState + 1);*/
		m_pAnimationCom->SetAnimFrame(m_ePlayerState);
	}

	OBJ_DIRECTION	GetPlayerDirection() { return m_ePlayerDir; }
	void			SetPlayerDirection();
	bool			GetPlayerDiagonal() { return m_bIsDiagonal; }
	//0913 임시 코드
	_int			GetPlayerCoin() { return m_iPlayerCoin; }
	PLAYERHP		GetPlayerHP() { return m_tPlayerHP; }
	_bool			GetPlayerInven() { return m_bInven; }


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

	float m_fMoveSpeed;
	PLAYERSTATE m_ePlayerState;
	OBJ_DIRECTION m_ePlayerDir;
	bool m_bIsDiagonal;

	_vec3 m_vPlayerCurrPos;
	_vec3 m_vPlayerPrevPos;
	_vec3 m_vPlayerDir;
	int	  m_iPlayerDir;

	//0913 임시 코드
	_int		m_iPlayerCoin;
	PLAYERHP	m_tPlayerHP;
	_bool		m_bInven;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

