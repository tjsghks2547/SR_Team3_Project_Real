#pragma once


#include "Base.h"
#include "Define.h"
#include "Engine_Define.h"



class CLoading : public CBase
{

public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END};

private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();


public:
	LOADINGID			Get_LoadingID() { return m_eID; }
	CRITICAL_SECTION*	Get_Crt() { return &m_Crt; }
	_bool				Get_Finish() { return m_bFinish; }


	const _tchar* Get_String() { return m_szLoading; }

public:
	HRESULT   Ready_Loading(LOADINGID eLoading);
	_uint     Loading_Stage();
	_uint	  Loading_AnimData();

public:
	static unsigned int __stdcall Thread_Main(void* pArg);

private:
	LPDIRECT3DDEVICE9  m_pGraphicDev; 
	_bool			   m_bFinish;

	HANDLE			   m_hThread;
	LOADINGID          m_eID;

	CRITICAL_SECTION   m_Crt;


	_tchar			m_szLoading[128];


public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void Free();


};

