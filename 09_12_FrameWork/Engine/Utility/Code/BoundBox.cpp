#include "BoundBox.h"

CBoundBox::CBoundBox()
    : m_pMesh(nullptr)
    , m_pVecMin(0.f,0.f,0.f)
    , m_pVecMax(0.f,0.f,0.f)
{
    
}

CBoundBox::CBoundBox(LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev)
    , m_pMesh(nullptr)
    , m_pVecMin(0.f,0.f,0.f)
    , m_pVecMax(0.f,0.f,0.f)
{

}

CBoundBox::CBoundBox(const CBoundBox& rhs)
    :CComponent(rhs)
    , m_pMesh(rhs.m_pMesh)
    , m_pVecMin(rhs.m_pVecMin)
    , m_pVecMax(rhs.m_pVecMax)
{
}

CBoundBox::~CBoundBox()
{
}

HRESULT CBoundBox::Ready_Buffer()
{
    if (FAILED(D3DXCreateBox(m_pGraphicDev, 1.5f, 1.5f, 1.5f, &m_pMesh, NULL))) {
        // 冠胶 积己 角菩 贸府
        int a = 4; 
    }

    D3DXVECTOR3* pVertices;

   //m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertices);
   //D3DXComputeBoundingBox(pVertices, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &m_pVecMin, &m_pVecMax);
   //m_pMesh->UnlockVertexBuffer();

    return S_OK;
}

void CBoundBox::Render_Buffer() 
{
    m_pGraphicDev->SetTexture(0, 0);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pMesh->DrawSubset(0);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    
}

_int CBoundBox::Update_Component(const _float& fTimeDelta)  
{


    return 0;   
}

void CBoundBox::LateUpdate_Component()
{
}

CBoundBox* CBoundBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoundBox* pBoundBox = new CBoundBox(pGraphicDev);

    if (FAILED(pBoundBox->Ready_Buffer()))   
    {
        Safe_Release(pBoundBox);
        MSG_BOX("CBoundBox Create Failed");
        return nullptr;
    }

    return pBoundBox;
}

CComponent* CBoundBox::Clone()
{
    return new CBoundBox(*this);    
}

void CBoundBox::Free()
{
    CComponent::Free();
}


