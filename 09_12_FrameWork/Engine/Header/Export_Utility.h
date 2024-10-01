#pragma once

#include "Management.h"
#include "ComponentMgr.h"
#include "Renderer.h"
#include "LightMgr.h"
#include "CollisionMgr.h"
#include "SoundManager.h"


#include "PipeBoardCom.h"
#include "PipeCom.h"
#include "PlantBulletCom.h"

#include "TriCol.h"
#include "RcCol.h"
#include "Transform.h"
#include "Texture.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "Camera.h"
#include "../Client/Header/DynamicCamera.h"

#include "State.h"
#include "StateController.h"
#include "Animation.h"

#include "CubeTex.h"
#include "Calculator.h"

#include "AnimationTex.h"
#include "CStartMap.h"
#include "MapTex.h"
#include "Collider.h"

// �ִϸ��̼� �����ӿ�ũ�׽�Ʈ
#include "Animator2.h"
#include "Animation2.h"





BEGIN(Engine)

inline HRESULT				Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);

inline CComponent*			Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
inline	HRESULT				Set_Scene(CScene* pScene);
inline	_int				Update_Scene(const _float& fTimeDelta);
inline	void				LateUpdate_Scene(const _float& fTimeDelta);
inline	void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

//9�� 5�� ���� �߰��� �ڵ� 
inline CGameObject*         Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag);
inline CScene* Get_CurScenePtr();

// ComponentMgr

inline HRESULT	 Ready_Proto(const _tchar* pComponentTag, CComponent* pComponent);
inline CComponent* Clone_Proto(const _tchar* pComponentTag);


//renderer

inline void  Add_RenderGroup(RENDERID eType, CGameObject* pGameObject);
inline void  Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void  Clear_RenderGroup();


//light

inline  HRESULT  Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
								const D3DLIGHT9* pLightInfo,
								const _uint& iIndex);

//sound
inline void Sound_Initialize();
inline void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume);
inline void PlayBGM(_tchar* pSoundKey, float fVolume);
inline void StopSound(CHANNELID eID);
inline void StopAll();
inline void SetChannelVolume(CHANNELID eID, float fVolume);
inline _tchar* Get_CurBGM();

inline void		Release_Utility();


//
#include "Export_Utility.inl"

END
