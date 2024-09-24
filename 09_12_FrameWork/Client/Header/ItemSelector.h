#pragma once
#include "GameObject.h"
#include "Item.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;

END
class CInvenUI;

class CItemSelector : public Engine::CGameObject
{
private:
	explicit CItemSelector(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItemSelector();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	   ResetIdx() { m_iCurIdx = -5; }
private:
	HRESULT    Add_Component();
	void	   Key_Input(const _float& fTimeDelta);

	void	   Update_ItemInfo();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	//±ÍÂú¾Æ¼­ °Á ÀÌ°É·Î Çß¾î¿© ¿åÇÒÅ×¸é ÇØºÁ..!
	Engine::CTexture* m_pTexButtonCom;
	Engine::CTransform* m_pTransButtonCom;


	CInvenUI* m_pInven;

	_int		m_iCurIdx;
	_int		m_iPrevIdx;

	CItem::ITEM_INFO	m_tCopyInfo;

public:
	static CItemSelector* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

