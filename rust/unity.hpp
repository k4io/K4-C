#pragma once

#include "../memory/il2cpp.hpp"

#include "rust.hpp"


namespace unity {
	//static auto set_lockstate = reinterpret_cast<void(*)(CursorLockMode)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Cursor"), _("set_lockState"), 1, _("value"), _("UnityEngine"))));

	static auto get_width = reinterpret_cast<int (*)()>(0);

	static auto get_height = reinterpret_cast<int (*)()>(0);
	
	static auto get_main_camera = reinterpret_cast<uintptr_t(*)()>(0);

	static auto IgnoreLayerCollision = reinterpret_cast<void(*)(layer, layer, bool)>(0);

	static auto LineOfSight = reinterpret_cast<bool(*)(Vector3, Vector3, int, float)>(0);

	static auto LineOfSightInternal = reinterpret_cast<bool(*)(Vector3, Vector3, int, float, float, float, uintptr_t)>(0);

	static auto get_shader = reinterpret_cast<uintptr_t(*)(uintptr_t material)>(0);

	static auto set_shader = reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(0);

	static auto get_realtimesincestartup = reinterpret_cast<float(*)()>(0);

	static auto Raycast = reinterpret_cast<bool (*)(Ray, float, int)>(0);
	
	static auto CheckCapsule = reinterpret_cast<bool (*)(Vector3, Vector3, float, int, int)>(0);
	
	static auto LoadFromFile = reinterpret_cast<uintptr_t(*)(System::string path)>(0);

	static auto LoadAsset = reinterpret_cast<uintptr_t(*)(uintptr_t bundle, System::string path, uintptr_t type)>(0);

	static auto get_Scattering = reinterpret_cast<uintptr_t(*)(uintptr_t)>(0);

	void init_unity() {
		get_shader = reinterpret_cast<uintptr_t(*)(uintptr_t material)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oMaterialSetShader));

		set_shader = reinterpret_cast<void(*)(uintptr_t, uintptr_t)>(il2cpp::methods::resolve_icall(_("UnityEngine.Material::set_shader()")));

		get_width = reinterpret_cast<int (*)()>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oScreenGetWidth));

		get_height = reinterpret_cast<int (*)()>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oScreenGetHeight));

		get_main_camera = reinterpret_cast<uintptr_t(*)()>(il2cpp::methods::resolve_icall(_("UnityEngine.Camera::get_main()")));

		IgnoreLayerCollision = reinterpret_cast<void(*)(layer, layer, bool)>(il2cpp::methods::resolve_icall(_("UnityEngine.Physics::IgnoreLayerCollision()")));

		LineOfSight = reinterpret_cast<bool(*)(Vector3, Vector3, int, float)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oLineOfSight));

		//LineOfSightRadius = reinterpret_cast<bool(*)(Vector3, Vector3, Layers, float, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GamePhysics"), _("LineOfSightRadius"), 5, _(""), _(""))));

		//GetKey = reinterpret_cast<bool(*)(KeyCode)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Input"), _("GetKeyInt"), 1, _(""), _("UnityEngine"))));

		get_realtimesincestartup = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oGetRealtimeSinceStartup));

		Raycast = reinterpret_cast<bool (*)(Ray, float, int)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oRaycast));

		LineOfSightInternal = reinterpret_cast<bool(*)(Vector3, Vector3, int, float, float, float, uintptr_t)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oLineOfSightInternal));

		LoadFromFile = reinterpret_cast<uintptr_t(*)(System::string path)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oLoadFromFile));

		LoadAsset = reinterpret_cast<uintptr_t(*)(uintptr_t bundle, System::string path, uintptr_t type)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oLoadAsset));

		CheckCapsule = reinterpret_cast<bool (*)(Vector3, Vector3, float, int, int)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oCheckCapsule));
		
		get_Scattering = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oGetScattering));
	}

	uintptr_t bundle;
	uintptr_t bundle2;
	uintptr_t galaxy_bundle;
	uintptr_t bundle_font;
	uintptr_t chams_shader_normal;
	uintptr_t chams_shader_seethru;
	uintptr_t chams_shader_wireframe;
	uintptr_t chams_shader_lit;
	uintptr_t font_ptr;

	uintptr_t galaxy_material;
	/*
	bool is_visible(Vector3 source, Vector3 destination, base_player* entity, float radius = .5f) {
		//auto layer = (int)Layers::ProjectileLineOfSightCheck | (int)Layers::Terrain;

		return LineOfSightRadius(source, destination, Layers(10551296), radius, 0, entity) && LineOfSightRadius(destination, source, Layers(10551296), radius, 0, entity);
	}
*/
	bool GetKey(int key) {
		return !(!GetAsyncKeyState(key));
	}	

	uintptr_t GetType(const char* space, const char* name) {
		return il2cpp::type_object(space, name);
	}

	bool LineOfSightRadius(Vector3 source, Vector3 destination, uintptr_t ent, float p1 = 0.18f) {
		__try {
			//auto layer = (int)Layers::ProjectileLineOfSightCheck | (int)Layers::Terrain | (int)Layers::z;
			int layer = vars->combat.pierce ? 10551296 : 1218519041;
			//typedef bool (*AAA)(Vector3, Vector3, Layers, float, uintptr_t);
			//if (destination.is_empty() || source.is_empty()) return false;
			//return ((AAA)(mem::game_assembly_base + oLineOfSightRadius))(source, destination, (Layers)layer, p1, ent);
			return LineOfSightInternal(source, destination, layer, p1, 0, 0, ent);
		}
		__except(true) { return false; }
	}

	bool is_visible(Vector3 source, Vector3 destination, uintptr_t ent, float p1 = 0.18f) {
		return LineOfSightRadius(source, destination, ent, p1)
			&& LineOfSightRadius(destination, source, ent, p1);
	}

	//auto camera = unity::get_main_camera();
	uintptr_t camera = 0;

	VMatrix get_view_matrix() {
		if (!camera) {
			camera = unity::get_main_camera();
		}

		auto camera_ = mem::read<uintptr_t>(camera + 0x10);
		if (!camera_) {
			camera = unity::get_main_camera();
			return {};
		}

		auto matrix = mem::read<VMatrix>(camera_ + 0x2E4);
		if (!matrix.m) {
			camera = unity::get_main_camera();
			return {};
		}

		return matrix;
	}

	Vector3 get_camera_pos() {
		if (!camera) {
			camera = unity::get_main_camera();
		}

		auto camera_ = mem::read<uintptr_t>(camera + 0x10);
		if (!camera_) {
			camera = unity::get_main_camera();
			return {};
		}

		auto matrix = mem::read<Vector3>(camera_ + 0x42C);
		if (!matrix.x || !matrix.y) {
			camera = unity::get_main_camera();
			return {};
		}


		return matrix;
	}
	float get_fov(Vector3 Pos);
}