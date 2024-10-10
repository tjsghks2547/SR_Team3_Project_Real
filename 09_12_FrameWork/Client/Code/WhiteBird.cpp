#include "pch.h"
#include "WhiteBird.h"

CWhiteBird::CWhiteBird(LPDIRECT3DDEVICE9 pGraphicDev)
	:CQuestNPC(pGraphicDev), m_bEnter(false)
{
}

CWhiteBird::~CWhiteBird()
{
}

HRESULT CWhiteBird::Ready_GameObject()
{
    CQuestNPC::Ready_GameObject();

    SetObjectType(OBJ_TYPE::TALK_ABLE);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/WhiteBird.png", &m_pNPCTex);
    m_pAnimatorCom->CreateAnimation(L"WhiteBirdIdle", m_pNPCTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7);
    m_pAnimatorCom->CreateAnimation(L"WhiteBirdTalk", m_pNPCTex, _vec2(0.f, 256.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 3);

    m_tInfo.pName = L"흰 새";
    m_tInfo.pContent = L"왁왁!!! 물의 증표를 도둑 맞았닥!!!으악!!! 방금 북쪽으로 갔억!!!! 따라가봑!!!! 어서어석!!!!!왁윽악!!읶ㄲㄲ";

    m_tQuestInfo.pQuestTitle = L"(메인)물의 증표를 찾아서...";
    m_tQuestInfo.pQuestContent = L"다짜고짜 물의 증표를 찾아오라고 한다. 느낌표는 또 왜 저렇게 큰 것이지? 약간 기분이 더럽지만 궁금하니 도와줘볼까?";

    _vec3 vMarkPos = m_pTransformCom->m_vInfo[INFO_POS];
    vMarkPos.y += 35.f;
    m_pMarkTransformCom->m_vInfo[INFO_POS] = vMarkPos;
    m_pMarkTransformCom->m_vScale = { 20.f, 20.f, 20.f };

    return S_OK;

}

void CWhiteBird::LateReady_GameObject()
{
    CQuestNPC::LateReady_GameObject();
}

_int CWhiteBird::Update_GameObject(const _float& fTimeDelta)
{
    CQuestNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (!m_bQuestClear && m_bQuestAccept)
    {
        if (m_pInven->Find_Item(CItem::QUEST, CItem::EXTICKET))
        {
            m_bQuestSucess = true;
        }
    }
    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CWhiteBird::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWhiteBird::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    if (!m_bEnter)
    {
        m_pGraphicDev->SetTexture(0, m_pNPCTex);
        m_pAnimatorCom->Play(L"WhiteBirdIdle", true);
        m_pAnimatorCom->render();
    }

    if (m_bEnter)
    {
        m_pGraphicDev->SetTexture(0, m_pNPCTex);
        m_pAnimatorCom->Play(L"WhiteBirdTalk", true);
        m_pAnimatorCom->render();
    }

    m_pColliderCom->Render_Buffer();

    if (!m_bQuestClear)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pMarkTransformCom->Get_WorldMatrix());

        if (!m_bQuestAccept)
        {
            m_pGraphicDev->SetTexture(0, m_pTex[EXCLAMATION]);
            m_pMarkAnimatorCom->Play(L"ExclamMarkAnim", true);
            m_pMarkAnimatorCom->render();
            return;
        }
        if (m_bQuestSucess)
        {
            m_pGraphicDev->SetTexture(0, m_pTex[YELLOWQUEST]);
            m_pMarkAnimatorCom->Play(L"YellowQuestAnim", true);
            m_pMarkAnimatorCom->render();
            return;
        }

        m_pGraphicDev->SetTexture(0, m_pTex[GRAYQUEST]);
        m_pMarkAnimatorCom->Play(L"GrayQuestAnim", true);
        m_pMarkAnimatorCom->render();

    }
}

void CWhiteBird::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    m_bEnter = true;

    // [S]버튼 출력.
    // [S]버튼 클릭 시 텍스트박스 출력 + 텍스트 출력
    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작
        if (!m_bConversation)
        {
            if (!m_bQuestAccept)
            {
                m_bConversation = m_bConversation ? false : true;
                // 여기에 new퀘스트 UI 띄우기
                m_bQuestAccept = true; // 조건 문 뒤에 true로 바꿔줌.
                m_pQuestUI->Add_Quest(m_tQuestInfo);

                m_pQuestAcceptUI->CallQuestAcceptUI(true);
                m_pQuestAcceptUI->Set_Text(m_tQuestInfo);
                //m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화

                return;
            }
        }

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출

            // 최초에는 기본 퀘스트 말풍을 보여줘야해서 아이템을 가지고 있더라도 false 상태로 출력하기 위해
            //if (!m_bQuestClear && m_bQuestAccept)
            if (m_bQuestSucess)
            {
                m_bQuestClear = true;
                m_tInfo.pContent = L"물의 증표다! 야호오 ~ ";
                m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
                m_pQuestUI->Get_QuestArray()->pop_back();
            }
        }

    }

    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }
}

void CWhiteBird::OnCollisionEnter(CGameObject* _pOther)
{
}

void CWhiteBird::OnCollisionExit(CGameObject* _pOther)
{
    m_bEnter = false;
    m_pInterButton->CallButton(false);
}

HRESULT CWhiteBird::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
    m_pTransformCom->Set_Pos(1170.f, 25.f, 1320.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


    return S_OK;
}

CWhiteBird* CWhiteBird::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWhiteBird* pNPC = new CWhiteBird(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CWhiteBird Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CWhiteBird::Free()
{
    Engine::CGameObject::Free();
}
