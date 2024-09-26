#pragma once

#include "BoundBox.h"
#include "GameObject.h"


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
	void Lateupdate();

private:
	UINT m_arrCheck[(UINT)GROUP_TYPE::GROUP_END]; // 32바이트 32개. uint는 4바이트이브로 32비트

public:
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);

	void Reset()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::GROUP_END);
	}

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CComponent* _pLeftCol, CComponent* _pRightCol);

};

END