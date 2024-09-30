#include "pch.h"
#include "Object.h"
#include "Export_Utility.h"
#include "ImGuiManger.h"

CObject::CObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CObject::~CObject()
{
}

HRESULT CObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//여기서 텍스처 다 넣자 

	m_vecTexture.resize(100);


	for (int i = 0; i < m_vecTexture.size(); i++)
	{
		char filePath[256];
		sprintf_s(filePath, "../Bin/Resource/Texture/Object/object%d.png", i);

		HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecTexture[i]);
		if (FAILED(hr))
		{
			MSG_BOX("Object Texture load failed");
		}
	}
	return S_OK;
}



_int CObject::Update_GameObject(const _float& fTimeDelta)
{

	Add_RenderGroup(RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	if (m_strTextureKey == L"Wall")
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[0]);

	}

	if (m_strTextureKey == L"Super")
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[1]);

	}

	if (m_strTextureKey == L"TelephonePole")
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[2]);

	}

	if (m_strTextureKey == L"BusStation")
	{
		//auto iter = find_if(m_mapTexture.begin(), m_mapTexture.end(),);
		m_pGraphicDev->SetTexture(0, m_vecTexture[3]);

	}

	if (m_strTextureKey == L"BreadStore")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[4]);
	}

	if (m_strTextureKey == L"Telephone2Pole")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[5]);
	}

	if (m_strTextureKey == L"Bench")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[6]);
	}

	if (m_strTextureKey == L"Carpet")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[7]);
	}

	if (m_strTextureKey == L"Ogu_House_Floor")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[8]);
	}

	if (m_strTextureKey == L"Ogu_House_Table")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[9]);
	}

	if (m_strTextureKey == L"Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[10]);
	}

	if (m_strTextureKey == L"Street_Lamp")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[11]);
	}

	if (m_strTextureKey == L"No_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[12]);
	}


	if (m_strTextureKey == L"One_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[13]);
	}

	if (m_strTextureKey == L"One_No_Flower_Pot")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[14]);
	}

	if (m_strTextureKey == L"Forest_Tree")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[15]);
	}

	if (m_strTextureKey == L"Long_Wall")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[16]);
	}

	if (m_strTextureKey == L"Background_Wall")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[17]);
	}

	if (m_strTextureKey == L"Cart")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[18]);
	}

	if (m_strTextureKey == L"Tutorial_Store2")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[19]);
	}

	if (m_strTextureKey == L"One_Tree")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[20]);
	}

	if (m_strTextureKey == L"Background_Sky")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[21]);
	}

	if (m_strTextureKey == L"Bucket")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[22]);
	}

	if (m_strTextureKey == L"Trash_Can")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[23]);
	}

	if (m_strTextureKey == L"bicycle")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[24]);
	}

	if (m_strTextureKey == L"Mini_Chair")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[25]);
	}


	if (m_strTextureKey == L"Fence")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[26]);
	}

	if (m_strTextureKey == L"Grass_collection")
	{
		m_pGraphicDev->SetTexture(0, m_vecTexture[27]);
	}


	/*Telephone2Pole
			Bench
			Carpet
			Ogu_House_Floor
			Ogu_House_Table*/
	//Ogu_House_Table



	m_pBufferCom->Render_Buffer();

	m_pBoundBox->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CObject::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	
	 
	// 9월 23일 추가 선환 충돌관련 박스 
	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;	


}

HRESULT CObject::SetUp_Material()
{
	return S_OK;
}

void CObject::Key_Input(const _float& fTimeDelta)
{
}

CObject* CObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObject* pObject = new CObject(pGraphicDev);

	if(FAILED(pObject->Ready_GameObject()))
	{
		Safe_Release(pObject);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pObject;
}

void CObject::Free()
{
	Engine::CGameObject::Free();
}

void CObject::OnCollision(CGameObject* _pOther)
{
	int a = 4; 
}
