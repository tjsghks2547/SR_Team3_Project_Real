#include "PlantBulletCom.h"

CPlantBulletCom::CPlantBulletCom()
{
}

CPlantBulletCom::CPlantBulletCom(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CPlantBulletCom::CPlantBulletCom(const CPlantBulletCom& rhs)
{
}

CPlantBulletCom::~CPlantBulletCom()
{
}

HRESULT CPlantBulletCom::Ready_Bullet()
{
    return S_OK;
}

_int CPlantBulletCom::Update_Component(const _float& fTimeDelta)
{
    return 0;
}

void CPlantBulletCom::LateUpdate_Component()
{
}

CPlantBulletCom* CPlantBulletCom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlantBulletCom* pCom = new CPlantBulletCom(pGraphicDev);

    if (FAILED(pCom->Ready_Bullet()))
    {
        Safe_Release(pCom);
        MSG_BOX("PipeBoardCom Create Failed");
        return nullptr;
    }

    return pCom;
}

CComponent* CPlantBulletCom::Clone()
{
    return new CPlantBulletCom(*this);
}
void CPlantBulletCom::Free()
{
    CComponent::Free();
}