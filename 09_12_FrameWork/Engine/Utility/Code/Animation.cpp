#include "Animation.h"

CAnimation::CAnimation()
	: m_eCurrentDir(OBJ_DIRECTION::OBJDIR_FRONT)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bIsDiagonal(false)
{
}

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_eCurrentDir(OBJ_DIRECTION::OBJDIR_FRONT)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bIsDiagonal(false)
{
	//pGraphicDev->AddRef();
}

CAnimation::CAnimation(const CAnimation& rhs)
	:CVIBuffer(rhs)
	, m_eCurrentDir(OBJ_DIRECTION::OBJDIR_FRONT)
	, currentFrame(0)
	, m_fAccTime(0.2f)
	, m_bIsDiagonal(false)
{
	m_currentFrameCount = rhs.m_currentFrameCount;
	m_vecFrameCount = rhs.m_vecFrameCount;

	for (int i = 0; i < OBJ_DIRECTION::OBJDIR_END; i++)
	{
		m_currentFramePlay[i] = rhs.m_currentFramePlay[i];

	}
	m_vecFramePlay = rhs.m_vecFramePlay;
}

CAnimation::~CAnimation()
{
}

HRESULT CAnimation::Ready_Animation(
	const vector<_vec2> _vecFrameCount,
	const vector<vector<int>*> _vecFramePlay)
{
	// Animation.dat에 읽은 정보 저장하기
	m_vecFrameCount.clear();
	for (auto iter : _vecFrameCount)
	{
		m_vecFrameCount.push_back(iter);
	}

	m_vecFramePlay.clear();
	for (auto iter : _vecFramePlay)
	{
		m_vecFramePlay.push_back(iter);
	}



	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;


	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[3].vPosition = { -1.f, -1.f, 0.f };

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	SetAnimFrame(PLAYERSTATE::PLY_IDLE);
	return S_OK;
}

_int CAnimation::Update_Component(const _float& fTimeDelta)
{
	m_fAccTime -= fTimeDelta;

	if (m_fAccTime <= 0.f)
	{
		m_fAccTime += 0.2f;

		currentFrame++;
		UpdateUV();
		if (currentFrame >= m_vecFramePlay[m_eCurrentState][m_eCurrentDir].size() - 1)
		{
			currentFrame = 0;
		}
	}
	return 0;
}

void CAnimation::LateUpdate_Component(const _float& fTimeDelta)
{
}

void CAnimation::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const vector<_vec2> _vecFrameCount,
	const vector<vector<int>*> _vecFramePlay)
{
	CAnimation* pAnimation = new CAnimation(pGraphicDev);

	if (FAILED(pAnimation->Ready_Animation(_vecFrameCount, _vecFramePlay)))
	{
		Safe_Release(pAnimation);
		MSG_BOX("Animation Create Failed");
		return nullptr;
	}

	return pAnimation;
}

CComponent* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	CComponent::Free();
}

void CAnimation::SetAnimFrame(PLAYERSTATE _eObjState, bool _bDiagonal)
{
	currentFrame = 0;
	if (_bDiagonal)
		m_eCurrentState = _eObjState;// static_cast<PLAYERSTATE>(_eObjState + 1);
	else
		m_eCurrentState = _eObjState;

	m_currentFrameCount = m_vecFrameCount[m_eCurrentState];

	// 상하좌우 방향
	for (int i = 0; i < OBJ_DIRECTION::OBJDIR_END; i++)
	{
		m_currentFramePlay[i].clear();
		for (auto iter : m_vecFramePlay[m_eCurrentState][i])
		{
			m_currentFramePlay[i].push_back(iter);

		}
	}

	m_vecCurrentFrameUV.x = 1.f / m_currentFrameCount.x;
	m_vecCurrentFrameUV.y = 1.f / m_currentFrameCount.y;

	UpdateUV();
}