#include "Animatior2.h"

CAnimatior2::CAnimatior2()
{
}

CAnimatior2::CAnimatior2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CAnimatior2::CAnimatior2(const CAnimatior2& rhs)
	:CComponent(rhs)
	
{
}

CAnimatior2::~CAnimatior2()
{
}

void CAnimatior2::CreateAnimation(const wstring& _strName, IDirect3DTexture9* _pTex, _vec2 _vLT, _vec2 _vSliceSize, _vec2 _vStep, float _fDuration, UINT _iFrameCount)
{

}

CAnimation2* CAnimatior2::FindAnimation(const wstring& _strName)
{

	return nullptr;
}

void CAnimatior2::LateReady_Component()
{

}


_int CAnimatior2::Update_Component(const _float& fTimeDelta)
{

	return _int();
}

void CAnimatior2::LateUpdate_Component(const _float& fTimeDelta)
{

}



void CAnimatior2::Play(const wstring& _strName, bool _bRepeat)
{

}


CComponent* CAnimatior2::Clone()
{
	return nullptr;
}

void CAnimatior2::Free()
{
}
