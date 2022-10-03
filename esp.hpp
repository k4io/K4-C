#pragma once
#include "RenderClass.h"
#include "vars.hpp"

#include "rust/features/player_esp.hpp"
//#include "rust/classes.hpp"

D2D1::ColorF HSVD(float h, float s, float v, float a = 1.0f) 
{ 
	float r, g, b; ColorConvertHSVtoRGB(h, s, v, r, g, b); return D2D1::ColorF(r, g, b, a);
}

void DrawPlayer(BasePlayer* ply, bool npc)
{
	if (!ply) return;
	if (!esp::local_player) return;

	esp::do_chams(ply);

	esp::bounds_t bounds;

	struct bone_t {
		Vector3 screen;
		int8_t     index;
		bool       on_screen;
		Vector3 world_position;
		bool visible;
	};

	std::array<bone_t, 20> bones = {
		bone_t{ Vector3{}, 2, false, Vector3{}, false },  // l_hip
		bone_t{ Vector3{}, 3, false, Vector3{}, false },  // l_knee
		bone_t{ Vector3{}, 6, false, Vector3{}, false },  // l_ankle_scale
		bone_t{ Vector3{}, 5, false, Vector3{}, false },  // l_toe
		bone_t{ Vector3{}, 24, false, Vector3{}, false }, // l_upperarm
		bone_t{ Vector3{}, 25, false, Vector3{}, false }, // l_forearm
		bone_t{ Vector3{}, 26, false, Vector3{}, false }, // l_hand
		bone_t{ Vector3{}, 27, false, Vector3{}, false }, // l_index1

		bone_t{ Vector3{}, 48, false, Vector3{}, false }, // jaw
		bone_t{ Vector3{}, 18, false, Vector3{}, false }, // spine1
		bone_t{ Vector3{}, 21, false, Vector3{}, false }, // spine3
		bone_t{ Vector3{}, 1, false, Vector3{}, false },  // pelvis
		bone_t{ Vector3{}, 23, false, Vector3{}, false }, // l_clavicle

		bone_t{ Vector3{}, 17, false, Vector3{}, false }, // r_ankle_scale
		bone_t{ Vector3{}, 15, false, Vector3{}, false }, // r_foot
		bone_t{ Vector3{}, 14, false, Vector3{}, false }, // r_knee
		bone_t{ Vector3{}, 55, false, Vector3{}, false }, // r_upperarm
		bone_t{ Vector3{}, 56, false, Vector3{}, false }, // r_forearm
		bone_t{ Vector3{}, 57, false, Vector3{}, false }, // r_hand
		bone_t{ Vector3{}, 76, false, Vector3{}, false }  // r_ulna
	};

	bool is_visible = false, is_teammate = ply->is_teammate(esp::local_player);
	auto camera_position = unity::get_camera_pos();

	const auto get_bounds = [&](esp::bounds_t& out, float expand = 0) -> bool {
		bounds = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

		for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {

			auto Transform = ply->model()->boneTransforms()->get(bone_idx);

			world_position = Transform->get_position();

			if (bone_idx == 48) // lol
				world_position.y += 0.2f;

			bone_screen = WorldToScreen(world_position);
			if (bone_screen.x < bounds.left)
				bounds.left = bone_screen.x;
			if (bone_screen.x > bounds.right)
				bounds.right = bone_screen.x;
			if (bone_screen.y < bounds.top)
				bounds.top = bone_screen.y;
			if (bone_screen.y > bounds.bottom)
				bounds.bottom = bone_screen.y;
			on_screen = true;
		}

		if (bounds.left == FLT_MAX)
			return false;
		if (bounds.right == FLT_MIN)
			return false;
		if (bounds.top == FLT_MAX)
			return false;
		if (bounds.bottom == FLT_MIN)
			return false;

		bounds.left -= expand;
		bounds.right += expand;
		bounds.top -= expand;
		bounds.bottom += expand;

		bounds.center = bounds.left + ((bounds.right - bounds.left) / 2);

		out = bounds;

		return true;
	};

	if (get_bounds(bounds, 4)) {
		is_visible = unity::is_visible(camera_position, bones[8].world_position, (uintptr_t)esp::local_player);
		//for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {
		//	if (is_visible) break;
		//	is_visible = unity::is_visible(camera_position, world_position, (uintptr_t)esp::local_player);
		//}

		//safezone flag???

		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;

		auto name = ply->_displayName()->str;
		auto activeitem = ply->GetActiveItem();


		//boxes
		auto box_color = is_visible ? vars->colors.players.boxes.visible : vars->colors.players.boxes.invisible;
		switch (vars->visual.boxtype)
		{
		case 1: //full box
			render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, { 0,0,0,1 }, 2.f);
			render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			break;
		case 2: //corner box

			//top left
			render.Line({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			//top right
			render.Line({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			//bottom left
			render.Line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			//bottom right
			render.Line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, { 0,0,0,1 }, 2.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, { 0,0,0,1 }, 1.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 1.f);
			break;
		case 3: //3d cube
			//LMAO I WILL DO THIS LATER
			break;
		}


		//health bar
		const auto cur_health = ply->health();
		const auto max_health = (npc ? ply->maxHealth() : 100);
		const auto health_pc = min(cur_health / max_health, 1);
		const auto health_color =
			HSVD((health_pc * .25f), 1, .875f * 1);
		const auto h = (bounds.bottom - bounds.top) * health_pc;
		switch (vars->visual.hpbar)
		{
		case 1:
			render.FillRectangle({ bounds.left - 7, bounds.top - 1 }, { 4, box_height + 3 }, { 0,0,0,1 });
			render.FillRectangle({ bounds.left - 6, bounds.top }, { 2, box_height - h + 1 }, health_color);
			break;
		case 2:
			render.FillRectangle({ bounds.left, bounds.bottom + 6 }, { box_width + 1, 4 }, { 0,0,0,1 });
			render.FillRectangle({ bounds.left + 1, bounds.bottom + 7 }, { ((box_width / max_health) * cur_health) - 1, 2 }, health_color);
			break;
		}


		//skeleton
		if (vars->visual.skeleton)
		{
			//jaw -> spine4
			//spine4 -- l_upperarm -> l_lowerarm -> l_hand -> (make hands)
			//spine4 -- r_upperarm -> r_lowerarm -> r_hand -> (make hands)
			//spine4 -> spine3
			//spine3 -> pelvis
			//pelvis -- l_knee -> l_ankle_scale -> l_foot
			//pelvis -- r_knee -> r_ankle_scale -> r_foot
			/*
			for (size_t i = 0; i < 17; i++)
			{
				int id = skeleton_boneids[i++];
				auto transform = player->model()->boneTransforms()->get(id);
				Vector3 world_position = transform->get_bone_position();
				Vector3 v1 = WorldToScreen(world_position);
				if (id > 17) break;
				transform = player->model()->boneTransforms()->get(id);
				world_position = transform->get_bone_position();
				Vector3 v2 = WorldToScreen(world_position);
				if (v1.y >= 1080 || v1.x >= 1920 || v1.x <= 0 || v1.y <= 0) continue;
				if (v2.y >= 1080 || v2.x >= 1920 || v2.x <= 0 || v2.y <= 0) continue;
				gui::line(Vector2(v1.x, v1.y), Vector2(v2.x, v2.y), health_color);
			}
			*/

			//jaw
			auto Transform = ply->model()->boneTransforms()->get(48);
			if (!Transform) return;
			Vector3 world_position = Transform->get_position();
			Vector3 jaw = WorldToScreen(world_position);

			//spine4
			Transform = ply->model()->boneTransforms()->get(22);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 spine4 = WorldToScreen(world_position);

			//spine3
			Transform = ply->model()->boneTransforms()->get(21);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 spine3 = WorldToScreen(world_position);

			//pelvis
			Transform = ply->model()->boneTransforms()->get(7);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 pelvis = WorldToScreen(world_position);

			//l_hip
			Transform = ply->model()->boneTransforms()->get(3);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_hip = WorldToScreen(world_position);

			//r_knee
			Transform = ply->model()->boneTransforms()->get(14);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_knee = WorldToScreen(world_position);

			//l_ankle_scale
			Transform = ply->model()->boneTransforms()->get(6);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_ankle_scale = WorldToScreen(world_position);

			//r_ankle_scale
			Transform = ply->model()->boneTransforms()->get(17);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_ankle_scale = WorldToScreen(world_position);

			//r_foot
			Transform = ply->model()->boneTransforms()->get(15);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_foot = WorldToScreen(world_position);

			//l_foot
			Transform = ply->model()->boneTransforms()->get(4);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_foot = WorldToScreen(world_position);

			//r_upperarm
			Transform = ply->model()->boneTransforms()->get(55);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_upperarm = WorldToScreen(world_position);

			//l_upperarm
			Transform = ply->model()->boneTransforms()->get(24);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_upperarm = WorldToScreen(world_position);

			//r_forearm
			Transform = ply->model()->boneTransforms()->get(56);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_forearm = WorldToScreen(world_position);

			//l_forearm
			Transform = ply->model()->boneTransforms()->get(25);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_forearm = WorldToScreen(world_position);

			//r_hip
			Transform = ply->model()->boneTransforms()->get(13);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_hip = WorldToScreen(world_position);

			//l_knee
			Transform = ply->model()->boneTransforms()->get(2);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_knee = WorldToScreen(world_position);

			//l_hand
			Transform = ply->model()->boneTransforms()->get(26);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 l_hand = WorldToScreen(world_position);

			//r_hand
			Transform = ply->model()->boneTransforms()->get(57);
			if (!Transform) return;
			world_position = Transform->get_position();
			Vector3 r_hand = WorldToScreen(world_position);

			if (jaw.y >= 1080 || jaw.x >= 1920 || jaw.x <= 0 || jaw.y <= 0) return;
			if (spine4.y >= 1080 || spine4.x >= 1920 || spine4.x <= 0 || spine4.y <= 0) return;
			if (spine4.y >= 1080 || spine4.x >= 1920 || spine4.x <= 0 || spine4.y <= 0) return;
			if (spine4.y >= 1080 || spine4.x >= 1920 || spine4.x <= 0 || spine4.y <= 0) return;
			if (l_upperarm.y >= 1080 || l_upperarm.x >= 1920 || l_upperarm.x <= 0 || l_upperarm.y <= 0) return;
			if (r_upperarm.y >= 1080 || r_upperarm.x >= 1920 || r_upperarm.x <= 0 || r_upperarm.y <= 0) return;
			if (spine3.y >= 1080 || spine3.x >= 1920 || spine3.x <= 0 || spine3.y <= 0) return;
			if (pelvis.y >= 1080 || pelvis.x >= 1920 || pelvis.x <= 0 || pelvis.y <= 0) return;
			if (pelvis.y >= 1080 || pelvis.x >= 1920 || pelvis.x <= 0 || pelvis.y <= 0) return;
			if (l_knee.y >= 1080 || l_knee.x >= 1920 || l_knee.x <= 0 || l_knee.y <= 0) return;
			if (r_knee.y >= 1080 || r_knee.x >= 1920 || r_knee.x <= 0 || r_knee.y <= 0) return;
			if (l_hand.y >= 1080 || l_hand.x >= 1920 || l_hand.x <= 0 || l_hand.y <= 0) return;
			if (r_hand.y >= 1080 || r_hand.x >= 1920 || r_hand.x <= 0 || r_hand.y <= 0) return;
			if (r_ankle_scale.y >= 1080 || r_ankle_scale.x >= 1920 || r_ankle_scale.x <= 0 || r_ankle_scale.y <= 0) return;
			if (l_ankle_scale.y >= 1080 || l_ankle_scale.x >= 1920 || l_ankle_scale.x <= 0 || l_ankle_scale.y <= 0) return;

			pelvis.y += 0.2;
			l_hip.y += 0.2;

			render.Line(Vector2(jaw.x, jaw.y), Vector2(spine4.x, spine4.y), health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(spine3.x, spine3.y), health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(l_upperarm.x, l_upperarm.y), health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(r_upperarm.x, r_upperarm.y), health_color);
			render.Line(Vector2(l_upperarm.x, l_upperarm.y), Vector2(l_forearm.x, l_forearm.y), health_color);
			render.Line(Vector2(r_upperarm.x, r_upperarm.y), Vector2(r_forearm.x, r_forearm.y), health_color);
			render.Line(Vector2(l_forearm.x, l_forearm.y), Vector2(l_hand.x, l_hand.y), health_color);
			render.Line(Vector2(r_forearm.x, r_forearm.y), Vector2(r_hand.x, r_hand.y), health_color);
			render.Line(Vector2(spine3.x, spine3.y), Vector2(pelvis.x, pelvis.y), health_color);
			render.Line(Vector2(l_hip.x, l_hip.y), Vector2(l_knee.x, l_knee.y), health_color);
			render.Line(Vector2(pelvis.x, pelvis.y), Vector2(r_knee.x, r_knee.y), health_color);
			render.Line(Vector2(l_knee.x, l_knee.y), Vector2(l_ankle_scale.x, l_ankle_scale.y), health_color);
			render.Line(Vector2(r_knee.x, r_knee.y), Vector2(r_ankle_scale.x, r_ankle_scale.y), health_color);
			render.Line(Vector2(r_ankle_scale.x, r_ankle_scale.y), Vector2(r_foot.x, r_foot.y), health_color);

			//HANDS??
		}


		//name, flags and distance
		if (name)
		{
			auto Transform = ply->model()->boneTransforms()->get(48);
			if (!Transform) return;

			auto position = Transform->get_position();

			auto distance = esp::local_player->model()->boneTransforms()->get(48)->get_position().distance(position);
			//const char* new_name = ;
			// PLAYER NAME

			//const wchar_t* nw;

			if (vars->visual.nameesp) {
				auto name_color = is_visible ? vars->colors.players.details.name.visible : vars->colors.players.details.name.invisible;
				auto half = (bounds.right - bounds.left) / 2;

				if (vars->visual.woundedflag) {
					if (HasPlayerFlag(ply, rust::classes::PlayerFlags::Wounded)) {
						render.String({ bounds.right, bounds.top + 1 }, _(L"Wounded"), { 0, 0, 0, 1 });
						render.String({ bounds.right, bounds.top }, _(L"Wounded"), FLOAT4TOD3DCOLOR(name_color));
					}
				}

				if (vars->visual.distance)
				{
					auto dist_color = is_visible ? vars->colors.players.details.distance.visible : vars->colors.players.details.distance.invisible;
					auto nstr = string::wformat(_(L"[%dm]"), (int)distance);
					render.StringCenter({ bounds.center, bounds.bottom + 5 }, nstr, { 0, 0, 0, 1 });
					render.StringCenter({ bounds.center, bounds.bottom + 4}, nstr, FLOAT4TOD3DCOLOR(dist_color));
					//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.bottom + 1.f, 79.f + half * 2 + 80.f , 30 }, nstr, gui::Color(0, 0, 0, 120), true, 10.5);
					//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.bottom, 80.f + half * 2 + 80.f , 30 }, nstr, gui::Color(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1), true, 10);
				}

				render.StringCenter({ bounds.center, bounds.top - 14 }, name, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center, bounds.top - 13 }, name, FLOAT4TOD3DCOLOR(name_color));
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.top - 23.f, 79.f + half * 2 + 80.f , 30 }, name, gui::Color(0, 0, 0, 120), true, 10.5);
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.top - 24.f, 80.f + half * 2 + 80.f , 30 }, name, gui::Color(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1), true, 10);
			}
			// PLAYER NAME
		}
	}
}

void DrawPlayerHotbar(aim_target target) {
	Vector2 screen = { 0, 0 }; 
	esp::out_w2s(target.pos, screen);

	//create window length based off amount of items in belt
	struct it {
		wchar_t* name;
		int count;
	};
	std::vector<it> items = {};

	if (target.ent && !target.is_heli && !target.teammate)
	{
		System::list<Item*>* belt = ((BasePlayer*)target.ent)->get_belt_items();
		if (!belt) return;
		Item* result = nullptr;
		belt->for_each([&](Item* item, int32_t idx) {
			{
				if (!item) return;
				const auto name = item->get_weapon_name();
				if (name)
				{
					const auto amt = item->GetAmount();
					items.push_back({ name, amt });
				}
			}
		});

		//check for draw wearable?

		if (items.size() > 0)
		{
			auto player_name = ((BasePlayer*)target.ent)->_displayName()->str;

			// 
			// box has player name
			// *separator*
			// *item* x*amount*
			// *item* x*amount*
			// *item* x*amount*
			//

			Vector2 start = { 150, 150 };
			Vector2 sz = { 200, 50.f + (items.size() * 20.f) };

			//filled gradient
			render.FillRoundedRectangle_GradientLinear(start, sz,
				{ 0.14f, 0.14f, 0.14f, 0.65f },
				{ 209.f / 255.f, 90.f / 255.f, 79.f / 255.f, 0.65f }, 
				2);
			
			std::wstring name_str(player_name); name_str = name_str + _(L"'s items");
			render.String({ start.x + 10, start.y + 5 }, name_str.c_str(), { 0.71, 0.71, 0.71, 1 });

			//seperator
			render.Line({ start.x, start.y + 25 }, { start.x + sz.x, start.y + 25 }, { 0.21, 0.21, 0.21, 1 }, 1);

			//outline
			render.RoundedRectangle(start, sz, { 0.21, 0.21, 0.21, 1 }, 2, 1);

			start.x += 10; //left-side margin
			start.y += 30; //margin from seperator

			int k = 0;
			for (auto i : items)
			{
				auto name = i.name;
				auto amount = i.count;

				std::wstring ws(name);
				auto astr = std::to_string(amount);
				std::wstring wastr(astr.begin(), astr.end());
				ws = ws + _(L" - x") + wastr;

				render.String({ start.x, start.y + (k++ * 20) }, ws.c_str(), { 0.71, 0.71, 0.71, 1 });
			}
		}
	}
}

void iterate_entities() {
	auto get_client_entities = [&]() {
		esp::client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
	};
	if (!esp::client_entities)
		get_client_entities();

	rust::classes::list* entity_list = (rust::classes::list*)esp::client_entities;

	auto list_value = entity_list->get_value<uintptr_t>();
	if (!list_value) {
		//get_client_entities();
		return;
	}

	auto size = entity_list->get_size();
	if (!size) {
		//get_client_entities();
		return;
	}

	auto buffer = entity_list->get_buffer<uintptr_t>();
	if (!buffer) {
		//get_client_entities();
		return;
	}

	bool found_lp = false;
	Vector4 esp_color(1, 0, 1, 1);

	uintptr_t esp_name;

	for (int i = 0; i <= size; i++) {
		auto current_object = *reinterpret_cast<uintptr_t*>(buffer + 0x20 + (i * 0x8));
		if (!current_object || current_object <= 100000)
			continue;

		auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
		if (!base_object || base_object <= 100000)
			continue;

		auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
		if (!object || object <= 100000)
			continue;

		WORD tag = *reinterpret_cast<WORD*>(object + 0x54);

		bool is_looking_at_entity = false;


		auto ent = *reinterpret_cast<BaseEntity**>(base_object + 0x28);
		if (!ent)
			continue;

		auto ent_class = *reinterpret_cast<uintptr_t*>(ent);
		if (!ent_class)
			continue;

		auto entity_class_name = (char*)*reinterpret_cast<uintptr_t*>(ent_class + 0x10);
		if (!entity_class_name)
			continue;


		auto game_object = *reinterpret_cast<uintptr_t*>(object + 0x30);
		if (!game_object)
			continue;

		auto Transform = *reinterpret_cast<uintptr_t*>(game_object + 0x8);
		if (!Transform)
			continue;

		auto visual_state = *reinterpret_cast<uintptr_t*>(Transform + 0x38);
		if (!visual_state)
			continue;

		auto world_position = *reinterpret_cast<Vector3*>(visual_state + 0x90);
		if (world_position.is_empty())
			continue;

		uintptr_t object_name_ptr = mem::read<uintptr_t>(object + 0x60);
		if (!object_name_ptr)
			continue;

		auto object_name = *reinterpret_cast<esp::rust_str*>(object_name_ptr);
		if (!object_name.zpad)
			continue;

		auto ent_net = *reinterpret_cast<Networkable**>(ent + 0x58);
		auto ent_id = ent_net->get_id();

		esp::matrix = unity::get_view_matrix();

		if (tag == 6)
		{
			auto entity = (BasePlayer*)ent;

			//hit player for silent melee, but not here as may crash due to not being run from a game thread

			//check valid
			if (!entity->is_alive()
				|| (entity->is_sleeping() && !vars->visual.sleeper_esp))
				continue;

			//check npc
			bool npc = false;
			if (vars->visual.npc_esp
				&& entity->playerModel()->isnpc())
				npc = true;

			//local player chams, player average velocity
			if (entity->is_local_player())
				esp::local_player = entity;
			else {
				if (esp::local_player)
				{
					auto target = aim_target();
					if (vars->combat.bodyaim)
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::pelvis)->get_position();
					else
						target.pos = ent->model()->boneTransforms()->get(48)->get_position();

					auto distance = esp::local_player->model()->boneTransforms()->get(48)->get_position().get_3d_dist(target.pos);
					target.distance = distance;

					auto fov = unity::get_fov(target.pos);
					target.fov = fov;

					target.ent = (BaseCombatEntity*)ent;

					target.network_id = ent_id;

					auto visible = ent->is_visible(esp::local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
					target.visible = visible;

					//if (distance < 300.0f)
					//{
					//	visibleplayerswithin300m.push_back(target);
					//}

					if (target < esp::best_target
						|| !esp::best_target.ent->is_alive()
						|| (target.ent && ((BasePlayer*)target.ent)->userID() == ((BasePlayer*)esp::best_target.ent)->userID()))
					{
						esp::best_target.pos = target.pos;
						esp::best_target.distance = target.distance;
						esp::best_target.fov = target.fov;
						esp::best_target.ent = target.ent;
						esp::best_target.visible = visible;

						auto vel = target.ent->GetWorldVelocity();

						float next_frame = esp::best_target.last_frame + get_deltaTime();
						if (get_fixedTime() > next_frame)
						{
							//new frame, record velocity, record frame
							esp::best_target.last_frame = get_fixedTime();
							if (esp::best_target.velocity_list.size() < 90) //0.03125 * 30 = 0.9 seconds
								esp::best_target.velocity_list.push_back(vel);
							else
							{
								esp::best_target.velocity_list.pop_back();
								esp::best_target.velocity_list.insert(esp::best_target.velocity_list.begin(), 1, vel);
							}
							float avgx = 0.f;
							float avgy = 0.f;
							float avgz = 0.f;
							int count = 0;
							for (auto v : esp::best_target.velocity_list)
							{
								if (v.is_empty()) break;
								avgx += v.x;
								avgy += v.y;
								avgz += v.z;
								count += 1;
							}
							avgx /= count; avgy /= count; avgz /= count;
							esp::best_target.avg_vel = Vector3(avgx, avgy, avgz);
						}
					}

					if (target < esp::best_target
						&& target.fov < vars->combat.aimbotfov)
						esp::best_target = target;
					if (esp::best_target.fov > vars->combat.aimbotfov)
						esp::best_target = aim_target();
				}

				if (vars->visual.playeresp && esp::local_player)
				{
					DrawPlayer(entity, npc);

					//offscreen indicator?
					//silent melee?
				}
			}
		}

		if (esp::local_player)
		{
			//selected entity
			if (esp::selected_entity_id == ent_id) {

				Vector2 w2s_position = {};
				if (esp::out_w2s(world_position, w2s_position))
				{
					esp_color = Vector4(54, 116, 186, 255);
					w2s_position.y += 10;
					if (esp::selected_entity_id == ent_id) {
						render.StringCenter(w2s_position, _(L"[selected]"), { 0.f, 1.f, 0.f });
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(("[selected]")), esp_color);
						w2s_position.y += 10;
						render.StringCenter(w2s_position, string::wformat(_(L"[%d]"), (int)ent_id), {0.f, 1.f, 0.f});
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[%d]"), (int)ent_id)), esp_color);
					}
				}
			}

			//auto upgrade?

			if (vars->visual.stash ||
				vars->misc.norecycler ||
				vars->visual.stone_ore ||
				vars->visual.sulfur_ore ||
				vars->visual.metal_ore ||
				vars->visual.traps ||
				vars->visual.animal ||
				vars->visual.barrels ||
				vars->visual.food ||
				vars->visual.vehicles ||
				vars->visual.airdrops ||
				vars->visual.ladder ||
				vars->visual.cloth ||
				vars->visual.corpses)
			{
				float dist = 10.f;
				if (esp::local_player && vars->visual.distance)
					dist = esp::local_player->model()->boneTransforms()->get(48)->get_position().distance(world_position);

				//dropped items
				if (*(int*)(entity_class_name) == 'porD') {
					if (!vars->visual.dropped_items)
						continue;

					if (*(int*)(entity_class_name + 40) == 'kcab')
						continue;

					if (*(int*)(object_name.zpad) == 'orra')
						continue;

					if (*(int*)(object_name.zpad + 12) == 'ian.')
						continue;

					auto _Item = *reinterpret_cast<uintptr_t*>(ent + 0x170); //public Item item; // 0x170
					if (!_Item)
						continue;

					auto item = reinterpret_cast<Item*>(_Item);

					auto item_name = item->get_weapon_name();

					esp_color = Vector4(196, 124, 0, 255);

					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
						esp::draw_weapon_icon(item, w2s_position);
					//esp::draw_item(w2s_position, 0, esp_color, item_name);

					if (vars->visual.distance)
					{
						auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
						w2s_position.y += 12;
						render.StringCenter(w2s_position, nstr, FLOAT4TOD3DCOLOR(vars->colors.players.details.distance.visible));
						//draw_text(w2s_position, const_cast<wchar_t*>(nstr), Vector4(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1));
					}
					continue;
				}

				//ladder
				if (vars->visual.ladder && !strcmp(entity_class_name, _("BaseLadder"))) {
					esp_name = il2cpp::methods::new_string(_("Ladder"));
					esp_color = Vector4(0, 219, 58, 255);

					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{

						//esp::draw_item(w2s_position, esp_name, esp_color);
					}
					continue;
				}

				//tc
				if (vars->visual.tc_esp && *(int*)(entity_class_name) == 'liuB' && *(int*)(entity_class_name + 8) == 'virP') {

					//rpc stuf

					auto authorizedPlayers_wrapper = *reinterpret_cast<uintptr_t*>(ent + 0x598);
					if (!authorizedPlayers_wrapper)
						continue;
					const auto authorizedPlayers_list = *reinterpret_cast<System::list<PlayerNameID*>**>(authorizedPlayers_wrapper + 0x10);
					if (!authorizedPlayers_list)
						continue;

					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{

						//esp::draw_tool_cupboard(w2s_position, il2cpp::methods::new_string(_("Tool Cupboard")), Vector4(255, 0, 0, 255), authorizedPlayers_list);
					}
					continue;
				}

				//heli
				if (*(int*)(entity_class_name + 4) == 'ileH' && vars->visual.heli_esp) {
					auto base_heli = reinterpret_cast<BaseHelicopter*>(ent);

					Vector2 rearrotor, beam, mainrotor;
					esp::out_w2s(base_heli->model()->boneTransforms()->get(22)->get_position(), rearrotor);
					esp::out_w2s(base_heli->model()->boneTransforms()->get(19)->get_position(), mainrotor);
					esp::out_w2s(base_heli->model()->boneTransforms()->get(56)->get_position(), beam);
					esp_name = il2cpp::methods::new_string(("Patrol-heli"));
					esp_color = Vector4(232, 232, 232, 255);


					const Vector2 diff = { (beam.x + rearrotor.x) / 2, (beam.y + rearrotor.y) / 2 };

					const float h = max(beam.y, rearrotor.y) - min(beam.y, rearrotor.y);
					const float y = diff.y;
					const float w = (max(beam.x, rearrotor.x) - min(beam.x, rearrotor.x));
					float x = diff.x - h / 4;

					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{

						//esp::draw_item(w2s_position, esp_name, esp_color);
					}

					if (base_heli->is_alive())
					{
						auto target = aim_target();
						target.pos = base_heli->model()->boneTransforms()->get(19)->get_position();

						auto distance = esp::local_player->model()->boneTransforms()->get(48)->get_position().get_3d_dist(target.pos);
						target.distance = distance;

						auto fov = unity::get_fov(target.pos);
						target.fov = fov;

						target.network_id = ent_id;

						if (fov < vars->combat.aimbotfov)
						{
							target.ent = base_heli;

							auto visible = esp::local_player->is_visible(esp::local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
							target.visible = visible;

							if (target < esp::best_target)
							{
								esp::best_target = target;
								esp::best_target.is_heli = true;
							}
							else if (esp::best_target.is_heli)
								esp::best_target.pos = target.pos;
							else esp::best_target.is_heli = false;
						}
						else
						{
							esp::best_target = aim_target();
						}
					}
					else esp::best_target.is_heli = false;

					esp::draw_heli(x, y, w, h);
					continue;
				}

				//stash
				if (vars->visual.stash && *(int*)(object_name.zpad + 46) == '_hsa') {
					esp_name = il2cpp::methods::new_string(_("Stash"));
				}

				//recycler
				else if (vars->misc.norecycler && *(int*)(entity_class_name) == 'yceR' && get_fixedTime() > esp::last_recycler + 0.35f) {
					esp_name = il2cpp::methods::new_string(_("Recycler"));
					esp_color = Vector4(232, 232, 232, 255);
					if (esp::local_player->model()->boneTransforms()->get(48)->get_position().distance(world_position) < 4.5f)
					{
						ent->ServerRPC(_(L"SVSwitch"));
						esp::last_recycler = get_fixedTime();
					}
				}

				//stone
				else if (vars->visual.stone_ore && (*(int*)(object_name.zpad + 52) == 'nots' || *(int*)(object_name.zpad + 47) == 'nots')) {
					esp_name = il2cpp::methods::new_string(_("Stone Ore"));
					esp_color = Vector4(232, 232, 232, 255);
				}

				//sulfur
				else if (vars->visual.sulfur_ore && (*(int*)(object_name.zpad + 52) == 'flus' || *(int*)(object_name.zpad + 47) == 'flus')) {
					esp_name = il2cpp::methods::new_string((_("Sulfur Ore")));
					esp_color = Vector4(203, 207, 0, 255);
				}

				//metal
				else if (vars->visual.metal_ore && (*(int*)(object_name.zpad + 52) == 'atem' || *(int*)(object_name.zpad + 47) == 'atem')) {
					esp_name = il2cpp::methods::new_string(_("Metal Ore"));
					esp_color = Vector4(145, 145, 145, 255);
				}

				//traps
				else if (vars->visual.traps && (*(int*)(object_name.zpad + 36) == 'terr' || *(int*)(object_name.zpad + 43) == 'tnug' || *(int*)(object_name.zpad + 38) == 'rtra')) {
					if (*(int*)(object_name.zpad + 36) == 'terr')
						esp_name = il2cpp::methods::new_string(_("Auto Turret*"));
					else if (*(int*)(object_name.zpad + 43) == 'tnug')
						esp_name = il2cpp::methods::new_string(_("Shotgun Trap*"));
					else if (*(int*)(object_name.zpad + 38) == 'rtra')
						esp_name = il2cpp::methods::new_string(_("Bear Trap*"));

					esp_color = Vector4(255, 166, 0, 255);
				}

				//vehicles
				else if (vars->visual.vehicles && *(int*)(entity_class_name + 4) == 'iheV') {
					esp_name = il2cpp::methods::new_string(_("Vehicle"));
					esp_color = Vector4(0, 161, 219, 255);
				}

				//airdrop
				else if (vars->visual.airdrops && *(int*)(object_name.zpad + 39) == 'pord') {
					esp_name = il2cpp::methods::new_string(_("Airdrop"));
					esp_color = Vector4(0, 161, 219, 255);
				}

				//cloth
				else if (vars->visual.cloth && *(int*)(object_name.zpad + 52) == 'c-pm') {
					esp_name = il2cpp::methods::new_string(_("Cloth"));
					esp_color = Vector4(0, 219, 58, 255);
				}

				//corpse
				else if (vars->visual.corpses && *(int*)(object_name.zpad + 29) == 'proc') {
					esp_name = il2cpp::methods::new_string(_("Player Corpse"));
					esp_color = Vector4(230, 230, 230, 255);
				}
				else if (tag != 6)
					continue;

				//stash open?
				if (tag != 6) {
					if (*(int*)(entity_class_name) == 'satS') {
						auto flag = *reinterpret_cast<int*>(ent + 0x128);
						if (flag != 2048)
							continue;
					}
					//auto s = il2cpp::methods::new_string(string::format(_("[%dm]"), (int)local_player->eyes()->get_position().distance(world_position)));
					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{
						esp::draw_item(w2s_position, esp_name, esp_color);

						if (vars->visual.distance)
						{
							auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
							w2s_position.y += 11;
							render.StringCenter(w2s_position, nstr, { 0,0,0,1 });
							w2s_position.y += 1;
							render.StringCenter(w2s_position, nstr, FLOAT4TOD3DCOLOR(vars->colors.players.details.distance.visible));
						}
						//if (vars->visual.distance
						//	&& local_player)
						//	esp::draw_item(Vector2(w2s_position.x, w2s_position.y += 10), s, esp_color);
					}

					continue;
				}
			}
		}
	}

	if (esp::best_target.ent
		&& vars->visual.hotbar_esp)
		DrawPlayerHotbar(esp::best_target);
}

void Crosshair1() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;

	render.Line(Vector2{ x, y - 6 }, Vector2{ x, y + 6 }, FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
	render.Line(Vector2{ x - 6, y }, Vector2{ x + 6, y }, FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
}
void Crosshair2() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;

	render.Line(Vector2{ x, y - 5 }, Vector2{ x , y + 5 }, FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
	render.Line(Vector2{ x - 5, y }, Vector2{ x + 5, y }, FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
}
void Crosshair3() { //Circle crosshair
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
}

int indicators_on_screen = 0;

void IndicatorDesync() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 20 }, { 120, 4 }, vars->desyncTime, 1.0f);
}
void IndicatorReload() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	if(vars->time_since_last_shot < vars->reload)
		render.ProgressBar({ x - 60, y + 30 }, { 120, 4 }, vars->time_since_last_shot, (vars->reload - 0.2f));
}
void IndicatorFlyhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;

}
void IndicatorSpeedhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 40 }, { 120, 4 }, vars->speedhack, 1.0f);
}

void DrawSnapline() {
	Vector2 start = vars->visual.snapline == 1 ? Vector2(vars->ScreenX / 2, 0) :
		vars->visual.snapline == 2 ? Vector2(vars->ScreenX / 2, vars->ScreenY / 2) :
		vars->visual.snapline == 3 ? Vector2(vars->ScreenX / 2, vars->ScreenY) :
		Vector2(vars->ScreenX / 2, vars->ScreenY);
}

void DrawFov() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->colors.ui.fov.visible), vars->combat.aimbotfov, 0.7f);
}

void Watermark() {
	render.FillRectangle(Vector2(7, 7), Vector2(60, 16), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
	render.FillRectangle(Vector2(6, 6), Vector2(62, 18), D2D1::ColorF(0.11f, 0.11f, 0.11f));
	render.FillRectangle(Vector2(7, 21), Vector2(60, 2), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
	render.String({ 14,7 }, _(L"rust"), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
}

void new_frame() {

	float Y = GetSystemMetrics(SM_CYSCREEN);
	float X = GetSystemMetrics(SM_CXSCREEN);
	Vector2 vScreen = render.CanvasSize();

	vars->ScreenX = vScreen.x;
	vars->ScreenY = vScreen.y;

	//Draw crosshairs
	if (vars->visual.crosshair1)
		Crosshair1();
	if (vars->visual.crosshair2)
		Crosshair2();
	if (vars->visual.crosshair3)
		Crosshair3();

	//Draw indicators
	if (vars->visual.desync_indicator)
		IndicatorDesync();
	if (vars->combat.always_reload)
		IndicatorReload();
	if (vars->visual.flyhack_indicator)
		IndicatorFlyhack();
	if (vars->visual.speedhack_indicator)
		IndicatorSpeedhack();

	//Draw FOV
	if (vars->visual.show_fov)
		DrawFov();

	//Draw watermark
	Watermark();

	iterate_entities();
}