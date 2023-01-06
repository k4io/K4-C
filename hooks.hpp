#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include "nine_meter_poggers.hpp"

#include "esp.hpp"

#include "memory/defs.h"
#include "memory/minhook/include/MinHook.h"
//#include "gui/OnGUI.hpp"
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)


void dhook(void* func, void** og, void* dtr) {
	if (MH_Initialize() != MH_OK && MH_Initialize() != MH_ERROR_ALREADY_INITIALIZED) {
		return;
	}
	MH_CreateHook(func, dtr, og);
	MH_EnableHook(func);
}

int fp = 0;
int spin = 0;
int jitter = 0;

namespace hooks {
	namespace orig {
		static auto baseplayer_client_input = reinterpret_cast<void (*)(BasePlayer*, InputState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ClientInput"), -1, _(""), _(""))));
		static auto requestuserinfo = reinterpret_cast<void (*)(Client*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnRequestUserInformation"), 1, _(""), _(""))));
		static auto playermodel_updatelocalvelocity = reinterpret_cast<void (*)(PlayerModel*, Vector3, Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerModel"), _("UpdateLocalVelocity"), 2, _(""), _(""))));
		static auto BaseProjectile_OnSignal = reinterpret_cast<void (*)(BaseProjectile*, int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("OnSignal"), 2, _(""), _(""))));
		static auto playerwalkmovement_client_input = reinterpret_cast<void (*)(PlayerWalkMovement*, uintptr_t, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), -1, _(""), _(""))));
		static auto DoFixedUpdate = reinterpret_cast<void (*)(PlayerWalkMovement*, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("DoFixedUpdate"), -1, _(""), _(""))));
		static auto UpdateVelocity = reinterpret_cast<void (*)(PlayerWalkMovement*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("UpdateVelocity"), -1, _(""), _(""))));
		static auto blocksprint = reinterpret_cast<void (*)(BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("BlockSprint"), 1, _(""), _(""))));
		static auto OnNetworkMessage = reinterpret_cast<void (*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnNetworkMessage"), 1, _(""), _(""))));
		static auto IsConnected = reinterpret_cast<bool (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("IsConnected"), 0, _(""), _("Network"))));
		static auto Run = reinterpret_cast<System::string*(*)(uintptr_t, System::string*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ConsoleSystem"), _("Run"), 0, _(""), _(""))));
		
		static auto pifu = reinterpret_cast<void(*)(PlayerInput*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerInput"), _("FrameUpdate"), 0, _(""), _(""))));


		static auto get_bodyleanoffset = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_BodyLeanOffset"), 0, _(""), _(""))));
		static auto EyePositionForPlayer = reinterpret_cast<Vector3(*)(BaseMountable*, BasePlayer*, Vector4)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMountable"), _("EyePositionForPlayer"), 2, _(""), _(""))));
		static auto isdown = reinterpret_cast<bool(*)(InputState*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("InputState"), _("IsDown"), 1, _(""), _(""))));
		static auto eokadoattack = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("FlintStrikeWeapon"), _("DoAttack"), 0, _(""), _(""))));
		static auto baseprojectile_launchprojectile = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("LaunchProjectile"), 0, _(""), _(""))));
		static auto attackent_addpunch = reinterpret_cast<void(*)(uintptr_t, Vector3, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("HeldEntity"), _("AddPunch"), 0, _(""), _(""))));
		static auto baseprojectile_createprojectile = reinterpret_cast<uintptr_t(*)(BaseProjectile*, System::string, Vector3, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("CreateProjectile"), 0, _(""), _(""))));
		static auto DoHit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("DoHit"), -1, _(""), _(""))));
		
		static auto createeffect = reinterpret_cast<GameObject* (*)(System::string, Effect*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("EffectLibrary"), _("CreateEffect"), 2, _(""), _(""))));

		static auto _update = reinterpret_cast<void (*)(Projectile*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("Update"), 0, _(""), _(""))));

		static auto VisUpdateUsingCulling = reinterpret_cast<void (*)(void*, float, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("VisUpdateUsingCulling"), 1, _(""), _(""))));
		uintptr_t playerprojectileattack;
		uintptr_t createbuilding;
		uintptr_t playerprojectilericochet;
		uintptr_t playerprojectileupdate;
		uintptr_t baseprojectilecreateprojectile;
		uintptr_t serverrpc_projectileshoot;
		uintptr_t serverrpc_processattack;
	}

#pragma region functions

	//static auto serverrpc_projecileshoot = rb::pattern::find_rel(
	//	_("GameAssembly.dll"), _("4C 8B 0D ? ? ? ? 4C 8B 74 24"));
	static auto serverrpc_projecileshoot = rb::pattern::find_rel(
		_("GameAssembly.dll"), _("4C 8B 0D ? ? ? ? 48 8B 75 28"));

	static auto serverrpc_uint = rb::pattern::find_rel(
		_("GameAssembly.dll"), _("74 3A 4C 8B 0D ? ? ? ? 48 8B CB"));

	static auto set_sprinting = reinterpret_cast<void (*)(ModelState*, bool value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_sprinting"), -1, _(""), _(""))));

	static auto draw_get = reinterpret_cast<void (*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Get"), 0, _(""), _("UnityEngine"))));

	static auto set_flying = reinterpret_cast<void (*)(ModelState * model_state, bool value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_flying"), 1, _(""), _(""))));

	static auto get_ducked = reinterpret_cast<bool (*)(ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("get_ducked"), 0, _(""), _(""))));

	static auto OnLand = reinterpret_cast<void (*)(BasePlayer*, float fVelocity)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("OnLand"), 1, _("fVelocity"), _(""), 1)));

	static auto change_code_rpc = reinterpret_cast<void (*)(BasePlayer*, System::string, uintptr_t, bool, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_string_bool_Address); //BaseEntity$$ServerRPC<string, bool> Address

	static auto ServerRPC_int = reinterpret_cast<void (*)(BaseProjectile*, System::string funcName, unsigned int arg1, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uint_);
	
	//static auto get_resourcePath = reinterpret_cast<System::string (*)(uintptr_t)>(mem::game_assembly_base + offsets::Method$ResourceRef_method);

	static auto PerformanceUI_Update = reinterpret_cast<void (*)(void*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PerformanceUI"), _("Update"), -1, _(""), _("Facepunch"))));

	void init_hooks() {
		orig::VisUpdateUsingCulling = reinterpret_cast<void (*)(void*, float, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("VisUpdateUsingCulling"), 1, _(""), _(""))));
		orig::requestuserinfo = reinterpret_cast<void (*)(Client*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnRequestUserInformation"), 1, _(""), _(""))));
		orig::pifu = reinterpret_cast<void(*)(PlayerInput*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerInput"), _("FrameUpdate"), 0, _(""), _(""))));
		orig::createeffect = reinterpret_cast<GameObject * (*)(System::string, Effect*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("EffectLibrary"), _("CreateEffect"), 2, _(""), _(""))));
		orig::IsConnected = reinterpret_cast<bool (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("IsConnected"), 0, _(""), _("Network"))));
		orig::OnNetworkMessage = reinterpret_cast<void (*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnNetworkMessage"), 1, _(""), _(""))));
		orig::BaseProjectile_OnSignal = reinterpret_cast<void (*)(BaseProjectile*, int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("OnSignal"), 2, _(""), _(""))));
		orig::baseplayer_client_input = reinterpret_cast<void (*)(BasePlayer*, InputState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ClientInput"), -1, _(""), _(""))));
		orig::playerwalkmovement_client_input = reinterpret_cast<void (*)(PlayerWalkMovement*, uintptr_t, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), -1, _(""), _(""))));
		orig::DoFixedUpdate = reinterpret_cast<void (*)(PlayerWalkMovement*, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("DoFixedUpdate"), -1, _(""), _(""))));
		orig::blocksprint = reinterpret_cast<void (*)(BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("BlockSprint"), 1, _(""), _(""))));
		orig::Run = reinterpret_cast<System::string*(*)(uintptr_t, System::string*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ConsoleSystem"), _("Run"), 0, _(""), _(""))));
		orig::playermodel_updatelocalvelocity = reinterpret_cast<void (*)(PlayerModel*, Vector3, Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerModel"), _("UpdateLocalVelocity"), 2, _(""), _(""))));
		orig::get_bodyleanoffset = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_BodyLeanOffset"), 0, _(""), _(""))));

		orig::isdown = reinterpret_cast<bool(*)(InputState*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("InputState"), _("IsDown"), 1, _(""), _(""))));

		orig::UpdateVelocity = reinterpret_cast<void (*)(PlayerWalkMovement*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("UpdateVelocity"), -1, _(""), _(""))));

		orig::EyePositionForPlayer = reinterpret_cast<Vector3(*)(BaseMountable*, BasePlayer*, Vector4)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMountable"), _("EyePositionForPlayer"), 2, _(""), _(""))));

		orig::eokadoattack = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("FlintStrikeWeapon"), _("DoAttack"), 0, _(""), _(""))));

		orig::baseprojectile_launchprojectile = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("LaunchProjectile"), 0, _(""), _(""))));

		orig::baseprojectile_createprojectile = reinterpret_cast<uintptr_t(*)(BaseProjectile*, System::string, Vector3, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("CreateProjectile"), 0, _(""), _(""))));

		orig::DoHit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("DoHit"), -1, _(""), _(""))));

		orig::attackent_addpunch = reinterpret_cast<void(*)(uintptr_t, Vector3, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("HeldEntity"), _("AddPunch"), 0, _(""), _(""))));

		orig::_update = reinterpret_cast<void (*)(Projectile*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("Update"), 0, _(""), _(""))));

		serverrpc_projecileshoot = rb::pattern::find_rel(
			_("GameAssembly.dll"), _("4C 8B 0D ? ? ? ? 48 8B 75 28"));

		serverrpc_uint = rb::pattern::find_rel(
			_("GameAssembly.dll"), _("74 3A 4C 8B 0D ? ? ? ? 48 8B CB"));

		ServerRPC_int = reinterpret_cast<void (*)(BaseProjectile*, System::string funcName, unsigned int arg1, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uint_);

		//get_resourcePath = reinterpret_cast<System::string(*)(uintptr_t)>(mem::game_assembly_base + offsets::Method$ResourceRef_method);

		change_code_rpc = reinterpret_cast<void (*)(BasePlayer*, System::string, uintptr_t, bool, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_string_bool_Address);

		set_sprinting = reinterpret_cast<void (*)(ModelState*, bool value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_sprinting"), -1, _(""), _(""))));

		draw_get = reinterpret_cast<void (*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Get"), 0, _(""), _("UnityEngine"))));

		set_flying = reinterpret_cast<void (*)(ModelState * model_state, bool value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_flying"), 1, _(""), _(""))));

		get_ducked = reinterpret_cast<bool (*)(ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("get_ducked"), 0, _(""), _(""))));

		PerformanceUI_Update = reinterpret_cast<void (*)(void*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PerformanceUI"), _("Update"), -1, _(""), _("Facepunch"))));

		OnLand = reinterpret_cast<void (*)(BasePlayer*, float fVelocity)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("OnLand"), 1, _("fVelocity"), _(""), 1)));

		_DoHit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("DoHit"), -1, _(""), _(""))));
	}

#pragma endregion

#pragma optimize("", off)		
#pragma code_seg(".text")
	inline int64_t get_rbx_value()
	{
		static __declspec(allocate(".text")) char fn_stub[] = {
			0x48,
			0x89,
			0xD8, // mov    rax,rbx
			0xc3 // ret
		};

		return ((int64_t(*)())(&fn_stub))();
	}
#pragma code_seg(pop)
#pragma optimize("", on)

#define ptr_assert(val) \
    if(val == 0)        \
    return
	uintptr_t client_entities;
	
	bool flying = false, is_speeding = false, is_lagging = false, has_intialized_methods = false, wake = true, launchedmelee = false;
	float nextActionTime = 0, period = 1.4721, last_gesture_rpc = 0.f, last_revive_rpc = 0.f, last_refill_rpc = 0.f, last_fwd_fly = 0.f;
	Vector3 m_debugcam_toggle_pos, m_debugcam_pos, tempmelvel, tempmelpos, lastMovementLinePos;
	uintptr_t do_fixed_update_ptr, client_input_ptr, bodylean_ptr, mounteyepos_ptr, isdown_ptr, __go;
	bool iii = false, iiii = false;

	void get_skydome()
	{
		uintptr_t m_skydome = NULL;
		uintptr_t m_camera = NULL;
		uintptr_t last_object = NULL;
		{
			auto gom = mem::read<uintptr_t>(mem::unity_player_base + 0x17C1F18);
			auto current_tagged_base = mem::read<uintptr_t>(gom + 0x08);
			auto current_tagged_obj = mem::read<uintptr_t>(current_tagged_base + 0x10);

			while (current_tagged_obj &&
				current_tagged_obj != last_object &&
				(!m_skydome || !m_camera)) {
				INT16 tag = mem::read<uintptr_t>(current_tagged_obj + 0x54);

				if (!m_camera) {
					if (tag == 5) {
						uintptr_t objClass = mem::read<uintptr_t>(current_tagged_obj + 0x30);

						uintptr_t ent = mem::read<uintptr_t>(objClass + 0x18);

						m_camera = mem::read<uintptr_t>(current_tagged_obj + 0x18);
					}
				}

				if (!m_skydome)
				{
					m_skydome = mem::read<uintptr_t>(current_tagged_obj + 0x18);
				}

				if (tag == 20011) {
					uint64_t p = mem::read<uint64_t>(current_tagged_obj + 0x30);
					uint64_t p1 = mem::read<uint64_t>(p + 0x18);
					uint64_t tod_sky = mem::read<uint64_t>(p1 + 0x28);

					const auto TOD_Day = *reinterpret_cast<uintptr_t*>(tod_sky + 0x50);
					const auto TOD_Night = *reinterpret_cast<uintptr_t*>(tod_sky + 0x58);
					const auto TOD_Stars = *reinterpret_cast<uintptr_t*>(tod_sky + 0x70);

					const auto tod_component = *reinterpret_cast<uintptr_t*>(tod_sky + 0xA8);

					Vector4 sun_color = Vector4(vars->colors.sun[0],
						vars->colors.sun[1],
						vars->colors.sun[2],
						vars->colors.sun[3]);
					Vector4 moon_color = Vector4(vars->colors.moon[0],
						vars->colors.moon[1],
						vars->colors.moon[2],
						vars->colors.moon[3]);

					//*(Vector4**)(mem::read<uintptr_t>(TOD_Day + 0x10) + 0x10) = &sun_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Day + 0x18) + 0x10) = &sun_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Day + 0x20) + 0x10) = &sun_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Day + 0x28) + 0x10) = &sun_color;

					//*(Vector4**)(mem::read<uintptr_t>(TOD_Night + 0x10) + 0x10) = &moon_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Night + 0x18) + 0x10) = &moon_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Night + 0x20) + 0x10) = &moon_color;
					//*(Vector4**)(mem::read<uintptr_t>(TOD_Night + 0x28) + 0x10) = &moon_color;

					* (float*)(TOD_Night + 0x50) = vars->visual.night;
					*(float*)(TOD_Night + 0x54) = vars->visual.night;
					*(float*)(TOD_Day + 0x50) = vars->visual.day;
					*(float*)(TOD_Day + 0x54) = vars->visual.day;
					*(float*)(TOD_Stars + 0x14) = vars->visual.staramount;


					RenderSettings::set_ambientLight(tod_sky,
						{
							vars->visual.ambcol[0],
							vars->visual.ambcol[1],
							vars->visual.ambcol[2],
							vars->visual.ambcol[3]
						});

				}
				last_object = current_tagged_obj;
				current_tagged_base = mem::read<uintptr_t>(current_tagged_base + 0x8);
				current_tagged_obj = mem::read<uintptr_t>(current_tagged_base + 0x10);
			}
		}
	}

	static uintptr_t* serverrpc_projecshoot;
	static uintptr_t* serverrpc_playerprojectileattack;
	static uintptr_t* serverrpc_playerprojectileupdate;

	int pppid = 0;

	void hk_serverrpc_projectileshoot(int64_t rcx, int64_t rdx, int64_t r9, int64_t projectileShoot, int64_t arg5) {
		Projectile* p;
		Vector3 rpc_position;
		float time = get_fixedTime();
		float travel_t = 0.f;
		int simulations = 0;
		bool ismelee = false;
		do {
			if (!vars->local_player)
				break;

			auto Item = vars->local_player->GetActiveItem();

			if (!Item)
				break;

			auto baseprojectile = vars->local_player->GetActiveItem()->GetHeldEntity<BaseProjectile>();
			if (!baseprojectile)
				break;

			auto wep_class_name = *(const char**)(*(uintptr_t*)(uintptr_t)baseprojectile + 0x10);
			if (*(int*)(wep_class_name + 4) == 'eleM' || *(int*)(wep_class_name) == 'ddaP') {
				//is melee weapon
				if (vars->combat.psilentmelee) {
					ismelee = true;
				}
				else break;
			}

			vars->matrix = unity::get_view_matrix();
			auto camera_pos = unity::get_camera_pos();

			//aim_target target = vars->local_player->get_aimbot_target(camera_pos);
			aim_target target = vars->best_target;

			/*
				struct projectileshoot_projectile {
		bool ShouldPool; // 0x10
		bool _disposed; // 0x11
		int projectileID; // 0x14
		Vector3 startPos; // 0x18
		Vector3 startVel; // 0x24
		int seed; // 0x30
	};
	struct projectileshoot {
		bool ShouldPool; // 0x10
		bool _disposed; // 0x11
		int ammoType; // 0x14
		projectileshoot_projectile projectiles[]; //0x18
		//std::vector<projectileshoot_projectile> projectiles;
	};
			*/

			uintptr_t shoot_list = *(uintptr_t*)(*(uintptr_t*)(projectileShoot + 0x18));// + 0x10);

			if(!misc::emulated)
				shoot_list = *(uintptr_t*)(*(uintptr_t*)(projectileShoot + 0x18) + 0x10);

			auto size = *(int*)(*(uintptr_t*)(projectileShoot + 0x18) + 0x18);

			Vector3 aimbot_velocity = Vector3(0,0,0);

			const auto stats = baseprojectile->get_stats(Item->get_item_definition_id());

			Vector3 aim_angle;

			bool manipulated = false;

			Vector3 original_vel{};

			bool vis_fat = false;
			//get position of player with velocity
			//get direction to that
			//calculate perfect Y velocity in loop to find where projectile lands
			

			//calc new target position with velocity
			Vector3 target_velocity, target_pos;
			if (target.ent)
			{
				if (!target.ent->is_alive())
					target.Reset();
				target_pos = target.pos;
			}

			//new_pos = new_pos.multiply(target_velocity);

			for (int i = 0; i < size; i++) {
				uintptr_t projshoot = 0;
				if (misc::emulated)
				{
					if (i == size)
					{
						misc::emulated = false;
					}
				}
				else projshoot = *(uintptr_t*)(shoot_list + 0x20 + i * 0x8);

				original_vel = *reinterpret_cast<Vector3*>(projshoot + 0x24);
				rpc_position = *reinterpret_cast<Vector3*>(projshoot + 0x18);

				target.visible = target.ent->is_visible(rpc_position, target.pos);

				if (misc::best_target == Vector3(0, 0, 0)
					&& vars->combat.thick_bullet
					&& vars->combat.shoot_at_fatbullet
					&& !target.visible)
				{
					for (auto v : sphere1m)
					{
						if (target.ent)
						{
							if (target.ent->is_visible(target_pos, target_pos + v)
								&& target.ent->is_visible(target_pos + v, rpc_position))
								misc::best_target = target_pos + v;
						}
					}
				}

				//NEW POGCHAMP MANIPULATOR

				if (vars->combat.manipulator2
					&& target.ent->is_alive()
					&& ((!(!GetAsyncKeyState(vars->keybinds.manipulator)) || misc::manipulate_vis)
						&& settings::RealGangstaShit != Vector3(0, 0, 0)))
				{
					if (settings::FatHitPosition != Vector3(0, 0, 0))
					{
						target_pos = settings::FatHitPosition;
					}
					rpc_position = settings::RealGangstaShit;
					*reinterpret_cast<Vector3*>(projshoot + 0x18) = rpc_position;
					manipulated = true;
					target.visible = true;
				}
				
				else if (vars->combat.manipulator
					&& target.ent->is_alive()
					&& (!(!GetAsyncKeyState(vars->keybinds.manipulator))
						|| misc::manipulate_vis))
				{
					auto b = rpc_position;
					if (misc::best_lean != Vector3(0, 0, 0))
					{
						// rpc_position += misc::manipulate(vars->local_player, target.pos);
						//rpc_position += (misc::best_lean);
						rpc_position = misc::best_lean;
						if (vars->visual.angles)
							Sphere(rpc_position, 0.05f, { 0, 1, 0, 1 }, 10.f, 100.f);
						if (rpc_position != b)
						{
							manipulated = true;
							target.visible = true;
							*reinterpret_cast<Vector3*>(projshoot + 0x18) = rpc_position;
							if (misc::best_target != Vector3(0, 0, 0))
							{
								target_pos = misc::best_target;
								//vis_fat = true;
							}
						}
					}
				}
			}
			if (vars->combat.HitScan) {
				auto n = ((BasePlayer*)target.ent)->GetVisibleBone(rpc_position);
				if(!n.is_empty())
					target_pos = n;
			}
			target_pos.y += 0.2f;

			if (!ismelee) {
				BaseProjectile* projectile_list = *reinterpret_cast<BaseProjectile**>(
					*reinterpret_cast<uintptr_t*>((uintptr_t)baseprojectile + 0x398) + 0x10); //createdProjectiles;
				for (int i = 0; i < projectile_list->get_size(); i++)
				{
					auto projectile = *(BaseProjectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
					p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
					Vector3 a;

					auto tt = 0.f;
					auto pt = 0.f;
					auto dr = p->drag();
					auto gr = p->gravityModifier();

					Vector3 u = {};
					if (vars->combat.bestvelocity)
						misc::lower_velocity(target, rpc_position, target_pos, original_vel, stats.initial_velocity, aimbot_velocity, a, travel_t, pt, dr, gr, p, u);
					else misc::get_prediction(target, rpc_position, target_pos, original_vel, aimbot_velocity, a, travel_t, pt, dr, gr, u);

					if (vars->combat.thick_bullet)
						*reinterpret_cast<float*>(projectile + 0x2C) = vars->combat.thickness > 1.f ? 1.f : vars->combat.thickness;
					else
						*reinterpret_cast<float*>(projectile + 0x2C) = 0.01f;
					break;
				}
			}
			else {
				Vector3 a{}, u{};
				auto pt = 0.f;
				auto dr = stats.drag;
				auto gr = stats.gravity_modifier;
				misc::get_prediction(target, rpc_position, target_pos, original_vel, aimbot_velocity, a, travel_t, pt, dr, gr, u);
			}
			for (int i = 0; i < size; i++) {
				auto projectile = *(uintptr_t*)(shoot_list + 0x20 + i * 0x8);

				if (vars->combat.psilentmelee || unity::GetKey(vars->keybinds.psilent)) {
					if (target.ent && target.ent->health() > 0 && !target.teammate) {
						if (vars->combat.vischeck ? (target.visible || vis_fat) : true)
						{

							if (vars->combat.manipulator
								&& target.ent->is_alive()
								&& (!(!GetAsyncKeyState(vars->keybinds.manipulator))
									|| misc::manipulate_vis))
							{
								auto b = rpc_position;
								if (misc::best_lean != Vector3(0, 0, 0))
								{
									// rpc_position += misc::manipulate(vars->local_player, target.pos);
									//rpc_position += (misc::best_lean);
									rpc_position = misc::best_lean;
									if (vars->visual.angles)
										Sphere(rpc_position, 0.05f, { 0, 1, 0, 1 }, 10.f, 100.f);
									if (rpc_position != b)
									{
										manipulated = true;
										target.visible = true;
										if (misc::best_target != Vector3(0, 0, 0))
										{
											target_pos = misc::best_target;
											vis_fat = true;
										}
									}
								}
							}

							launchedmelee = true;
							tempmelvel = aimbot_velocity;
							tempmelpos = rpc_position;
						}
					}
				}
				if (target.ent && vars->combat.vischeck ? (target.visible || manipulated || misc::autoshot) : true && !target.teammate) {
					*reinterpret_cast<Vector3*>(projectile + 0x24) = aimbot_velocity; //startvel
					*reinterpret_cast<Vector3*>(projectile + 0x18) = rpc_position; //startpos
					//Sphere(target_pos, 0.05f, { r, g, b, 1 }, 5.f, 100.f);
				}
			}
			if (!ismelee) {
				BaseProjectile* projectile_list = *reinterpret_cast<BaseProjectile**>(
					*reinterpret_cast<uintptr_t*>((uintptr_t)baseprojectile + 0x398) + 0x10); //createdProjectiles;
				for (int i = 0; i < projectile_list->get_size(); i++) {
					auto projectile = *(BaseProjectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
					p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);

					if (!projectile)
						continue;

					if (vars->visual.visthick)
						p->transform()->SetLocalScale(p->transform()->GetLocalScale() * vars->visual.visthickness);

					if (vars->combat.bullet_tp)
					{
						//check traveltime
						if ((vars->desyncTime * vars->combat.tpmultiplier) > travel_t) {
							p->traveledTime(travel_t);
							p->SetInitialDistance(target.distance - 1);
						}
						else if (vars->desyncTime > 0.f)
						{
							Vector3 r = rpc_position;
							Vector3 v = aimbot_velocity;
							Vector3 n = rpc_position + (aimbot_velocity * (vars->desyncTime * vars->combat.tpmultiplier));
							p->traveledTime(vars->desyncTime);
							p->SetInitialDistance((r.distance(n) * vars->combat.tpmultiplier) - 1);
						}

						vars->local_player->console_echo(string::wformat(_(L"[matrix]: ProjectileShoot (bullet tp) spawned bullet at distance %dm"), (int)p->initialDistance()));
					}

					if (vars->combat.psilent || unity::GetKey(vars->keybinds.psilent)) {
						if (target.ent && target.ent->health() > 0 && !target.teammate) {
							if (vars->combat.vischeck ? (target.visible || vis_fat) : true)
							{
								projectile->set_current_velocity(aimbot_velocity);
								original_vel = aimbot_velocity;
								if (manipulated) {
									auto g = get_gameObject((uintptr_t)projectile);
									auto t = get_game_object_transform(g);
									set_position(t, rpc_position);
								}
							}
						}
					}

					if (vars->combat.thick_bullet)
						projectile->set_projectile_thickness(vars->combat.thickness > 1.f ? 1.f : vars->combat.thickness);//*reinterpret_cast<float*>(projectile + thickness_addr) = vars->combat.thickness > 1.f ? 1.f : vars->combat.thickness;
					else
						projectile->set_projectile_thickness(0.01f);

					if (vars->visual.visthick)
						projectile->transform()->SetLocalScale(p->transform()->GetLocalScale() * vars->visual.visthickness);

					//p->integrity(1.f);
					//float t = p->traveledTime();

					//create and SEND FAKE copy of projectile so game only updates original
					//Projectile* fake = new Projectile(*p);
					//p->initialVelocity(aimbot_velocity);
					fired_projectile f = { p, nullptr, time, 1, target };

					//p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
					//*reinterpret_cast<uintptr_t*>((uintptr_t)projectile_list + 0x20 + i * 0x8) = (uintptr_t)fake;

					//for (size_t i = 0; i < 32; i++)
					//	if (misc::fired_projectiles[i].fired_at <= 10.f) {
					//		misc::fired_projectiles[i] = f;
					//		break;
					//	}

					//if (!vars->combat.bullet_tp)
					//	p->SetInitialDistance(0);

					p->currentVelocity(original_vel);
					p->initialVelocity(original_vel);
					p->previousVelocity(original_vel);
					p->currentPosition(rpc_position);
					p->previousPosition(rpc_position);
					p->sentPosition(rpc_position);
					p->prevSentVelocity(rpc_position); //?
					p->launchTime(unity::get_realtimesincestartup());
					p->traveledTime(0);
					if (vars->combat.manipulator2
						&& (unity::GetKey(vars->keybinds.manipulator)
							&& settings::RealGangstaShit != Vector3(0, 0, 0)))
					{
						p->currentVelocity(aimbot_velocity);
						p->initialVelocity(aimbot_velocity);
						p->previousVelocity(aimbot_velocity);
					}
					
					if (vars->combat.targetbehindwall)
					{
						//p->projectileID(p->projectileID() - 1);
					}
				}
				if (vars->combat.targetbehindwall)
				{
					reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(hooks::orig::serverrpc_projectileshoot)(rcx, rdx, r9, projectileShoot, arg5);

					for (int i = 0; i < projectile_list->get_size(); i++) {
						auto projectile = *(BaseProjectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
						p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
						((Projectile1*)p)->Launch1();
					}

					misc::fixed_time_last_shot = get_fixedTime();
					misc::just_shot = true;
					misc::did_reload = false;
					typedef void(*cclear)(uintptr_t);
					((cclear)(mem::game_assembly_base + 15617184))((uintptr_t)baseprojectile + 0x398); //"System.Collections.Generic.List<Projectile>$$Clear",
					return;
				}
			}

			if (misc::autoshot)
				misc::autoshot = false;
		} while (0);

		vars->local_player->console_echo(string::wformat(_(L"[matrix]: ProjectileShoot (prediction) simulated %i times before hit!"), simulations));
		reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(hooks::orig::serverrpc_projectileshoot)(rcx, rdx, r9, projectileShoot, arg5);
		//pppid++;
		//calls base.serverrpc<projectileshoot>("clproject", x) ^^
		//loops through created projectiles and launches with
		//projectile.launch(); - this function has a while loop
		//based upon is alive? && (traveleddist < initialdist OR time < 0.1f)
		//while loop calls only UpdateVelocity(fixedDeltaTime);
		// 
		misc::fixed_time_last_shot = get_fixedTime();
		misc::just_shot = true;
		misc::did_reload = false;
	}

	protobuf::PlayerProjectileUpdate* tppu;
	void hk_serverrpc_playerprojectileupdate(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		auto  projectile = reinterpret_cast<Projectile*>(get_rbx_value());
		auto  ppu = reinterpret_cast<protobuf::PlayerProjectileUpdate*>(_ppa);
		tppu = ppu;
		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::playerprojectileupdate);

		//call fake domovement? after called set current position etc
		//projectile->DoMovement(misc::tickDeltaTime, projectile);
		//return;
		//printf(_("ppu called\n"));
		//Sphere(projectile->currentPosition(), 0.1f, { 1, 1, 0, 1 }, 5.f, 100.f);
		//if (vars->combat.pierce) {
		//	auto ht = (HitTest*)projectile->hitTest();
		//	if(!ht)
		//		return orig_fn(rcx, rdx, r9, _ppa, arg5);
		//	auto ent = ht->get_hit_entity();
		//	if(!ent)
		//		return orig_fn(rcx, rdx, r9, _ppa, arg5);
		//	auto gameobj = ent->get_game_object();
		//	if(!gameobj)
		//		return orig_fn(rcx, rdx, r9, _ppa, arg5);
		//	vars->local_player->console_echo(_(L"[matrix]: PlayerProjectileUpdate - classname"));
		//	auto s = std::string(gameobj->get_class_name());
		//	vars->local_player->console_echo(std::wstring(s.begin(), s.end()).c_str());
		//	if (!strcmp(gameobj->get_class_name(), _("CargoShip"))
		//		|| !strcmp(gameobj->get_class_name(), _("BaseOven"))
		//		|| !strcmp(gameobj->get_class_name(), _("TreeEntity"))
		//		|| !strcmp(gameobj->get_class_name(), _("OreResourceEntity"))
		//		|| !strcmp(gameobj->get_class_name(), _("CH47HelicopterAIController"))
		//		|| !strcmp(gameobj->get_class_name(), _("MiniCopter"))
		//		|| !strcmp(gameobj->get_class_name(), _("BoxStorage"))
		//		|| !strcmp(gameobj->get_class_name(), _("Workbench"))
		//		|| !strcmp(gameobj->get_class_name(), _("BuildingBlock"))
		//		|| !strcmp(gameobj->get_class_name(), _("VendingMachine"))
		//		|| !strcmp(gameobj->get_class_name(), _("Barricade"))
		//		|| !strcmp(gameobj->get_class_name(), _("BuildingPrivlidge"))
		//		|| !strcmp(gameobj->get_class_name(), _("LootContainer"))
		//		|| !strcmp(gameobj->get_class_name(), _("HackableLockedCrate"))
		//		|| !strcmp(gameobj->get_class_name(), _("ResourceEntity"))
		//		|| !strcmp(gameobj->get_class_name(), _("RidableHorse"))
		//		|| !strcmp(gameobj->get_class_name(), _("MotorRowboat"))
		//		|| !strcmp(gameobj->get_class_name(), _("ScrapTransportHelicopter"))
		//		|| !strcmp(gameobj->get_class_name(), _("JunkPile"))
		//		|| !strcmp(gameobj->get_class_name(), _("MiningQuarry"))
		//		|| !strcmp(gameobj->get_class_name(), _("WaterCatcher"))
		//		|| !strcmp(gameobj->get_class_name(), _("RHIB"))) {
		//		projectile->integrity(1);
		//		vars->local_player->console_echo(_(L"[matrix]: PlayerProjectileUpdate - returned for pierce!"));
		//		return;
		//	}
		//}

		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}

	void hk_serverrpc_playerprojectilericochet(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		auto        projectile = reinterpret_cast<Projectile*>(get_rbx_value());
		auto  ppa = reinterpret_cast<protobuf::PlayerProjectileRicochet*>(_ppa);
		

		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::playerprojectilericochet);

		//if (vars->combat.ricochet
		//	&& projectile->authoritative())
		//{
		//	Vector3 hitpos = ppa->hitPosition;
		//	Vector3 normal = ppa->hitNormal;
		//	Vector3 invel = ppa->inVelocity;
		//	Vector3 outvel = ppa->outVelocity;
		//
		//	Vector3 direction = outvel.Normalized();
		//
		//	Vector3 targetpos = vars->best_target.player->model()->boneTransforms()->get(48)->get_bone_position();
		//	Vector3 direction_to_target = targetpos - hitpos;
		//
		//	Vector3 newVelocity = direction_to_target.Normalized() * outvel.Length();
		//
		//	safe_write((uintptr_t)_ppa + 0x30, newVelocity, Vector3);
		//	projectile->currentVelocity(newVelocity);
		//	//safe_write((uintptr_t)ppa + 0x18, targetpos, Vector3);
		//
		//	Sphere(hitpos, 0.2f, col(1, 0.1, 0.2, 1), 10, 100);
		//}
		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}

	void hk_serverrpc_playerprojectileattack(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		auto        projectile = reinterpret_cast<BaseProjectile*>(get_rbx_value());
		auto  ppa = reinterpret_cast<protobuf::PlayerProjectileAttack*>(_ppa);
		auto& attack = ppa->playerAttack->attack;

		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::playerprojectileattack);

		auto camera_pos = unity::get_camera_pos();
		if (camera_pos.is_empty())
			return orig_fn(rcx, rdx, r9, _ppa, arg5);

		do {
			if (!vars->local_player)
				break;

			auto hit_test = projectile->get_hit_test();
			if (!hit_test)
				break;
			 
			if (!hit_test->gameObject())
				break;

			//if (vars->combat.throughwall)
			//	attack->hitMaterialID = 3711166224;
				//attack->hitMaterialID = 3711166224;

			if (vars->combat.pierce) {
				auto ht = (HitTest*)projectile->get_hit_test();
				if (!ht)
					return orig_fn(rcx, rdx, r9, _ppa, arg5);
				auto ent = ht->get_hit_entity();
				if (!ent)
					return orig_fn(rcx, rdx, r9, _ppa, arg5);
				auto gameobj = ent;// ->get_game_object();
				if (!gameobj)
					return orig_fn(rcx, rdx, r9, _ppa, arg5);
				if (!strcmp(gameobj->get_class_name(), _("CargoShip"))
					|| !strcmp(gameobj->get_class_name(), _("BaseOven"))
					|| !strcmp(gameobj->get_class_name(), _("TreeEntity"))
					|| !strcmp(gameobj->get_class_name(), _("OreResourceEntity"))
					|| !strcmp(gameobj->get_class_name(), _("CH47HelicopterAIController"))
					|| !strcmp(gameobj->get_class_name(), _("MiniCopter"))
					|| !strcmp(gameobj->get_class_name(), _("BoxStorage"))
					|| !strcmp(gameobj->get_class_name(), _("Workbench"))
					|| !strcmp(gameobj->get_class_name(), _("VendingMachine"))
					|| !strcmp(gameobj->get_class_name(), _("Barricade"))
					|| !strcmp(gameobj->get_class_name(), _("BuildingPrivlidge"))
					|| !strcmp(gameobj->get_class_name(), _("LootContainer"))
					|| !strcmp(gameobj->get_class_name(), _("HackableLockedCrate"))
					|| !strcmp(gameobj->get_class_name(), _("ResourceEntity"))
					|| !strcmp(gameobj->get_class_name(), _("RidableHorse"))
					|| !strcmp(gameobj->get_class_name(), _("MotorRowboat"))
					|| !strcmp(gameobj->get_class_name(), _("BuildingBlock"))
					|| !strcmp(gameobj->get_class_name(), _("ScrapTransportHelicopter"))
					|| !strcmp(gameobj->get_class_name(), _("JunkPile"))
					|| !strcmp(gameobj->get_class_name(), _("MiningQuarry"))
					|| !strcmp(gameobj->get_class_name(), _("WaterCatcher"))
					|| !strcmp(gameobj->get_class_name(), _("RHIB"))) {
					projectile->set_integrity(1);
					return;
				}
			}

			auto layer = hit_test->gameObject()->get_layer();
			auto prefab_name = hit_test->gameObject()->get_prefab_name();

			aim_target target = vars->best_target;//vars->local_player->get_aimbot_target(camera_pos);

			if (!target.ent)
				break;

			auto hit_entity = (BasePlayer*)hit_test->HitEntity();
			if (layer == layer::Player_Server) {
				if (hit_entity->is_teammate(vars->local_player)) {
					hit_test->ignoreEntity() = hit_entity;
					return;
				}
			}

			auto trans = hit_test->HitTransform();
			if (trans) {
				auto pos = trans->position();
				if (!pos.is_empty() && vars->visual.hitpoint)
				{
					hit h = { pos, get_fixedTime() };
					hitpoints.push_back(h);
				}
			}

			if (!vars->combat.hitbox_override)
				break;

			/*
StringPool::Get(xorstr_("head")) = 698017942
StringPool::Get(xorstr_("pelvis")) = 2811218643
StringPool::Get(xorstr_("r_hip")) = 2331610670
StringPool::Get(xorstr_("r_foot")) = 920055401
StringPool::Get(xorstr_("spine1")) = 3771021956
StringPool::Get(xorstr_("l_hand")) = 736328754
StringPool::Get(xorstr_("r_upperarm")) = 3901657145
StringPool::Get(xorstr_("l_knee")) = 3892428003
StringPool::Get(xorstr_("spine4")) = 827230707
*/

			if (!target.is_heli && vars->combat.hitbox_override) {
				int hitbone = 698017942;
				int hitpartid = 2173623152;

				struct hitt {
					int64_t hitbone;
					int64_t partid;
				};

				hitt test[1] = { 
					{ 0, 0 }
				};
				if (target.ent->model()) {
					if (target.ent->model()->boneTransforms()) {
						if (vars->combat.hitbox == 0
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::head)->position(), projectile->get_current_position()))
							test[0] = { 698017942, 2173623152 };
						if (vars->combat.hitbox == 1
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::pelvis)->position(), projectile->get_current_position()))
							test[0] = { 1031402764, 1750816991 };
						if (vars->combat.hitbox == 2
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::spine4)->position(), projectile->get_current_position()))
							test[0] = { 3901657145, 1750816991 };
						if (vars->combat.hitbox == 3
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::r_hand)->position(), projectile->get_current_position()))
							test[0] = { 102231371, 1750816991 };
						if (vars->combat.hitbox == 4
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::penis)->position(), projectile->get_current_position()))
							test[0] = { 612182976, 2173623152 };
						if (vars->combat.hitbox == 5
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::l_hip)->position(), projectile->get_current_position()))
							test[0] = { 3892428003, 1750816991 };
						if (vars->combat.hitbox == 6
							&& vars->local_player->is_visible(target.ent->model()->boneTransforms()->get(
								(int)Bone_List::r_foot)->position(), projectile->get_current_position()))
							test[0] = { 920055401, 1750816991 };
					}
				}

				attack->hitBone = test[0].hitbone;
				attack->hitPartID = test[0].partid;
				attack->hitPositionLocal = { -.1f, -.1f, 0 };
				attack->hitNormalLocal = { 0, -1, 0 };
			}
			else if (!target.is_heli && vars->combat.randomize) {
				switch (my_rand() % 10) {
				case 0: // Head
					attack->hitBone   = 698017942;
					attack->hitPartID = 2173623152;
					break;
				case 1: // Chest
					attack->hitBone	  = 1031402764;
					attack->hitPartID = 1750816991;
					break;
				case 2: // LHand
					attack->hitBone   = 182688154;
					attack->hitPartID = 1750816991;
					break;
				case 3: // RHand
					attack->hitBone   = 102231371;
					attack->hitPartID = 1750816991;
					break;
				case 4: // pelvis
					attack->hitBone   = 2811218643;
					attack->hitPartID = 1750816991;
					break;
				case 5: // r_hip
					attack->hitBone   = 2331610670;
					attack->hitPartID = 1750816991;
					break;
				case 6: // r_foot
					attack->hitBone   = 920055401;
					attack->hitPartID = 1750816991;
					break;
				case 7: // spine1
					attack->hitBone =   3771021956;
					attack->hitPartID = 1750816991;
					break;
				case 8: // l_upperarm
					attack->hitBone   = 3901657145;
					attack->hitPartID = 1750816991;
					break;
				case 9: // l_knee
					attack->hitBone   = 3892428003;
					attack->hitPartID = 1750816991;
					break;
				}
				attack->hitPositionLocal = { -.1f, -.1f, 0 };
				attack->hitNormalLocal = { 0, -1, 0 };
			}
			else if(target.is_heli && vars->combat.weakspots) {
				auto weakspots = ((BaseHelicopter*)target.ent)->weakspots();
				if (!weakspots)
					break;

				auto size = weakspots->get_size();//*reinterpret_cast<uintptr_t*>(weakspots + 0x18);

				bool tail_alive = false;
				bool main_alive = false;
				for (int i = 0; i < size; i++) {
					auto weakspot = weakspots->get(i);//*(uintptr_t*)(weakspots + 0x20 + i * 0x8);
					if (!weakspot)
						continue;
					auto health = *reinterpret_cast<float*>(weakspot + 0x24);
					if (health > 0) {
						if (i == 0) {
							main_alive = true;
						}
						else {
							tail_alive = true;
						}
					}
				}

				if (tail_alive) {
					attack->hitBone = 2699525250;
					attack->hitPartID = 2306822461;
					attack->hitPositionLocal = { .9f, -.4f, .1f };
					attack->hitNormalLocal = { .9f, -.4f, .1f };
				}
				else if (main_alive) {
					attack->hitBone = 224139191;
					attack->hitPartID = 2306822461;
					attack->hitPositionLocal = { .9f, -.4f, .1f };
					attack->hitNormalLocal = { .9f, -.4f, .1f };
				}
			}


			if (layer == layer::Deployed) {
				hit_test->ignoreEntity() = hit_entity;
				return;
			}
			//if (layer != layer::Player_Server) {
			//	hit_test->ignoreEntity() = hit_entity;
			//	return;
			//}
		} while (0);

		//printf("ppa called\n");

		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}

	void hk_serverrpc_doplace(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		//auto  planner = *reinterpret_cast<uintptr_t*>(get_rbx_value());
		auto  ppa = (protobuf::CreateBuilding * )_ppa;
		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::createbuilding);

		if(!vars->local_player)
			return orig_fn(rcx, rdx, r9, _ppa, arg5);

		auto held = vars->local_player->GetActiveItem()->GetHeldEntity<Planner>();
		if (!held)
			return orig_fn(rcx, rdx, r9, _ppa, arg5);
		
		//uintptr_t guide = *reinterpret_cast<uintptr_t*>(held + 0x210);
		auto guide = held->guide();

		if(!guide)
			return orig_fn(rcx, rdx, r9, _ppa, arg5);

		if (GetAsyncKeyState(0x38)) { 
			auto position = *reinterpret_cast<Vector3*>(guide + 0x18 + 0x34);
			auto normal = *reinterpret_cast<Vector3*>(guide + 0x18 + 0x40);
			
			auto ogid = *reinterpret_cast<unsigned int*>(_ppa + 0x14);

			auto build_id = *reinterpret_cast<unsigned int*>(_ppa + 0x14);
			auto tranny = esp::find_transform_by_id(build_id);
			if (GetAsyncKeyState(0x39))
			{
				*reinterpret_cast<unsigned int*>(_ppa + 0x14) = vars->local_player->net()->get_id();
				tranny = vars->local_player->transform();
			}
			else
				*reinterpret_cast<unsigned int*>(_ppa + 0x14) = vars->selected_entity_id;


			if (tranny) {
				//transform* tr = get_transform((base_player*)FindEntity);
				//transform* Traaa = get_transform//FindEntity->_get_transform();
				*reinterpret_cast<Vector3*>(_ppa + 0x20) = ((Transform*)tranny)->InverseTransformPoint(position);
				*reinterpret_cast<Vector3*>(_ppa + 0x2C) = ((Transform*)tranny)->InverseTransformDirection(normal);

				vars->local_player->console_echo(string::wformat(_(L"[matrix] DoPlace - Spoofed %d to %d with position (%d, %d, %d)"), 
					(int)ogid,
					(int)vars->selected_entity_id,
					(int)position.x,
					(int)position.y,
					(int)position.z
				));
			}
		}
		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}
	 
	void hk_playerwalkmovement_ClientInput(PlayerWalkMovement* player_walk_movement, uintptr_t inputstate, ModelState* model_state) {

		auto baseplayer = vars->local_player;
		//__try {
		//	player_walk_movement->ClientInput((InputState*)inputstate, model_state);
		orig::playerwalkmovement_client_input(player_walk_movement, inputstate, model_state);
		//}
		//__except (true) {  }
		Vector3 vel = player_walk_movement->get_TargetMovement();
		if (!baseplayer) return;
		auto dont_move = false;
		//if (baseplayer) baseplayer->console_echo(_(L"before manipulator"));
		if ((vars->combat.manipulator2 || vars->combat.manipulator)
			&& (unity::GetKey(vars->keybinds.manipulator)
				|| misc::manipulate_vis))
		{
			if (vars->misc.freezeondesync) {
				//manipulatingw
				player_walk_movement->set_TargetMovement({ 0, 0, 0 });
				return;
			}
		}
		//if (baseplayer) baseplayer->console_echo(_(L"baseplayer"));
		if (!baseplayer || baseplayer->is_sleeping()) 
			return;
		//if (baseplayer) baseplayer->console_echo(_(L"baseplayer after"));
		auto time = unity::get_realtimesincestartup();//UnityEngine::Time::get_realtimeSinceStartup();
		float _timeSinceLastTick = time - baseplayer->lastSentTickTime();
		if (baseplayer && !baseplayer->is_sleeping()) {
			if (unity::GetKey(vars->keybinds.tp)) {
				if (vars->last_teleport_time + 20.f < get_fixedTime()) {
					vars->last_teleport_time = get_fixedTime();

					Vector3 a = Vector3::Zero();
					auto fwd = baseplayer->eyes()->body_forward() * 5;
					auto right = baseplayer->eyes()->body_right() * 5;
					auto up = Vector3(0, 5, 0);

					if (unity::GetKey(0x57)) //W
					{
						a = fwd;
					}
					else if (unity::GetKey(0x41)) //A
					{
						a = -right;
					}
					else if (unity::GetKey(0x53)) //S
					{
						a = -fwd;
					}
					else if (unity::GetKey(0x44)) //D
					{
						a = right;
					}
					else {
						a = up;
					}

					player_walk_movement->teleport_to(baseplayer->transform()->position() + a, baseplayer);
				}
			}
		}
		if (baseplayer && !baseplayer->is_sleeping()) {

			if (vars->misc.flyhack_stop) {
				auto trans = baseplayer->transform();
				if (trans) {
					Vector3 curr = trans->position();
					Vector3 old = misc::cLastTickPos;
					Vector3 v4 = (curr - misc::cLastTickPos);
					Vector3 ov = Vector3(curr.x, curr.y, curr.z);

					if (settings::vert_flyhack >= 2.9f)
						ov = Vector3(ov.x, curr.y < old.y ? (curr.y - 0.3f) : old.y, ov.z);
					if (settings::hor_flyhack >= 6.f)
						ov = Vector3(old.x, ov.y, old.z);
					
					if (settings::vert_flyhack >= 2.8f) {
						if (ov != curr) //disallow positive movement on Y
						{
							if (ov.y > 0) {
								player_walk_movement->teleport_to(ov, baseplayer);
							}
						}
					}
					if (settings::hor_flyhack >= 6.f) {
						if (ov != curr) //disallow movement on X,Z
						{
							if (ov.x != 0
								|| ov.z != 0) {
								player_walk_movement->teleport_to(ov, baseplayer);
							}
						}
					}

					//if (settings::vert_flyhack >= 2.5f
					//	|| settings::hor_flyhack >= 6.f) {
					//	if (ov != curr)
					//		player_walk_movement->teleport_to(ov, baseplayer);
					//	dont_move = true;
					//}
				}
			}

			auto trans = baseplayer->transform();
			if (trans)
			{
				if (vars->visual.movementline) {
					if (!trans->position().is_empty()
						&& !misc::cLastTickPos.is_empty())
						Line(trans->position(), misc::cLastTickPos, { r * 100, g * 100, b * 100, 1 }, 5.f, false, false);
				}
				auto eyes = baseplayer->eyes();
				if (eyes) {
					misc::cLastTickEyePos = eyes->position();//get_transform(baseplayer)->get_bone_position();//baseplayer->eyes()->get_position();
					misc::cLastTickPos = trans->position();//_get_transform(baseplayer)->get_position();//get_transform(baseplayer)->get_bone_position();//baseplayer->eyes()->get_position();
					misc::ticks.AddPoint(misc::cLastTickPos);
					misc::ServerUpdate(get_deltaTime(), baseplayer);
				}
			}
		}
		else if (!baseplayer || baseplayer->is_sleeping())
		{
			settings::vert_flyhack = 0.f; settings::hor_flyhack = 0.f;
		}

		if (baseplayer && !baseplayer->is_sleeping())
		{
			float max_speed = (player_walk_movement->get_swimming() || player_walk_movement->get_ducking() > 0.5) ? 1.7f : 5.5f;

			set_sprinting(model_state, true);
			flying = player_walk_movement->get_flying();

			//if (vars->misc.silentwalk && vars->keybinds.silentwalk) {
			//	set_onLadder(model_state, true);
			//}
			//else set_onLadder(model_state, false);

			if (vars->misc.interactive_debug)
				model_state->set_flag(ModelState_Flag::Mounted);


			if (vars->misc.always_sprint) {
				if (vel.length() > 0.f) {
					Vector3 target_vel = Vector3(vel.x / vel.length() * max_speed, vel.y, vel.z / vel.length() * max_speed);
					player_walk_movement->set_TargetMovement(target_vel);
				}
			}

			if (vars->misc.infinite_jump)
				if (unity::GetKeyDown(KeyCode::Space))
					misc::autobot::do_jump(player_walk_movement, model_state);

			auto wpn = baseplayer->GetActiveItem();
			auto held = wpn ? wpn->GetHeldEntity<BaseProjectile>() : nullptr;

			if (vars->combat.always_reload
				&& held)
			{
				auto cap = held->ammo_cap();
				auto ammo = held->ammo_left();
				if (ammo < cap
					&& !misc::started_reload
					&& misc::did_reload
					&& misc::time_since_last_shot < 0.2f
					&& held->primaryMagazine()->CanReload(baseplayer))
				{
					misc::started_reload = true;
					misc::did_reload = false;
					misc::time_since_last_shot = 0.21f;
					misc::fixed_time_last_shot = get_fixedTime() - 0.2f;
					misc::just_shot = true;
				}

				misc::time_since_last_shot = (get_fixedTime() - misc::fixed_time_last_shot);
				vars->time_since_last_shot = misc::time_since_last_shot;
				if (misc::just_shot && (misc::time_since_last_shot > 0.2f))
				{
					held->ServerRPC(_(L"StartReload"));
					baseplayer->console_echo(_(L"[matrix]: ClientInput - starting reload"));
					//baseplayer->SendSignalBroadcast(Signal::Reload); //does this cause animation? YES
					misc::just_shot = false;
					misc::started_reload = false;
					misc::last_reload_time = get_fixedTime();
				}


				float reloadtime = *reinterpret_cast<float*>((uintptr_t)held + 0x2B8);//held->reloadTime();

				if (misc::time_since_last_shot > reloadtime//-10% for faster reloads than normal >:)
					&& !misc::did_reload)
				{
					baseplayer->console_echo(_(L"[matrix]: ClientInput - finishing reload"));
					held->ServerRPC(_(L"Reload"));
					misc::did_reload = true;
					misc::time_since_last_shot = 0;
					misc::fixed_time_last_shot = 0;
				}
			}
			else
			{
				misc::time_since_last_shot = 0;
			}
			if (!baseplayer->is_sleeping()) {

				//auto nm = il2cpp::methods::object_new(il2cpp::init_class(_("GameObject"), _("UnityEngine")));
				//create(nm, _(L""));
				//if (vars->misc.autofarm
				//	&& !misc::hasNavigator) {
				//	misc::hasNavigator = true;
				//	auto comp = (BaseNavigator*)add_component((uintptr_t)((Networkable*)baseplayer)->GetComponent<GameObject>(unity::GetType(_("UnityEngine"), _("GameObject"))),
				//		il2cpp::type_object(_(""), _("BaseNavigator")));
				//	if (misc::node.ent == 0)
				//		misc::node.ent = (BaseEntity*)baseplayer->find_closest(_("OreResourceEntity"), (Networkable*)baseplayer, 200.f);
				//	comp->Destination() = misc::node.ent->get_transform()->get_position();
				//}

				switch (vars->misc.walkto)
				{
				case 0: //None	
					misc::node.ent = 0;
					misc::node.path.clear();
					misc::node.pos = Vector3(0, 0, 0);
					misc::node.steps = 0;
					break;
				case 1: //Marker
				{
					auto note = baseplayer->ClientCurrentMapNote();
					if (note) {
						auto pos = baseplayer->transform()->position();
						Vector3 marker_pos = note->worldPosition;
						if (marker_pos.is_empty())
							break;
						misc::node.pos = marker_pos;
						misc::autobot::pathfind(player_walk_movement, marker_pos);
					}
					break;
				}
				case 2: //Stone
					misc::autobot::auto_farm(player_walk_movement, _("OreResourceEntity"), _("stone"));
					break;
				case 3: //Sulfur
					misc::autobot::auto_farm(player_walk_movement, _("OreResourceEntity"), _("sulfur"));
					break;
				case 4: //Metal
					misc::autobot::auto_farm(player_walk_movement, _("OreResourceEntity"), _("metal"));
					break;
				case 5: //Player
					misc::autobot::walktoplayer(player_walk_movement, vars->misc.playerselected);
					break;
				case 6: //Barrels
					misc::autobot::auto_farm(player_walk_movement, _(""), _("barrel"));
					break;
				}
			}

			//maybe find max amount of velocity we can have before speedhack violations kick in?
			if (vars->misc.antispeed)
			{
				Vector3 current_velocity = player_walk_movement->get_TargetMovement();

				float mul = misc::speedhackDistance + 4;

				if (mul > .1f && misc::speedhackCooldownEnd < get_fixedTime())//start cooldown
				{
					misc::lastSpeedhackReset = get_fixedTime();
					misc::speedhackCooldownEnd = get_fixedTime() + 20.f;
				}
				else //still on cooldown
				{
					auto timeleft = misc::speedhackCooldownEnd - get_fixedTime(); //will be 10
					//auto remainder = (timeleft - 10.f) < 0 ? 0 : (timeleft - 10.f);
					auto remainder = timeleft > 20 ? 20 : (timeleft < 0 ? 0 : timeleft);
					Vector3 new_vel = current_velocity;
					float fslowdown = (mul / 1.f);
					if (mul > .3f)
					{
						//auto fslowdown = (mul / 3.f);//percent to slow down
						fslowdown = (mul / 0.5f);//percent to slow down
						std::clamp(fslowdown, 0.f, 1.f);
						Vector3 new_vel = Vector3(current_velocity.x * (2.f - fslowdown), current_velocity.y, current_velocity.z * (2.f - fslowdown));
					}
					else new_vel = Vector3(current_velocity.x * (2.f - (remainder / 20.f)), current_velocity.y, current_velocity.z * (2.f - (remainder / 20.f)));

					player_walk_movement->set_TargetMovement(new_vel);
				}
			}

			if (dont_move)
				player_walk_movement->set_TargetMovement(Vector3(0, 0, 0));

			if (vars->misc.flywall)
			{
				if (unity::GetKey(vars->keybinds.flywall))
				{
					if (settings::vert_flyhack > 2.f
						|| settings::hor_flyhack > 4.f)
						last_fwd_fly = get_fixedTime();

					if (last_fwd_fly + 0.3f > get_fixedTime())
					{
						auto p = ((baseplayer->eyes()->position() + baseplayer->eyes()->body_forward()) - baseplayer->eyes()->position()).Normalized();
						p = p * 10.f; p.y = 0;
						player_walk_movement->set_TargetMovement(p);



						//maybe check here for a place i can fly into within 1m left right fwd back up
					}
					else {
						//check if there is obstruction above, if so, move backwards, else move upwards
						auto headpos = baseplayer->eyes()->position();
						auto height = baseplayer->movement()->capsule()->GetHeight();
						auto _r = baseplayer->eyes()->body_right();
						auto _f = baseplayer->eyes()->body_forward();
						auto abovepos = headpos; abovepos.y += .9f;
						auto rightpos = abovepos + _r; rightpos.y += .9f;
						auto leftpos = abovepos - _r; leftpos.y += .9f;
						auto fwdpos = abovepos + _f; fwdpos.y += .9f;
						auto backpos = abovepos - _f; backpos.y += .9f;

						auto r = baseplayer->movement()->capsule()->GetRadius();

						headpos.y -= 1.f;
						bool blocked_above = unity::CheckCapsule(
							abovepos,
							headpos,
							.4f,
							1503731969,
							2);
						bool blocked_right = unity::CheckCapsule(
							rightpos,
							Vector3(rightpos.x, rightpos.y -= 1.3f, rightpos.z),
							.4f,
							1503731969,
							2);
						bool blocked_left = unity::CheckCapsule(
							leftpos,
							Vector3(leftpos.x, leftpos.y -= 1.3f, leftpos.z),
							.4f,
							1503731969,
							2);
						bool blocked_fwd = unity::CheckCapsule(
							fwdpos,
							Vector3(fwdpos.x, fwdpos.y -= 1.3f, fwdpos.z),
							.4f,
							1503731969,
							2);
						bool blocked_back = unity::CheckCapsule(
							backpos,
							Vector3(backpos.x, backpos.y -= 1.3f, backpos.z),
							.4f,
							1503731969,
							2);

						//if (blocked_right) Capsule(rightpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 1, 0, 0, 1 }, .01f, false);
						//else Capsule(rightpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 0, 1, 0, 1 }, .01f, false);
						//
						//if (blocked_left) Capsule(leftpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 1, 0, 0, 1 }, .01f, false);
						//else Capsule(leftpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 0, 1, 0, 1 }, .01f, false);
						//
						//if (blocked_fwd) Capsule(fwdpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 1, 0, 0, 1 }, .01f, false);
						//else Capsule(fwdpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 0, 1, 0, 1 }, .01f, false);
						//
						//if (blocked_back) Capsule(backpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 1, 0, 0, 1 }, .01f, false);
						//else Capsule(backpos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 0, 1, 0, 1 }, .01f, false);

						if (blocked_above) {
							//Capsule(abovepos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 1, 0, 0, 1 }, .01f, false);

							if (blocked_back) {
								if (blocked_right) {
									if (blocked_left) {
										if (blocked_fwd) {
											//blocked in all directions.................... ???????????????????
										}
										else {
											//not blocked on fwd above so move fwd
											auto p = (fwdpos - baseplayer->eyes()->position()).Normalized();
											p = p * 5.f; p.y = 0;
											player_walk_movement->set_TargetMovement(p);
										}
									}
									else {
										//not blocked on left above so move left
										auto p = (leftpos - baseplayer->eyes()->position()).Normalized();
										p = p * 5.f; p.y = 0;
										player_walk_movement->set_TargetMovement(p);
									}
								}
								else {
									//not blocked on right above so move right
									auto p = (rightpos - baseplayer->eyes()->position()).Normalized();
									p = p * 5.f; p.y = 0;
									player_walk_movement->set_TargetMovement(p);
								}
							}
							else {
								//not blocked on back above so move back
								auto p = (backpos - baseplayer->eyes()->position()).Normalized();
								p = p * 5.f; p.y = 0;
								player_walk_movement->set_TargetMovement(p);
							}
						}
						else {
							//Capsule(abovepos, baseplayer->eyes()->rotation(), .4f, 1.3f, { 0, 1, 0, 1 }, .01f, false);
							player_walk_movement->set_TargetMovement(Vector3(0, 5, 0));
						}
					}
				}
			}
		}
	}

	void hk_projectile_launchprojectile(BaseProjectile* pr)
	{
		auto held = vars->local_player->GetActiveItem()->GetHeldEntity<BaseProjectile>();
		if (vars->combat.doubletap
			&& !vars->combat.rapidfire
			&& held)
		{
			auto m = held->repeatDelay() * .75f; //we can shoot 25% faster??? more bullets?? :DDD
		
			int r = vars->desyncTime / m;
			if (r > 1)
			{
				vars->local_player->console_echo(string::wformat(_(L"[matrix]: Launching %d projectiles!"), r));
				for (size_t i = 0; i < r; i++)
				{
					orig::baseprojectile_launchprojectile((uintptr_t)pr);
					held->remove_ammo();
				}
				return;
			}
		}
		//orig::baseprojectile_launchprojectile((uintptr_t)pr);
		if (misc::manual || misc::autoshot) {
			held->remove_ammo();
			auto viewmodel = get_activemodel();
			viewmodelplay((uintptr_t)held->viewModel(), _(L"attack"), 0);

			misc::manual = false;
			misc::autoshot = false;
		}
		//
		//if (vars->combat.shoot_at_fatbullet) {
		//	auto p = (Projectile*)pr;
		//	auto pos = p->transform()->position();
		//	auto vel = p->currentVelocity();
		//	auto nextupdatepos = pos;
		//	auto nextupdatevel = vel;
		//
		//	Vector3 lastposition;
		//	for (float travelTime = p->traveledTime(); travelTime < 8.f; travelTime += 0.03125f)
		//	{
		//		nextupdatepos += nextupdatevel * 0.03125f;
		//		if (!unity::is_visible(nextupdatepos, lastposition, 0))
		//			break;
		//		if(!lastposition.is_empty())
		//			Line(nextupdatepos, lastposition, { 1, 1, 1, 1 }, 5.f, false, false);
		//
		//		if (nextupdatepos.distance(vars->best_target.pos) <= 1.f)
		//			Line(nextupdatepos, vars->best_target.pos, { 0, 1, 0, 1 }, 5.f, false, false);
		//
		//		nextupdatevel.y -= 9.81f * p->gravityModifier() * 0.03125f;	
		//		nextupdatevel -= nextupdatevel * p->drag() * 0.03125f;
		//		lastposition = nextupdatepos;
		//	}
		//
		//}

		return orig::baseprojectile_launchprojectile((uintptr_t)pr);
	}

	void hk_projectile_update(uintptr_t pr) {
		//printf("projectile update called\n");
		if (vars->combat.pierce) {
			auto p = (Projectile*)pr;
			p->integrity(999.f);
		}
		if (launchedmelee) {
			((Projectile*)pr)->currentVelocity(tempmelvel);
			((Projectile*)pr)->currentPosition(tempmelpos);
			((Projectile*)pr)->transform()->setposition(tempmelpos);

			((Projectile*)pr)->transform()->SetLocalScale(((Projectile*)pr)->transform()->GetLocalScale() * vars->visual.visthickness);
			((BaseProjectile*)pr)->set_projectile_thickness(vars->combat.thickness);
			tempmelvel = {};
			tempmelpos = {};
			launchedmelee = false;
			return _update((Projectile*)pr);
		}
		if (vars->combat.thickness > 1.f) {
			for (;;) {
				auto p = (Projectile*)pr;
				if (!p->hitTest()) break;
				if (p->penetrationPower() <= .5f) break;
				auto pos = p->currentPosition();
				auto target = vars->best_target.pos;
				Vector3 vel = p->currentVelocity();
				Vector3 dir = vel.normalize();
				Vector3 traveledThisUpdate = vel * (get_deltaTime() * 2.f);
				Vector3 newpos = pos + traveledThisUpdate;
				Line(pos, newpos, { r, g, b, 1 }, 10.f, true, true);
				float offset = 0.f;
				float thick = vars->combat.thickness;

				if (misc::LineCircleIntersection(target, (thick > 1.f ? thick : 1.f), pos, newpos, offset)) {
					for (float i = 0; i < 1.f; i += 0.01f) {
						auto n = pos.Lerp(newpos, i);
						auto dist = n.distance(target);
						if (dist < (thick > 1.f ? thick : 1.f)
							&& unity::is_visible(n, target, 0)) {
							//calculate velocity, traveltime at this point
							float travel = 0.f;
							const float step = 0.003;
							auto vpos = pos;
							auto vvel = vel;
							for (; travel < 0.3f; travel += step) {
								vpos += vvel * step;
								vvel.y -= 9.81f * p->gravityModifier() * step;
								vvel -= vvel * p->drag() * step;
								if (vpos.distance(target) < thick) {
									//p->transform()->setposition(vpos);
									Sphere(vpos, .5f, { r, g, b, 1 }, 10.f, false);
									Line(pos, vpos, { 0, 1, 1, 1 }, 10.f, true, true);
									Line(vpos, target, { 0, 1, 0, 1 }, 10.f, true, true);
									
									g_UpdateReusable = Projectile1::CreatePlayerProjectileUpdate();
									((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->position = vpos;
									((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->projectileID = p->projectileID();
									((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->traveltime = p->traveledTime() + travel;
									((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->velocity = vvel;
									vars->local_player->SendProjectileUpdate((uintptr_t)g_UpdateReusable);

									auto hitpos = vpos;
									if (vpos.distance(target) > 1.f)
										hitpos.MoveTowards(target, hitpos, thick - 1.f);
									p->transform()->setposition(hitpos);
									auto bonetrans = ((BasePlayer*)vars->best_target.ent)->get_bone_Transform(48);
									HitTest* ht = (HitTest*)p->hitTest();
									ht->DidHit() = true;
									ht->HitEntity() = vars->best_target.ent;
									ht->HitTransform() = bonetrans;
									ht->HitPoint() = bonetrans->InverseTransformPoint(hitpos);
									ht->HitNormal() = bonetrans->InverseTransformDirection(hitpos);
									Ray r(p->transform()->position(), hitpos);
									safe_write(ht + 0x14, r, Ray);
									_DoHit(p, ht, vpos, HitNormalWorld((uintptr_t)ht));
									break;
								}
							}
						}
					}
				}
				break;
			}
		}
		if (vars->combat.throughwall)
		{
			for (size_t i = 0; i <= 0; i++)
			{
				auto p = (Projectile*)pr;
				if (!p->hitTest()) break;
				if (p->penetrationPower() <= .5f) break;

				auto headtrans = ((BasePlayer*)vars->best_target.ent)->get_bone_Transform(48);
				if (!headtrans) break;
				Vector3 vel = p->currentVelocity();
				Vector3 dir = vel.normalize();
				Vector3 traveledThisUpdate = vel * get_deltaTime();

				DWORD64 sistat = il2cpp::init_class(_("RaycastHit"), _("UnityEngine"));
				RaycastHit* hitInfo = (RaycastHit*)il2cpp::methods::object_new(sistat);
				if (!hitInfo) break;
				//RaycastHit* hitInfo;

				//Sphere(p->currentPosition(), .05f, { 1, 1, 1, 1 }, 10.f, false);

				if (!GamePhysics::Trace(
					Ray(p->currentPosition(), dir),
					0.f,
					hitInfo,
					traveledThisUpdate.length() + 1.f,
					8388608 | 2097152 | 65536, 0))
					break;

				Line(hitInfo->point(), p->currentPosition(), { 1, 0, 0, 1 }, 10.f, false, false);
				Line(hitInfo->point(), vars->best_target.pos, { 1, 0, 0, 1 }, 10.f, false, false);

				if (!g_UpdateReusable)
					g_UpdateReusable = Projectile1::CreatePlayerProjectileUpdate();

				//_update((Projectile*)pr);

				auto ppu = (protobuf::PlayerProjectileUpdate*)g_UpdateReusable;

				auto newpos = hitInfo->point() - (dir * .1f);
				ppu->projectileID = p->projectileID();
				ppu->traveltime = p->traveledTime() + p->currentPosition().distance(hitInfo->point()) / traveledThisUpdate.length();
				ppu->position = newpos;
				ppu->velocity = p->currentVelocity();
				vars->local_player->SendProjectileUpdate((uintptr_t)ppu);
				//orig_fn(rcx, rdx, r9, _ppa, arg5);

				auto trans = vars->best_target.ent->transform();//((BasePlayer*)vars->best_target.ent)->get_bone_Transform(48);
				if (!trans) break;
				float dist = p->currentPosition().distance(headtrans->position());

				p->traveledDistance(p->traveledDistance() + dist);
				p->traveledTime(p->traveledTime() + (dist / p->currentVelocity().Length()));

				auto ht = (HitTest*)p->hitTest();

				p->integrity(1);
				Sphere(hitInfo->point(), 0.02f, { 1, 1, 1, 1 }, 10.f, false);
				Sphere(ppu->position, 0.02f, { 0, 1, 0, 1 }, 10.f, false);
				Sphere(newpos, 0.02f, { 0, 0, 1, 1 }, 10.f, false);

				auto btrans = hitInfo->transform();

				ht->set_hit_Transform(headtrans);
				ht->set_hit_entity((BasePlayer*)vars->best_target.ent);
				ht->set_attack_ray(Ray(hitInfo->point(), dir.Normalized()));
				auto v = btrans->InverseTransformPoint(headtrans->position());
				Sphere(v, 0.1f, { 0, 1, 1, 1 }, 10.f, false);
				ht->set_hit_point(v);
				ht->set_hit_normal(Vector3(0, 0, 0));
				ht->set_did_hit(true);
				ht->set_hit_distance(dist);
				ht->set_max_distance(999.f);

				//ht->HitTransform() = trans;
				//ht->HitEntity() = vars->best_target.ent;
				//ht->AttackRay() = ;
				//ht->HitPoint() = trans->InverseTransformPoint(trans->position());
				//ht->HitNormal() = Vector3(0, 0, 0);
				//ht->DidHit() = true;
				//ht->HitDistance() = dist;
				//ht->MaxDistance() = 999.f;
				_DoHit(p, ht, headtrans->position(), Vector3(0, 0, 0));
				g_UpdateReusable = 0;
			}
		}
		if (vars->combat.targetbehindwall) {
			//return _update((Projectile*)pr);
			((Projectile1*)pr)->Update();
		}
		else
			return _update((Projectile*)pr);
	}

	void hk_performance_update(void* instance) {
		if (wake) {
			__go = il2cpp::methods::object_new(il2cpp::init_class(_("GameObject"), _("UnityEngine")));
			create(__go, _(L""));
			add_component(__go, il2cpp::type_object(_(""), _("DevControls")));
			dont_destroy_on_load(__go);
			wake = false;
		}
		PerformanceUI_Update(instance);
	}

	void fakeorig_clientinput(BasePlayer* self, InputState* state) {
		//printf("clientinput recreation start\n");
		orig::pifu(self->input());
		self->modelState()->setaiming(false);
		self->modelState()->setducked(false);
		self->modelState()->setsprinting(false);
		self->modelState()->setjumped(false);
		self->modelState()->set_water_level(self->WaterFactor());
		self->voiceRecorder()->ClientInput(state);
		//printf("clientinput recreation middle\n");
		if (self->HasLocalControls() && !NeedsKeyboard::AnyActive()) {
			self->Belt()->ClientInput(state);
		}
		else {
			UIInventory::Close();
			MapInterface::SetOpen(false);
		}
		if (self->Frozen()) return;
		self->HeldEntityInput();
		//TimeWarning::New(_(L"movement.ClientInput"), 0.1f);
		self->movement()->ClientInput(state, self->modelState());
		hk_playerwalkmovement_ClientInput(self->movement(), (uintptr_t)input, self->modelState());
			//if ()
				//self->movement()->ClientInput(state, self->modelState());
		//TimeWarning::New(_(L"UseAction"), 0.1f);
		self->UseAction(state);
		if (Buttons::Chat()->IsDown)
		{
			typedef void(*AA)();
			((AA)(mem::game_assembly_base + oChatOpen))();
		}

		//typedef void(*AA)(bool);
		//((AA)(mem::game_assembly_base + oSetGestureMenuOpen))(Buttons::Gestures()->IsDown);

		//TimeWarning::New(_(L"MapInterface Update"), 0.1f);
		MapInterface::SetOpen(Buttons::Map()->IsDown);

		//printf("clientinput recreation end\n");
		return;
		//rust.gc.reset(1f);
		//using (TimeWarning.New("MapInterface Update", 0.1f))
		//{
		//	MapInterface.DoPlayerUpdate();
		//	MapInterface.SetOpen(Buttons.Map.IsDown);
		//}
	}

	float o_capsuleHeight = 0;
	float o_capsuleCenter = 0;
	float o_capsuleHeightDucked = 0;
	float o_capsuleCenterDucked = 0;
	float o_capsuleradius = 0;

	bool wireplaced = false;

	void hk_baseplayer_ClientInput(BasePlayer* baseplayer, InputState* state)
	{
		//printf("clientinput start\n");
		int echocount = 0;
		//printf("clientinput start\n");
		if (!hooks::client_input_ptr)
			hooks::client_input_ptr = mem::hook_virtual_function(_("PlayerWalkMovement"), _("ClientInput"), &hooks::hk_playerwalkmovement_ClientInput);

		//if(!do_fixed_update_ptr)
			//do_fixed_update_ptr = mem::hook_virtual_function(_("PlayerWalkMovement"), _("DoFixedUpdate"), &hk_dofixedupdate);

		//printf("clientinput\n");
		//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

		//static uintptr_t* serverrpc_projecshoot = 0;
			//if (!serverrpc_projecshoot) {
			//	auto method_serverrpc_projecshoot = *reinterpret_cast<uintptr_t*>(hooks::serverrpc_projecileshoot); //Method$BaseEntity.ServerRPC<ProjectileShoot>()
			//	typedef void(*CALL)(uintptr_t*, int64_t);
			//	((CALL)(mem::game_assembly_base + 0x2D9300))(&method_serverrpc_projecshoot, 0);
			//	if (method_serverrpc_projecshoot) {
			//		serverrpc_projecshoot = **(uintptr_t***)(method_serverrpc_projecshoot + 0x30);
			//
			//		hooks::orig::serverrpc_projectileshoot = *serverrpc_projecshoot;
			//
			//		*serverrpc_projecshoot = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_projectileshoot);
			//	}
			//}
			//static uintptr_t* serverrpc_projecattack = 0;
			//if (!serverrpc_projecattack) {
			//	auto method_serverrpc_projecattack = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileAttack___); //Method$BaseEntity.ServerRPC<ProjectileShoot>()
			//	typedef void(*CALL)(uintptr_t*, int64_t);
			//	((CALL)(mem::game_assembly_base + 0x2D9300))(&method_serverrpc_projecattack, 0);
			//	if (method_serverrpc_projecattack) {
			//		serverrpc_projecattack = **(uintptr_t***)(method_serverrpc_projecattack + 0x30);
			//
			//		hooks::orig::playerprojectileattack = *serverrpc_projecattack;
			//
			//		*serverrpc_projecattack = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_playerprojectileattack);
			//	}
			//}
			//
			//static uintptr_t* serverrpc_projecupdate = 0;
			//if (!serverrpc_projecupdate) {
			//	auto method_serverrpc_projecupdate = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___); //Method$BaseEntity.ServerRPC<ProjectileShoot>()
			//	typedef void(*CALL)(uintptr_t*, int64_t);
			//	((CALL)(mem::game_assembly_base + 0x2D9300))(&method_serverrpc_projecupdate, 0);
			//	if (method_serverrpc_projecupdate) {
			//		serverrpc_projecupdate = **(uintptr_t***)(method_serverrpc_projecupdate + 0x30);
			//
			//		hooks::orig::playerprojectileupdate = *serverrpc_projecupdate;
			//
			//		*serverrpc_projecupdate = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_playerprojectileupdate);
			//	}
			//}

		if (!has_intialized_methods) {
			auto il2cpp_codegen_initialize_method = reinterpret_cast<void (*)(unsigned int)>(il2cpp::methods::intialize_method);
			//56229 for real rust or 56204 for cracked rust
			for (int i = 0; i <
				56229//56204 //56229 = real rust
				; i++) {
				il2cpp_codegen_initialize_method(i);
			}
			has_intialized_methods = true;
		}

#pragma region static_method_hooks
		static uintptr_t* serverrpc_createbuilding;
		if (!serverrpc_projecshoot) {
			//auto method_serverrpc_projecshoot = *reinterpret_cast<uintptr_t*>(hooks::serverrpc_projecileshoot);
			auto method_serverrpc_projecshoot = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileShoot___);

			if (method_serverrpc_projecshoot) {
				serverrpc_projecshoot = **(uintptr_t***)(method_serverrpc_projecshoot + 0x30);

				hooks::orig::serverrpc_projectileshoot = *serverrpc_projecshoot;

				*serverrpc_projecshoot = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_projectileshoot);
			}
		}
		if (!serverrpc_playerprojectileattack) {
			auto method_serverrpc_playerprojectileattack = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileAttack___);//Method$BaseEntity_ServerRPC_PlayerProjectileAttack___
		
			if (method_serverrpc_playerprojectileattack) {
				serverrpc_playerprojectileattack = **(uintptr_t***)(method_serverrpc_playerprojectileattack + 0x30);
		
				hooks::orig::playerprojectileattack = *serverrpc_playerprojectileattack;
		
				*serverrpc_playerprojectileattack = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_playerprojectileattack);
			}
		}
		//if (!serverrpc_playerprojectileupdate) {
		//	auto method_serverrpc_playerprojectileupdate = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileUpdate___);//Method$BaseEntity_ServerRPC_PlayerProjectileAttack___
		//
		//	if (method_serverrpc_playerprojectileupdate) {
		//		serverrpc_playerprojectileupdate = **(uintptr_t***)(method_serverrpc_playerprojectileupdate + 0x30);
		//
		//		hooks::orig::playerprojectileupdate = *serverrpc_playerprojectileupdate;
		//
		//		*serverrpc_playerprojectileupdate = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_playerprojectileupdate);
		//	}
		//}
		if (!serverrpc_createbuilding) {
			auto method_serverrpc_createbuilding = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_CreateBuilding___);
		
			if (method_serverrpc_createbuilding) {
				serverrpc_createbuilding = **(uintptr_t***)(method_serverrpc_createbuilding + 0x30);
		
				hooks::orig::createbuilding = *serverrpc_createbuilding;
		
				*serverrpc_createbuilding = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_doplace);
			}
		}
#pragma endregion

		orig::baseplayer_client_input(baseplayer, state);


		if (baseplayer) {
			baseplayer->modelState()->remove_flag(ModelState_Flag::Flying);

			auto mountable = baseplayer->GetMounted();
			auto tick_time = baseplayer->lastSentTickTime();
			vars->desyncTime = (unity::get_realtimesincestartup() - tick_time) - 0.03125 * 3;
			auto fixed_time = get_fixedTime();
			get_skydome();

			auto wpn = baseplayer->GetActiveItem();
			auto held = wpn ? wpn->GetHeldEntity<BaseProjectile>() : nullptr;
			float time = get_fixedTime();

			auto item2 = baseplayer->get_active_weapon();
			bool IsMounted = baseplayer->GetMounted() ? 1 : 0;
			float maxVelocity = get_maxspeed(baseplayer);
			if (IsMounted)
				maxVelocity *= 3;
			float _timeSinceLastTick = unity::get_realtimesincestartup() - baseplayer->lastSentTickTime();
			float timeSinceLastTickClamped = max(0.f, min(_timeSinceLastTick, 1.f));
			float mm_eye = 0.1f + (timeSinceLastTickClamped + 2.f / 60.f) * 1.5f * maxVelocity;

			//desync on key
			if (unity::GetKey(vars->keybinds.desync_ok)
				|| ((vars->combat.manipulator2 || vars->combat.manipulator)
					&& (unity::GetKey(vars->keybinds.manipulator)
						|| misc::manipulate_vis)))
				baseplayer->clientTickInterval() = .99f;
			else if (!is_lagging && !is_speeding)
				baseplayer->clientTickInterval() = 0.05f;

			//printf("%.2f", baseplayer->modelState()->get_water_level());

			if (vars->best_target.ent && held && wpn)
			{
				//old manip
				if (vars->combat.manipulator && ((unity::GetKey(vars->keybinds.manipulator))
					|| misc::manipulate_vis))
				{
					Vector3 target = vars->best_target.pos;
					auto mag_ammo = held->ammo_left();
					if (!strcmp(held->get_class_name(), _("BaseMelee"))
						&& vars->combat.psilentmelee)
						mag_ammo = 1;
					float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
					if (misc::can_manipulate(baseplayer, target, mm_eye))
						if (nextshot < time
							&& (held->timeSinceDeploy() > held->deployDelay() ||
								!strcmp(held->get_class_name(), _("BowWeapon")) ||
								!strcmp(held->get_class_name(), _("CompoundBowWeapon")) ||
								!strcmp(held->get_class_name(), _("CrossbowWeapon")))
							&& mag_ammo > 0)
						{
							auto v = baseplayer->eyes()->position() + misc::best_lean;
							baseplayer->console_echo(string::wformat(_(L"[matrix]: ClientInput - manipulator attempted shot from position (%d, %d, %d) with desync of %d"), (int)v.x, (int)v.y, (int)v.z, (int)(vars->desyncTime * 100.f)));

							misc::manual = true;
							if (!strcmp(held->get_class_name(), _("BaseMelee"))
								&& vars->combat.psilentmelee)
								((BaseMelee*)held)->DoThrow();
							else {
								hk_projectile_launchprojectile(held);
							}
							misc::best_target = Vector3(0, 0, 0);
							baseplayer->SendClientTick();
						}
				}
				else if ((vars->combat.manipulator2
					&& vars->best_target.ent->is_alive()
					&& !(!GetAsyncKeyState(vars->keybinds.manipulator2)))
					|| misc::manipulate_vis)
				{
					Vector3 target = vars->best_target.pos;
					auto mag_ammo = held->ammo_left();
					if (wpn && held && vars->best_target.ent) {
						float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
						if (CanManipulate(held, (BasePlayer*)vars->best_target.ent, baseplayer->input()->state()))
							if (nextshot < time
								&& (held->timeSinceDeploy() > held->deployDelay() ||
									!strcmp(held->get_class_name(), _("BowWeapon")) ||
									!strcmp(held->get_class_name(), _("CompoundBowWeapon")) ||
									!strcmp(held->get_class_name(), _("CrossbowWeapon")))
								&& held->ammo_left() > 0)
							{
								auto v = settings::RealGangstaShit;//baseplayer->eyes()->get_position() + misc::best_lean;
								baseplayer->console_echo(string::wformat(_(L"[matrix]: ClientInput - manipulator attempted shot from position (%d, %d, %d) with desync of %d"), (int)v.x, (int)v.y, (int)v.z, (int)(vars->desyncTime * 100.f)));

								misc::manual = true;
								hk_projectile_launchprojectile(held);
								baseplayer->SendClientTick();
							}
					}
				}

				if (((vars->combat.autoshoot || unity::GetKey(vars->keybinds.autoshoot))
					&& vars->best_target.ent->is_alive())
					|| vars->wants_shoot)
				{
					Vector3 target = vars->best_target.pos;
					auto mag_ammo = held->ammo_left();
					float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
					if ((baseplayer->is_visible(target, baseplayer->model()->boneTransforms()->get(48)->position())
						&& get_fixedTime() > nextshot
						&& held->timeSinceDeploy() > held->deployDelay()
						&& mag_ammo > 0))
					{
						misc::autoshot = true;
						misc::autobot::wants_shoot = false;
						hk_projectile_launchprojectile(held);
						baseplayer->SendClientTick();
					}
					else if (!baseplayer->is_visible(baseplayer->model()->boneTransforms()->get(48)->position(), target)
						&& vars->combat.manipulator
						&& mag_ammo > 0)
					{
						if (misc::can_manipulate(baseplayer, target, 9.f))
						{//maybe check more later idk
							misc::manipulate_vis = true;
						}
						else misc::manipulate_vis = false;
					}
					vars->wants_shoot = false;
				}
				else misc::manipulate_vis = false;
			}
			else misc::manipulate_vis = false;

			//orig::baseplayer_client_input(baseplayer, state);
			if (vars->misc.autoknock) {
				auto d = baseplayer->resolve_closest_entity(4);
				if (strcmp(d.first.ent->get_class_name(), _("Door"))) {
					d.first.ent->ServerRPC(_(L"RPC_KnockDoor"));
				}
			}

			if (mountable) {
				if (vars->combat.always_shoot) {
					mountable->canwielditem() = true;
				}
			}
			if (vars->misc.automini) {
				auto mountedvehicle = baseplayer->GetMountedVehicle();
				if(mountedvehicle) {
					auto maxspeed = vars->misc.minicfg.speed;
					auto target = vars->misc.minicfg.target;
					auto mode = vars->misc.minicfg.mode;
					auto height = vars->misc.minicfg.maxheight;

					if (!strcmp(mountedvehicle->get_class_name(), _("MiniCopter"))) {

						//if speed == 0 || max, disregard max speed
						//if mode == hover, hover in current position
						//if mode == flyto, fly to target at height & speed, hover above target when within 10m of X,Z

						auto mini = (MiniCopter*)mountedvehicle;

						printf("cachedPitch: %.2f\n", mini->cachedPitch());
						printf("cachedYaw: %.2f\n", mini->cachedYaw());
						printf("cachedRoll: %.2f\n", mini->cachedRoll());

						printf("get_rotation.x: %.2f\n", baseplayer->transform()->get_rotation().x);
						printf("get_rotation.y: %.2f\n", baseplayer->transform()->get_rotation().y);
						printf("get_rotation.z: %.2f\n", baseplayer->transform()->get_rotation().z);
						printf("get_rotation.w: %.2f\n", baseplayer->transform()->get_rotation().w);

						printf("rotorSpeed: %.2f\n", mini->rotorSpeed());
						printf("currentThrottle: %.2f\n", mini->currentThrottle());

						printf("targetmovement: (%.2f, %.2f, %.2f)\n",
							baseplayer->movement()->get_TargetMovement().x,
							baseplayer->movement()->get_TargetMovement().y,
							baseplayer->movement()->get_TargetMovement().z
						);

						printf("mouseDelta: (%.2f, %.2f, %.2f)\n",
							state->current()->mouseDelta().x,
							state->current()->mouseDelta().y,
							state->current()->mouseDelta().z
							);

						auto cmd = state->current()->mouseDelta();

						auto levelout = [&]() {
							auto current_pitch = baseplayer->transform()->get_rotation().z;
							auto current_yaw = baseplayer->transform()->get_rotation().y;
							auto current_roll = baseplayer->transform()->get_rotation().x;

							if (current_yaw > 0) {
								Buttons::SetLeft(false);
								Buttons::SetRight(true);
							} else if (current_yaw > 0) {
								Buttons::SetLeft(true);
								Buttons::SetRight(false);
							}

							if (current_pitch > 0) //tilt backwards
								state->current()->mouseDelta() = Vector3(cmd.x, cmd.y - .3f, cmd.z);
							else if (current_pitch < 0) //tilt forwards
								state->current()->mouseDelta() = Vector3(cmd.x, cmd.y + .3f, cmd.z);
							else state->current()->mouseDelta() = Vector3(0, 0, 0);
							
							if (current_roll > 0) //tilt right
								state->current()->mouseDelta() = Vector3(cmd.x - .3f, cmd.y, cmd.z);
							else if (current_roll < 0) //tilt left
								state->current()->mouseDelta() = Vector3(cmd.x + .3f, cmd.y, cmd.z);
							else state->current()->mouseDelta() = Vector3(0, 0, 0);
						};

						switch (mode) {
						case (int)FlyMode::Hover:
						{
							if (!mini->transform()) break;
							auto mpos = mini->transform()->position();
							auto targetpos = Vector3(mpos.x, vars->misc.minicfg.maxheight, mpos.z);

							if (mpos.y < targetpos.y) {
								Buttons::SetBackwards(false);
								Buttons::SetForward(true);
							}
							else {
								Buttons::SetForward(false);
								Buttons::SetBackwards(true);
							}

							if (mpos.distance(targetpos) < 1.f) {
								Buttons::SetBackwards(false);
								Buttons::SetForward(false);
							}

							if (vars->misc.minicfg.visualize) {
								Line(mpos, targetpos, { r * 100, g * 100, b * 100, 255 }, 0.01f, true, true);
								Capsule(mpos, mini->transform()->get_rotation(), 1.f, 1.f, { r, g, b, 255 }, 0.01f, true);
							}

							levelout();
							break;
						}
						case (int)FlyMode::ToTarget:
						{
							//record position
							//attempt to fly within 10m of position X,Z

							break;
						}
						case (int)FlyMode::None:
						{
							//reset recorded position
							Buttons::SetForward(0);
							Buttons::SetBackwards(0);
							Buttons::SetRight(0);
							Buttons::SetLeft(0);
							break;
						}
						}

					}
				}
			}

			if (unity::GetKey(0x37))//(GetAsyncKeyState(0x37))
			{
				auto look = baseplayer->_lookingAtEntity();
				if (look)
				{
					auto net2 = *reinterpret_cast<Networkable**>(look + 0x58);
					auto look_id = net2->get_id();
					vars->selected_entity_id = look_id;
				}
			}

			bool kyslol = false;

			if (vars->misc.TakeFallDamage && unity::GetKey(vars->keybinds.suicide)) {

				OnLand(baseplayer, -8.0001f - 100);
			}

			if (vars->misc.admin_mode)
				baseplayer->set_player_flag(PlayerFlags::IsAdmin);

			baseplayer->fov();

			typedef void(*_set_rayleigh)(float);
			((_set_rayleigh)(mem::game_assembly_base + oSetRayleigh))(vars->visual.rayleigh);

			if (vars->visual.customtime)
				Convar::Admin::Set_admintime(vars->visual.time);

			//RenderSettings::set_ambientIntensity(vars->visual.ambient);

			if (held) {
				if (!strcmp(held->get_class_name(), _("Planner"))) {
					auto planner = reinterpret_cast<Planner*>(held);
					if (unity::GetKeyDown(KeyCode::RightArrow))
					{
						auto v = planner->rotationoffset();
						planner->rotationoffset(Vector3(v.x, v.y += 10, v.z));
						baseplayer->console_echo(string::wformat(_(L"[matrix]: ClientInput - rotate building right (%d, %d, %d)"), (int)v.x, (int)v.y, (int)v.z));
					}
					else if (unity::GetKeyDown(KeyCode::LeftArrow))
					{
						auto v = planner->rotationoffset();
						planner->rotationoffset(Vector3(v.x, v.y -= 10, v.z));
						baseplayer->console_echo(string::wformat(_(L"[matrix]: ClientInput - rotate building left (%d, %d, %d)"), (int)v.x, (int)v.y, (int)v.z));
					}
				}

				if (vars->misc.wireplay) {
					if (unity::GetKey(vars->keybinds.wireplay)) {
						if (!strcmp(held->get_class_name(), _("WireTool"))) {
							if (!wireplaced) {
								auto playerpos = vars->best_target.pos;

								//typedef void(*A)(uintptr_t, Vector3);
								//((A)(mem::game_assembly_base + 0xB136C0))((uintptr_t)held, playerpos);

								playerpos.y += .5f;

								auto pendingline = mem::read<uintptr_t>((uintptr_t)held + 0x248); //private ClientIOLine pendingLine

								if (pendingline) {
									auto _line = mem::read<uintptr_t>(pendingline + 0x20);
									//set first position to playerpos or sum

									Vector3 p1 = playerpos + Vector3(1, 1, 0);
									Vector3 p2 = playerpos + Vector3(0, 0, 0);
									Vector3 p3 = playerpos + Vector3(0, 2, 0);
									Vector3 p4 = playerpos + Vector3(0, 0, 0);
									Vector3 p5 = playerpos + Vector3(-1, 1, 0);

									//auto createlist = [&]() {
									//	typedef uint64_t(__stdcall* PoolGet)(uint64_t);
									//	uint64_t update = *reinterpret_cast<uint64_t*>(mem::game_assembly_base + 57069704); //"Method$Facepunch.Pool.GetList<Vector3>"
									//	if (!update)
									//		return (System::list<Vector3>*)0;
									//	uint64_t p = ((PoolGet)(mem::game_assembly_base + poollist_offset))(update);
									//	if (!p)
									//		return (System::list<Vector3>*)0;
									//	return (System::list<Vector3>*)p;
									//};
									//
									//auto lst = createlist();
									//lst->add(0, p1);
									//lst->add(1, p2);
									//lst->add(2, p3);
									//lst->add(3, p4);
									//lst->add(4, p5);

									typedef void(*B)(uintptr_t, Vector3);
									((B)(mem::game_assembly_base + 0x9AB6A0))(pendingline, p1);
									((B)(mem::game_assembly_base + 0x9AB6A0))(pendingline, p2);
									((B)(mem::game_assembly_base + 0x9AB6A0))(pendingline, p3);
									((B)(mem::game_assembly_base + 0x9AB6A0))(pendingline, p4);
									((B)(mem::game_assembly_base + 0x9AB6A0))(pendingline, p5);

									//public void SetPosition(int index, Vector3 position) { }
									typedef void(*Z)(uintptr_t, int, Vector3);
									((Z)(mem::game_assembly_base + 0x1B69A00))(_line, 0, p5);
								}
								wireplaced = true;
							}
						}
						else wireplaced = false;
					}
					else wireplaced = false;
				}

				if (vars->misc.weaponspam) {
					if (unity::GetKey(vars->keybinds.weaponspam)) {
						held->SendSignalBroadcast(Signal::Attack);
					}
				}
				//skin changer stuff?
				if (vars->misc.skinchanger) {
					/*
					auto kl = mem::read<uintptr_t>(mem::game_assembly_base + 57026592);
					auto fieldz = mem::read<uintptr_t>(kl + 0xB8);
					auto all = mem::read<System::Array<Skinnable*>*>(fieldz);

					//wprintf(_(L"START\n"));
					auto sz = all->size();
					//for (int i = 0; i < sz; i++)

					//skinnable loop
					for (int i = 0; i < sz; i++)
					{
						auto c = all->get(i);
						if (c) {
							auto gp = mem::read<System::Array<SkinnableGroup*>*>((uintptr_t)c + 0x60);
							auto gpsz = gp->size();
							wprintf(_(L"---------------------(%d)---------------------\nName: "));
							wprintf(c->Name()->str);
							wprintf(_(L"\nObject name: "));
							wprintf(c->get_name()->str);
							wprintf(_(L"\nItemName: "));
							wprintf(c->ItemName()->str);
							wprintf(_(L"\nViewModelPrefabName: "));
							wprintf(c->ViewmodelPrefabName()->str);
							
							
							wprintf(_(L"\nMesh download paths:"));
							auto as = mem::read< System::Array<System::string*>*>((uintptr_t)c + 0x50);
							if (as) {
								for (int k = 0; k < as->size(); k++) {
									auto c2 = as->get(k);
									if (c2)
										wprintf(L"\t[%d]: %ls\n", k, c2->str);
								}
							}
							
							wprintf(_(L"\nEntityPrefabName: "));
							wprintf(c->EntityPrefabName()->str);
							wprintf(L"\nGROUP SIZE: %d\n", gpsz);
							for (int k = 0; k < gpsz; k++) {
								auto c2 = gp->get(k);
								if (c2) {
									auto name = c2->Name;
									if (wcscmp(name->str, _(L"MAIN"))) {
										wprintf(L"\t[%d] Name: %ls\n", k, name->str);
									}
								}
							}

							auto model = get_activemodel();
							auto renderers = ((Networkable*)model)->GetComponentsInChildren(unity::GetType(_("UnityEngine"), _("Renderer")));
							if (renderers)
							{
								auto sz = *reinterpret_cast<int*>(renderers + 0x18);

								for (int i = 0; i < sz; i++) {
									auto renderer = *reinterpret_cast<Renderer**>(renderers + 0x20 + i * 0x8);
									if (!renderer) return;
									if (c->_sourceMaterials->size() >= i)
										renderer->SetMaterial(c->_sourceMaterials->get(i));
									//renderer->SetMaterials(c->_sourceMaterials);
								}
							}

							wprintf(_(L"------------------------------------------\n"));
						}
					}
					*/
					//workshopskin cache loop //Rust_Workshop_WorkshopSkin_c*
					
					typedef Skin* (*A)(unsigned long);
					auto sk = ((A)(mem::game_assembly_base + 0x10587B0))(2249370680);

					typedef void* (*B)(Skin*, GameObject*);
					((B)(mem::game_assembly_base + 0x10471A0))(sk, held->GetComponent<GameObject>(unity::GetType(_("UnityEngine"), _("GameObject"))));
				
					auto model = get_activemodel();
					auto renderers = ((Networkable*)model)->GetComponentsInChildren(unity::GetType(_("UnityEngine"), _("Renderer")));
					if (renderers)
					{
						auto sz = *reinterpret_cast<int*>(renderers + 0x18);
						auto mats = sk->materials;
						auto matsz = mats->size();
						for (int i = 0; i < sz; i++) {
							auto renderer = *reinterpret_cast<Renderer**>(renderers + 0x20 + i * 0x8);
							if (!renderer) return;
							if (matsz >= i)
								renderer->SetMaterial(mats->get(i));
						}
					}
				}
				if (vars->misc.hammerspam) {
					if (unity::GetKey(vars->keybinds.hammerspam)
						&& !strcmp(held->get_class_name(), _("Hammer"))) {
						((BaseMelee*)held)->DoAttack();
					}
				}
				if (vars->visual.grenadeprediction) {
					auto w = ((ThrownWeapon*)held);
					auto eyepos = baseplayer->eyes()->position();
					auto normalized = baseplayer->eyes()->body_forward().normalize();
					auto d = 1.f;
					auto velocity = w->GetInheritedVelocity(baseplayer, normalized) + normalized * w->maxThrowVelocity() * d + baseplayer->GetWorldVelocity() * 0.5f;

					Vector3 p = eyepos;
					Vector3 lp = p;
					Vector3 vel = velocity;

					for (float travelTime = 0.f; travelTime < 8.f; travelTime += 0.03125f)
					{
						p += vel * 0.03125f;
						if (!unity::is_visible(p, lp, 0))
						{
							break;
							//bounce?
						}
						Line(p, lp, { r, g, b, 1 }, 0.01f, false, false);
						vel.y -= 9.81f * 1.f * 0.03125f;
						vel -= vel * 0.1f * 0.03125f;
						lp = p;
					}
					auto points = 36;
					float step = M_PI_2 / points;
					float x, z, c = 0;
					lp = Vector3::Zero();
					Sphere(p, 0.5f, { r, g, b, 1 }, 0.01f, false);

					for (auto pair : grenade_map) {
						auto current = pair.second;
						if (!current->positions.empty() && !current->endposition.is_empty() && !current->ply->is_sleeping()) {
							Vector3 lastpos = current->positions[0];
							for (auto v : current->positions) {
								Line(lastpos, v, { r, g, b, 1 }, 0.01f, false, false);
								lastpos = v;
							}

							p = current->endposition;
							Sphere(p, 0.5f, { r, g, b, 1 }, 0.01f, false);
						}
					}
				}
				if (vars->visual.rocketprediction
					&& vars->local_player->is_alive()) {
					if (!strcmp(held->get_class_name(), _("BaseLauncher"))) {
						auto mag = *reinterpret_cast<uintptr_t*>((uintptr_t)held + 0x2C0); //public BaseProjectile.Magazine primaryMagazine; // 0x2C0
						auto ammo = *reinterpret_cast<uintptr_t*>((uintptr_t)mag + 0x20); //public ItemDefinition ammoType; // 0x20
						auto mod = ((Networkable*)ammo)->GetComponent(unity::GetType(_(""), _("ItemModProjectile")));
						auto projectile = (Projectile*)((Networkable*)mod)->GetComponent(unity::GetType(_(""), _("Projectile")));

						auto normalized = baseplayer->eyes()->body_forward().normalize();

						Vector3 position = baseplayer->eyes()->position(), lastposition = position, velCheck = normalized * ((ItemModProjectile*)mod)->projectileVelocity();

						auto stats = held->get_stats(wpn->get_item_definition_id());
						for (float travelTime = 0.f; travelTime < 8.f; travelTime += 0.03125f)
						{
							position += velCheck * 0.03125f;
							if (!unity::is_visible(position, lastposition, 0))
								break;
							Line(position, lastposition, { r, g, b, 1 }, 0.02f, false, false);
							velCheck.y -= 9.81f * stats.gravity_modifier * 0.03125f;
							velCheck -= velCheck * stats.drag * 0.03125f;
							lastposition = position;
						}
						Sphere(position, 0.5f, { r, g, b, 1 }, 0.01f, false);
					}
				}
				if (vars->combat.silent_melee || unity::GetKey(vars->keybinds.silentmelee)
					&& vars->best_target.distance < vars->combat.melee_range)
				{
					auto hit_player = [&]() {
						auto Item = baseplayer->GetActiveItem();
						if (Item) {
							auto melee = Item->GetHeldEntity<BaseMelee>();
							if (melee) {
								auto class_name = melee->get_class_name();
								if (*(int*)(class_name + 4) == 'eleM' || *(int*)(class_name + 4) == 'mmah') {
									aim_target target = vars->best_target;
									if (target.ent) {
										auto world_position = target.ent->model()->boneTransforms()->get(48)->position();
										auto local = baseplayer->ClosestPoint(world_position);
										auto camera = baseplayer->model()->boneTransforms()->get(48)->position();

										if (camera.get_3d_dist(world_position) >= 4.2f)
											return;


										target.visible = baseplayer->is_visible(camera, local);

										attack_melee(target, (BaseProjectile*)melee, baseplayer, true);
									}
								}
							}
						}
					};
					hit_player();
				}
				if (vars->misc.autorefill) {
					if (!wcscmp(wpn->get_weapon_name(), _(L"Jackhammer"))
						&& last_refill_rpc + 0.3f < get_fixedTime()) {
						LocalPlayer::ItemCommand(wpn->uid(), _(L"refill"));
						last_refill_rpc = get_fixedTime();
					}
				}
			}

			if (vars->misc.automed) {
				auto hpleft = 100 - baseplayer->health();
				if (hpleft > 20) { //use syringe/medkit
					auto s = misc::FindFromHotbar(baseplayer, _(L"Med"));
					if (s < 0)
						s = misc::FindFromHotbar(baseplayer, _(L"Bandage"));
					if (s >= 0)
					{
						auto cs = baseplayer->Belt()->GetSelectedSlot();
						if (s != cs)
							baseplayer->Belt()->SetSelectedSlot(s);
						if (held->timeSinceDeploy() > held->deployDelay()
							&& held->nextAttackTime() < get_fixedTime()) {
							held->ServerRPC(_(L"UseSelf"));
						}
					}
				}
				else if (hpleft < 20 && hpleft > 1) { //use bandage
					auto s = misc::FindFromHotbar(baseplayer, _(L"Bandage"));
					if (s < 0)
						s = misc::FindFromHotbar(baseplayer, _(L"Med"));
					if (s >= 0)
					{
						auto cs = baseplayer->Belt()->GetSelectedSlot();
						if (s != cs)
							baseplayer->Belt()->SetSelectedSlot(s);
						if (held->timeSinceDeploy() > held->deployDelay()
							&& held->nextAttackTime() < get_fixedTime()) {
							held->ServerRPC(_(L"UseSelf"));
						}
					}
				}
			}

			if (!vars->misc.fake_lag || unity::GetKey(vars->keybinds.fakelag)) {
				if (!is_lagging && !flying && vars->misc.fake_lag && !is_speeding) {
					baseplayer->clientTickInterval() = 0.4f;
					is_lagging = true;
				}
			}
			else if (is_lagging && flying || is_lagging && is_speeding) {
				baseplayer->clientTickInterval() = 0.03f;
				is_lagging = false;
			}
			else if (is_lagging && !vars->misc.fake_lag) {
				baseplayer->clientTickInterval() = 0.05f;
				is_lagging = false;
			}

			if (vars->misc.eyeoffset || unity::GetKey(vars->keybinds.neck))
			{
				baseplayer->eyes()->set_view_offset(Vector3(0, vars->misc.PlayerEyes, 0));
			}

			if (auto movement = baseplayer->movement()) {
				if (vars->misc.spiderman) {
					movement->set_ground_angles_new(-1);
				}
				if (vars->misc.infinite_jump) {
					movement->set_land_time(0);
					movement->set_jump_time(0);
					movement->set_ground_time(100000);
				}

				if (vars->misc.gravity)
					movement->set_gravity_multiplier(1.75f);
				else
					movement->set_gravity_multiplier(2.35f);

				if (vars->combat.always_shoot) {
					if (auto ModelState = baseplayer->modelState()) {
						ModelState->set_flag(ModelState_Flag::OnGround);
						mem::write<float>((uint64_t)movement + 0x4C, 1); //private float <Grounded>k__BackingField; // 0x4C
					}
				}
			}
			auto item = baseplayer->GetActiveItem();

			//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
						//float mm_eye = ((0.01f + ((vars->desyncTime + 2.f / 60.f + 0.125f) * baseplayer->max_velocity())));

			if (vars->misc.speedhack && unity::GetKey(vars->keybinds.timescale)) {
				set_timeScale(vars->misc.speedhackspeed);
				is_speeding = true;
			}
			else
			{
				set_timeScale(1);
				is_speeding = false;
			}
			__try {
				auto target = vars->best_target; //baseplayer->get_aimbot_target(unity::get_camera_pos());
				if (item) {
					auto baseprojectile = *reinterpret_cast<BaseProjectile**>((uintptr_t)item + heldEntity);//item->GetHeldEntity<BaseProjectile>();
					if (baseprojectile) {
						auto wep_class_name = *(const char**)(*(uintptr_t*)(uintptr_t)baseprojectile + 0x10);

						auto attack = [&](aim_target target, bool is_tree) {
							auto gathering = ((BaseMelee*)baseprojectile)->gathering();

							auto baseprojectile = item2->GetHeldEntity<BaseProjectile>();

							if (is_tree) {
								if (!(gathering->tree()->gatherDamage() > 1)) {
									return;
								}
							}
							else {
								if (!(gathering->ore()->gatherDamage() > 1)) {
									return;
								}
							}

							attack_melee(target, baseprojectile, baseplayer);
						};

						if (target.ent
							&& vars->combat.aimbot
							&& unity::GetKey(vars->keybinds.aimbot))
						{
							//predict aiming direction
										//
										// normalize angles
										// smooth angles
										// set bodyangles to result
										//
							Vector3 eyes = baseplayer->eyes()->position();
							Vector3 aim_dir;
							Vector3 aim_vel;

							auto m = *reinterpret_cast<uintptr_t*>((uintptr_t)baseprojectile + 0x2C0); //public BaseProjectile.Magazine primaryMagazine; // 0x2C0
							auto ammo = *reinterpret_cast<uintptr_t*>((uintptr_t)m + 0x20); //public ItemDefinition ammoType; // 0x20
							auto mod = ((Networkable*)ammo)->GetComponent(unity::GetType(_(""), _("ItemModProjectile")));
							auto projectile = (Projectile*)((Networkable*)mod)->GetComponent(unity::GetType(_(""), _("Projectile")));

							//auto v = projectile->initialVelocity();//((base_projectile*)projectile)->get_item_mod_projectile()->get_projectile_velocity();
										//auto v1 = ((base_projectile*)projectile)->projectileVelocityScale();
										//if (vars->combat.fast_bullet)
										//	v1 *= 1.49f;
										//v *= v1;
							auto vel = projectile->initialVelocity();
							//auto vel = (getmodifiedaimcone(0, eyes - target.pos, true)).Normalized() * v;

							//misc::get_prediction(target, eyes, target.pos, vel, aim_vel, aim_dir, (Projectile*)projectile, true);


							Vector3 va = baseplayer->input()->bodyAngles();
							Vector2 vb = { va.x, va.y };

							auto calc = [&](const Vector3& src, const Vector3& dst) {
								Vector3 d = src - dst;
								return Vector2(RAD2DEG(Vector3::my_asin(d.y / d.length())), RAD2DEG(-Vector3::my_atan2(d.x, -d.z)));
							};
							auto normalize = [&](float& yaw, float& pitch) {
								//baseplayer->console_echo(string::wformat(_(L"[matrix]: ClientInput - yaw: %d, pitch: %d"), (int)yaw, (int)pitch));
								if (pitch < -270) pitch = -270;
								else if (pitch > 180) pitch = 180;
								if (yaw < -360) yaw = -360;
								else if (yaw > 360) yaw = 360;
							};
							auto step = [&](Vector2& angles) {
								bool smooth = true;
								Vector3 v = va;
								Vector2 va = { v.x, v.y };
								Vector2 angles_step = angles - va;
								normalize(angles_step.x, angles_step.y);

								if (smooth) {
									float factor_pitch = vars->combat.aimbot_smooth;
									if (angles_step.x < 0.f) {
										if (factor_pitch > std::fabs(angles_step.x)) {
											factor_pitch = std::fabs(angles_step.x);
										}
										angles.x = va.x - factor_pitch;
									}
									else {
										if (factor_pitch > angles_step.x) {
											factor_pitch = angles_step.x;
										}
										angles.x = va.x + factor_pitch;
									}
								}
								if (smooth) {
									float factor_yaw = vars->combat.aimbot_smooth;
									if (angles_step.y < 0.f) {
										if (factor_yaw > std::fabs(angles_step.y)) {
											factor_yaw = std::fabs(angles_step.y);
										}
										angles.y = va.y - factor_yaw;
									}
									else {
										if (factor_yaw > angles_step.y) {
											factor_yaw = angles_step.y;
										}
										angles.y = va.y + factor_yaw;
									}
								}
							};

							Vector2 offset = calc(eyes, target.pos) - vb;
							Vector2 ai = vb + offset;
							step(ai);
							step(ai);
							normalize(ai.x, ai.y);
							Vector3 i = { ai.x, ai.y, 0.0f };
							baseplayer->input()->bodyAngles() = i;
						}

						if (vars->misc.silent_farm) {
							auto entity = baseplayer->resolve_closest_entity(4, false);
							if (entity.first.found && entity.first.ent) {
								if (*(int*)(wep_class_name + 4) == 'eleM' || *(int*)(wep_class_name + 4) == 'mmah') {
									attack(entity.first, entity.second);
								}
							}
						}

						if (vars->misc.autofarmbarrel) {
							auto entity = baseplayer->resolve_closest_entity(2, false);
							if (entity.first.ent) {
								if (*(int*)(wep_class_name + 4) == 'eleM' || *(int*)(wep_class_name + 4) == 'mmah'
									&& entity.first.distance < 1.f) {
									//entity.first.pos = entity.first.ent->ClosestPoint(baseplayer->eyes()->position());
									attack(entity.first, entity.second);
								}
							}
						}

						if (!(*(int*)(wep_class_name + 4) == 'eleM' && *(int*)(wep_class_name) == 'esaB')) {
							if (unity::GetKey(VK_LBUTTON) && vars->misc.instant_med) {
								//const auto item_id = item->info()->itemid;
								const auto item_id = item->info()->itemid;

								if (item_id == 1079279582 || item_id == -2072273936) {
									auto time = get_time();
									if (baseprojectile->timeSinceDeploy() > baseprojectile->deployDelay() && baseprojectile->nextAttackTime() <= get_time()) {
										if (time > nextActionTime) {
											nextActionTime = time + period;
											baseprojectile->ServerRPC(_(L"UseSelf"));
										}
									}
								}
							}
							else if (unity::GetKey(VK_RBUTTON) && vars->misc.instant_med) {
								const auto item_id = item->info()->itemid;

								if (item_id == 1079279582 || item_id == -2072273936) {
									vars->matrix = unity::get_view_matrix();
									auto camera_pos = unity::get_camera_pos();

									auto target = vars->best_target; //baseplayer->get_aimbot_target(camera_pos);

									if (target.ent && target.distance < 5) {
										auto net = *reinterpret_cast<Networkable**>((uintptr_t)target.ent + 0x58);
										if (net) {
											auto id = net->get_id();
											if (id) {
												auto method_addr = mem::read<uintptr_t>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_uint);
												if (method_addr) {
													auto time = get_time();
													if (baseprojectile->timeSinceDeploy() > baseprojectile->deployDelay() && baseprojectile->nextAttackTime() <= get_time()) {
														if (time > nextActionTime) {
															nextActionTime = time + period;
															ServerRPC_int(baseprojectile, System::string(_(L"UseOther")), id, method_addr);
														}
													}
												}
											}
										}
									}
								}
							}

							//////weapon mods/////
							if (*(int*)(wep_class_name) == 'esaB' && *(int*)(wep_class_name + 4) == 'jorP' || *(int*)(wep_class_name) == 'nilF') {
								if (item->is_weapon()) {
									//const auto item_id = item->info()->itemid;

									if (*(int*)(wep_class_name) == 'nilF'
										&& vars->combat.instaeoka) {
										mem::write<float>((uint64_t)baseprojectile + 0x3A0, 1.f); //eoka success fraction
										mem::write<bool>((uint64_t)baseprojectile + 0x3B0, true); //eoka _didSparkThisFrame
									}
									else {
										if (vars->combat.rapidfire)
											baseprojectile->repeatDelay() = vars->combat.firerate;
										if (vars->combat.automatic)
											baseprojectile->automatic() = true;
									}

									baseprojectile->projectileVelocityScale() = vars->combat.fast_bullet;
									if (vars->combat.nospread)
										baseprojectile->set_no_spread();
									baseprojectile->set_recoil();
								}
							}
						}
						else //is BaseMelee
						{
							//melee manipulator? lol
						}
					}
				}
			}
			__except (true) {}

			if (vars->misc.instant_revive) {
				auto Target = baseplayer->resolve_closest_player(5);
				if (Target.ent)
				{
					if (vars->misc.revivefriendsonly ? ((BasePlayer*)Target.ent)->isFriend() : true)
					{
						//Line(baseplayer->eyes()->position(), Target.pos, { 1, 1, 1, 1 }, 0.01f, false, false);
						if (!Target.is_heli
							&& Target.distance <= 5
							&& HasPlayerFlag((BasePlayer*)Target.ent, PlayerFlags::Wounded)
							&& get_fixedTime() > last_revive_rpc + .5f)
						{
							Target.ent->ServerRPC(_(L"RPC_Assist"));
							last_revive_rpc = get_fixedTime();
						}
					}
				}
			}

			if (vars->misc.gesture_spam > 1
				&& get_fixedTime() > last_gesture_rpc + 0.35f)
			{
				switch (vars->misc.gesture_spam) {
				case 0:
					break;
				case 1:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"clap"));
					break;
				case 2:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"friendly"));
					break;
				case 3:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"thumbsdown"));
					break;
				case 4:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"thumbsup"));
					break;
				case 5:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"ok"));
					break;
				case 6:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"point"));
					break;
				case 7:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"shrug"));
					break;
				case 8:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"victory"));
					break;
				case 9:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"wave"));
					break;
				case 10:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"dance.cabbagepatch"));
					break;
				case 11:
					baseplayer->SendSignalBroadcast(Signal::Gesture, _(L"dance.twist"));
					break;
				}
				last_gesture_rpc = get_fixedTime();
			}


			unity::IgnoreLayerCollision(layer::PlayerMovement, layer::Water, !vars->misc.no_playercollision);
			unity::IgnoreLayerCollision(layer::PlayerMovement, layer::Tree, vars->misc.no_playercollision);
			unity::IgnoreLayerCollision(layer::PlayerMovement, layer::AI, vars->misc.no_playercollision);
			unity::IgnoreLayerCollision(layer::Physics_Projectile, layer::Deployed, vars->misc.no_playercollision);
			unity::IgnoreLayerCollision(layer::Physics_Projectile, layer::Tree, vars->misc.no_playercollision);

			draw_get();
			if (tick_time > vars->tick_time_when_called + 10) {
				unity::camera = unity::get_main_camera();
				vars->tick_time_when_called = tick_time;
			}

			if (vars->misc.autofarm
				|| vars->misc.walktomarker) {
				if (misc::node.pos != Vector3(0, 0, 0))
				{
					auto dir = (misc::node.pos - baseplayer->eyes()->position()).Normalized();
					((InputState*)state)->set_aim_angles(dir);
				}
			}
		}
		else vars->visual.snapline = 0;

		//printf("before end lientinput original\n");
		if (baseplayer)
		{
			if (!iii) {
				vars->misc.capsuleHeight = baseplayer->movement()->capsuleHeight();
				vars->misc.capsuleCenter = baseplayer->movement()->capsuleCenter();
				vars->misc.capsuleHeightDucked = baseplayer->movement()->capsuleHeightDucked();
				vars->misc.capsuleCenterDucked = baseplayer->movement()->capsuleCenterDucked();
				vars->misc.capsuleradius = baseplayer->movement()->capsule()->GetRadius();
				o_capsuleHeight = baseplayer->movement()->capsuleHeight();
				o_capsuleCenter = baseplayer->movement()->capsuleCenter();
				o_capsuleHeightDucked = baseplayer->movement()->capsuleHeightDucked();
				o_capsuleCenterDucked = baseplayer->movement()->capsuleCenterDucked();
				o_capsuleradius = baseplayer->movement()->capsule()->GetRadius();

				iii = true;
			}
			if (unity::GetKey(vars->keybinds.capsule)) {
				baseplayer->movement()->capsuleHeight() = vars->misc.capsuleHeight;
				baseplayer->movement()->capsuleCenter() = vars->misc.capsuleCenter;
				baseplayer->movement()->capsuleHeightDucked() = vars->misc.capsuleHeightDucked;
				baseplayer->movement()->capsuleCenterDucked() = vars->misc.capsuleCenterDucked;
				baseplayer->movement()->capsule()->SetRadius(vars->misc.capsuleradius);
			}
			else {
				baseplayer->movement()->capsuleHeight() = o_capsuleHeight;
				baseplayer->movement()->capsuleCenter() = o_capsuleCenter;
				baseplayer->movement()->capsuleHeightDucked() = o_capsuleHeightDucked;
				baseplayer->movement()->capsuleCenterDucked() = o_capsuleCenterDucked;
				baseplayer->movement()->capsule()->SetRadius(o_capsuleradius);
			}

			if (vars->visual.showcapsule)
			{
				auto pos = baseplayer->transform()->position();
				pos.y += 0.7f;
				pos.y += baseplayer->modelState()->has_flag(ModelState_Flag::Ducked) ? vars->misc.capsuleCenterDucked : vars->misc.capsuleCenter;
				Capsule(pos,
					baseplayer->eyes()->rotation(),
					baseplayer->movement()->capsule()->GetRadius(),
					baseplayer->movement()->capsule()->GetHeight(),
					{ r, g, b, 1 },
					0.01f,
					false);
			}

			//printf("last part of clientinput hook\n");
			if (baseplayer->is_sleeping()) return;
			auto model_state = baseplayer->modelState();

			Vector3 real_angles = safe_read(state + 0x18, Vector3);
			Vector3 spin_angles = Vector3::Zero();
			bool targeted = vars->targetted;
			int jitter_speed = 10;
			int spin_speed = vars->misc.spinspeed;
			//printf("antiaim stuff\n");
			switch (vars->misc.antiaim)
			{
			case 0: //x = yaw (up/down), y = pitch (spin), z = roll?????;
					//if (targeted)
					//	spin_angles = Vector3(-999.f, (rand() % 999 + -999), (rand() % 999 + -999));
				break;
			case 1: //backwards
				spin_angles.y = real_angles.y + (targeted ? (rand() % -180 + 1) : 180.f);
				break;
			case 2: //backwards (down)
				spin_angles.x = (targeted ? 999.f : -999.f);
				spin_angles.z = 0.f;
				spin_angles.y = real_angles.y + 180.f;
				break;
			case 3: //backwards (up)
				spin_angles.x = (targeted ? -999.f : 999.f);
				spin_angles.z = (targeted ? -999.f : 999.f);
				spin_angles.y = real_angles.y + 180.f;
				break;
			case 4: //left
				spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				break;
			case 5: //left (down)
				spin_angles.x = (targeted ? 999.f : -999.f);
				spin_angles.z = 0.f;
				spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				break;
			case 6: //left (up)
				spin_angles.x = (targeted ? -999.f : 999.f);
				spin_angles.z = (targeted ? -999.f : 999.f);
				spin_angles.y = real_angles.y + (targeted ? (rand() % -90 + 1) : 90.f);
				break;
			case 7: //right
				spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				break;
			case 8: //right (down)
				spin_angles.x = (targeted ? 999.f : -999.f);
				spin_angles.z = 0.f;
				spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				break;
			case 9: //right (up)
				spin_angles.x = (targeted ? -999.f : 999.f);
				spin_angles.z = (targeted ? -999.f : 999.f);
				spin_angles.y = real_angles.y + (targeted ? (rand() % 90 + 1) : -90.f);
				break;
			case 10: //jitter
				if (jitter <= jitter_speed * 1)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
				}
				else if (jitter <= jitter_speed * 2)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : 45.f);
				}
				else if (jitter <= jitter_speed * 3)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 180 + 1) : -180.f);
					jitter = 1;
				}
				jitter = jitter + 1;
				spin_angles.y = real_angles.y;
				break;
			case 11: //jitter (down)
				if (jitter <= jitter_speed * 1)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
				}
				else if (jitter <= jitter_speed * 2)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % -45 + 1) : 45.f);
				}
				else if (jitter <= jitter_speed * 3)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % -180 + 1) : 180.f);
					jitter = 1;
				}
				jitter = jitter + 1;
				spin_angles.x = (targeted ? (rand() % 999 + 1) : -999.f);
				spin_angles.z = 0.f;
				spin_angles.y = real_angles.y;
				break;
			case 12: //jitter (up)
				if (jitter <= jitter_speed * 1)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % -45 + 1) : 45.f);
				}
				else if (jitter <= jitter_speed * 2)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 45 + 1) : -45.f);
				}
				else if (jitter <= jitter_speed * 3)
				{
					spin_angles.y = real_angles.y + (targeted ? (rand() % 180 + 1) : -180.f);
					jitter = 1;
				}
				jitter = jitter + 1;
				spin_angles.x = (targeted ? -999.f : 999.f);
				spin_angles.z = (targeted ? -999.f : 999.f);
				spin_angles.y = real_angles.y;
				break;
			case 13: //spin
				spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
				if (spin > (360 / spin_speed))
					spin = 1;
				break;
			case 14: //spin (down)
				spin_angles.x = (targeted ? 999.f : -999.f);
				spin_angles.z = 0.f;
				spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
				if (spin > (360 / spin_speed))
					spin = 1;
				break;
			case 15: //spin (up)
				spin_angles.x = (targeted ? -999.f : 999.f);
				spin_angles.y = targeted ? -(real_angles.y + (spin_speed * spin++)) : real_angles.y + (spin_speed * spin++);
				spin_angles.z = (targeted ? -999.f : 999.f);
				if (spin > (360 / spin_speed))
					spin = 1;
				break;
			case 16: //random
				spin_angles = Vector3((rand() % 999 + -999), (rand() % 999 + -999), (rand() % 999 + -999));
				break;
			}

			if (spin_angles != Vector3::Zero())
				((InputState*)state)->set_aim_angles(spin_angles);
			vars->targetted = false;
		}
		/*
		__try {
			sol::state lua;
			lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::io);

			//auto core = lua["cheat"].get_or_create<sol::table>();

			lua.new_usertype<Vector2>("Vector2",
				"x", &Vector2::x,
				"y", &Vector2::y);

			lua.new_usertype<Vector3>("Vector3",
				"x", &Vector3::x,
				"y", &Vector3::y,
				"z", &Vector3::z);

			lua.new_usertype<Vector4>("Vector4",
				"x", &Vector4::x,
				"y", &Vector4::y,
				"z", &Vector4::z,
				"w", &Vector4::w);

			lua.new_usertype<lw::color>("color",
				"r", &lw::color::r,
				"g", &lw::color::g,
				"b", &lw::color::b,
				"a", &lw::color::a);

			auto gui = lua["draw"].get_or_create<sol::table>();

			gui.set_function("rect", &lw::draw::Rect);
			gui.set_function("filledrect", &lw::draw::FillRect);
			gui.set_function("circle", &lw::draw::Circle);
			gui.set_function("filledcircle", &lw::draw::FillCircle);
			gui.set_function("text", &lw::draw::Text);
			gui.set_function("textcentered", &lw::draw::TextCentered);
			gui.set_function("line", &lw::draw::Line);
			gui.set_function("line3d", &lw::draw::Line3d);
			gui.set_function("sphere3d", &lw::draw::Sphere3d);

			lua.new_usertype<lw::Eyes>("PlayerEyes",
				"getpos", &lw::Eyes::GetPosition,
				"getrot", &lw::Eyes::GetRotation,
				"bodyfwd", &lw::Eyes::BodyForward,
				"bodyright", &lw::Eyes::BodyRight,
				"getviewoffset", &lw::Eyes::GetViewOffset,
				"setviewoffset", &lw::Eyes::SetViewOffset);

			lua.new_usertype<lw::Player>("Player",
				"settargetmovement", &lw::Player::SetTargetMovement,
				"gettargetmovement", &lw::Player::GetTargetMovement,
				"isalive", &lw::Player::IsAlive,
				"gethealth", &lw::Player::GetHealth,
				"getbonepos", &lw::Player::GetBonePos,
				"getname", &lw::Player::GetName,
				"isnpc", &lw::Player::IsNpc,
				"userid", &lw::Player::GetUserId,
				"serverrpc", &lw::Player::Rpc,
				"islocalplayer", &lw::Player::IsLocalPlayer,
				"isfriend", &lw::Player::IsFriend,
				//"geteyes", &lw::Player::GetEyes,
				"isteammate", &lw::Player::IsTeammate);

			lua.new_usertype<lw::Entity>("Entity",
				"getpos", &lw::Entity::GetPosition,
				"classname", &lw::Entity::GetClassname,
				"rpc", &lw::Entity::Rpc);

			auto core = lua["cheat"].get_or_create<sol::table>();

			core.set_function("getplayer", &lw::entities::GetPlayer);
			core.set_function("getentity", &lw::entities::GetEntity);
			core.set_function("lineofsight", &lw::misc::LineOfSight);
			core.set_function("worldtoscreen", &lw::misc::w2s);
			core.set_function("iskeydown", &lw::misc::IsKeyPressed);
			core.set_function("playerlistsize", &lw::entities::PlayerListSize);
			core.set_function("entitylistsize", &lw::entities::EntityListSize);
			core.set_function("realtimesincestartup", &lw::misc::TimeSinceStartup);
			core.set_function("fixedtime", &lw::misc::FixedTime);

			core["desynctime"] = vars->desyncTime;
			core["flyhackDistanceY"] = settings::vert_flyhack;
			core["flyhackDistanceX"] = settings::hor_flyhack;

			for (auto pair : vars->loaded_lua_list) {
				if (*pair.second) {

					auto filename = vars->data_dir + _("scripts\\") + pair.first + _(".lua");
					lua.script_file(filename);

					sol::protected_function func = lua[_("clientinput")];
					//func.set_error_handler(lua[_("errorhandler")]);
					if(func.valid())
						auto f = (std::function<void()>)func();
				}
			}
		}
		__except (true) {}
		*/
		return;
	}
}