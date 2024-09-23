#pragma once
#include "GameObject.h"
#include "Export_Utility.h"

#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CPlayer;
class CItemSelector;

class CInvenUI : public Engine::CGameObject
{
private:
	explicit CInvenUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInvenUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void      LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();


public:
	_bool	Is_Empty(CItem::ITEMTYPE _Itemfilter) { return m_ItemList[_Itemfilter].empty(); }
	void	Add_Item(CItem* _Item)
	{
		int eType = _Item->Get_ItemInfo().eType;
		if (m_ItemList[eType].size() > 14)
			return;

		m_ItemList[eType].push_back(_Item);
		_Item->Set_ItemPos(m_vInvenPos);

		Set_InvenPos(m_ItemList[eType].size());
	};
	void	Set_InvenPos(int _iInvenCount)
	{
		if (_iInvenCount % 5 == 0)
		{
			m_vInvenPos = { -459.5f,  39.5f - (m_InvenInterval.y * (_iInvenCount / 5)), 0.1f };
			return;
		}
		m_vInvenPos.x += m_InvenInterval.x;
	}

	CItem::ITEMTYPE			Get_CurFilter() { return m_tItemFilter; }
	_int					Get_FilterItemCount(CItem::ITEMTYPE _eItemfilter) { return m_ItemList[_eItemfilter].size(); }
	_vec3					Get_CurItemPos(CItem::ITEMTYPE _Itemfilter, int _idx) { return m_ItemList[_Itemfilter].at(_idx)->Get_ItemPos(); }
	CItem::ITEM_INFO		Get_ItemInfo(CItem::ITEMTYPE _Itemfilter, int _idx)
	{
		return m_ItemList[_Itemfilter].at(_idx)->Get_ItemInfo();
	}
private:
	HRESULT    Add_Component();

	void	   Key_Input(const _float& fTimeDelta);

private:
	enum INVEN_UI { BACKGROUND, DEFAULT, FRAME, EQUIP, CONSUM, OTHER, QUEST, ICONBUTTON, INVENUI_END };

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[INVENUI_END];
	Engine::CTransform* m_pTransformCom[INVENUI_END];

	CItemSelector* m_pItemSelector;
	_bool				m_bCursorCreate;

private:
	// 0913 임시 추가 코드
	CPlayer* m_pPlayer;

	vector<CItem*>	m_ItemList[CItem::TYPE_END];

	_vec3			m_vInvenPos; // 인벤 칸 위치
	_vec2			m_InvenInterval; // 간격

	CItem::ITEMTYPE  m_tItemFilter;

public:
	static CInvenUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

