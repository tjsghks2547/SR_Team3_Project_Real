#pragma once
#include "GameObject.h"
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "Export_Utility.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

struct MONSTER_HP
{
	_int	iCurHP;
	_int	iMaxHP;
};

class CMonster : public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:

	virtual			HRESULT			Ready_GameObject();
	virtual			void			LateReady_GameObject();
	virtual			_int			Update_GameObject(const _float& fTimeDelta);
	virtual			void			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void			Render_GameObject();

	virtual			void			OnCollision(CGameObject* _pOther);
public:
	void			SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }
	void			KnockBack(const _float& fTimeDelta, _vec3 vKnockBackDir);

	void			SetInvincible(_bool value = true) { m_bInvincible = value; }
	bool			IsInvincible() { return m_bInvincible; }
	void			DurationInvincible(const _float& fTimeDelta);

	MONSTER_HP		GetMonsterHP() { return m_tMonsterHP; }
	void			SetMonsterCurHP(_int _SetHP)
	{
		m_tMonsterHP.iCurHP += _SetHP;
		if (m_tMonsterHP.iCurHP > m_tMonsterHP.iMaxHP)
			m_tMonsterHP.iCurHP = m_tMonsterHP.iMaxHP;

		if (m_tMonsterHP.iCurHP < 0)
			m_tMonsterHP.iCurHP = 0;
	}

private:
	HRESULT			Add_Component();

private:
	IDirect3DTexture9* m_Texture;
	_int m_iImageID;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CCollider* m_pBoundBox;
	Engine::CTexture* m_pTexture;

	MONSTER_HP m_tMonsterHP;

	CPlayer* m_CPlayer;
	_bool m_bKnockBackTrigger;
	_vec3 m_vKnockBackDir;

	_bool m_bInvincible;
public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

