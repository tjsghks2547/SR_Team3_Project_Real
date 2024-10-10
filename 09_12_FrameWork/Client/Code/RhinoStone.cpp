#include "pch.h"
#include "RhinoStone.h"
#include "ctime"
#include "Rhino.h"
#include "Player.h"

Engine::_uint CRhinoStone::m_iStaticID = 0;

CRhinoStone::CRhinoStone(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_bPosSet(false), m_bRotation(false)
	, m_fTime(0), m_bDead(false)
{
}

CRhinoStone::~CRhinoStone()
{
}

HRESULT CRhinoStone::Ready_GameObject()
{
	//temp
	m_iIndex = m_iStaticID;
	m_iStaticID++;

	Engine::CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_eState = STONE_DEAD;

	//리노에도 만들어야하륻ㅅ
	srand(unsigned(time(NULL))); //난수 만들기

	SetObjectType(OBJ_TYPE::DESTROY_ABLE);

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/ArenaMonster/RhinoStone.png", &m_pStoneTex);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneAppear", m_pStoneTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.1428f, 6);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneDestroy", m_pStoneTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.1428f, 6);
	m_pAnimatorCom->CreateAnimation(L"RhinoStoneSurvive", m_pStoneTex, _vec2(1536.f, 0.f), _vec2(256.f, 256.f), _vec2(0.f, 0.f), 0.5f, 0);

	return S_OK;
}

void CRhinoStone::LateReady_GameObject()
{
	Engine::CGameObject::LateReady_GameObject();

	m_pRhino = dynamic_cast<CRhino*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Rhino"));
	NULL_CHECK_RETURN(m_pRhino);

	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);
}

_int CRhinoStone::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eState == STONE_DEAD)
		return 0;

	m_fTime += fTimeDelta;

	Add_RenderGroup(RENDER_ALPHA, this);

	if (!m_bPosSet)
	{
		m_bPosSet = true;

		_int iRandomX = 350 + (rand() % 300);
		_int iRandomZ = 220 + (rand() % 350);
		m_pTransformCom->m_vInfo[INFO_POS].x = iRandomX;
		m_pTransformCom->m_vInfo[INFO_POS].z = iRandomZ;

		//_float fX = 350 + m_iIndex * 40.f;
		//_float fZ = 220 + m_iIndex * 40.f;
		//m_pTransformCom->m_vInfo[INFO_POS].x = fX;
		//m_pTransformCom->m_vInfo[INFO_POS].z = fZ;
	}

	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CRhinoStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRhinoStone::Render_GameObject()
{
	if (m_eState == STONE_DEAD)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//월드행렬 가져옴

	switch (m_eState)
	{
	case STONE_TARGET: // 타겟 지정
		if (!m_bRotation) // 회전 안했다면.
		{
			m_pTransformCom->Rotation(ROT_X, 80.f);
			m_pTransformCom->m_vInfo[INFO_POS].y = 10.f;
			m_bRotation = true;
		}
		//얘는 애니가 아니고 텍스쳐라서 일케 해줘야함.
		m_pTextureCom->Set_Texture();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 컬링 ㄴㄴ
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pColliderCom->Render_Buffer();

		if (m_fTime >= 3.f) // 3초 후
		{
			m_eState = STONE_APPEAR; // 돌생성애니 
			m_fTime = 0.f; // 시간 리셋
		}
		return;// (여기 들어올 경우 여기까지만하고 리턴 시킴)

	case STONE_APPEAR: // 돌생성
		if (m_bRotation) // 회전 했다면 원래대로 돌려줌.
		{
			m_pTransformCom->Rotation(ROT_X, -80.f);
			m_pTransformCom->m_vInfo[INFO_POS].y = 43.f;
			m_bRotation = false;
		}
		m_pAnimatorCom->Play(L"RhinoStoneAppear", false); // 애니 실행
		if (m_pAnimatorCom->GetAnimation()->IsFinish())
		{
			m_pAnimatorCom->SetAnimationFrame(L"RhinoStoneAppear", 0);
			m_fTime = 0.f; // 시간 리쏐
			m_eState = STONE_SURVIVE; // 살아남은 돌
		}
		break;

	case STONE_SURVIVE:
		m_pAnimatorCom->Play(L"RhinoStoneSurvive", true); // 그냥 하나만 출력임
		m_fTime = 0.f;
		break;

	case STONE_BROKEN:
		m_pAnimatorCom->Play(L"RhinoStoneDestroy", false); // 충돌 브로큰 할거
		if (m_pAnimatorCom->GetAnimation()->IsFinish())
		{
			m_pAnimatorCom->SetAnimationFrame(L"RhinoStoneDestroy", 0);
			m_eState = STONE_DEAD; // 상태를 죽음으로
			m_bPosSet = false;
			//m_bRotation = false;
			m_fTime = 0.f;
			return;
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // 컬링 ㄴㄴ

	m_pGraphicDev->SetTexture(0, m_pStoneTex);
	m_pAnimatorCom->render();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // 컬링다시 ㅇㅇ

	m_pColliderCom->Render_Buffer();

}

void CRhinoStone::OnCollision(CGameObject* _pOther)
{
	if (m_bDead)
		m_bIsActive = false;

	if (m_eState != STONE_SURVIVE)
		return;

	if (_pOther->GetObjectKey() == L"Rhino")
	{
		m_pRhino->Set_CurHP(-100);
		m_bPosSet = false;
		m_eState = STONE_BROKEN;
		m_pRhino->Set_Stun();
	}

	if (_pOther->GetObjectKey() == L"Player")
	{
		m_pPlayer->SetPlayerCurHP(-1);
	}

}

HRESULT CRhinoStone::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StoneCircle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureCircle", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 60.f, 60.f, 30.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 43.f, 0.0f };

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pColliderCom->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CRhinoStone* CRhinoStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRhinoStone* pStone = new CRhinoStone(pGraphicDev);

	if (FAILED(pStone->Ready_GameObject()))
	{
		Safe_Release(pStone);
		MSG_BOX("RhinoStone Create Failed");
		return nullptr;
	}

	return pStone;
}

void CRhinoStone::Free()
{
	Engine::CGameObject::Free();
}
