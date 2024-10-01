#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CPipeBoardCom;
class CCollider;

END

class CPipeBoard :public Engine::CGameObject
{
private:
	explicit CPipeBoard(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPipeBoard();

public:	
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:	
	HRESULT    Add_Component();
	void	On_Interacted();
	void	On_Exit();
	void	Reset_Connected();
	void	Check_Connected(CGameObject* _pPipe, _int _eID);
	void Key_Input(const _float& fTimeDelta);

private:
	bool m_bIsInteracting;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CPipeBoardCom* m_pPipeBoardCom;
	Engine::CCollider* m_pBoundBox;

public:
	static CPipeBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
};