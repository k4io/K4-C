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
#define oButtons_TypeInfo 48577128
#define oLineOfSightRadius 6962880
#define oChatOpen 0xA76C00 //public static void open() { } <- inside 'ConVar::Chat'
#define oCanReload 0x544530 // public bool CanReload(BasePlayer owner) { }
#define oSetGestureMenuOpen 0x770760 //private void SetGestureMenuOpen(bool wantsOpen) { }
#define oSphereCast 0x24E0800
#define oGetWorldVelocity 0xB33BB0
#define oSetRayleigh 0xAACFC0 //public static void set_atmosphere_rayleigh(float value) { }
#define oConsoleSystem_GetAll 13821344 //"ConsoleSystem.Index$$get_All"
#define oConvarGraphics 48764840
#define oConvarClient 48611392
#define oConvarAdmin 49062152
#define oTerrainMeta_TypeInfo 48702360
#define oPlayerEyes_TypeInfo 49065520
#define oPlayerBelt_TypeInfo 49064064

//REAL RUST
namespace offsets {
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 49020048;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 49020872;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 49021496;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 49021288;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 48931200;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 16887120;// method address

    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 49024200;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 0;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uint = 49022120;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 15817216;// method address
}