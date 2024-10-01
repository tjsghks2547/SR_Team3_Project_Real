#include "CollisionMgr.h"
#include "Management.h"


IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
	:m_arrCheck{}
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

	// üũ �� �� �� 1�� �ִ��� �˻��ϴ� �ڵ�

	//�� 32�� �˻� 


	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::GROUP_END; ++iRow)
	{

		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::GROUP_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				//���� �ߴ��� �ɷ������� ���� 1������
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
	// �� ���� ���� �׷� Ÿ���� ������
	// ū ���� ��(��Ʈ)�� ���

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
	map<ULONGLONG, bool>::iterator iter;	

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{

		// �浹ü�� �������� ���� ��� 
		if (vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider") == nullptr || vecLeft[i]->Is_Active() == false)
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// �浹ü�� ���ų�, �ڱ� �ڽŰ��� �浹�� ��� 
			if (nullptr == vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider") || vecRight[j]->Is_Active() == false || vecLeft[i] == vecRight[j])
			{
				continue;
			}


			// �� �浹ü ���� ���̵� ����
			COLLIDER_ID ID;
			ID.Left_id  = dynamic_cast<CCollider*>(vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider"))->GetID();
			ID.Right_id = dynamic_cast<CCollider*>(vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider"))->GetID();
			ID.ID; // �̰� ���� Ű������ ��� 

			iter = m_mapColInfo.find(ID.ID);			

			// �浹 ������ �� ��� ������ ��� ��� ( �浹���� �ʾҴٷ�) �� ó�� �ʿ� insert �Ǵ°�� 
			// �̹� insert �Ǿ� ������ �ǳʶ�
			if(m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}


			if (IsCollision(vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider"), vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider")))
			{
				//���� �浹 �� ���
				
				if(iter->second)
				{
					// ���ݵ� �浹���ε� �������� �浹�ϰ� �ִ� ���					
					vecLeft[i]->OnCollision(vecRight[j]);
					vecRight[j]->OnCollision(vecLeft[i]);
				}
				else
				{
					// �������� �浹���� �ʾҴ�.
					vecLeft[i]->OnCollisionEnter(vecRight[j]);
					vecRight[j]->OnCollisionEnter(vecLeft[i]);
					iter->second = true;
				}
			}

			else
			{
				//���� �浹 ���� ��� 
				if(iter->second)
				{
					// �������� �浹�ϰ� �־���.
					vecLeft[i]->OnCollisionExit(vecRight[j]);
					vecRight[j]->OnCollisionExit(vecLeft[i]);
					iter->second = false;
				}
			}

		}
	}
}

bool CCollisionMgr::IsCollision(CComponent* _pLeftCol, CComponent* _pRightCol)
{

	CCollider* pLeftBoundBox = dynamic_cast<CCollider*>(_pLeftCol);
	CCollider* pRightBoundBox = dynamic_cast<CCollider*>(_pRightCol);

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


