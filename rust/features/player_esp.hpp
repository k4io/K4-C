#pragma once
#include "../../mmisc.hpp"
#include <array>
#include <cstdint>

#include "../../memory/il2cpp.hpp"
#include "../../settings.hpp"

#include "../classes.hpp"
#include "../rust.hpp"
#include <map>

namespace esp {
	std::map<unsigned int, unsigned int> selected_entity_parent_mapping = {};
	unsigned int selected_entity_id;
	float time_last_upgrade = 0.f;
	float rl_time = 0.f;
	uintptr_t client_entities;
	BasePlayer* local_player;
	VMatrix matrix;
	aim_target best_target = aim_target();
	uintptr_t closest_building_block = 0;

	std::vector<aim_target> visibleplayerswithin300m = {};

	bool out_w2s(const Vector3& position, Vector2& screen_pos) {
		if (!matrix.m) {
			return false;
		}

		Vector2 out;
		const auto temp = matrix.transpose();

		auto translation_vector = Vector3{ temp[3][0], temp[3][1], temp[3][2] };
		auto up = Vector3{ temp[1][0], temp[1][1], temp[1][2] };
		auto right = Vector3{ temp[0][0], temp[0][1], temp[0][2] };

		float w = translation_vector.dot(position) + temp[3][3];

		if (w < 0.098f) {
			return false;
		}

		float x = up.dot(position) + temp._24;
		float y = right.dot(position) + temp._14;

		out.x = (static_cast<float>(unity::get_width()) / 2) * (1 + y / w);
		out.y = (static_cast<float>(unity::get_height()) / 2) * (1 - x / w);

		screen_pos = out;

		return true;
	}

	void draw_heli(float x, float y, float w, float h);

	void draw_tool_cupboard(Vector2 w2s_position, uintptr_t label, Vector4 color, System::list<PlayerNameID*>* authorizedPlayers_list);

	void draw_hackable_crate(Vector2 w2s_position, uintptr_t crate, Vector4 color);

	void draw_item(Vector2 w2s_position, uintptr_t label, Vector4 color, wchar_t* name = _(L""));

	void draw_player(BasePlayer* ent, bool is_npc);

	void rock_chams(BaseEntity* ent);

	void do_chams(BasePlayer* ent);

	void draw_target_hotbar(aim_target target);

	void draw_name(Vector3 position, wchar_t* name);

	void draw_middle(aim_target target);

	void draw_weapon_icon(Item* item, Vector2 w2s_position);

	void offscreen_indicator(Vector3 position);

	void draw_target_fov(col color, Vector2 o, float r);

	void draw_text(Vector2 worldpos, wchar_t* str, Vector4 col, bool outline = true);

	uintptr_t shader;

	float last_recycler = 0.f;
	
	void iterate_players(bool draw = true) {
		//visibleplayerswithin300m.clear();
		//best_target = ;
		auto get_client_entities = [&]() {
			client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
		};
		if (!client_entities)
			get_client_entities();

		rust::classes::list* entity_list = (rust::classes::list*)client_entities;

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

		//if (local_player)
		//	local_player->console_echo(string::wformat(_(L"[trap]: OnGui - clientEntities size: %d"), size));

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

			auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
			if (!object_name.zpad)
				continue;

			auto ent_net = *reinterpret_cast<Networkable**>(ent + 0x58);
			auto ent_id = ent_net->get_id();


			esp::matrix = unity::get_view_matrix();

			//players
			if (tag == 6)
			{
				BasePlayer* entity = (BasePlayer*)ent;

				
				auto hit_player = [&]() {
					auto Item = esp::local_player->GetActiveItem();
					if (Item) {
						auto baseprojectile = Item->GetHeldEntity<BaseProjectile>();
						if (baseprojectile) {
							auto class_name = baseprojectile->get_class_name();
							if (*(int*)(class_name + 4) == 'eleM' || *(int*)(class_name + 4) == 'mmah') {
								auto world_position = ent->model()->boneTransforms()->get(48)->position();
								auto local = ClosestPoint(esp::local_player, world_position);
								auto camera = esp::local_player->model()->boneTransforms()->get(48)->position();

								if (camera.get_3d_dist(world_position) >= 4.2f)
									return;

								aim_target target = esp::best_target;

								attack_melee(target, (BaseMelee*)baseprojectile, true);
							}
						}
					}
				};

				if (!entity->is_alive())
					continue;

				if (entity->is_sleeping() && !vars->visual.sleeper_esp)
					continue;

				bool is_npc = false;

				if (entity->playerModel()->isnpc()) {
					is_npc = true;

					if (!vars->visual.npc_esp)
						continue;
				}

				if (entity->is_local_player())
				{
					local_player = reinterpret_cast<BasePlayer*>(ent);
					do_chams(local_player);
				}
				else {
					if (esp::local_player) {
						auto target = aim_target();
						if (vars->combat.bodyaim)
							target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::pelvis)->position();
						else
							target.pos = ent->model()->boneTransforms()->get(48)->position();

						auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos);
						target.distance = distance;

						auto fov = unity::get_fov(target.pos);
						target.fov = fov;

						target.ent = (BaseCombatEntity*)ent;

						target.network_id = ent_id;

						auto visible = ent->is_visible(esp::local_player->model()->boneTransforms()->get(48)->position(), target.pos);
						target.visible = visible;

						//if (distance < 300.0f)
						//{
						//	visibleplayerswithin300m.push_back(target);
						//}

						if (target < best_target
							|| !best_target.ent->is_alive()
							|| (target.ent && ((BasePlayer*)target.ent)->userID() == ((BasePlayer*)best_target.ent)->userID()))
						{
							best_target.pos = target.pos;
							best_target.distance = target.distance;
							best_target.fov = target.fov;
							best_target.ent = target.ent;
							best_target.visible = visible;

							auto vel = target.ent->GetWorldVelocity();

							float next_frame = best_target.last_frame + get_deltaTime();
							if (get_fixedTime() > next_frame)
							{
								//new frame, record velocity, record frame
								best_target.last_frame = get_fixedTime();
								if (best_target.velocity_list.size() < 90) //0.03125 * 30 = 0.9 seconds
									best_target.velocity_list.push_back(vel);
								else
								{
									best_target.velocity_list.pop_back();
									best_target.velocity_list.insert(best_target.velocity_list.begin(), 1, vel);
								}
								float avgx = 0.f;
								float avgy = 0.f;
								float avgz = 0.f;
								int count = 0;
								for (auto v : best_target.velocity_list)
								{
									if (v.is_empty()) break;
									avgx += v.x;
									avgy += v.y;
									avgz += v.z;
									count += 1;
								}
								avgx /= count; avgy /= count; avgz /= count;
								best_target.avg_vel = Vector3(avgx, avgy, avgz);
							}
						}

						if (target < best_target
							&& target.fov < vars->combat.aimbotfov)
							best_target = target;
						if (best_target.fov > vars->combat.aimbotfov)
							best_target = aim_target();
					}

					if (draw && vars->visual.playeresp && local_player) {

						draw_player(((BasePlayer*)ent), is_npc);

						if (vars->visual.offscreen_indicator
							&& !is_npc)
						{
							offscreen_indicator(((BasePlayer*)ent)->eyes()->position());
						}

						if (vars->combat.silent_melee || unity::GetKey(vars->keybinds.silentmelee))
							hit_player();
					}
				}
				continue;
			}

			if (local_player)
			{
				if (GetAsyncKeyState(0x37))
				{
					auto look = local_player->_lookingAtEntity();
					if (look)
					{
						auto net2 = *reinterpret_cast<Networkable**>(look + 0x58);
						auto look_id = net2->get_id();
						if (look_id == ent_id)
						{
							//is_looking_at_entity = true;

							//if (!map_contains_key(selected_entity_parent_mapping, selected_entity_id))
							//	selected_entity_parent_mapping.insert(std::make_pair(selected_entity_id, 0));
							//else //does contain the key
							//	if (selected_entity_parent_mapping[selected_entity_id] == 0)
							//		selected_entity_parent_mapping[selected_entity_id] = ent_id;

							selected_entity_id = ent_id;
						}
					}
				}

				if (selected_entity_id == ent_id) {

					Vector2 w2s_position = {};
					if (out_w2s(world_position, w2s_position))
					{
						esp_color = Vector4(54, 116, 186, 255);
						w2s_position.y += 10;
						if (selected_entity_id == ent_id) {
							esp::draw_item(w2s_position, il2cpp::methods::new_string(("[selected]")), esp_color);
							w2s_position.y += 10;
							esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[%d]"), (int)ent_id)), esp_color);
						}
					}
				}

				if (*(int*)(entity_class_name) == 'kcaH' && *(int*)(entity_class_name + 14) == 'tarC') {
					auto flag = *reinterpret_cast<int*>(ent + 0x128);
					if (flag != 128 && flag != 256)
						continue;

					Vector2 w2s_position = {};
					if (out_w2s(world_position, w2s_position))
						esp::draw_hackable_crate(w2s_position, (uintptr_t)ent, { 0.45, 0.72, 1, 0.8 });
				}

				if (vars->misc.auto_upgrade)
				{
					if (!LI_FIND(strcmp)(entity_class_name, _("BuildingBlock")))
					{
						auto lpos = local_player->transform()->position();
						float dist_to_new = lpos.distance(world_position);
						if (!closest_building_block)
							closest_building_block = (uintptr_t)ent;
						else if (dist_to_new < lpos.distance(((BaseEntity*)closest_building_block)->transform()->position()))
							closest_building_block = (uintptr_t)ent;
					}
				}

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
						dist = esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position);

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
						if (out_w2s(world_position, w2s_position))
							draw_weapon_icon(item, w2s_position);
						//esp::draw_item(w2s_position, 0, esp_color, item_name);

						if (vars->visual.distance)
						{
							auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
							w2s_position.y += 12;
							draw_text(w2s_position, const_cast<wchar_t*>(nstr), Vector4(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1));
						}
						continue;
					}

					//ladder
					if (vars->visual.ladder && !strcmp(entity_class_name, _("BaseLadder"))) {
						esp_name = il2cpp::methods::new_string(_("Ladder"));
						esp_color = Vector4(0, 219, 58, 255);

						Vector2 w2s_position = {};
						if (out_w2s(world_position, w2s_position))
							esp::draw_item(w2s_position, esp_name, esp_color);
						continue;
						//map of vector3,id
						//display id
						//if (map_contains_key(selected_entity_parent_mapping, ent_id))
						//{
						//	w2s_position.y += 10;
						//	if (selected_entity_id == ent_id)
						//		esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[parent: %d]"), selected_entity_parent_mapping[ent_id])), esp_color);
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
						if (out_w2s(world_position, w2s_position))
							esp::draw_tool_cupboard(w2s_position, il2cpp::methods::new_string(_("Tool Cupboard")), Vector4(255, 0, 0, 255), authorizedPlayers_list);
						continue;
					}

					//heli
					if (*(int*)(entity_class_name + 4) == 'ileH' && vars->visual.heli_esp) {
						auto base_heli = reinterpret_cast<BaseHelicopter*>(ent);

						Vector2 rearrotor, beam, mainrotor;
						out_w2s(base_heli->model()->boneTransforms()->get(22)->position(), rearrotor);
						out_w2s(base_heli->model()->boneTransforms()->get(19)->position(), mainrotor);
						out_w2s(base_heli->model()->boneTransforms()->get(56)->position(), beam);
						esp_name = il2cpp::methods::new_string(("Patrol-heli"));
						esp_color = Vector4(232, 232, 232, 255);


						const Vector2 diff = { (beam.x + rearrotor.x) / 2, (beam.y + rearrotor.y) / 2 };

						const float h = max(beam.y, rearrotor.y) - min(beam.y, rearrotor.y);
						const float y = diff.y;
						const float w = (max(beam.x, rearrotor.x) - min(beam.x, rearrotor.x));
						float x = diff.x - h / 4;

						Vector2 w2s_position = {};
						if (out_w2s(world_position, w2s_position))
							esp::draw_item(w2s_position, esp_name, esp_color);

						if (base_heli->is_alive())
						{
							auto target = aim_target();
							target.pos = base_heli->model()->boneTransforms()->get(19)->position();

							auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos);
							target.distance = distance;

							auto fov = unity::get_fov(target.pos);
							target.fov = fov;

							target.network_id = ent_id;

							if (fov < vars->combat.aimbotfov)
							{
								target.ent = base_heli;

								auto visible = local_player->is_visible(esp::local_player->model()->boneTransforms()->get(48)->position(), target.pos);
								target.visible = visible;

								if (target < best_target)
								{
									best_target = target;
									best_target.is_heli = true;
								}
								else if (best_target.is_heli)
									best_target.pos = target.pos;
								else best_target.is_heli = false;
							}
							else
							{
								best_target = aim_target();
							}
						}
						else best_target.is_heli = false;

						draw_heli(x, y, w, h);
						continue;
					}

					//stash
					if (vars->visual.stash && *(int*)(object_name.zpad + 46) == '_hsa') {
						esp_name = il2cpp::methods::new_string(_("Stash"));
					}

					//recycler
					else if (vars->misc.norecycler && *(int*)(entity_class_name) == 'yceR' && get_fixedTime() > last_recycler + 0.35f) {
						esp_name = il2cpp::methods::new_string(_("Recycler"));
						esp_color = Vector4(232, 232, 232, 255);
						if (esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 4.5f)
						{
							ent->ServerRPC(_(L"SVSwitch"));
							last_recycler = get_fixedTime();
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
						if (out_w2s(world_position, w2s_position))
						{
							esp::draw_item(w2s_position, esp_name, esp_color);

							if (vars->visual.distance)
							{
								auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
								w2s_position.y += 11;
								draw_text(w2s_position, const_cast<wchar_t*>(nstr), Vector4(0, 0, 0, 1));
								w2s_position.y += 1;
								draw_text(w2s_position, const_cast<wchar_t*>(nstr), Vector4(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1));
							}
							//if (vars->visual.distance
							//	&& local_player)
							//	esp::draw_item(Vector2(w2s_position.x, w2s_position.y += 10), s, esp_color);
						}

						continue;
					}
				}
				else if (tag != 6)
					continue;
			}
		}

		//if (!found_lp)
		//	local_player = 0;

		/*
		return;
		
		*/
		esp::draw_target_hotbar(best_target);
		esp::draw_middle(best_target);
	}
	

	void draw_teammates() {
		if (!esp::local_player)
			return;

		auto team = mem::read<uintptr_t>((uintptr_t)local_player + clientTeam);

		auto member = mem::read<uintptr_t>(team + 0x30);

		auto size = mem::read<int>(member + 0x18);

		auto list = mem::read<uintptr_t>(member + 0x10);

		for (int i = 0; i < size; i++) {
			auto ent = mem::read<uintptr_t>(list + 0x20 + i * 0x8);

			auto online = mem::read<bool>(ent + 0x38);

			if (!online && !vars->visual.sleeper_esp)
				continue;

			auto id = mem::read<unsigned long>(ent + 0x20);

			if (id == esp::local_player->userID())
				continue;

			auto position = mem::read<Vector3>(ent + 0x2C);
			auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().distance(position);
			if (distance < 350.f)
				continue;

			auto player_name = (str)(*reinterpret_cast<uintptr_t*>(ent + 0x18));
			auto name = player_name->str;
			Vector2 out;
			esp::out_w2s(position, out);
			esp::draw_name({ out.x, out.y, 0 }, name);
		}
	}

	Transform* find_transform_by_id(unsigned int id_to_check) {
		auto list = (rust::classes::list*)(cliententities);
		auto value = list->get_value<uintptr_t>();
		if (!value) return nullptr;
		auto sz = list->get_size();
		if (!sz) return nullptr;
		auto buffer = list->get_buffer<uintptr_t>();

		for (size_t i = 0; i < sz; i++)
		{
			auto current = *reinterpret_cast<uintptr_t*>(buffer + 0x20 + (i * 0x8));
			if (!current)
				continue;
			auto base_object = *reinterpret_cast<uintptr_t*>(current + 0x10);
			if (!base_object)
				continue;
			auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
			if (!object)
				continue;

			WORD tag = *reinterpret_cast<WORD*>(object + 0x54);

			auto ent = *reinterpret_cast<BaseEntity**>(base_object + 0x28);

			auto net = *reinterpret_cast<Networkable**>(ent + 0x58);

			auto game_object = *reinterpret_cast<uintptr_t*>(object + 0x30);
			if (!game_object)
				continue;

			auto Transform = *reinterpret_cast<uintptr_t*>(game_object + 0x8);
			if (!Transform)
				continue;

			unsigned int id = net->get_id();

			if (id == id_to_check)
				return ent->transform();
		}
	}

	void start() {
		//esp::draw_target_snap_line();
		//draw_raid();
		
		//draw_teammates();
		iterate_players();
	}
}