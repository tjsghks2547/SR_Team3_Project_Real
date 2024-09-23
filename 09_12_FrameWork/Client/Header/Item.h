#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include "Export_System.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CItem : public Engine::CGameObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItem();

public:

	enum ITEMTYPE { EQUIP, CONSUM, OTHER, QUEST, TYPE_END };

	enum EQUIPITEM { EXPLORE, EQUIP_END };
	enum CONSUMITEM { MINI_FRUIT, MIDDLE_FRUIT, BIG_FRUIT, CONSUM_END };
	enum OTHERITEM { LEAF, BRANCH, OTHER_END };
	enum QUESTITEM { NET, DRAWTOOL, QUEST_END };

	struct ITEM_INFO
	{
		ITEMTYPE			eType;
		_int				eItemEnum;
		const _tchar*		pName;
		const _tchar*		pInfo;
		_int				iPrice;
	};

public:
	ITEM_INFO   Get_ItemInfo() { return m_tInfo; }
	_vec3		Get_ItemPos() 
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		return vPos;
	}

	void		Set_ItemPos(_vec3 _ItemPos) 
	{ m_pTransformCom->m_vInfo[INFO_POS] = _ItemPos; }

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

protected:
	HRESULT    Add_Component();

protected:
	Engine::CRcTex*				m_pBufferCom;
	CTexture*					m_pTextureCom;
	CTransform*					m_pTransformCom;
	//vector<Engine::CTexture*>				m_pTextureCom[TYPE_END];
	//vector<Engine::CTransform*>			m_pTransformCom[TYPE_END];

protected:
	ITEM_INFO				m_tInfo;

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free();
};

