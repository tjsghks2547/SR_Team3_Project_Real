#pragma once
#include "Item.h"

class CPartyHat : public CItem
{
private:
	explicit CPartyHat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPartyHat();

public:

	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CPartyHat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

};

