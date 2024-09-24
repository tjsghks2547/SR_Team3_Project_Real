#pragma once

#include "BoundBox.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr 
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void init();
	void update();

private:

	map<GROUP_TYPE, CBoundBox*> m_mapCollisionGroup;
	bool isCollision(CBoundBox* _pLeftBoundBox, CBoundBox* _pRightBoundBox);
	



};

END