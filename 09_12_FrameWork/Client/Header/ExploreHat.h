#pragma once
#include "Item.h"

class CExploreHat : public CItem
{
private:
	explicit CExploreHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExploreHat();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CExploreHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

