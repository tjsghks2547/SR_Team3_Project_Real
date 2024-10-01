#pragma once
#include "Item.h"
class CBranch : public CItem
{
private:
	explicit CBranch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBranch();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	//virtual	   void	    Use_Item();

private:
	HRESULT    Add_Component();

	CTexture* m_pCountRCTextureCom;
	CTransform* m_pCountRCTransformCom;
public:
	static CBranch* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

