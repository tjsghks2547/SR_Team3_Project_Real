#include "pch.h"
#include "ResMgr.h"

IMPLEMENT_SINGLETON(CResMgr)

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{

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
}

void CResMgr::update()
{
}

void CResMgr::Render()
{
}

void CResMgr::Release()
{
}
