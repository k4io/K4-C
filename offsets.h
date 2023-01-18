#pragma once
#include "dump.h"
/*
#define oLineOfSightRadius 0x52CE70
#define oSphereCast 0x2272C50
#define oConsoleSystem_GetAll 36220464
#define oConvar 52694112
#define oTerrainMeta_TypeInfo 52694448
//RUSTICALAND
namespace offsets {
    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 52510592;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 23323040;// method address
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 52512520;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 52512272;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 52353360;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 52353464;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 52383392;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 20225552;// "Address":,"Method$BaseEntity.ServerRPC<uint>()"
    constexpr auto Method$BaseEntity_ServerRPC_uint = 52509528;// "Name": "BaseEntity$$ServerRPC<uint>"
    constexpr auto BaseEntity$$ServerRPC_uint_ = 23321600;// method address
    constexpr auto convar = 46502296; //"ConVar_Graphics_c*"
}
*/
#define oButtons_TypeInfo 48462704
#define oLineOfSightRadius 6957488
#define oChatOpen 0xA756E0 //public static void open() { } <- inside 'ConVar::Chat'
#define oCanReload 0x543180 // public bool CanReload(BasePlayer owner) { }
#define oSetGestureMenuOpen 0x76F240 //private void SetGestureMenuOpen(bool wantsOpen) { }
#define oSphereCast 0x24CD390
#define oGetWorldVelocity 0xB2C4F0
#define oSetRayleigh 0xAABA40 //public static void set_atmosphere_rayleigh(float value) { }
#define oConsoleSystem_GetAll 13790944 //"ConsoleSystem.Index$$get_All"
#define oConvarGraphics 48650200
#define oConvarClient 48496952
#define oConvarAdmin 48947432
#define oTerrainMeta_TypeInfo 48587160
#define oPlayerEyes_TypeInfo 48949696
#define oPlayerBelt_TypeInfo 48948296
#define oHitTest_TypeInfo 48667992

#define oDevControlsOnGui 0x698C20
#define oViewmodelPlay 0x6B3090
#define oset_ambientIntensity 0x2459B10
#define osetAmbientColor 0x353410
#define oGetMountedVehicle 0x75A010
#define oGetMounted 0x75A060
#define oRaycastGetTrans 0x24CE030
#define oDoAttack 0x51DFC0
#define oDontDestroyOnLoad 0x246D6E0
#define oInternal_CreateGameObject 0x2467BF0
#define oInternal_AddComponentWithType 0x2467630
#define oHasAmmo 0x566CA0
#define oGet_Renderers 0x347870
#define oSet_material 0x245D330
#define oGet_material 0x245CF60
#define oSet_float 0x2450CD0
#define oSet_color 0x2451910
#define oSet_shader 0x2451A80
#define oGet_shader 0x24518D0
#define oBaseMeleeCanHit 0x3454D0
#define oServRpc 0xB31C70
#define oGetParentVelocity 0xB2C150
#define oSendProjUpdate 0x76EF00
#define oGamephysTrace 0x6A3A80
#define oGet_localscale 0x2479300
#define oSet_localscale 0x2479C90
#define oLaunchProjectile 0x5273A0
#define oCapColliderGetHeight 0x24C5870
#define oCapColliderSetHeight 0x24C59D0
#define oCapColliderGetRadius 0x24C58B0
#define oCapColliderSetRadius 0x24C5A20
#define oBaseMeleeDoThrow 0x51E1B0
#define oBaseEntGetItem 0x361AC0
#define oLocalPlayerItemCmd 0x702E00
#define oGet_activemodel 0x9C9510
#define oBasePlayerUseAction 0x773230
#define oPwmClientInput 0x729850
#define oTimeWarningNew 0x361AC0
#define oNeedsKeyboardAnyActive 0x344E50
#define oBasePlayerHasLocalControls 0x75B3A0
#define oBasePlayerWaterFactor 0x774060
#define oObjGetName 0x367DC0
#define oLocalPlayerGetEnt 0x703810
#define oBaseEntClosestPoint 0xB27D50
#define oThrownWpnGetInheritedVel 0x51E970
#define oBasePlayerForcePosTo 0x757210
#define oTerrainCollisionGetIgnore 0x7EC500
#define oTerrainSampleHeight 0x24D46F0
#define oTerrainHeightMapGetHeight 0x7F6970
#define oTerrainHeightMapGetNormal 0x7F7360
#define oTransGetPos 0x24794F0
#define oTransSetPos 0x2479DF0
#define oTransGetRot 0x2479660
#define oTransSetRot 0x2479F70
#define oBasePlayerGetSpeed 0x75ADE0
#define oBasePlayerIsSwimming 0x75E2B0
#define oDdrawLine 0xA11520
#define oSkinSetGetIndex 0x8B53E0
#define oModelStateSet_OnLadder 0x1C21970
#define oBasePlayerHasPlayerFlag 0x75B410
#define oStringPoolGet 0x8B8110
#define oTimeSetTimescale 0x24765B0
#define oFloorToInt 0x2469480
#define oPhysicsGetGravity 0x24CD870
#define oProjGetIsAlive 0x68C4F0
#define oFindShader 0x245E0A0
#define oObjSetHideFlags 0x246F370
#define oObjSetName 0x1B6E1E0
#define oMaterialSetInt 0x2450D50
#define oTimeGetDeltaTime 0x24762F0
#define oTimeGetTimescale 0x2476470
#define oTimeGetSmoothDeltaTime 0x2476440
#define oTimeGetFixedDeltaTime 0x2476530
#define oTimeGetFixedTime 0x2476350
#define oPlayerModelGetIsNpc 0x7176E0
#define oTimeGetTime 0x24764A0
#define oInverseTransformPoint 0x2477800
#define oInverseTransformDir 0x2477740
#define oGet_localworldtomatrix 0x2479350
#define oCompGetComponent 0x2465880
#define oCompGetComponentsInChildren 0x2465960
#define oTransGetUp 0x24796B0
#define oQuaternionLookRot 0x2470800
#define oStartAttackCooldown 0x6D01C0
#define oBaseMeleeProcessAttack 0x51F120
#define oGetProjectileVelocityScale 0x525810
#define oRendererGetMaterial 0x245CF60
#define oRendererSetMaterial 0x245D330
#define oBasePlayerGetMaxSpeed 0x758C80
#define oSendSignalBroadcast 0xB319D0
#define oBaseEntBoundsPadding 0xB27350
#define oPlayerEyesGetPos 0x6FA520
#define oPlayerEyesGetCenter 0x6F9F20
#define oPlayerEyesGetRot 0x6FA9D0
#define oPlayerEyesHeadFwd 0x6F9230
#define oPlayerEyesBodyFwd 0x6F6210
#define oPlayerEyesHeadRight 0x6F94C0
#define oPlayerEyesBodyRight 0x6F63B0
#define oBasePlayerSendClientTick 0x76E5C0
#define oBasePlayerGetJumpHeight 0x758AB0
#define oBasePlayerGetRadius 0x75ACB0
#define oBasePlayerGetHeight 0x758050
#define oUpdateAmmoDisplay 0x5298C0
#define oShotFired 0x528FA0
#define oDidAttackClientSide 0x53D920
#define oGetModifiedAimCone 0x5CA670
#define oCanAffordUpgrade 0x52FA80
#define oCanChangeToGrade 0x52FC20
#define oUpgradeToGrade 0x533210
#define oModelStateSetJumped 0x1C21930
#define oModelStateSetSprint 0x1C21880
#define oModelStateSetAiming 0x1C21890
#define oModelStateSetDucked 0x1C217D0
#define oPwmJump 0x72BB90
#define oRigidBodySetVel 0x24CEB70
#define oRigidBodyGetVel 0x24CE650
#define oBasePlayerConsoleMessage 0x754D90
#define oBaseMovementTeleportTo 0x71BA90
#define oBaseProjectileGetAimCone 0x5240D0
#define oAnimCurveEval 0x243E010
#define oGuidToPath 0x8ACDB0
#define oBasePlayerGetBounds 0x7577D0
#define oBasePlayerGetMounted 0x75A060
#define oBaseNetworkableGetShortPrefabName 0x6D37A0
#define oGameObjGetTrans 0x2468190
#define oModularCarGetMaxFwdSpeed 0xACB1D0
#define oPlayerBeltGetItemInSlot 0x6F5790
#define oCompGetTrans 0x2465CE0
#define oDdrawText 0xA12520
#define oTimeGetFrameCount 0x18163B0
#define oCompGetGameObject 0x2465CA0
#define oBasePlayerBoundsPadding 0x8722B0
#define oGetParentEnt 0x6D2620
#define oMaterialGetShader 0xA27520
#define oMaterialSetShader 0x2451A80
#define oScreenGetWidth 0x245DE40
#define oScreenGetHeight 0x245DDE0
#define oCameraGetMain 0x2442C00
//#define oIgnoreLayerCollision
#define oLineOfSight 0x6A2D30
#define oGetRealtimeSinceStartup 0x24763E0
#define oRaycast 0x24CB900
#define oLineOfSightInternal 0x6A2390
#define oLoadFromFile 0x243A510
#define oLoadAsset 0x243A270
#define oCheckCapsule 0x6A1560
#define oGetScattering 0x3477D0

#define oProjUpdate 0x68C190
#define oDdrawSphere 0xA12290
#define oDdrawCapsule 0xA10D20
#define oProjRetire 0x68B1B0
#define oGamePhysTraceAll 0x6A39B0
#define oHitPointWorld 0x7864E0
#define oHitNormalWorld 0x7863D0
#define oAssetNameCacheGetName 0x8A68D0
#define oProjDoHit 0x6881B0
#define oGetMagnitude 0x247D840
#define oGetRandomVel 0x7B54C0
#define oProjSetEffectScale 0x68B790
#define oProjLaunch 0x68A370

#define oBasePlayerClientInput 0x750D80
#define oBaseProjectileLaunchProjectile 0x5273A0
#define oDdrawGet 0xA110A0
#define oModelStateSetFlying 0x1C218F0
#define oModelStateGetDucked 0x1C217D0
#define oOnLand 0x760CE0
#define oPerformanceUIUpdate 0xDAA350
#define oInternal_CreateGameObject 0x2467BF0
#define oInternal_AddComponentWithType 0x2467630

//REAL RUST
namespace offsets {
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 48889928;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 48890760;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 48891384;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 48891176;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 48798992;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 16851856;// method address

    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 48894088;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 0;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uint = 48892008;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 15780560;// method address
}

/*
//ALKAD
#define oLineOfSightRadius 0x7DE0C0
#define oSphereCast 0x24FFD00
#define oConsoleSystem_GetAll 38895632 //"ConsoleSystem.Index$$get_All"
#define oConvar 56626808
#define oTerrainMeta_TypeInfo 56627184
#define oPlayerEyes_TypeInfo 56512408
#define oGetWorldVelocity 0x82EDB0

namespace offsets { 
    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 56859744;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 14739920;// method address
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 56861592;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 56861336;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 56861112;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 56861224;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 56740008;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 7162400;// method address
    constexpr auto Method$BaseEntity_ServerRPC_uint = 56858680;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 14738480;// method address
}
*/