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


#define oLineOfSightRadius 0x7CD740
#define oSphereCast 0x250D820
#define oConsoleSystem_GetAll 38951728 //"ConsoleSystem.Index$$get_All"
#define oConvar 56684744
#define oTerrainMeta_TypeInfo 56685096
#define oPlayerEyes_TypeInfo 56570360
#define oGetWorldVelocity 0x83EE50
//REAL RUST
namespace offsets {
    constexpr auto Method_BaseEntity_ServerRPC_string_bool_address = 56917592;// address "Name": "Method$BaseEntity.ServerRPC<string, bool>()",
    constexpr auto BaseEntity$$ServerRPC_string_bool_Address = 17911952;// method address
    constexpr auto Method$BaseEntity_ServerRPC_CreateBuilding___ = 56919448;// "Name": "Method$BaseEntity.ServerRPC<CreateBuilding>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileAttack___ = 56919208;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileAttack>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileShoot___ = 56918952;// "Name": "Method$BaseEntity.ServerRPC<ProjectileShoot>()"
    constexpr auto Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___ = 56919080;// "Name": "Method$BaseEntity.ServerRPC<PlayerProjectileUpdate>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_Address = 56797864;// "Name": "Method$Facepunch.Pool.GetList<TraceInfo>()"
    constexpr auto Method$Facepunch_Pool_GetList_TraceInfo_MethodAddress = 7094432;// method address
    constexpr auto Method$BaseEntity_ServerRPC_uint = 56916528;// "Address":,"Method$BaseEntity.ServerRPC<uint>()" //both addresses
    constexpr auto BaseEntity$$ServerRPC_uint_ = 17910512;// method address
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