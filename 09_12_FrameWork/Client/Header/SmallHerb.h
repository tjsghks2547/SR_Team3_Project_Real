#pragma once
#include "Item.h"
class CSmallHerb : public CItem
{
private:
	explicit CSmallHerb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSmallHerb();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual	   void	    Use_Item();

private:
	HRESULT    Add_Component();

	CTexture* m_pCountRCTextureCom;
	CTransform* m_pCountRCTransformCom;

public:
	static CSmallHerb* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

