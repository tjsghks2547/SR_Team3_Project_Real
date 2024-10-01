#pragma once
#include "Component.h"

BEGIN(Engine)

class  ENGINE_DLL CPlantBullet :public CComponent
{
private:
	explicit CPlantBullet();
	explicit CPlantBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlantBullet(const CPlantBullet& rhs);
	virtual ~CPlantBullet();

public:
	HRESULT					Ready_Bullet();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

public:

private:


public:
	static CPlantBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();
};

END