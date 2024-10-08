#pragma once
#include "Player.h"
#include "PlayerInteractionBox.h"
#include "ExamineButton.h"
#include "CloseButton.h"
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
	virtual	  void		LateReady_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);
	virtual void SetPlayer(CPlayer* _Player) { m_CPlayer = _Player; }

private:	
	HRESULT    Add_Component();
	void	On_Interacted();
	void	On_Exit();
	void	Reset_Connected();
	void	Check_Connected(CGameObject* _pPipe, _int _eID);
	void Key_Input(const _float& fTimeDelta);

private:	
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTransform* m_pTexTransformCom;	
	Engine::CPipeBoardCom* m_pPipeBoardCom;
	Engine::CCollider* m_pBoundBox;

private:
	_bool m_bIsInteracting;
	CExamineButton* m_pExamineButton;
	CCloseButton* m_pCloseButton;

protected:
	CPlayer* m_CPlayer;

public:
	static CPipeBoard* Create(LPDIRECT3DDEVICE9 pGraphicDev);	

private:
	virtual void Free();
};