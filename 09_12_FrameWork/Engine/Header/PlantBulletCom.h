#pragma once
#include "Component.h"

BEGIN(Engine)

class  ENGINE_DLL CPlantBulletCom :public CComponent
{
private:
	explicit CPlantBulletCom();
	explicit CPlantBulletCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlantBulletCom(const CPlantBulletCom& rhs);
	virtual ~CPlantBulletCom();

public:
	HRESULT					Ready_Bullet();
	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component();

public:

private:


public:
	static CPlantBulletCom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void			Free();
};

END