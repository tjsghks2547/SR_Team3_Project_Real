#pragma once
#include "GameObject.h"
#include "Export_Utility.h"
#include "Export_System.h"
//#include "Player.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CPlayer;
class CItem : public Engine::CGameObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CItem();

public:

	enum ITEMTYPE { EQUIP, CONSUM, OTHER, QUEST, TYPE_END };
#pragma 
	enum EQUIPITEM { EXPLORE, PARTY, MOHICAN, ENKY, APPLE, POOP, PENGUIN, EQUIP_END };
	enum CONSUMITEM {
		SMALL_FRUIT, MIDDLE_FRUIT, BIG_FRUIT,
		SMALL_ROOT, MIDDLE_ROOT, BIG_ROOT,
		SMALL_HERB, MIDDLE_HERB, BIG_HERB,
		COOK_SMALLFISH, COOK_MIDDLEFISH, COOK_BIGFISH, CONSUM_END
	};
	enum OTHERITEM { LEAF, BRANCH, OTHER_END };
	enum QUESTITEM { NET, DRAWTOOL, EXTICKET, QUEST_END };

	struct ITEM_INFO
	{
		ITEMTYPE			eType;
		_int				eItemEnum;
		const _tchar* pName;
		const _tchar* pInfo;
		_int				iPrice;
		_int				iItemCount;
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
	{
		m_pTransformCom->m_vInfo[INFO_POS] = _ItemPos;
	}
	void		Set_QuickPos(_vec3 _ItemPos)
	{
		m_pQuickTransformCom->m_vInfo[INFO_POS] = _ItemPos;
		m_pQuickTransformCom->m_vScale = { 27.f, 27.f, 0.1f };
	}
	void		Set_ItemCount(_int _AddItemCount) { m_tInfo.iItemCount += _AddItemCount; }

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	virtual   void      Use_Item() {}
	void				Render_QuickItem();
	void				Render_ItemView();

protected:
	HRESULT    Add_Component();

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

	Engine::CTransform* m_pQuickTransformCom;

	Engine::CTransform* m_pViewTransformCom;


protected:
	ITEM_INFO				m_tInfo;
	CPlayer* m_pPlayer;

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free();
};

