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

#define oButtons_TypeInfo 56933720
#define oLineOfSightRadius 9521792
#define oChatOpen 0x642950 //public static void open() { } <- inside 'ConVar::Chat'
#define oSetGestureMenuOpen 0x5EAAA0 //private void SetGestureMenuOpen(bool wantsOpen) { }
#define oSphereCast 0x2536110
#define oGetWorldVelocity 0xC9C290
#define oSetRayleigh 0x46A7D0 //public static void set_atmosphere_rayleigh(float value) { }
#define oConsoleSystem_GetAll 39068192 //"ConsoleSystem.Index$$get_All"
#define oConvar 56922288
#define oTerrainMeta_TypeInfo 56922664
#define oPlayerEyes_TypeInfo 56804248
#define oPlayerBelt_TypeInfo 56906904
//REAL RUST
namespace offsets {
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 57146920;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 57146696;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 57146432;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 57146528;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 57027720;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 7485296;// method address

    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 57145048;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 18282320;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uint = 57143944;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 18280880;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uintstring = 57144040;// "Address":,"Method$BaseEntity.ServerRPC<uint, string>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uintstring_ = 18279552;// method address
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