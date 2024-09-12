#include "pch.h"
#include "DynamicCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"


CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    :CCamera(pGraphicDev)
    ,m_bFix(false)
    ,m_bCheck(false)
    ,m_pPlayer(nullptr)
{

   
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye
    , const _vec3* pAt
    , const _vec3* pUp
    , const _float& _fFov
    , const _float& _fAspect
    , const _float& _fNear
    , const _float& _fFar)
{
    m_vEye = *pEye;
    m_vAt = *pAt;
    m_vUp = *pUp;

    m_fFov = _fFov;
    m_fAspect = _fAspect;
    m_fNear = _fNear;
    m_fFar = _fFar;

    FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

    return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CCamera::Update_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

   //여기서 부터 다시하기 ( 9월 5일 ) 
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
  


    //플레이어가 밟고 있는 발판이 필요
   //CTerrainTex* pTerrainTex = dynamic_cast<CTerrainTex*>(Engine::Get_Component(ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
   //NULL_CHECK(pTerrainTex);
   //
   //_float fY =  m_pCCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainTex->Get_VtxPos(), VTXCNTX, VTXCNTZ);
   //
   //m_pTransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);

 
    return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{

    // 여기다가 마우스 움직일지 안움직일지를 결정하기 ( 일단 보류 ) 
    if (false == m_bFix)
    {
        Mouse_Fix();
        Mouse_Move();
    }


    CCamera::LateUpdate_GameObject(fTimeDelta);
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev
                                        ,const _vec3* pEye
                                        ,const _vec3* pAt
                                        ,const _vec3* pUp
                                        ,const _float& _fFov
                                        ,const _float& _fAspect
                                        ,const _float& _fNear
                                        ,const _float& _fFar)
{

    CDynamicCamera* pCDynamicCamera = new CDynamicCamera(pGraphicDev);

    if (FAILED(pCDynamicCamera->Ready_GameObject(pEye,pAt,pUp,_fFov,_fAspect,_fNear,_fFar)))
    {
        
        Safe_Release(pCDynamicCamera);
        MSG_BOX("Dynamic Camera Create Failed");
        return nullptr;
    }

    return pCDynamicCamera;
}


void CDynamicCamera::Free()
{
    CCamera::Free();
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
    _matrix   matCamWorld;
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView); // 카메라의 월드행렬을 구해줌
    
    if(Engine::Get_DIKeyState(DIK_W)& 0x80)
    {
        _vec3 vLook; 
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 150.f; 

        m_vEye += vLength;    // 해당 위치로 가까워 지는 것인지 위치 의미 
        m_vAt  += vLength;  // 해당 각도로 가까워지는 의미 

    
    }

    if(Engine::Get_DIKeyState(DIK_S)& 0x80)
    {
        _vec3  vLook;
        memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

        _vec3  vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 150.f;

       m_vEye -= vLength; 
       m_vAt  -= vLength;
    }

    if(Engine::Get_DIKeyState(DIK_A)& 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 50.f; 

        m_vEye -= vLength; 
        m_vAt -= vLength;   
    }


    if (Engine::Get_DIKeyState(DIK_D) & 0x80)
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 50.f;

        m_vEye += vLength;
        m_vAt  += vLength;
    }

    if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
    {
        if (m_bCheck)
            return;

        m_bCheck = true;

        if (m_bFix)
            m_bFix = false;
        else
            m_bFix = true;
    }

    else
        m_bCheck = false;

    if (false == m_bFix)
        return;
    

}

void CDynamicCamera::Mouse_Move()
{
    _matrix   matCamWorld; 
    D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

    _long  dwMouseMove(0);


    if(dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
    {
        _vec3 vUp{ 0.f, 1.f, 0.f };
        _vec3 vLook = m_vAt - m_vEye; 

        _matrix  matRot;

        D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

        m_vAt = m_vEye + vLook; 
    }

   


    if(dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
    {
        _vec3 vRight;
        memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

        _vec3 vLook = m_vAt - m_vEye; 

        _matrix  matRot;
        D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f)); // 화면 y는 800x600에서 600 이므로 최대 60도 회전을 의미한다.
        // 간단히 보충설명하면 마우스가 y축으로 움직이는걸 컴퓨터에선 원의 호의 위에서 움직이는것처럼 생각한다. 
        D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

        m_vAt = m_vEye + vLook; 
    }


   


}

void CDynamicCamera::Mouse_Fix()
{
    POINT	ptMouse{ WINCX >> 1, WINCY >> 1 }; // 화면 가운데로 하는거 비트연산으로 하는이유는 계산 속도가 빨라서
    
    ClientToScreen(g_hWnd, &ptMouse);   
    SetCursorPos(ptMouse.x, ptMouse.y); 
}



