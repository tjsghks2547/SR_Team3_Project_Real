#pragma once
#include "NPC.h"


class CExpressMonkey : public CNPC
{
	// 1. 충돌 시 S버튼 활성화 (대화하기[S])
	// 2. TextBoxUI랑 NPC 이름, NPC 대화내용 출력 글씨 하나씩 출력
	//	  0.05초에 한글자씩..? 출력^_^;;
	//    1) 말풍 하나만 띄워보기
	//    2) 말풍 여러 개 띄우고 스페이스바 클릭 시 텍스트 전부 출력 + 다음 텍스트 출력 
	// 3. 플레이어에게 퀘스트 주기
private:
	explicit CExpressMonkey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	 ~CExpressMonkey();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

private:
	HRESULT    Add_Component();

public:
	static CExpressMonkey* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

