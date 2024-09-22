#include "pch.h"
#include "Imgui_ObjectTool.h"
#include "GraphicDev.h"
#include "Object.h"
#include "Define.h"
#include <sstream>
#include <commdlg.h>
#include <cwctype>
#include <cstring>


IMPLEMENT_SINGLETON(CImgui_ObjectTool);


CImgui_ObjectTool::CImgui_ObjectTool()
	: m_bShowObjectWindow(false)
	, m_bShowInstallObjectWindow(false)
	, m_pCurObjectTransform(nullptr)
	, m_sCurTextureData(L"")
	, m_sCurTextureKey(L"")
	, m_vecPickPos(0.f,0.f,0.f)
	, m_strCurObjectName(L"")
	, index(0)
	, deleteCount(0)
	, m_wCurReadTextureKey(L"Test")
{

}

CImgui_ObjectTool::~CImgui_ObjectTool()
{

}


wstring StringToTChar(const std::string& str)
{
	wstring wstr;
	wstr.assign(str.begin(), str.end());

	return wstr;
}


string WstringTostring(const std::wstring& wstr)
{
	string str;
	str.assign(wstr.begin(), wstr.end());

	return str;
}


std::wstring extractNumber(const std::wstring& input) {
	std::wstring result;
	for (wchar_t ch : input) {
		if (iswdigit(ch)) {
			result += ch; // 숫자이면 result에 추가
		}
	}
	return result;
}

std::wstring removeDigits(const std::wstring& input) {
	std::wstring result;
	for (wchar_t wch : input) {
		if (!std::iswdigit(wch)) {  // 숫자가 아닌 경우
			result += wch;
		}
	}
	return result;
}

const wchar_t* findInVector(const std::vector<const wchar_t*>& vec, const wchar_t* target) {
	for (auto it = vec.begin(); it != vec.end(); ++it) {
		if (wcscmp(*it, target) == 0) {  // 내용 비교
			return *it;  // 일치하는 요소 반환
		}
	}
	return nullptr;  // 일치하는 요소가 없는 경우
}

void CImgui_ObjectTool::init()
{
	m_vecObjectTexture.resize(20);
	//m_vecObjectList.resize(100);
	

	char filePath[256];
	sprintf_s(filePath, "../Bin/Resource/Texture/wall.png");

	// 여기서 오류남 
	HRESULT hr = LoadTextureFromFile(CGraphicDev::GetInstance()->Get_GraphicDev(), filePath, &m_vecObjectTexture[0]);
	if (FAILED(hr))
	{
		MSG_BOX("Object Texutr load Failed");
	}

}

void CImgui_ObjectTool::update()
{

	if (ImGui::BeginMenuBar())
	{
		//내용..
		if (ImGui::BeginMenu("Object"))
		{
			if (ImGui::MenuItem("Object Texture List"))
			{
				m_bShowObjectWindow = true;
			}

			if (ImGui::MenuItem("Installed Objects List"))
			{
				m_bShowInstallObjectWindow = true;
			}

			ImGui::EndMenu();
		}
		// 
		//끝나면 End
		ImGui::EndMenuBar();
	}


	if(m_bShowObjectWindow)
	{
		ShowObjectWindow();	
		static bool wasLeftButtonDown = false;

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
			// ImGui 창 밖에서의 마우스 입력 처리
			if (LONG mouseState = Engine::Get_DIMouseState(DIM_LB) & 0x80)
			{
				// 여기다가 조건식으로 오브젝트 창이 열렸을때만 반응하도록 하면 되겠다.
				// 레이까지 적용완료
				// 마우스 레이 코드 
				

				 //= Get_DIMouseState(DIM_LB);	

				if (mouseState & 0x80) // 마우스 왼쪽 버튼이 눌렸다면
				{
					// 이전 상태가 눌리지 않았을 때만 처리
					if (!wasLeftButtonDown)
					{
						// 클릭 이벤트 처리
						// 여기에서 원하는 작업 수행
						m_vecPickPos = PickingOnTerrain();
						InstallObject(m_sCurTextureKey,m_strCurObjectName,m_sCurTextureData);

						m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", m_vecObjectList.back(), L"Com_Transform"));
						if (m_pCurObjectTransform == nullptr)
						{
							MSG_BOX("m_pCurObjectTransform is nullptr");
							//m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Imgui_Object1", m_strCurObjectName, L"Com_Transform"));
							//if (m_pCurObjectTransform == nullptr)
							//{
							//	MSG_BOX("m_pCurObjectTransform is nullptr");
							//}
						}

						if (m_pCurObjectTransform != nullptr)
							m_pCurObjectTransform->Set_Pos(m_vecPickPos.x, 0, m_vecPickPos.z);

						
					}

					// 현재 상태를 업데이트
					wasLeftButtonDown = true;
					
				}
			}

			else
			{
				// 버튼이 눌리지 않으면 상태를 업데이트
				wasLeftButtonDown = false;
			}
		}
		else
		{
			// ImGui 창 안에서의 마우스 입력 처리
		}
	}

	if(m_bShowInstallObjectWindow)
	{
		ShowInstalledObjectWindow();

	}


	
}

void CImgui_ObjectTool::render()
{

}

void CImgui_ObjectTool::ShowObjectWindow()
{

	if (m_bShowObjectWindow)
	{
		ImGui::Begin("Object Texture List", &m_bShowObjectWindow);

		ImVec2 imageSize(100, 100);  // 출력할 이미지 크기

		if (ImGui::ImageButton("Wall", m_vecObjectTexture[0], imageSize))
		{

			//여기가 문제내 
			m_sCurTextureData  = L"Wall";
			m_sCurTextureKey   = L"Wall";
			m_strCurObjectName = L"Wall";

		}

		//일단 오브젝트 생성해보기 
		ImGui::End();

	}

}

void CImgui_ObjectTool::ShowInstalledObjectWindow()
{
	if (m_bShowInstallObjectWindow)
			{
				ImGui::Begin("Installed Obejct List", &m_bShowInstallObjectWindow);
		
				ImGui::BeginChild("LeftChild", ImVec2(200, 0), true); // 가로 200, 세로 자동(채움), 테두리 있음
				ImGui::Text("Installed Obejct");

				//여기서 지금 오브젝트거 다 가져오기
				map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
				//CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr()

				for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
				{
					const _tchar* layerKey = iter->first;

					if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
					{
						for (auto objectIter = iter->second->GetLayerGameObjectPtr().begin(); objectIter != iter->second->GetLayerGameObjectPtr().end(); ++objectIter)
						{
							//if(ImGui::Selectable())

							
							//if (ImGui::Selectable(objectIter->second->GetObjectKey().c_str()))
							if(ImGui::Selectable(WstringTostring(objectIter->second->GetObjectKey()).c_str()))
							{
								// 클릭 시 처리할 내용
								for(auto iter = m_vecObjectList.begin(); iter!= m_vecObjectList.end(); iter++)
								{
									if(wcscmp(*iter,objectIter->second->GetObjectKey().c_str())==0)
									{
										// 그럼 여기다가 찾은그걸 넣어주자 
										CurClickObject = *iter;	
									}
								}

								// 여기서 벡터의
								m_pCurObjectTransform = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", CurClickObject, L"Com_Transform"));	

								if (m_pCurObjectTransform == nullptr)
								{
									MSG_BOX("m_pCurObjectTransform is nullptr");
								}
							}
						}
					}
				}
				ImGui::EndChild();

				ImGui::SameLine(); // 같은 줄에 다음 요소를 배치하기 위해 사용

				ImGui::BeginChild("RightChild", ImVec2(300, 0), true);
				ImGui::Text("Control Scale and Pos");

				if (!ImGui::CollapsingHeader("Setting"))	
				{
					if (m_pCurObjectTransform != nullptr)
					{
					
						if (m_pPreObjectTransform != m_pCurObjectTransform) {
							floatPosArrayTest[0] = m_pCurObjectTransform->Get_WorldMatrix()->_41;
							floatPosArrayTest[1] = m_pCurObjectTransform->Get_WorldMatrix()->_42;
							floatPosArrayTest[2] = m_pCurObjectTransform->Get_WorldMatrix()->_43;
							//m_pPreObjectTransform = m_pCurObjectTransform; // 이전 Transform 업데이트
						}
						ImGui::Text(u8"위치 좌표");
						ImGui::SameLine(50.0f, 0.0f);
						
						ImGui::SliderFloat3("##1", floatPosArrayTest, 0.f, 10000.f);

						//static	float floatPosArray[3] = { m_pCurObjectTransform->Get_WorldMatrix()->_41,m_pCurObjectTransform->Get_WorldMatrix()->_42, m_pCurObjectTransform->Get_WorldMatrix()->_43 };
						//ImGui::SliderFloat3("##1", floatPosArray, 0.f, 10000.f);
						
						m_pCurObjectTransform->Set_Pos(floatPosArrayTest[0], floatPosArrayTest[1], floatPosArrayTest[2]);

						// 현재 같은프레임이라 입력이 따로안됨
						ImGui::BeginChild("TestChild", ImVec2(200, 200), true);
						ImGui::Text(u8"회전");
						//if (m_pPreObjectTransform != m_pCurObjectTransform)
						//{
						//	
						//}m_pPreObjectTransform != m_pCurObjectTransform
						if (m_pPreObjectTransform != m_pCurObjectTransform)
						{
							floatRotationArray[0] = m_pCurObjectTransform->Rotation_x;
							floatRotationArray[1] = m_pCurObjectTransform->Rotation_y;
							floatRotationArray[2] = m_pCurObjectTransform->Rotation_z;

							prevRotationArray[0] = m_pCurObjectTransform->Rotation_x;
							prevRotationArray[1] = m_pCurObjectTransform->Rotation_y;
							prevRotationArray[2] = m_pCurObjectTransform->Rotation_z;

							m_pPreObjectTransform = m_pCurObjectTransform;
						}
						//static float floatRotationArray[3] = { 0.f,0.f,0.f };
						//static float prevRotationArray[3] = { 0.f, 0.f, 0.f }; // 이전 회전값 저장용

						ImGui::SliderFloat3("##1", floatRotationArray, 0.f, 360.f);


						//현재 업데이트가 돌아서 계속 회전하게됨 이럴땐 어떻게 해줘야할지 생각하기 
						//m_pTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);


						// x축 회전
						if (floatRotationArray[0] != prevRotationArray[0])
						{
							m_pCurObjectTransform->Rotation(ROT_X, -prevRotationArray[0] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_X, floatRotationArray[0] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_x = floatRotationArray[0];
							prevRotationArray[0] = floatRotationArray[0];
						}

						// y축 회전	
						if (floatRotationArray[1] != prevRotationArray[1])
						{
							m_pCurObjectTransform->Rotation(ROT_Y, -prevRotationArray[1] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_Y, floatRotationArray[1] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_y = floatRotationArray[1];
							prevRotationArray[1] = floatRotationArray[1];
						}

						// z축 회전
						if (floatRotationArray[2] != prevRotationArray[2])
						{
							m_pCurObjectTransform->Rotation(ROT_Z, -prevRotationArray[2] * 3.14f / 180.f);
							m_pCurObjectTransform->Rotation(ROT_Z, floatRotationArray[2] * 3.14f / 180.f);

							m_pCurObjectTransform->Rotation_z = floatRotationArray[2];
							prevRotationArray[2] = floatRotationArray[2];
						}
						ImGui::EndChild();

						ImGui::BeginChild("Scale", ImVec2(200, 200), true);
						ImGui::Text(u8"크기");
						if (m_pPreObjectTransform != m_pCurObjectTransform)
						{
							floatScaleArray[0] = m_pCurObjectTransform->Get_WorldMatrix()->_11;
							floatScaleArray[1] = m_pCurObjectTransform->Get_WorldMatrix()->_22;
							floatScaleArray[2] = m_pCurObjectTransform->Get_WorldMatrix()->_33;

							prevScaleArray[0] = m_pCurObjectTransform->Get_WorldMatrix()->_11;
							prevScaleArray[1] = m_pCurObjectTransform->Get_WorldMatrix()->_22;
							prevScaleArray[2] = m_pCurObjectTransform->Get_WorldMatrix()->_33;

							m_pPreObjectTransform = m_pCurObjectTransform;
						}
						//static float floatScaleArray[3] = { 1.f,1.f,1.f };
						//static float prevScaleArray[3] = { 1.f, 1.f, 1.f }; // 이전 크기값 저장용

						ImGui::SliderFloat3("##1", floatScaleArray, 0.f, 100.f);

						// x축 크기
						if (floatScaleArray[0] != prevScaleArray[0])
						{
							m_pCurObjectTransform->m_vScale = { -prevScaleArray[0], floatScaleArray[1], floatScaleArray[2] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							//m_pCurObjectTransform->Rotation_x = floatRotationArray[0];
							//m_pCurObjectTransform->Get_WorldMatrix()->_11 = floatScaleArray[0];
							prevScaleArray[0] = floatScaleArray[0];
						}

						// y축 크기
						if (floatScaleArray[1] != prevScaleArray[1])
						{
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], -prevScaleArray[1], floatScaleArray[2] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							prevScaleArray[1] = floatScaleArray[1];
						}

						// z축 크기 

						if (floatScaleArray[2] != prevScaleArray[2])
						{
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], -prevScaleArray[1] };
							m_pCurObjectTransform->m_vScale = { floatScaleArray[0], floatScaleArray[1], floatScaleArray[2] };

							prevScaleArray[2] = floatScaleArray[2];
						}

						ImGui::EndChild();

					}

				
				}

				//삭제버튼 누를시 활성화
				if(ImGui::Button(u8"삭제", ImVec2(100.f, 0.f)))
				{

					map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
					
					auto iter = mapLayer.find(L"Layer_GameLogic");

					if(iter == mapLayer.end())
					{
						MSG_BOX("Not Find");
					}

					else
					{
						CLayer* pLayer = iter->second;

						map<const _tchar*, CGameObject*>& mapObject = pLayer->GetLayerGameObjectPtr();

						auto iter = mapObject.find(CurClickObject);
						
								
						if(iter == mapObject.end())
						{
							MSG_BOX("Not_Find_Object");
						}

						else
						{
							
							// 메인 랜더 끝이나면 이벤트 매니저같은거 만들어서 삭제처리하기. 
							mapObject.erase(CurClickObject);		
							
						}
					}

				}

				ImGui::EndChild();
				
			
				ImGui::End();
			}

}

void CImgui_ObjectTool::InstallObject(wstring _TextureKey, wstring _CurTextureData, wstring _forConvert)
{

	Engine::CLayer* pLayer = CLayer::Create();	
	if (pLayer == nullptr)	
	{
		MSG_BOX("pLayer nullptr Error");	
	}
	
	
	Engine::CGameObject* pGameObject = nullptr;	
	
	pGameObject = CObject::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
	if (pGameObject == nullptr)
	{
		MSG_BOX("CObject nullptr Error");
	}
	
	auto iter = find(m_vecObjectList.begin(), m_vecObjectList.end(), m_strCurObjectName.c_str());
	
	map<const _tchar*, CLayer*>& pMapLayer = Engine::Get_CurScenePtr()->GetLayerMapPtr();	

	// 새로운 객체를 만들어야하네 계속
	wstring* dynamicWstring = new wstring(_forConvert + to_wstring(index));	
	//dynamicWstring = &_forConvert;		
	m_vecObjectList.push_back((*dynamicWstring).c_str());	
	pMapLayer[L"Layer_GameLogic"]->Add_GameObject((*dynamicWstring).c_str(), pGameObject);	
	pGameObject->SetObjectKey(*dynamicWstring);		// imgui 창에 나올 이름				
	pGameObject->SetTextureKey(m_sCurTextureKey);       // imgui 설정한 텍스처
	index++;
}

void CImgui_ObjectTool::Save()
{
	// ㅇㅋ 세이브 기능 구현완료 
	OPENFILENAME ofn;       // OPENFILENAME 구조체 선언
	wchar_t szFile[260];       // 파일 이름을 저장할 버퍼 (최대 260자)

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들 (여기선 콘솔 창을 사용)
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0'; // 초기 파일 이름 (빈 문자열)
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";  // 파일 형식 필터 (텍스트 파일과 모든 파일)
	ofn.nFilterIndex = 1;   // 기본 선택 파일 형식 (1 = Text Files)
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:\\Users\\tjsgh\\Desktop\\9월 19일 프레임워크 작업\\Client\\Map";  // 초기 디렉토리 (NULL이면 기본 디렉토리 사용)
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// 파일 저장 대화 상자 표시
	if (GetSaveFileName(&ofn) == TRUE) {
		// 파일 경로가 선택되면 메시지 박스로 파일 경로를 표시
		 
		HANDLE hFile = CreateFile(ofn.lpstrFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MSG_BOX("hFile not open");
		}

		// 여기서 오브젝트의 SetTextureKey와 object의 월드 매트릭스 값을 저장하면될듯 

		map<const _tchar*, CLayer*>& pLayerMap = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
		
		for (auto iter = pLayerMap.begin(); iter != pLayerMap.end(); ++iter)
		{
			const _tchar* layerKey = iter->first;

		
			if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
			{
				// 여기서 작업 파일내용에 들어갈것들
			
				
				//이게 해당 레이어 포인터 
				map<const _tchar*, CGameObject*>& pMapObject = iter->second->GetLayerGameObjectPtr();
				
				for(auto iter2 = pMapObject.begin(); iter2 != pMapObject.end(); ++iter2)
				{
					wstring TextureInfo = iter2->second->GetTextureKey();
					DWORD bytesToWrite = 100;
					//DWORD bytesToWrite = sizeof(wstring);
					DWORD bytesWritten = 0;	
					//
					if (!WriteFile(hFile, TextureInfo.c_str(), bytesToWrite, &bytesWritten, NULL))
					{
						MSG_BOX("FAILED TO WRITE TO TextureKey TO FILE");	
						CloseHandle(hFile);	
					}
					
					wstring ObjectName = iter2->second->GetObjectKey();
					CTransform* pTransform =  dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", ObjectName.c_str(), L"Com_Transform"));
					const D3DXMATRIX* WorldMatrix = pTransform->Get_WorldMatrix();	
					D3DXMATRIX Test = *WorldMatrix;

					bytesToWrite = sizeof(D3DXMATRIX);
					bytesWritten = 0;	

					if(!WriteFile(hFile, Test, bytesToWrite, &bytesWritten,NULL))
					{
						MSG_BOX("FAILED TO WRITE TO MATRIX TO FILE");
						CloseHandle(hFile);
					}
				}
			}
		}

		MSG_BOX("SUCCESS");
		 
	}
	else {
		// 오류나 취소 시, 오류 코드를 확인
		DWORD error = CommDlgExtendedError();
		if (error != 0) {
			MSG_BOX("Error");
			//char errorMsg[100];
			//sprintf(errorMsg, "Error code: %lu", error);
			//MessageBox(g_hWnd, errorMsg, "Error", MB_OK | MB_ICONERROR);
		}
	}
}

void CImgui_ObjectTool::Read()
{
	OPENFILENAME ofn;       // OPENFILENAME 구조체 선언
	wchar_t szFile[260];   // 파일 이름을 저장할 버퍼 (최대 260자)

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // 초기 파일 이름 (빈 문자열)
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Text Files\0*.txt\0All Files\0*.*\0";  // 파일 형식 필터
	ofn.nFilterIndex = 1;      // 기본 선택 파일 형식
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:\\Users\\tjsgh\\Desktop\\9월 19일 프레임워크 작업\\Client\\Map";  // 초기 디렉토리
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	// 파일 열기 대화상자 표시
	if (GetOpenFileName(&ofn)) {
		// 파일 열기
		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	
		if (hFile != INVALID_HANDLE_VALUE) {
			
			

			// 순서 문자열의 읽기 -> 해당 월드매트릭스값 가져오기 
			//wstring	teststring = L"";

			DWORD bytesRead = 0;


			if(!ReadFile(hFile,	&m_wCurReadTextureKey,sizeof(wstring),&bytesRead,NULL))
			{
				MSG_BOX("FAILED TO READ TextrueKey");
				CloseHandle(hFile);
			}

		

			
			D3DXMATRIX worldmatrix;
			bytesRead = 0; 
			
			if(!ReadFile(hFile, worldmatrix, sizeof(D3DXMATRIX),&bytesRead,NULL))	
			{
				MSG_BOX("FAILED TO READ WORLDMAXTIRX");
				CloseHandle(hFile);
			}


			CloseHandle(hFile);  // 파일 핸들 닫기	
		}
		else {
			MSG_BOX("파일을 열 수 없습니다.");
			//std::cerr << "파일을 열 수 없습니다." << std::endl;
		}
	}

	MSG_BOX("SUCCESS");
	

}


_vec3 CImgui_ObjectTool::PickingOnTerrain()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse); // 클라이언트 좌표화

	_vec3 vMousePos;

	D3DVIEWPORT9 ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&ViewPort);

	// 뷰 포트 -> 투영
	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;


	// 투영 -> 뷰 스페이스 
	_matrix matProj;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	_vec3 vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;

	// 뷰스페이스 -> 월드 
	_matrix matView;
	CGraphicDev::GetInstance()->Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView); // 여기서 현재 뷰가 업데이트가안됨
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView); // 위치 관련 이동
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 좌표에서 할거니깐 터레인의 월드좌표 점들을 가져와야함 
	CMapTex* pTerrainBufferCom = dynamic_cast<CMapTex*>(Engine::Get_Component(ID_STATIC, L"Layer_Environment", L"Map", L"Com_Buffer"));
	if (pTerrainBufferCom == nullptr)
	{
		MSG_BOX("pTerrainBufferCom is nullptr");
	}

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_Environment", L"Map", L"Com_Transform"));
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