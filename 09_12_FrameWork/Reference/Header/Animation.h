#pragma once

// 이미지 : 특정 애니메이션 모습이 담겨 있는 png파일 한 장 (Sprite_BabyOgu_Idle.png)
// 프레임 : 이미지 안에 있는 애니메이션 한 조각 (Idle이미지엔 7x4 프레임)

#include "VIBuffer.h"
#include "Export_Utility.h"
BEGIN(Engine)

class ENGINE_DLL CAnimation : public CVIBuffer//CComponent
{

private:
	explicit CAnimation();
	explicit CAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimation(const CAnimation& rhs);
	virtual ~CAnimation();

public:
	HRESULT					Ready_Animation(const vector<_vec2> _vecFrameCount,
		const vector<vector<int>*> _vecFramePlay);

	virtual  _int			Update_Component(const _float& fTimeDelta);
	virtual  void			LateUpdate_Component(const _float& fTimeDelta);
	virtual	 void   		Render_Buffer();

public:
	void SetAnimFrame(PLAYERSTATE _ePlayerState, bool _bDiagonal);
	void SetAnimDir(PLAYERSTATE _ePlayerState, int _dir, bool _bDiagonal)
	{
		if (_bDiagonal)
			m_eCurrentDir = round(_dir * 0.6 - 3);
		else
			m_eCurrentDir = log(_dir) / log(2); // 0,1,2,3

		SetAnimFrame(_ePlayerState, _bDiagonal);
	}

	void UpdateUV();
	int  GetCurrentFrame() { return currentFrame; }
	bool IsAnimationEnd() { return m_bAnimationEnd; }
	void SetAnimationPlaying() { m_bAnimationEnd = false; }
private:

	// Loading에서 Animation.dat 정보를 받아와서
	// 해당 오브젝트의 모든 행동 애니메이션이 담겨 있음
	vector<_vec2>		 m_vecFrameCount;
	vector<vector<int>*> m_vecFramePlay;

	// 현재 진행중인 상태에 대한 애니메이션 정보
	_vec2				 m_currentFrameCount; // 프레임 개수
	_vec2				 m_vecCurrentFrameUV; // 현재 프레임을 출력하기 위한 UV 비율
	vector<int>			 m_currentFramePlay[OBJ_DIRECTION::OBJDIR_END]; // 프레임 출력 순서


	PLAYERSTATE			m_eCurrentState;
	int					m_eCurrentDir;
	int					m_iCurrentDir;

	int					currentFrame;
	float				m_fAccTime;

	bool				m_bAnimationEnd;
public:
	static CAnimation* Create(LPDIRECT3DDEVICE9	      pGraphicDev,
		const vector<_vec2>		  _vecFrameCount,
		const vector<vector<int>*> _vecFramePlay);

	virtual CComponent* Clone();

private:
	virtual void			Free();


};

END