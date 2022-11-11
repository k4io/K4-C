#pragma once

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

#define oButtons_TypeInfo 56835632
#define oChatOpen 0x746E0 //public static void open() { } <- inside 'ConVar::Chat'
#define oSphereCast 0x2528E70
#define oGetWorldVelocity 0xC97710
#define oSetRayleigh 0x469380 //public static void set_atmosphere_rayleigh(float value) { }
#define oConsoleSystem_GetAll 39014272 //"ConsoleSystem.Index$$get_All"
#define oConvar 56843288
#define oTerrainMeta_TypeInfo 56843664
#define oPlayerEyes_TypeInfo 56726328
#define oPlayerBelt_TypeInfo 56828000
//REAL RUST
namespace offsets {
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 57071096;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 57070864;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 57070632;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 57070720;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 56950816;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 7150704;// method address

    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 57069232;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 18358864;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uint = 57068176;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 18357424;// method address

    constexpr auto Method$BaseEntity_ServerRPC_uintstring = 57068296;// "Address":,"Method$BaseEntity.ServerRPC<uint, string>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uintstring_ = 18356096;// method address
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