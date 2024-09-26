#pragma once
#include "Item.h"
class CSmallFruit : public CItem
{
private:
	explicit CSmallFruit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSmallFruit();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	   void	   Use_Item();
	//virtual	   void	   Render_QuickItem();
private:
	HRESULT    Add_Component();

	CTexture* m_pCountRCTextureCom;
	CTransform* m_pCountRCTransformCom;

public:
	static CSmallFruit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

