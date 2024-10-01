#pragma once

#include "Base.h"
#include "Component.h"
#include "Collider.h"

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
	OBJ_TYPE						   m_eObjType;
public:
	CComponent* Find_Component(COMPONENTID eID, const _tchar* pComponentTag);

public:
	virtual void  Free();


//==============================================================================
//0915 Seonhwan
protected:
	wstring m_strKey; // ���ӿ�����Ʈ�� �ڽ��� � ��Ī���� ������ִ��� �˱� ���ؼ�
	TAG m_eTag;
	//0920 Seonhwan
	wstring m_strTextureKey; // ���ӿ�����Ʈ�� �ؽ�ó Ű 
	_bool m_bIsActive;

//0915 Seonhwan
public:
	void SetObjectKey(wstring _strKey) { m_strKey = _strKey; }
	wstring GetObjectKey() { return m_strKey; }
	TAG Get_Tag() { return m_eTag; }
	void Set_Active(_bool _bIsActive) { m_bIsActive = _bIsActive; }
	_bool Is_Active() { return m_bIsActive; }

	//0920 Seonhwan
	
	void SetTextureKey(wstring _strKey) { m_strTextureKey = _strKey; }
	wstring GetTextureKey() { return m_strTextureKey; }
	void SetObjectType(OBJ_TYPE _eObjType) { m_eObjType = _eObjType; }
	OBJ_TYPE GetObjectType() { return m_eObjType; }

public:
	virtual void OnCollision(CGameObject* _pOther) {  };
	virtual void OnCollisionEnter(CGameObject* _pOther) {  };
	virtual void OnCollisionExit(CGameObject* _pOther) {  };	
};

END
