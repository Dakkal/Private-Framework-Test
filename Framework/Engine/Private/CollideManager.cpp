#include "CollideManager.h"
#include "Collider.h"
#include "GameObject.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CCollideManager);

CCollideManager::CCollideManager()
{
}

void CCollideManager::Check_Collision(const _uint iLevel, const LAYER_TAG& _eType1, const LAYER_TAG& _eType2)
{
	CGameObject* pObj1 = nullptr;
	CGameObject* pObj2 = nullptr;
	CCollider* pCol1 = nullptr;
	CCollider* pCol2 = nullptr;

	CHECKITER	iter;

	CGameInstance* pGameInstace = GET_INSTANCE(CGameInstance);

	auto Obj1 = pGameInstace->Get_LayerList(iLevel, _eType1);
	auto Obj2 = pGameInstace->Get_LayerList(iLevel, _eType2);;

	if (Obj1->empty() || Obj2->empty()) return;

	for (auto iter1 = Obj1->begin(); iter1 != Obj1->end(); ++iter1)
	{
		for (auto iter2 = Obj2->begin(); iter2 != Obj2->end(); ++iter2)
		{
			pObj1 = (*iter1);	pObj2 = (*iter2);

			if (pObj1 == pObj2 || nullptr == pObj1 || nullptr == pObj2) continue;

			pCol1 = dynamic_cast<CCollider*>(pObj1->Get_Component(TEXT("Com_Collider")));
			pCol2 = dynamic_cast<CCollider*>(pObj2->Get_Component(TEXT("Com_Collider")));

			if (nullptr == pCol1 || nullptr == pCol2) continue;
			else if (false == pCol1->Is_Active() || false == pCol2->Is_Active()) continue;

			Set_Info(iter, pCol1, pCol2);
			
			if (pCol1->IsCollision(pCol2)) // �浹
			{
				if (0 < pObj1->Get_Parts_Size() && 0 < pObj2->Get_Parts_Size())
				{
					Check_Part_Collision(pObj1, pObj2);
				}

				if (iter->second) // �������� �浹
				{
					if (nullptr == pObj1 || nullptr == pObj2) // �� �� �ϳ� ���� ����
					{
						pCol1->OnCollision_Exit(pObj2);
						pCol2->OnCollision_Exit(pObj1);
						iter->second = false;
					}
					else // ���� ���� ����
					{
						pCol1->OnCollision_Stay(pObj2);
						pCol2->OnCollision_Stay(pObj1);
					}
				}
				else // �̹��� �浹
				{
					if (nullptr != pObj1 && nullptr != pObj2) // �Ѵ� ������ ������ �ƴ� ���� �浹 ó��
					{
						pCol1->OnCollision_Enter(pObj2);
						pCol2->OnCollision_Enter(pObj1);
						iter->second = true;
					}
				}
			}
			else // �浹 X
			{
				if (iter->second) // ������ �浹
				{
					pCol1->OnCollision_Exit(pObj2);
					pCol2->OnCollision_Exit(pObj1);
					iter->second = false;
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CCollideManager::Check_Part_Collision(CGameObject* _pObj1, CGameObject* _pObj2)
{
	CCollider* pCol1 = nullptr;
	CCollider* pCol2 = nullptr;

	CHECKITER	iter;

	for (auto& pPart1 : _pObj1->Get_Parts())
	{
		for (auto& pPart2 : _pObj2->Get_Parts())
		{
			if (pPart1.second == pPart2.second || nullptr == pPart1.second || nullptr == pPart2.second) continue;

			pCol1 = dynamic_cast<CCollider*>(pPart1.second->Get_Component(TEXT("Com_Collider")));
			pCol2 = dynamic_cast<CCollider*>(pPart2.second->Get_Component(TEXT("Com_Collider")));

			if (nullptr == pCol1 || nullptr == pCol2) continue;
			else if (false == pCol1->Is_Active() || false == pCol2->Is_Active()) continue;

			Set_Info(iter, pCol1, pCol2);

			if (pCol1->IsCollision(pCol2)) // �浹
			{
				if (iter->second) // �������� �浹
				{
					if (nullptr == pPart1.second || nullptr == pPart2.second) // �� �� �ϳ� ���� ����
					{
						pCol1->OnCollision_Exit(pPart2.second);
						pCol2->OnCollision_Exit(pPart1.second);
						iter->second = false;
					}
					else // ���� ���� ����
					{
						pCol1->OnCollision_Stay(pPart2.second);
						pCol2->OnCollision_Stay(pPart1.second);
					}
				}
				else // �̹��� �浹
				{
					if (nullptr != pPart1.second && nullptr != pPart2.second) // �Ѵ� ������ ������ �ƴ� ���� �浹 ó��
					{
						pCol1->OnCollision_Enter(pPart2.second);
						pCol2->OnCollision_Enter(pPart1.second);
						iter->second = true;
					}
				}
			}
			else // �浹 X
			{
				if (iter->second) // ������ �浹
				{
					pCol1->OnCollision_Exit(pPart2.second);
					pCol2->OnCollision_Exit(pPart1.second);
					iter->second = false;
				}
			}
		}
	}
}

void CCollideManager::Set_Info(CHECKITER& _iter, CCollider* _pCol1, CCollider* _pCol2)
{
	COLLIDER_ID ID;

	ID.iLeft_id = _pCol1->Get_ID();
	ID.iRight_id = _pCol2->Get_ID();

	// ������ �浹 �˻� ���� ����
	_iter = m_mapColInfo.find(ID.ID);

	// ���ٸ� ���� �߰�
	if (m_mapColInfo.end() == _iter)
	{
		m_mapColInfo.insert({ ID.ID, false });
		_iter = m_mapColInfo.find(ID.ID);
	}
}

void CCollideManager::Free()
{
	__super::Free();
}
