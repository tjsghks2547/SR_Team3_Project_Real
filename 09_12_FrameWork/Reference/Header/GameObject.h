#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();


public:
	CComponent* Get_Component(COMPONENTID eID, const _tchar* pComponentTag);
	//0913
	_float				Get_ViewZ() const { return m_fViewZ; }

public:
	virtual  HRESULT  Ready_GameObject();
	virtual  _int	  Update_GameObject(const _float& fTimeDelta);
	virtual  void     LateUpdate_GameObject(const _float& fTimeDelta);
	virtual  void     Render_GameObject() PURE;
	
	//0913
	void			Compute_ViewZ(const _vec3* pPos);

protected:
	map<const _tchar*, CComponent*>    m_mapComponent[ID_END];
	LPDIRECT3DDEVICE9				   m_pGraphicDev;

	//0913
	_float									m_fViewZ;

private:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

protected:
	virtual void  Free();


//0915 Seonhwan
protected:
	string m_strKey; // 게임오브젝트가 자신이 어떤 명칭으로 저장되있는지 알기 위해서

//0915 Seonhwan
public:
	void SetObjectKey(string _strKey) { m_strKey = _strKey; }
	string GetObjectKey() { return m_strKey; }
};

END
