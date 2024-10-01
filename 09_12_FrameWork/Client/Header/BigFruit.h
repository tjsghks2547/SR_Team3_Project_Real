#pragma once
#include "Item.h"
class CBigFruit : public CItem
{
private:
	explicit CBigFruit(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBigFruit();

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
	static CBigFruit* Create(LPDIRECT3DDEVICE9 pGraphicDev);


};

