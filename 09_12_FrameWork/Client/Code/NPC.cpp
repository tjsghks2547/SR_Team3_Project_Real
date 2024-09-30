#include "pch.h"
#include "NPC.h"
#include "TextBox.h"

CNPC::CNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, m_pInterButton(nullptr), m_pTextBox(nullptr), m_pQuestAcceptUI(nullptr)
	//, m_pTex(nullptr)
	, m_bConversation(false)
	, m_bQuestAccept(false), m_bQuestSucess(false), m_bQuestClear(false)
{
	ZeroMemory(&m_pTex, sizeof(m_pTex));
}

CNPC::~CNPC()
{
}

HRESULT CNPC::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/UI/ExclamMarkAnim.png", &m_pTex[EXCLAMATION]);
	m_pMarkAnimatorCom->CreateAnimation(L"ExclamMarkAnim", m_pTex[EXCLAMATION], _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7, _vec2(2048.f, 256.f));

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/UI/GrayQuestAnim.png", &m_pTex[GRAYQUEST]);
	m_pMarkAnimatorCom->CreateAnimation(L"GrayQuestAnim", m_pTex[GRAYQUEST], _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 1, _vec2(512.f, 256.f));

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/UI/YellowQuestAnim.png", &m_pTex[YELLOWQUEST]);
	m_pMarkAnimatorCom->CreateAnimation(L"YellowQuestAnim", m_pTex[YELLOWQUEST], _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7, _vec2(2048.f, 256.f));

	//m_pMarkAnimatorCom[GRAYQUEST]->Play(L"GrayQuest", true);


	//m_pMarkAnimatorCom[YELLOWQUEST]->Play(L"YellowQuestAnim", true);

	return S_OK;
}

void CNPC::LateReady_GameObject()
{
	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);

	m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
	NULL_CHECK_RETURN(m_pInven);

	m_pQuestUI = dynamic_cast<CQuestUI*>(Engine::Get_GameObject(L"Layer_UI", L"Quest_UI"));
	NULL_CHECK_RETURN(m_pQuestUI);

	if (!m_pInterButton) //Interaction
	{
		m_pInterButton = CInteractionButton::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pInterButton);
	}
	if (!m_pTextBox)
	{
		m_pTextBox = CTextBox::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pInterButton);
	}
	if (!m_pQuestAcceptUI)
	{
		m_pQuestAcceptUI = CQuestAccept::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pQuestAcceptUI);
	}
}

_int CNPC::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pInterButton->Update_GameObject(fTimeDelta);
	m_pTextBox->Update_GameObject(fTimeDelta);
	m_pQuestAcceptUI->Update_GameObject(fTimeDelta);

	return iExit;
}

void CNPC::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pQuestAcceptUI->LateUpdate_GameObject(fTimeDelta);
}

void CNPC::Render_GameObject()
{

}

HRESULT CNPC::Add_Component()
{
	CComponent* pComponent = NULL;

	// 렉트텍스쳐
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pMarkAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_ExclamAni_Buffer", pComponent });

	pComponent = m_pMarkTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformMark", pComponent });
	m_pMarkTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.5f };
	m_pMarkTransformCom->m_vScale = { 10.f, 10.f, 1.f };

	return S_OK;
}



void CNPC::Free()
{
	Engine::CGameObject::Free();
}
