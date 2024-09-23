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
	virtual  void	  LateReady_GameObject();
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

public:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

public:
	virtual void  Free();


//==============================================================================
//0915 Seonhwan
protected:
	wstring m_strKey; // 게임오브젝트가 자신이 어떤 명칭으로 저장되있는지 알기 위해서

	//0920 Seonhwan
	wstring m_strTextureKey; // 게임오브젝트의 텍스처 키 

//0915 Seonhwan
public:
	void SetObjectKey(wstring _strKey) { m_strKey = _strKey; }
	wstring GetObjectKey() { return m_strKey; }

	//0920 Seonhwan
	
	void SetTextureKey(wstring _strKey) { m_strTextureKey = _strKey; }
	wstring GetTextureKey() { return m_strTextureKey; }

};

END
