#include "CollisionMgr.h"
#include "Management.h"


IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::init()
{
	Reset();
}

void CCollisionMgr::update()
{

	// 체크 된 값 즉 1이 있는지 검사하는 코드

	//행 32번 검사 


	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::GROUP_END; ++iRow)
	{

		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::GROUP_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				//여기 중단점 걸렸을때가 이제 1인지점
				//CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr()
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}

}

void CCollisionMgr::Lateupdate()
{

}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}

	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}

}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	const vector<CGameObject*>& vecLeft = CManagement::GetInstance()->GetCurScenePtr()->GetVecObjlist(_eLeft);
	const vector<CGameObject*>& vecRight = CManagement::GetInstance()->GetCurScenePtr()->GetVecObjlist(_eRight);

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{

		// 충돌체를 보유하지 않은 경우 
		if (vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_BoundBox") == nullptr)
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과의 충돌인 경우 
			if (nullptr == vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_BoundBox")
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}


			if (IsCollision(vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_BoundBox"), vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_BoundBox")))
			{
				//충돌 할 경우
				//MSG_BOX("충돌!");

				//여기서 
			}

			else
			{
				//충돌 안할 경우 
			}

		}
	}
}

bool CCollisionMgr::IsCollision(CComponent* _pLeftCol, CComponent* _pRightCol)
{

	CBoundBox* pLeftBoundBox = dynamic_cast<CBoundBox*>(_pLeftCol);
	CBoundBox* pRightBoundBox = dynamic_cast<CBoundBox*>(_pRightCol);

	if (pLeftBoundBox->GetWorldVecMin().x <= pRightBoundBox->GetWorldVecMax().x
		&& pLeftBoundBox->GetWorldVecMax().x >= pRightBoundBox->GetWorldVecMin().x
		&& pLeftBoundBox->GetWorldVecMin().y <= pRightBoundBox->GetWorldVecMax().y
		&& pLeftBoundBox->GetWorldVecMax().y >= pRightBoundBox->GetWorldVecMin().y
		&& pLeftBoundBox->GetWorldVecMin().z <= pRightBoundBox->GetWorldVecMax().z
		&& pLeftBoundBox->GetWorldVecMax().z >= pRightBoundBox->GetWorldVecMin().z)
	{
		pLeftBoundBox->m_bisCollision = true;
		pRightBoundBox->m_bisCollision = true;
		return true;
	}


	return false;
}


