#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CBoundBox : public CComponent 
{

private:
	explicit CBoundBox();
	explicit CBoundBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoundBox(const CBoundBox& rhs);
	virtual  ~CBoundBox();


public:
	virtual  HRESULT  Ready_Buffer();
	virtual  void     Render_Buffer();
	virtual  _int	  Update_Component(const _float& fTimeDelta);
	virtual  void	  LateUpdate_Component();


public:
	static CBoundBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	CComponent* Clone();


private:
	virtual void Free();


private:
	LPD3DXMESH m_pMesh; 
	_vec3	   m_pVecMin;
	_vec3	   m_pVecMax;
};
	
END	

