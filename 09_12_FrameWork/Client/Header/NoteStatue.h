#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END

class CNoteStatue :public Engine::CGameObject
{
private:
	explicit CNoteStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNoteStatue();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Play_Note();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

private:
	HRESULT    Add_Component();

public:
	static CNoteStatue* Create(LPDIRECT3DDEVICE9 pGraphicDev);	
	void Set_NoteID(_int _iID) { m_iNoteID = _iID; }
	void Set_Group(CGameObject* _pObj) { m_pGroup = _pObj; }
	void Key_Input(const _float& fTimeDelta);

private:
	_int m_iNoteID;
	CGameObject* m_pGroup;

private:
	virtual void Free();
};
