#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CInvenUI : public Engine::CGameObject
{
private:
	explicit CInvenUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInvenUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


private:
	HRESULT    Add_Component();

	void	   Key_Input(const _float& fTimeDelta);

private:
	enum INVEN_UI { BACKGROUND, DEFAULT, FRAME, EQUIP, CONSUM, OTHER, QUEST, ICONBUTTON, INVENUI_END };
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[INVENUI_END];
	Engine::CTransform* m_pTransformCom[INVENUI_END];


private:
	// 0913 임시 추가 코드
	CPlayer* m_pPlayer;

public:
	static CInvenUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

