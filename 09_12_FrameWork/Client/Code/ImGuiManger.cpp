#include "pch.h"
#include "ImGuiManger.h"
#include "Define.h"
#include "iostream"

IMPLEMENT_SINGLETON(CImGuiManger)

CImGuiManger::CImGuiManger()
	:testbool(false)
	,inttest(10)
	,floattest(5.5f)
	,m_pTerrainTexture(nullptr)
	,m_bshowTileTextureWindow(false)
	,iTileX(0)
	,iTileY(0)
	,fVtxItv(0)
	,m_bTerrainOnOff(false)
	,m_bWireFrameModeOnOff(false)
	,m_pGraphicDev(nullptr)
	,m_pMapTex(nullptr)
	,m_vecPickPos(0.f,0.f,0.f)
	,m_bImageButtonClick(false)
	,m_pCurTerrainTexture(nullptr)
	,m_bshowObjectTextrueWindow(false)
	,m_bshowInstalledObjectList(false)
{

}


CImGuiManger::~CImGuiManger()
{
}

void CImGuiManger::init()
{
	//초기화 하기 

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

	m_pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();


    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX9_Init(m_pGraphicDev);

	//ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("../TTF/Pretendard-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());


	ImGui::SetNextWindowSize(ImVec2(400, 400)); // Imgui 창 크기 
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	

	//이미지 파일 경로 넣기 
	//L"../Bin/Resource/Texture/SkyBox/burger%d.dds

	//IDirect3DTexture9* myTexture = nullptr;

	//m_mapComponent[ID_STATIC].insert({ L"Com_Animation", pComponent });
	//m_mapTexture.insert({L"Sand_Terrain",myTexture});

	m_vecTerrainTexture.resize(14);
	m_vecObjectTexture.resize(20);

	//LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), "../Bin/Resource/Texture/Map/Tile1.png", &myTexture);

	for(int i=1; i<m_vecTerrainTexture.size()+1; i++)
	{

		char filePath[256];
		sprintf_s(filePath, "../Bin/Resource/Texture/Map/Tile%d.png", i-1);

		HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecTerrainTexture[i-1]);
		if (FAILED(hr))
		{
			MSG_BOX("Terrain Texture load failed");
		}
	}

	char filePath[256];
	sprintf_s(filePath, "../Bin/Resource/Texture/wall.png");
	
	// 여기서 오류남 
	HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecObjectTexture[0]);
	if(FAILED(hr))
	{
		MSG_BOX("Object Texutr load Failed");
	}
	
	

}

void CImGuiManger::update()
{

	// 업데이트 

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Our state

	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//bool testbool = false;
	//int  inttest = 10;
	//float floattest = 5.5f;


	
	if (ImGui::Begin("Map Editor", NULL, ImGuiWindowFlags_MenuBar))
	{
		// File 메뉴바
		if (ImGui::BeginMenuBar())
		{
			//내용..
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
					int a = 4;

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		//Tile MenuBar
		if (ImGui::BeginMenuBar())
		{
			//내용..
			if (ImGui::BeginMenu("Tile"))
			{
				if (ImGui::MenuItem("Tile Texture List"))
				{
					m_bshowTileTextureWindow = true;
				}

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		//Terrain MenuBar
		if (ImGui::BeginMenuBar())
		{
			// Terrain 끄고 키기 
			if (ImGui::BeginMenu("Terrain"))
			{
				if (ImGui::MenuItem("On"))
				{
					m_bTerrainOnOff = true;
				}

				else if(ImGui::MenuItem("Off"))
				{
					m_bTerrainOnOff = false; 
				}


				else if(ImGui::MenuItem("WireFrameOn"))
				{
					m_bWireFrameModeOnOff = true;
				}

				else if (ImGui::MenuItem("WireFrameOff"))
				{
					m_bWireFrameModeOnOff = false;
				}

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}


		//Object MenuBar
		if (ImGui::BeginMenuBar())
		{
			//내용..
			if (ImGui::BeginMenu("Object"))
			{
				if (ImGui::MenuItem("Object Texture List"))
				{
					m_bshowObjectTextrueWindow = true;
				}

				if (ImGui::MenuItem("Installed Objects List"))
				{
					m_bshowInstalledObjectList = true; 
				}

				ImGui::EndMenu();
			}
			// 
			//끝나면 End
			ImGui::EndMenuBar();
		}
		

		if (!ImGui::CollapsingHeader("Setting"))
		{
			ImGui::Text(u8"좌표");
			ImGui::SameLine(50.0f, 0.0f);
			static float floatarray[3] = { 100.f,100.f,100.f };
			ImGui::SliderFloat3("##1", floatarray, -800.f, 800.f);
		}
			//return;

		// 이 밑에다가 내용들을 추가
		
		

		ImGui::Checkbox("Test Bool", &testbool);
		if (ImGui::Button("Click me!"))
		{
			testbool = true;
		}
		ImGui::SliderInt("Chose Int", &inttest, 1, 25);
		ImGui::SliderFloat("Chose Float", &floattest, 0.1f, 15.5f);


	}ImGui::End();


	// 추가된 오브젝트 리스트들 가져오기

	



	if (m_bshowTileTextureWindow)	
	{
		ShowTileMenuWindow();	
	}

	if (m_bshowObjectTextrueWindow)
	{
		ShowObjectMenuWindow();
	}


	if(m_bshowInstalledObjectList)
	{
		ShowInstalledObjectWindow();
	}




	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantCaptureMouse)
	{
		// ImGui 창 밖에서의 마우스 입력 처리
		if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		{

			// 여기다가 조건식으로 오브젝트 창이 열렸을때만 반응하도록 하면 되겠다.
			// 레이까지 적용완료
			// 마우스 레이 코드 
			//m_vecPickPos = PickingOnTerrain();


			
		}
	}
	else
	{
		// ImGui 창 안에서의 마우스 입력 처리
	}
	
	


}

void CImGuiManger::Render()
{
	ImGui::Render();	
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());	
	ImGui::EndFrame();	
}

void CImGuiManger::Release()
{
	// 릴리즈 
}

void CImGuiManger::ShowTileMenuWindow()
{
	// Tile Texture List 창 띄우기
	if (m_bshowTileTextureWindow)
	{
		// 새로운 창 생성
		ImGui::Begin("Tile Texture List", &m_bshowTileTextureWindow);
		// 여기에 창의 내용을 추가
		// 
		//현재 창 사이즈 가져오기 
		ImVec2 CurrentWindowSize = ImGui::GetWindowSize();


		ImVec2 imageSize(100, 100);  // 출력할 이미지 크기

		
		for (int i = 1; i < m_vecTerrainTexture.size() + 1; i++)
		{

			char label[16];
			sprintf_s(label, "Tile%d", i);
			// 이미지 출력
			//ImGui::Image((void*)m_vecTexture[i - 1], imageSize);
			if (ImGui::ImageButton(label, (void*)m_vecTerrainTexture[i - 1], imageSize)) 
			{
				// ImageButton이 클릭된 경우 호출할 함수
				OnTileImageButtonClick(i);  // 예시: 클릭된 타일의 인덱스를 함수로 전달
			};

			// 현재 창에서 남은 가로 공간 확인
			//ImVec2 availableSpace = ImGui::GetContentRegionAvail();

			// 남은 공간이 이미지 크기보다 크다면 같은 줄에 이미지 출력
			if (i % (int)(CurrentWindowSize.x/100) != 0)
			{
				ImGui::SameLine();  // 같은 줄에 출력
			}

			else if (i % (int)(CurrentWindowSize.x/100) == 0)
			{
				// 공간이 부족하면 다음 줄로 이동
				ImGui::NewLine();
			}
		}

		ImGui::NewLine();
		//ImGui::Text("Tile Texture List Content Here");

		if (!ImGui::CollapsingHeader(u8"타일 생성"))
		{	
			
			ImGui::Text(u8"생성할 타일의 행과 열의 수 설정");
			ImGui::NewLine();


			ImGui::PushItemWidth(100.0f);  // 너비를 100픽셀로 설정
			
			ImGui::Text("Tile X");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputInt("##Tile X", &iTileX);
			ImGui::SameLine(170.0f, 0.0f);
			ImGui::Text("Tile Y");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputInt("##Tile Y", &iTileY);
			ImGui::NewLine();
			ImGui::Text(u8"점 간격");  // 레이블 출력
			ImGui::SameLine();
			ImGui::InputFloat("##VtxItv" ,&fVtxItv);

			ImGui::NewLine();	
			if(ImGui::Button(u8"지형생성"))
			{
				
			}
			//ImGui::PopStyleColor(3);
			ImGui::PopItemWidth();  // 설정된 너비를 원래대로 복구
		}


		

		ImGui::End();
	}

}

void CImGuiManger::ShowObjectMenuWindow()
{

	if (m_bshowObjectTextrueWindow)
	{
		ImGui::Begin("Object Texture List", &m_bshowObjectTextrueWindow);

		ImVec2 imageSize(100, 100);  // 출력할 이미지 크기

		if (ImGui::ImageButton("Wall", m_vecObjectTexture[0], imageSize))
		{

			Engine::CLayer* pLayer = CLayer::Create();
			if (pLayer == nullptr)
			{
				MSG_BOX("pLayer nullptr Error");
			}


			Engine::CGameObject* pGameObject = nullptr;

			pGameObject = CObject::Create(m_pGraphicDev);
			if (pGameObject == nullptr)
			{
				MSG_BOX("CObject nullptr Error");
			}

			pLayer->Add_GameObject(L"Wall", pGameObject);

			pGameObject->SetObjectKey("Wall"); // 오브젝트 이름 추가한거

			CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr().insert({ L"Wall" , pLayer });

		}

		//일단 오브젝트 생성해보기 
		ImGui::End();

	}
}

void CImGuiManger::ShowInstalledObjectWindow()
{

	if(m_bshowInstalledObjectList)
	{
		ImGui::Begin("Installed Obejct List", &m_bshowInstalledObjectList);

		//여기서 지금 오브젝트거 다 가져오기
		map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
		//CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr()
		
		auto iter = mapLayer.begin();

		for(auto iter2 = iter->second->GetLayerGameObjectPtr().begin(); iter2 != iter->second->GetLayerGameObjectPtr().end(); iter2++)
		{
			ImGui::BulletText("%s",iter2->second->GetObjectKey());
		}

		//for (auto iter = mapLayer.begin(); iter != mapLayer.end(); iter++)
		//{
		//	
		//}


		ImGui::End();
	}



}

void CImGuiManger::OnTileImageButtonClick(int tileIndex)
{
	// 여기서 타일 이미지값 가져옴 
	// 여기서 버퍼 가지고 와서 해당 마우스좌표에 랜더링되게 해야함. 

	//m_pGraphicDev->SetTexture(0, m_vecTexture[tileIndex]);
	m_bImageButtonClick = true; 
	m_pCurTerrainTexture = m_vecTerrainTexture[tileIndex - 1];

}

_vec3 CImGuiManger::PickingOnTerrain()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표화

	_vec3 vMousePos;

	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	// 뷰 포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	// 투영 -> 뷰 스페이스 
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3 vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	// 뷰스페이스 -> 월드 
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView); // 여기서 현재 뷰가 업데이트가안됨
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView); // 위치 관련 이동
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 좌표에서 할거니깐 터레인의 월드좌표 점들을 가져와야함 
	CMapTex* pTerrainBufferCom = dynamic_cast<CMapTex*>(Engine::Get_Component(ID_STATIC, L"Layer_GameLogic", L"Map", L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
	{
		MSG_BOX("pTerrainBufferCom is nullptr");
	}

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", L"Map", L"Com_Transform"));
	if (pTerrainBufferCom == nullptr)
	{
		MSG_BOX("pTerrainTransCom is nullptr");
	}


	const _vec3* pTerrainTexPos = pTerrainBufferCom->Get_VtxPos(); // 여기도 문제가 생김( 다 0,0,0)을 가져오네
	_vec3* pWorldTerrainTexPos = new _vec3[VTXCNTX2 * VTXCNTZ2];

	// 터레인의 월드 매트릭스를 가져와야하는데
	_matrix matWorld;
	//Engine::CTransform* pTerrainTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", L"Terrain", L"Com_Transform"));
	pTerrainTransCom->Get_WorldMatrix(&matWorld);


	for (int i = 0; i < VTXCNTX2 * VTXCNTZ2; i++)
	{
		D3DXVec3TransformCoord(&pWorldTerrainTexPos[i], &pTerrainTexPos[i], &matWorld);
	}

	//제대로 계산되는지 ( 지금 항등행렬이 월드행렬이므로 그대로의 값이 나와야함)
	// if(*pWorldTerrainTexPos[1] == *pTerrainTexPos[1])
	//{
	//    int a = 4; 
	//}

	_ulong dwVtxId[3]{};
	_ulong dwVtexId_LeftUnder[3]{};
	_float fU, fV, fDist;

	for (_ulong i = 0; i < VTXCNTZ2 - 1; ++i)
	{
		for (_ulong j = 0; j < VTXCNTX2 - 1; ++j)
		{
			_ulong	dwIndex = i * VTXCNTX2 + j;

			// 오른쪽 위
			dwVtxId[0] = dwIndex + VTXCNTX2;
			dwVtxId[1] = dwIndex + VTXCNTX2 + 1;
			dwVtxId[2] = dwIndex + 1;

			


			if (D3DXIntersectTri(&pWorldTerrainTexPos[dwVtxId[1]],
				&pWorldTerrainTexPos[dwVtxId[2]],
				&pWorldTerrainTexPos[dwVtxId[0]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{

				return _vec3(
					pWorldTerrainTexPos[dwVtxId[0]].x + fU * (pWorldTerrainTexPos[dwVtxId[1]].x - pWorldTerrainTexPos[dwVtxId[0]].x) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[0]].x),
					0.f, // Z 좌표는 필요에 따라 계산
					pWorldTerrainTexPos[dwVtxId[0]].z + fU * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[0]].z) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].z - pWorldTerrainTexPos[dwVtxId[0]].z)
				);


				// 해당 사각형의 중점 이니깐 삼각형 2개모여서 만든 사각형의 중점을 리턴하면될듯

				//return _vec3(pWorldTerrainTexPos[dwVtxId[1]].x + fU * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[1]].x),
				//	0.f,
				//	pWorldTerrainTexPos[dwVtxId[1]].z + fV * (pWorldTerrainTexPos[dwVtxId[0]].z - pWorldTerrainTexPos[dwVtxId[1]].z));


				//Test (사각형의 중점 좌표 리턴) 
				//return _vec3(pWorldTerrainTexPos[dwVtxId[0]].x+0.5f, 0.f, pWorldTerrainTexPos[dwVtxId[0]].z-0.5f);
			}

			// 왼쪽 아래
			dwVtxId[0] = dwIndex + VTXCNTX2;
			dwVtxId[1] = dwIndex + 1;
			dwVtxId[2] = dwIndex;

			


			if (D3DXIntersectTri(&pWorldTerrainTexPos[dwVtxId[2]],
				&pWorldTerrainTexPos[dwVtxId[0]],
				&pWorldTerrainTexPos[dwVtxId[1]],
				&vRayPos, &vRayDir, &fU, &fV, &fDist))
			{
				// V1 + U(V2 - V1) + V(V3 - V1)
				return _vec3(
					pWorldTerrainTexPos[dwVtxId[0]].x + fU * (pWorldTerrainTexPos[dwVtxId[1]].x - pWorldTerrainTexPos[dwVtxId[0]].x) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].x - pWorldTerrainTexPos[dwVtxId[0]].x),
					0.f, // Z 좌표는 필요에 따라 계산
					pWorldTerrainTexPos[dwVtxId[0]].z + fU * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[0]].z) +
					fV * (pWorldTerrainTexPos[dwVtxId[2]].z - pWorldTerrainTexPos[dwVtxId[0]].z)
				);
	


				//return _vec3(pWorldTerrainTexPos[dwVtxId[2]].x + fU * (pWorldTerrainTexPos[dwVtxId[0]].x - pWorldTerrainTexPos[dwVtxId[2]].x),
				//	0.f,
				//	pWorldTerrainTexPos[dwVtxId[2]].z + fV * (pWorldTerrainTexPos[dwVtxId[1]].z - pWorldTerrainTexPos[dwVtxId[2]].z));
			}
		}
	}

	return _vec3(0.f, 0.f, 0.f);
}


//IDirect3DTexture9* CImGuiManger::FindTexture(wstring _wstr, map<wstring, IDirect3DTexture9*> _map)
//{
//
//	//auto iter = m_mapTexture.find(_wstr);	
//	//
//	//if (iter == m_mapTexture.end())
//	//{
//	//	MSG_BOX("no that texutre insert at m_mapTexture");
//	//}
//	//
//	//else if (iter != m_mapTexture.end())
//	//{
//	//	return iter->second;
//	//}
//
//
//	
//}


