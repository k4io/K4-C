#pragma once
#include "nine_meter_poggers.hpp"

#include "esp.hpp"

#include "memory/defs.h"
#include "memory/minhook/include/MinHook.h"
//#include "gui/OnGUI.hpp"
#define CALLED_BY(func,off) (reinterpret_cast<std::uint64_t>(_ReturnAddress()) > func && reinterpret_cast<std::uint64_t>(_ReturnAddress()) < func + off)

static auto dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("DontDestroyOnLoad"), 0, _(""), _("UnityEngine"))));

static auto create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_CreateGameObject"), 0, _(""), _("UnityEngine"))));

static auto add_component = reinterpret_cast<Component*(*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_AddComponentWithType"), 0, _(""), _("UnityEngine"))));

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
		static auto playermodel_updatelocalvelocity = reinterpret_cast<void (*)(PlayerModel*, Vector3, Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerModel"), _("UpdateLocalVelocity"), 2, _(""), _(""))));
		static auto BaseProjectile_OnSignal = reinterpret_cast<void (*)(BaseProjectile*, int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("OnSignal"), 2, _(""), _(""))));
		static auto playerwalkmovement_client_input = reinterpret_cast<void (*)(PlayerWalkMovement*, uintptr_t, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), -1, _(""), _(""))));
		static auto DoFixedUpdate = reinterpret_cast<void (*)(PlayerWalkMovement*, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("DoFixedUpdate"), -1, _(""), _(""))));
		static auto UpdateVelocity = reinterpret_cast<void (*)(PlayerWalkMovement*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("UpdateVelocity"), -1, _(""), _(""))));
		static auto blocksprint = reinterpret_cast<void (*)(BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("BlockSprint"), 1, _(""), _(""))));
		static auto OnNetworkMessage = reinterpret_cast<void (*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnNetworkMessage"), 1, _(""), _(""))));
		static auto IsConnected = reinterpret_cast<bool (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("IsConnected"), 0, _(""), _("Network"))));
		static auto Run = reinterpret_cast<System::string(*)(uintptr_t, System::string, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ConsoleSystem"), _("Run"), 0, _(""), _(""))));


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
	
	static auto ServerRPC_intstring = reinterpret_cast<void (*)(BaseEntity*, System::string funcName, unsigned int arg1, System::string, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uintstring_);
	
	//static auto get_resourcePath = reinterpret_cast<System::string (*)(uintptr_t)>(mem::game_assembly_base + offsets::Method$ResourceRef_method);

	static auto PerformanceUI_Update = reinterpret_cast<void (*)(void*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PerformanceUI"), _("Update"), -1, _(""), _("Facepunch"))));

	void init_hooks() {
		orig::createeffect = reinterpret_cast<GameObject * (*)(System::string, Effect*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("EffectLibrary"), _("CreateEffect"), 2, _(""), _(""))));
		ServerRPC_intstring = reinterpret_cast<void (*)(BaseEntity*, System::string funcName, unsigned int arg1, System::string, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uintstring_);
		dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("DontDestroyOnLoad"), 0, _(""), _("UnityEngine"))));
		create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_CreateGameObject"), 0, _(""), _("UnityEngine"))));
		add_component = reinterpret_cast<Component*(*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_AddComponentWithType"), 0, _(""), _("UnityEngine"))));

		orig::IsConnected = reinterpret_cast<bool (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("IsConnected"), 0, _(""), _("Network"))));
		orig::OnNetworkMessage = reinterpret_cast<void (*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Client"), _("OnNetworkMessage"), 1, _(""), _(""))));
		orig::BaseProjectile_OnSignal = reinterpret_cast<void (*)(BaseProjectile*, int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("OnSignal"), 2, _(""), _(""))));
		orig::baseplayer_client_input = reinterpret_cast<void (*)(BasePlayer*, InputState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ClientInput"), -1, _(""), _(""))));
		orig::playerwalkmovement_client_input = reinterpret_cast<void (*)(PlayerWalkMovement*, uintptr_t, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), -1, _(""), _(""))));
		orig::DoFixedUpdate = reinterpret_cast<void (*)(PlayerWalkMovement*, ModelState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("DoFixedUpdate"), -1, _(""), _(""))));
		orig::blocksprint = reinterpret_cast<void (*)(BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("BlockSprint"), 1, _(""), _(""))));
		orig::Run = reinterpret_cast<System::string(*)(uintptr_t, System::string, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ConsoleSystem"), _("Run"), 0, _(""), _(""))));
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

	bool flying = false, is_speeding = false, is_lagging = false, has_intialized_methods = false, wake = true;
	float nextActionTime = 0, period = 1.4721, last_gesture_rpc = 0.f, last_revive_rpc = 0.f;
	Vector3 m_debugcam_toggle_pos, m_debugcam_pos;
	uintptr_t do_fixed_update_ptr, client_input_ptr, bodylean_ptr, mounteyepos_ptr, isdown_ptr, __go;
	

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

						*(float*)(TOD_Night + 0x50) = vars->visual.night;
						*(float*)(TOD_Night + 0x54) = vars->visual.night;
						*(float*)(TOD_Day + 0x50) = vars->visual.day;
						*(float*)(TOD_Day + 0x54) = vars->visual.day;
						*(float*)(TOD_Stars + 0x14) = vars->visual.staramount;



						m_skydome = mem::read<uintptr_t>(current_tagged_obj + 0x18);
					}
				}
				last_object = current_tagged_obj;
				current_tagged_base = mem::read<uintptr_t>(current_tagged_base + 0x8);
				current_tagged_obj = mem::read<uintptr_t>(current_tagged_base + 0x10);
			}
		}
	}

	void hk_serverrpc_projectileshoot(int64_t rcx, int64_t rdx, int64_t r9, int64_t projectileShoot, int64_t arg5) {
		Projectile* p;
		Vector3 rpc_position;
		float time = get_fixedTime();
		float travel_t = 0.f;
		int simulations = 0;
		do {
			if (!esp::local_player)
				break;

			auto Item = esp::local_player->GetActiveItem();

			if (!Item)
				break;

			auto baseprojectile = esp::local_player->GetActiveItem()->GetHeldEntity<BaseProjectile>();
			if (!baseprojectile)
				break;

			auto wep_class_name = *(const char**)(*(uintptr_t*)(uintptr_t)baseprojectile + 0x10);
			if (*(int*)(wep_class_name + 4) == 'eleM' || *(int*)(wep_class_name) == 'ddaP')
				break;

			BaseProjectile* projectile_list = *reinterpret_cast<BaseProjectile**>(
				*reinterpret_cast<uintptr_t*>((uintptr_t)baseprojectile + 0x370) + 0x10); //createdProjectiles;

			esp::matrix = unity::get_view_matrix();
			auto camera_pos = unity::get_camera_pos();

			//aim_target target = esp::local_player->get_aimbot_target(camera_pos);
			aim_target target = esp::best_target;

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
					target = aim_target();
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
						// rpc_position += misc::manipulate(esp::local_player, target.pos);
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
								vis_fat = true;
							}
						}
					}
				}
				
			}
			target_pos.y += 0.2f;

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
				if (vars->combat.lower_vel)
					misc::lower_velocity(target, rpc_position, target_pos, original_vel, stats.initial_velocity, aimbot_velocity, a, travel_t, p, u);
				else misc::get_prediction(target, rpc_position, target_pos, original_vel, aimbot_velocity, a, travel_t, pt, dr, gr, u);

				if (vars->combat.thick_bullet)
					*reinterpret_cast<float*>(projectile + 0x2C) = vars->combat.thickness > 1.f ? 1.f : vars->combat.thickness;
				else
					*reinterpret_cast<float*>(projectile + 0x2C) = 0.01f;
				break;
			}

			for (int i = 0; i < size; i++) {
				auto projectile = *(uintptr_t*)(shoot_list + 0x20 + i * 0x8);
				if (target.ent && (target.visible || manipulated || misc::autoshot) && !target.teammate) {
					*reinterpret_cast<Vector3*>(projectile + 0x24) = aimbot_velocity; //startvel
					Sphere(target_pos, 0.05f, { r, g, b, 1 }, 5.f, 100.f);
				}
			}

			for (int i = 0; i < projectile_list->get_size(); i++) {
				auto projectile = *(BaseProjectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
				p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);

				if (!projectile)
					continue;

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
						Sphere(n, 0.1f, { 1, 1, 1, 1 }, 10.f, false);
						p->traveledTime(vars->desyncTime);
						p->SetInitialDistance(r.distance(n) - 1);
					}
					else p->SetInitialDistance(0);

					esp::local_player->console_echo(string::wformat(_(L"[matrix]: ProjectileShoot (bullet tp) spawned bullet at distance %dm"), (int)p->initialDistance()));
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
				
				p->integrity(1.f);
				float t = p->traveledTime();
				
				//create and SEND FAKE copy of projectile so game only updates original
				//Projectile* fake = new Projectile(*p);
				p->initialVelocity(aimbot_velocity);
				fired_projectile f = { p, nullptr, time, 1, target };
				
				//p = *(Projectile**)((uintptr_t)projectile_list + 0x20 + i * 0x8);
				//*reinterpret_cast<uintptr_t*>((uintptr_t)projectile_list + 0x20 + i * 0x8) = (uintptr_t)fake;
				
				for (size_t i = 0; i < 32; i++)
					if (misc::fired_projectiles[i].fired_at <= 10.f) {
						misc::fired_projectiles[i] = f;
						break;
					}

				if (!vars->combat.bullet_tp)
					p->SetInitialDistance(0);

				p->currentVelocity(original_vel);
				p->initialVelocity(original_vel);
				p->previousVelocity(original_vel);
				p->currentPosition(rpc_position);
				p->previousPosition(rpc_position);
				p->sentPosition(rpc_position);
				p->prevSentVelocity(rpc_position); //?
				p->launchTime(get_fixedTime());
				p->traveledTime(0);
				//p->projectileID(fp++);
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
					((Projectile1*)p)->Launch1();
				}
			}
			if (vars->combat.targetbehindwall)
			{
				typedef void(*cclear)(uintptr_t);
				((cclear)(mem::game_assembly_base + 15140672))((uintptr_t)baseprojectile + 0x370); //"System.Collections.Generic.List<Projectile>$$Clear",
			}

			if (misc::autoshot)
				misc::autoshot = false;
		} while (0);

		esp::local_player->console_echo(string::wformat(_(L"[matrix]: ProjectileShoot (prediction) simulated %i times before hit!"), simulations));
		reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(hooks::orig::serverrpc_projectileshoot)(rcx, rdx, r9, projectileShoot, arg5);
		
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

	void hk_serverrpc_playerprojectileupdate(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		auto  projectile = reinterpret_cast<Projectile*>(get_rbx_value());
		auto  ppu = reinterpret_cast<protobuf::PlayerProjectileUpdate*>(_ppa);

		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::playerprojectileupdate);

		//call fake domovement? after called set current position etc
		//projectile->DoMovement(misc::tickDeltaTime, projectile);
		//return;


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
		//	Vector3 targetpos = esp::best_target.player->model()->boneTransforms()->get(48)->get_bone_position();
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
			if (!esp::local_player)
				break;
			
			//Update((Projectile*)projectile); //many invalids with fat bullet manipulator as projectile position isnt updated before the hit

			auto hit_test = projectile->get_hit_test();
			if (!hit_test)
				break;

			if (!hit_test->gameObject())
				break;

			auto layer = hit_test->gameObject()->get_layer();
			auto prefab_name = hit_test->gameObject()->get_prefab_name();

			aim_target target = esp::best_target;//esp::local_player->get_aimbot_target(camera_pos);

			if (!target.ent)
				break;

			auto hit_entity = (BasePlayer*)hit_test->HitEntity();
			if (layer == rust::classes::layer::Player_Server) {
				if (hit_entity->is_teammate(esp::local_player)) {
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
				
				if (vars->combat.hitbox == 0
					 && esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						 (int)rust::classes::Bone_List::head)->position(), projectile->get_current_position()))
					test[0] = { 698017942, 2173623152 };
				if (vars->combat.hitbox == 1
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::pelvis)->position(), projectile->get_current_position()))
					test[0] = { 1031402764, 1750816991 };
				if (vars->combat.hitbox == 2
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::spine4)->position(), projectile->get_current_position()))
					test[0] = { 3901657145, 1750816991 };
				if (vars->combat.hitbox == 3
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::r_hand)->position(), projectile->get_current_position()))
					test[0] = { 102231371, 1750816991 };
				if (vars->combat.hitbox == 4
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::penis)->position(), projectile->get_current_position()))
					test[0] = { 612182976, 2173623152 };
				if (vars->combat.hitbox == 5
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::l_hip)->position(), projectile->get_current_position()))
					test[0] = { 3892428003, 1750816991 };
				if (vars->combat.hitbox == 6
					&& esp::local_player->is_visible(target.ent->model()->boneTransforms()->get(
						(int)rust::classes::Bone_List::r_foot)->position(), projectile->get_current_position()))
					test[0] = { 920055401, 1750816991 };


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

			if (layer == rust::classes::layer::Deployed) {
				hit_test->ignoreEntity() = hit_entity;
				return;
			}
			//if (layer != rust::classes::layer::Player_Server) {
			//	hit_test->ignoreEntity() = hit_entity;
			//	return;
			//}
		} while (0);



		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}

	void hk_serverrpc_doplace(int64_t rcx, int64_t rdx, int64_t r9, int64_t _ppa, int64_t arg5) {
		//auto  planner = *reinterpret_cast<uintptr_t*>(get_rbx_value());
		auto  ppa = (protobuf::CreateBuilding * )_ppa;
		const auto orig_fn =
			reinterpret_cast<void (*)(int64_t, int64_t, int64_t, int64_t, int64_t)>(
				hooks::orig::createbuilding);

		if(!esp::local_player)
			return orig_fn(rcx, rdx, r9, _ppa, arg5);

		auto held = esp::local_player->GetActiveItem()->GetHeldEntity<Planner>();
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

			if (GetAsyncKeyState(0x39))
			{
				*reinterpret_cast<unsigned int*>(_ppa + 0x14) = esp::local_player->net()->get_id();
			}
			else
				*reinterpret_cast<unsigned int*>(_ppa + 0x14) = esp::selected_entity_id;


			auto build_id = *reinterpret_cast<unsigned int*>(_ppa + 0x14);
			auto tranny = esp::find_transform_by_id(build_id);
			if (tranny) {
				//transform* tr = get_transform((base_player*)FindEntity);
				//transform* Traaa = get_transform//FindEntity->_get_transform();
				*reinterpret_cast<Vector3*>(_ppa + 0x20) = ((Transform*)tranny)->InverseTransformPoint(position);
				*reinterpret_cast<Vector3*>(_ppa + 0x2C) = ((Transform*)tranny)->InverseTransformDirection(normal);

				esp::local_player->console_echo(string::wformat(_(L"[matrix] DoPlace - Spoofed %d to %d with position (%d, %d, %d)"), 
					(int)ogid,
					(int)esp::selected_entity_id,
					(int)position.x,
					(int)position.y,
					(int)position.z
				));
			}
		}
		return orig_fn(rcx, rdx, r9, _ppa, arg5);
	}

	void hk_playerwalkmovement_ClientInput(PlayerWalkMovement* player_walk_movement, uintptr_t inputstate, ModelState* model_state) {
		//if (esp::local_player) esp::local_player->console_echo(_(L"PWM clientinput start"));
		if (player_walk_movement && inputstate && model_state) {
			orig::playerwalkmovement_client_input(player_walk_movement, inputstate, model_state);
			Vector3 vel = player_walk_movement->get_TargetMovement();
			auto loco = esp::local_player;
			auto dont_move = false;
			if ((vars->combat.manipulator2 || vars->combat.manipulator)
					&& (unity::GetKey(vars->keybinds.manipulator)
						|| misc::manipulate_vis))
			{
				//manipulating
				player_walk_movement->set_TargetMovement({ 0, 0, 0 });
				return;
			}
			if (!loco || loco->is_sleeping())
				return;
			auto time = unity::get_realtimesincestartup();//UnityEngine::Time::get_realtimeSinceStartup();
			float _timeSinceLastTick = time - loco->lastSentTickTime();
			if (loco && !loco->is_sleeping()) {
				if (unity::GetKey(vars->keybinds.tp)) {
					if (vars->last_teleport_time + 20.f < get_fixedTime()) {
						vars->last_teleport_time = get_fixedTime();

						Vector3 a = Vector3::Zero();
						auto fwd = loco->eyes()->body_forward() * 5;
						auto right = loco->eyes()->body_right() * 5;
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

						player_walk_movement->teleport_to(loco->transform()->position() + a, loco);
					}
				}
			}
			if (loco && !loco->is_sleeping() && vars->desyncTime < 0.f) {
				if (vars->misc.flyhack_stop) {
					Vector3 curr = esp::local_player->transform()->position();
					Vector3 old = misc::cLastTickPos;
					Vector3 v4 = (curr - misc::cLastTickPos);
					Vector3 ov = Vector3(curr.x, curr.y, curr.z);

					if (settings::vert_flyhack >= 2.5f)
						ov = Vector3(ov.x, curr.y < old.y ? (curr.y - 0.3f) : old.y, ov.z);
					if (settings::hor_flyhack >= 6.f)
						ov = Vector3(old.x, ov.y, old.z);

					if (settings::vert_flyhack >= 2.5f
						|| settings::hor_flyhack >= 6.f) {
						if (ov != curr)
							player_walk_movement->teleport_to(ov, loco);
						dont_move = true;
					}
				}

				if (loco->transform())
				{
					misc::cLastTickEyePos = loco->eyes()->position();//get_transform(esp::local_player)->get_bone_position();//baseplayer->eyes()->get_position();
					misc::cLastTickPos = loco->transform()->position();//_get_transform(esp::local_player)->get_position();//get_transform(esp::local_player)->get_bone_position();//baseplayer->eyes()->get_position();
					misc::ticks.AddPoint(misc::cLastTickPos);
					misc::ServerUpdate(get_deltaTime(), loco);
				}
			}
			else if (!loco || loco->is_sleeping())
			{
				settings::vert_flyhack = 0.f; settings::hor_flyhack = 0.f;
			}

			if (loco && !loco->is_sleeping() && vars->desyncTime < 0.f)
			{
				float max_speed = (player_walk_movement->get_swimming() || player_walk_movement->get_ducking() > 0.5) ? 1.7f : 5.5f;

				set_sprinting(model_state, true);
				flying = player_walk_movement->get_flying();

				if (vars->misc.silentwalk && vars->keybinds.silentwalk) {
					set_onLadder(model_state, true);
				}
				else set_onLadder(model_state, false);

				if (vars->misc.interactive_debug)
					model_state->set_flag(rust::classes::ModelState_Flag::Mounted);

				model_state->remove_flag(rust::classes::ModelState_Flag::Flying);

				if (vars->misc.always_sprint) {
					if (vel.length() > 0.f) {
						Vector3 target_vel = Vector3(vel.x / vel.length() * max_speed, vel.y, vel.z / vel.length() * max_speed);
						player_walk_movement->set_TargetMovement(target_vel);
					}
				}

				if (unity::GetKeyDown(rust::classes::KeyCode::Space)
					&& vars->misc.infinite_jump)
				{
					misc::autobot::do_jump(player_walk_movement, model_state);
				}

				auto wpn = esp::local_player->GetActiveItem();
				auto held = wpn ? wpn->GetHeldEntity<BaseProjectile>() : nullptr;
				if (vars->combat.always_reload
					&& held)
				{
					misc::time_since_last_shot = (get_fixedTime() - misc::fixed_time_last_shot);
					vars->time_since_last_shot = misc::time_since_last_shot;
					if (misc::just_shot && (misc::time_since_last_shot > 0.2f))
					{
						held->ServerRPC(_(L"StartReload"));
						esp::local_player->console_echo(_(L"[matrix]: ClientInput - starting reload"));
						//esp::local_player->SendSignalBroadcast(rust::classes::Signal::Reload); //does this cause animation? YES
						misc::just_shot = false;
					}
					float reloadtime = *reinterpret_cast<float*>((uintptr_t)held + 0x2B8);//held->reloadTime();
					vars->reload = reloadtime;

					if (misc::time_since_last_shot > reloadtime //-10% for faster reloads than normal >:)
						&& !misc::did_reload)
					{
						esp::local_player->console_echo(_(L"[matrix]: ClientInput - finishing reload"));
						held->ServerRPC(_(L"Reload"));
						misc::did_reload = true;
						misc::time_since_last_shot = 0;
					}
				}
				else
				{
					misc::time_since_last_shot = 0;
				}
				if (!loco->is_sleeping()) {

					//auto nm = il2cpp::methods::object_new(il2cpp::init_class(_("GameObject"), _("UnityEngine")));
					//create(nm, _(L""));
					//if (vars->misc.autofarm
					//	&& !misc::hasNavigator) {
					//	misc::hasNavigator = true;
					//	auto comp = (BaseNavigator*)add_component((uintptr_t)((Networkable*)loco)->GetComponent<GameObject>(unity::GetType(_("UnityEngine"), _("GameObject"))),
					//		il2cpp::type_object(_(""), _("BaseNavigator")));
					//	if (misc::node.ent == 0)
					//		misc::node.ent = (BaseEntity*)loco->find_closest(_("OreResourceEntity"), (Networkable*)loco, 200.f);
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
						auto note = esp::local_player->ClientCurrentMapNote();
						if (note) {
							auto pos = esp::local_player->transform()->position();
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
						set_onLadder(model_state, true);
						if (settings::vert_flyhack > 1.5f
							|| settings::hor_flyhack > 4.f)
						{
							return;
						}
						else
							player_walk_movement->set_TargetMovement(Vector3(0, 25, 0));
					}
					else
					{
						set_onLadder(model_state, false);
					}
				}
				//if (esp::local_player) esp::local_player->console_echo(_(L"PWM flywall end"));
			}
		}
		else return orig::playerwalkmovement_client_input(player_walk_movement, inputstate, model_state);
	}

	void hk_projectile_launchprojectile(BaseProjectile* p)
	{
		auto held = esp::local_player->GetActiveItem()->GetHeldEntity<BaseProjectile>();
		if (vars->combat.doubletap
			&& !vars->combat.rapidfire)
		{
			auto m = held->repeatDelay() * .75f; //we can shoot 25% faster??? more bullets?? :DDD

			int r = vars->desyncTime / m;
			if (r > 1)
			{
				esp::local_player->console_echo(string::wformat(_(L"[matrix]: Launching %d projectiles!"), r));
				for (size_t i = 0; i < r; i++)
				{
					orig::baseprojectile_launchprojectile((uintptr_t)p);
					held->remove_ammo();
				}

				//auto mag = *reinterpret_cast<int*>((uintptr_t)held + 0x2C0);//p->primaryMagazine();
				//auto c = *reinterpret_cast<int*>((uintptr_t)mag + 0x1C); //0x1C = public int contents;
				//*reinterpret_cast<int*>((uintptr_t)mag + 0x1C) = (c - r);
				return;
			}
		}
		orig::baseprojectile_launchprojectile((uintptr_t)p);
		if (misc::manual || misc::autoshot) {
			//auto mag = *reinterpret_cast<int*>((uintptr_t)held + 0x2C0);//p->primaryMagazine();
			//auto c = *reinterpret_cast<int*>((uintptr_t)mag + 0x1C); //0x1C = public int contents;
			//*reinterpret_cast<int*>((uintptr_t)mag + 0x1C) = (c - 1);

			held->remove_ammo();
			misc::manual = false;
			misc::autoshot = false;
		}

		return;
	}

	void hk_projectile_update(uintptr_t pr) {

		if (vars->combat.throughladder)
		{
			for (size_t i = 0; i <= 0; i++)
			{
				auto p = (Projectile*)pr;
				if (!p->hitTest())
					break;
				if (p->penetrationPower() <= .5f) break;

				Vector3 vel = p->currentVelocity();
				Vector3 dir = vel.normalize();
				Vector3 traveledThisUpdate = vel * get_deltaTime();
				RaycastHit hitInfo;

				typedef bool(*AA)(Ray, float, RaycastHit*, float, int, int);

				if (!((AA)(mem::game_assembly_base + 0x7F58A0))(Ray(p->currentPosition(), dir),
					0.f,
					&hitInfo,
					traveledThisUpdate.length(),
					(int)rust::classes::layer::Deployed,
					0))
					break;
				typedef BaseEntity* (*A)(RaycastHit*);
				BaseEntity* ladder = ((A)(mem::game_assembly_base + 0x7C5120))(&hitInfo);
				if (!ladder || *reinterpret_cast<unsigned int*>(ladder + 0x4C) != 2150203378)
					break;
				RaycastHit hitInfo2;
				if (!((AA)(mem::game_assembly_base + 0x7F58A0))(Ray(hitInfo.point(), dir),
					0.f,
					&hitInfo2,
					0.5f,
					(int)rust::classes::layer::Construction,
					0))
					break;
				Transform* ladder_transform = ladder->transform();
				p->traveledDistance(p->traveledDistance() + traveledThisUpdate.length());
				p->traveledTime(p->traveledTime() + get_deltaTime());

				Vector3 pointOutsideWall = hitInfo2.point() + hitInfo2.normal() * 0.0005f;
				Vector3 pointBehindWall = hitInfo2.point() - hitInfo2.normal() * 0.2f;

				auto test = (HitTest2*)p->hitTest();
				test->set_hit_transform(ladder_transform);
				test->set_hit_entity((BasePlayer*)ladder);
				test->set_attack_ray({ hitInfo.point(), { 0, 1.f, 0 } });
				test->set_max_distance(0.5f);
				test->set_hit_point(ladder_transform->InverseTransformPoint(pointOutsideWall));
				test->set_hit_normal(ladder_transform->InverseTransformDirection(hitInfo2.normal()));
				test->set_did_hit(true);
				Do_Hit(p, (uintptr_t)test, hitInfo2.point(), hitInfo2.normal());

				p->traveledDistance(p->traveledDistance() + 0.2f);

				p->previousPosition(pointBehindWall);
				p->currentPosition(pointBehindWall);
				Transform* p_trans = p->transform();
				p_trans->setposition(pointBehindWall);
				p->currentVelocity(vel * 0.5f);
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

	void hk_baseplayer_ClientInput(BasePlayer* baseplayer, InputState* state)
	{
		int echocount = 0;
		__try {
			//if(!do_fixed_update_ptr)
			//do_fixed_update_ptr = mem::hook_virtual_function(_("PlayerWalkMovement"), _("DoFixedUpdate"), &hk_dofixedupdate);

			//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
			if (!client_input_ptr)
				client_input_ptr = mem::hook_virtual_function(_("PlayerWalkMovement"), _("ClientInput"), &hk_playerwalkmovement_ClientInput);

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

			//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
			if (!has_intialized_methods) {
				auto il2cpp_codegen_initialize_method = reinterpret_cast<void (*)(unsigned int)>(il2cpp::methods::intialize_method);
				//56229 for real rust or 56204 for cracked rust
				for (int i = 0; i <
					56229//56204 //56229 = real rust
					; i++) {
					il2cpp_codegen_initialize_method(i);
				}
				has_intialized_methods = true;

				//HERE UNTIL THEY REMOVE IT AGAIN /S

				fired_projectile placeholder = { nullptr, 0, 1 };
				for (size_t i = 0; i < 32; i++)
					misc::fired_projectiles[i] = placeholder;
			}
			//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

#pragma region static_method_hooks
			static uintptr_t* serverrpc_projecshoot;
			if (!serverrpc_projecshoot) {
				//auto method_serverrpc_projecshoot = *reinterpret_cast<uintptr_t*>(hooks::serverrpc_projecileshoot);
				auto method_serverrpc_projecshoot = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileShoot___);
			
				if (method_serverrpc_projecshoot) {
					serverrpc_projecshoot = **(uintptr_t***)(method_serverrpc_projecshoot + 0x30);
			
					hooks::orig::serverrpc_projectileshoot = *serverrpc_projecshoot;
			
					*serverrpc_projecshoot = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_projectileshoot);
				}
			}
			
			static uintptr_t* serverrpc_playerprojectileattack;
			if (!serverrpc_playerprojectileattack) {
				auto method_serverrpc_playerprojectileattack = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_PlayerProjectileAttack___);//Method$BaseEntity_ServerRPC_PlayerProjectileAttack___
			
				if (method_serverrpc_playerprojectileattack) {
					serverrpc_playerprojectileattack = **(uintptr_t***)(method_serverrpc_playerprojectileattack + 0x30);
			
					hooks::orig::playerprojectileattack = *serverrpc_playerprojectileattack;
			
					*serverrpc_playerprojectileattack = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_playerprojectileattack);
				}
			}
			static uintptr_t* serverrpc_createbuilding;
			if (!serverrpc_createbuilding) {
				auto method_serverrpc_createbuilding = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_CreateBuilding___);
			
				if (method_serverrpc_createbuilding) {
					serverrpc_createbuilding = **(uintptr_t***)(method_serverrpc_createbuilding + 0x30);
			
					hooks::orig::createbuilding = *serverrpc_createbuilding;
			
					*serverrpc_createbuilding = reinterpret_cast<uintptr_t>(&hooks::hk_serverrpc_doplace);
				}
			}
#pragma endregion

			auto loco = esp::local_player;
			if (baseplayer && loco) {
				get_skydome();
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				auto fixed_time = get_fixedTime();
				auto tick_time = baseplayer->lastSentTickTime();
				vars->desyncTime = (unity::get_realtimesincestartup() - tick_time) - 0.03125 * 3;
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

				auto wpn = baseplayer->GetActiveItem();
				auto held = wpn ? wpn->GetHeldEntity<BaseProjectile>() : nullptr;
				float time = get_fixedTime();
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

				auto item2 = baseplayer->get_active_weapon();

				if (vars->misc.attack_on_mountables) {
					auto mountable = baseplayer->mounted();
					if (mountable)
						mountable->canwielditem() = true;
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (unity::GetKey(0x37))//(GetAsyncKeyState(0x37))
				{
					auto look = baseplayer->_lookingAtEntity();
					if (look)
					{
						auto net2 = *reinterpret_cast<Networkable**>(look + 0x58);
						auto look_id = net2->get_id();
						esp::selected_entity_id = look_id;
					}
				}

				bool kyslol = false;

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (vars->misc.TakeFallDamage && unity::GetKey(vars->keybinds.suicide)) {

					OnLand(baseplayer, -8.0001f - 100);
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (vars->misc.admin_mode)
					baseplayer->set_admin_flag(rust::classes::PlayerFlags::IsAdmin);

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				baseplayer->fov();

				typedef void(*_set_rayleigh)(float);
				((_set_rayleigh)(mem::game_assembly_base + oSetRayleigh))(vars->visual.rayleigh);

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (held) {
					if (!LI_FIND(strcmp)(held->get_class_name(), _("Planner"))) {
						auto planner = reinterpret_cast<Planner*>(held);
						if (unity::GetKeyDown(rust::classes::KeyCode::RightArrow))
						{
							auto v = planner->rotationoffset();
							planner->rotationoffset(Vector3(v.x, v.y += 10, v.z));
							esp::local_player->console_echo(string::wformat(_(L"[matrix]: ClientInput - rotate building right (%d, %d, %d)"), (int)v.x, (int)v.y, (int)v.z));
						}
						else if (unity::GetKeyDown(rust::classes::KeyCode::LeftArrow))
						{
							auto v = planner->rotationoffset();
							planner->rotationoffset(Vector3(v.x, v.y -= 10, v.z));
							esp::local_player->console_echo(string::wformat(_(L"[matrix]: ClientInput - rotate building left (%d, %d, %d)"), (int)v.x, (int)v.y, (int)v.z));
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
					if (vars->visual.rocketprediction) {
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
						&& esp::best_target.distance < vars->combat.melee_range)
					{
						auto hit_player = [&]() {
							auto Item = esp::local_player->GetActiveItem();
							if (Item) {
								auto melee = Item->GetHeldEntity<BaseMelee>();
								if (melee) {
									auto class_name = melee->get_class_name();
									if (*(int*)(class_name + 4) == 'eleM' || *(int*)(class_name + 4) == 'mmah') {
										aim_target target = esp::best_target;
										if (target.ent) {
											auto world_position = target.ent->model()->boneTransforms()->get(48)->position();
											auto local = esp::local_player->ClosestPoint(world_position);
											auto camera = esp::local_player->model()->boneTransforms()->get(48)->position();

											if (camera.get_3d_dist(world_position) >= 4.2f)
												return;


											target.visible = esp::local_player->is_visible(camera, local);

											attack_melee(target, (BaseProjectile*)melee, esp::local_player, true);
										}
									}
								}
							}
						};
						hit_player();
					}
					//if (!wcscmp(wpn->get_weapon_name(), _(L"Jackhammer"))) {
					//	auto method = mem::read<uintptr_t>(mem::game_assembly_base + offsets::Method$BaseEntity_ServerRPC_uintstring);
					//	if(method)
					//		ServerRPC_intstring(baseplayer, _(L"itemcmd"), wpn->uid(), _(L"refill"), method);
					//}
				}
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
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
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));


				//desync on key
				if (unity::GetKey(vars->keybinds.desync_ok)
					|| ((vars->combat.manipulator2 || vars->combat.manipulator)
						&& (unity::GetKey(vars->keybinds.manipulator)
							|| misc::manipulate_vis)))
					baseplayer->clientTickInterval() = 0.99f;
				else if (!is_lagging && !is_speeding)
					baseplayer->clientTickInterval() = 0.05f;

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

				if (!vars->misc.fake_lag || unity::GetKey(vars->keybinds.fakelag)) {
					if (!is_lagging && !flying && vars->misc.fake_lag && !is_speeding) {
						baseplayer->clientTickInterval() = 0.4f;
						is_lagging = true;
					}
				}
				else if (is_lagging && flying || is_lagging && is_speeding) {
					esp::local_player->clientTickInterval() = 0.03f;
					is_lagging = false;
				}
				else if (is_lagging && !vars->misc.fake_lag) {
					esp::local_player->clientTickInterval() = 0.05f;
					is_lagging = false;
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (vars->misc.eyeoffset || unity::GetKey(vars->keybinds.neck))
				{
					baseplayer->eyes()->set_view_offset(Vector3(0, vars->misc.PlayerEyes, 0));
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
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
							ModelState->set_flag(rust::classes::ModelState_Flag::OnGround);
							mem::write<float>((uint64_t)movement + 0x4C, 1); //private float <Grounded>k__BackingField; // 0x4C
						}
					}
				}
				auto item = baseplayer->GetActiveItem();

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				//float mm_eye = ((0.01f + ((vars->desyncTime + 2.f / 60.f + 0.125f) * baseplayer->max_velocity())));
				bool IsMounted = esp::local_player->modelState()->has_flag(rust::classes::ModelState_Flag::Mounted);
				float maxVelocity = get_maxspeed(esp::local_player);
				if (IsMounted)
					maxVelocity *= 4;
				float _timeSinceLastTick = unity::get_realtimesincestartup() - esp::local_player->lastSentTickTime();
				float timeSinceLastTickClamped = max(0.f, min(_timeSinceLastTick, 1.f));
				float mm_eye = 0.1f + (timeSinceLastTickClamped + 2.f / 60.f) * 1.5f * maxVelocity;

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (esp::best_target.ent && held && wpn)
				{
					//old manip
					if (vars->combat.manipulator && ((unity::GetKey(vars->keybinds.manipulator))
						|| misc::manipulate_vis))
					{
						Vector3 target = esp::best_target.pos;
						auto mag_ammo = held->ammo_left();
						float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
						if (misc::can_manipulate(baseplayer, target, mm_eye))
							if (nextshot < time
								&& (held->timeSinceDeploy() > held->deployDelay() ||
									!strcmp(held->get_class_name(), _("BowWeapon")) ||
									!strcmp(held->get_class_name(), _("CompoundBowWeapon")) ||
									!strcmp(held->get_class_name(), _("CrossbowWeapon")))
								&& mag_ammo > 0)
							{
								auto v = esp::local_player->eyes()->position() + misc::best_lean;
								esp::local_player->console_echo(string::wformat(_(L"[matrix]: ClientInput - manipulator attempted shot from position (%d, %d, %d) with desync of %d"), (int)v.x, (int)v.y, (int)v.z, (int)(vars->desyncTime * 100.f)));

								misc::manual = true;
								hk_projectile_launchprojectile(held);
								misc::best_target = Vector3(0, 0, 0);
								baseplayer->SendClientTick();
							}
					}
					else if ((vars->combat.manipulator2
						&& esp::best_target.ent->is_alive()
						&& !(!GetAsyncKeyState(vars->keybinds.manipulator)))
						|| misc::manipulate_vis)
					{
						Vector3 target = esp::best_target.pos;
						auto mag_ammo = held->ammo_left();
						if (wpn && held && esp::best_target.ent) {
							float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
							if (CanManipulate(held, (BasePlayer*)esp::best_target.ent, baseplayer->input()->state()))
								if (nextshot < time
									&& (held->timeSinceDeploy() > held->deployDelay() ||
										!strcmp(held->get_class_name(), _("BowWeapon")) ||
										!strcmp(held->get_class_name(), _("CompoundBowWeapon")) ||
										!strcmp(held->get_class_name(), _("CrossbowWeapon")))
									&& held->ammo_left() > 0)
								{
									auto v = settings::RealGangstaShit;//esp::local_player->eyes()->get_position() + misc::best_lean;
									esp::local_player->console_echo(string::wformat(_(L"[matrix]: ClientInput - manipulator attempted shot from position (%d, %d, %d) with desync of %d"), (int)v.x, (int)v.y, (int)v.z, (int)(vars->desyncTime * 100.f)));

									misc::manual = true;
									hooks::hk_projectile_launchprojectile(held);
									baseplayer->SendClientTick();
								}
						}
					}

					if (((vars->combat.autoshoot || unity::GetKey(vars->keybinds.autoshoot))
						&& esp::best_target.ent->is_alive())
						|| vars->wants_shoot)
					{
						Vector3 target = esp::best_target.pos;
						auto mag_ammo = held->ammo_left();
						float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
						if ((baseplayer->is_visible(target, baseplayer->model()->boneTransforms()->get(48)->position())
							&& get_fixedTime() > nextshot
							&& held->timeSinceDeploy() > held->deployDelay()
							&& mag_ammo > 0))
						{
							misc::autoshot = true;
							misc::autobot::wants_shoot = false;
							hooks::hk_projectile_launchprojectile(held);
							baseplayer->SendClientTick();
						}
						else if (!baseplayer->is_visible(baseplayer->model()->boneTransforms()->get(48)->position(), target)
							&& vars->combat.manipulator2
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

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (vars->misc.speedhack && unity::GetKey(vars->keybinds.timescale)) {
					set_timeScale(vars->misc.speedhackspeed);
					is_speeding = true;
				}
				else
				{
					set_timeScale(1);
					is_speeding = false;
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d before item"), echocount++));
				auto target = esp::best_target; //baseplayer->get_aimbot_target(unity::get_camera_pos());
				if (item) {
					auto baseprojectile = *reinterpret_cast<BaseProjectile**>((uintptr_t)item + heldEntity);//item->GetHeldEntity<BaseProjectile>();
					if (baseprojectile) {
						for (int i = 0; i < 32; i++) {
							break;
							auto current = misc::fired_projectiles[i];
							if (current.fired_at <= 2.f)
								continue;
							//kill original so no updates wasted by game
						//auto original = current.original;

							auto projectile = current.original;
							if (!projectile->authoritative())
								continue;

							//we will be the ones who update the projectile, not the game
						//auto delta = get_deltaTime();
						//auto updates = current.updates++;
						//auto next_update_time = current.fired_at + (updates * delta);
						//bool ret = false;
						//if (get_fixedTime() > next_update_time)
						//	if (projectile->IsAlive())
						//	{
						//		projectile->UpdateVelocity(delta, projectile);
						//	}

							float offset = 0.f;
							auto target = current.ent;
							if (vars->combat.thick_bullet
								&& projectile->authoritative()
								&& projectile->IsAlive()
								&& false)
								//&& vars->combat.thickness > 1.1f)//)
							{
								if (target.ent)
								{
									auto current_position = projectile->transform()->position();

									//transform* bonetrans = target.player->find_closest_bone(current_position, true
									//Transform* bonetrans = target.ent->model()->boneTransforms()->get(48);
									auto bone = ((BasePlayer*)esp::best_target.ent)->closest_bone(esp::local_player, current_position);

									if (bone)
									{
										Vector3 target_bone = bone->position; //target_bone.y -= 0.8f;
									//Sphere(target_bone, 2.2f, col(12, 150, 100, 50), 10.f, 100.f);

										if (misc::LineCircleIntersection(target_bone, vars->combat.thickness, current_position, projectile->previousPosition(), offset))
										{
											current_position = Vector3::move_towards(target_bone, current_position, vars->combat.thickness);
										}

										auto dist = target_bone.distance(current_position);


										//fuck with shit pussy wagon
										float num2 = 1.0f + 0.5f;
										float num8 = 2.0f / 60.0f;
										float num9 = 2.0f * max(max(get_deltaTime(), get_smoothdeltaTime()), get_fixeddeltaTime());
										float num11 = (vars->desyncTime + num8 + num9) * num2;
										//typedef Vector3(*gpv)(uintptr_t);
									//auto pv = ((gpv)(mem::game_assembly_base + 8331264))((uintptr_t)target.player);
										auto pv = target.ent->GetParentVelocity();
										float mag = pv.length();
										float num15 = 0.1f + num11 * mag + 0.1f;
										//dist -= num15;

										/*if (target.player->get_mountable())
										{
											Sphere(target_bone, 3.0f, col(12, 150, 100, 50), 10.f, 100.f);
											if (dist < 3.0f)
											{
												auto newpos = Vector3::move_towards(target_bone, current_position, 1.0f);
												set_position(get_transform((base_player*)projectile), newpos);

												HitTest* ht = (HitTest*)projectile->hitTest();
												ht->set_did_hit(true);
												ht->set_hit_entity(target.player);
												ht->set_hit_transform(bonetrans);
												ht->set_hit_point(InverseTransformPoint(bonetrans, newpos));
												ht->set_hit_normal(InverseTransformDirection(bonetrans, newpos));
												Ray r(get_position((uintptr_t)get_transform((base_player*)projectile)), newpos);
												safe_write(ht + 0x14, r, Ray);

												esp::local_player->console_echo(string::wformat(_(L"[matrix]: Fat bullet - Called with distance: %dm"), (int)dist));
												DoHit(projectile, ht, newpos, HitNormalWorld((uintptr_t)ht));
											}
										}*/
										if (dist < 1.8f)
										{
											auto newpos = Vector3::move_towards(current_position, target_bone, 1.0f);
											set_position(projectile->transform(), newpos);


											HitTest* ht = (HitTest*)projectile->hitTest();
											ht->DidHit() = true;
											ht->HitEntity() = target.ent;
											ht->HitTransform() = bone->transform;
											ht->HitPoint() = bone->transform->InverseTransformPoint(newpos);
											ht->HitNormal() = bone->transform->InverseTransformDirection(newpos);
											Ray r(projectile->transform()->position(), newpos);
											safe_write(ht + 0x14, r, Ray);
											_DoHit(projectile, ht, newpos, HitNormalWorld((uintptr_t)ht));
										}
									}
								}
							}
							else if (time - current.fired_at > 8.f) {
								misc::fired_projectiles[i] = { nullptr, nullptr, 1, 0 };
							}
						}

						auto wep_class_name = *(const char**)(*(uintptr_t*)(uintptr_t)baseprojectile + 0x10);

						auto attack = [&](aim_target target, bool is_tree) {
							auto gathering = ((BaseMelee*)baseprojectile)->gathering();

							auto baseprojectile = item2->get_base_projetile();

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

							attack_melee(target, baseprojectile, esp::local_player);
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
							Vector3 eyes = esp::local_player->eyes()->position();
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
								//esp::local_player->console_echo(string::wformat(_(L"[matrix]: ClientInput - yaw: %d, pitch: %d"), (int)yaw, (int)pitch));
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
									esp::matrix = unity::get_view_matrix();
									auto camera_pos = unity::get_camera_pos();

									auto target = esp::best_target; //baseplayer->get_aimbot_target(camera_pos);

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
										mem::write<float>((uint64_t)baseprojectile + 0x378, 1.f); //eoka success fraction
										mem::write<bool>((uint64_t)baseprojectile + 0x388, true); //eoka _didSparkThisFrame
									}
									else {
										if (vars->combat.rapidfire)
											baseprojectile->repeatDelay() = vars->combat.firerate;
										if (vars->combat.automatic)
											baseprojectile->automatic() = true;
									}

									if (vars->combat.fast_bullet)
										baseprojectile->projectileVelocityScale() = 1.48f;
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

				//baseplayer->console_echo(string::wformat(_(L"echocount %d after item"), echocount++));
				if (vars->misc.instant_revive) {
					auto Target = baseplayer->resolve_closest_player(5);
					if (Target.ent)
					{
						if (vars->misc.revivefriendsonly ? ((BasePlayer*)Target.ent)->isFriend() : true)
						{
							//Line(baseplayer->eyes()->position(), Target.pos, { 1, 1, 1, 1 }, 0.01f, false, false);
							if (!Target.is_heli 
								&& Target.distance <= 5 
								&& HasPlayerFlag((BasePlayer*)Target.ent, rust::classes::PlayerFlags::Wounded)
								&& get_fixedTime() > last_revive_rpc + .5f)
							{
								Target.ent->ServerRPC(_(L"RPC_Assist"));
								last_revive_rpc = get_fixedTime();
							}
						}
					}
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				if (vars->misc.gesture_spam > 1
					&& get_fixedTime() > last_gesture_rpc + 0.35f)
				{
					switch (vars->misc.gesture_spam) {
					case 0:
						break;
					case 1:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"clap"));
						break;
					case 2:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"friendly"));
						break;
					case 3:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"thumbsdown"));
						break;
					case 4:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"thumbsup"));
						break;
					case 5:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"ok"));
						break;
					case 6:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"point"));
						break;
					case 7:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"shrug"));
						break;
					case 8:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"victory"));
						break;
					case 9:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"wave"));
						break;
					case 10:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"dance.cabbagepatch"));
						break;
					case 11:
						esp::local_player->SendSignalBroadcast(rust::classes::Signal::Gesture, _(L"dance.twist"));
						break;
					}
					last_gesture_rpc = get_fixedTime();
				}

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				unity::IgnoreLayerCollision(rust::classes::layer::PlayerMovement, rust::classes::layer::Water, !vars->misc.no_playercollision);
				unity::IgnoreLayerCollision(rust::classes::layer::PlayerMovement, rust::classes::layer::Tree, vars->misc.no_playercollision);
				unity::IgnoreLayerCollision(rust::classes::layer::PlayerMovement, rust::classes::layer::AI, vars->misc.no_playercollision);

				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));
				draw_get();
				if (tick_time > vars->tick_time_when_called + 10) {
					unity::camera = unity::get_main_camera();
					vars->tick_time_when_called = tick_time;
				}
				//baseplayer->console_echo(string::wformat(_(L"echocount %d"), echocount++));

				if (vars->misc.autofarm
					|| vars->misc.walktomarker) {
					if (misc::node.pos != Vector3(0, 0, 0))
					{
						auto dir = (misc::node.pos - baseplayer->eyes()->position()).Normalized();
						state->set_aim_angles(dir);
					}
				}
				//baseplayer->console_echo(string::wformat(_(L"echocount %d end of main if"), echocount++));
			}
			else vars->visual.snapline = 0;


			//baseplayer->console_echo(string::wformat(_(L"echocount %d execute original"), echocount++));
			orig::baseplayer_client_input(baseplayer, state);

			//baseplayer->console_echo(string::wformat(_(L"echocount %d before spinbot"), echocount++));
			if (baseplayer)
			{
				if (baseplayer->is_sleeping()) return;
				auto model_state = baseplayer->modelState();

				Vector3 real_angles = safe_read(state + 0x18, Vector3);
				Vector3 spin_angles = Vector3::Zero();
				bool targeted = vars->targetted;
				int jitter_speed = 10;
				int spin_speed = vars->misc.spinspeed;
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
					state->set_aim_angles(spin_angles);
				vars->targetted = false;
			}

			//baseplayer->console_echo(string::wformat(_(L"echocount %d returning"), echocount++));
		}
		__except (true) {
			//baseplayer->console_echo(string::wformat(_(L"echocount %d __except return"), echocount++));
			return;
		}
	}
}
