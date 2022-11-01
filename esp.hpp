#pragma once
#include "RenderClass.h"
#include "vars.hpp"

#include "rust/features/player_esp.hpp"
//#include "rust/classes.hpp"

void DrawToolcupboard(Vector2 w2s, System::list<PlayerNameID*>* authed) 
{
	Vector2 screen = { 0, 0 }; 

	//create window length based off amount of items in belt
	std::vector<std::wstring> names = {};

	if (!w2s.empty())
	{
		if (!authed) return;
		Item* result = nullptr;
		authed->for_each([&](PlayerNameID* nameid, int32_t idx) {
			{
				if (nameid)
					names.push_back(nameid->get_username());
			}
		});

		Vector2 start = { w2s.x, w2s.y };
		Vector2 sz = { 130, 35.f + (names.size() * 20.f) };

		//filled gradient
		render.FillRoundedRectangle_GradientLinear(start, sz,
			{ 0.14f, 0.14f, 0.14f, 0.65f },
			{ 159.f / 255.f, 40.f / 255.f, 29.f / 255.f, 0.65f },
			2);

		render.String({ start.x + 10, start.y + 5 }, _(L"Tool cupboard"), { 0.71, 0.71, 0.71, 1 });

		//seperator
		render.Line({ start.x, start.y + 25 }, { start.x + sz.x, start.y + 25 }, { 0.11, 0.11, 0.11, 1 }, 1);

		//outline
		render.RoundedRectangle(start, sz, { 0.11, 0.11, 0.11, 1 }, 2, 1);

		if (names.size() > 0)
		{
			// 
			// box has player name
			// *separator*
			// *item* x*amount*
			// *item* x*amount*
			// *item* x*amount*
			//

			start.x += 10; //left-side margin
			start.y += 30; //margin from seperator

			int k = 0;
			for (auto i : names)
			{
				render.String({ start.x, start.y + (k++ * 20) }, i.c_str(), { 0.71, 0.71, 0.71, 1 });
			}
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
			Vector2 sz = { 200, 35.f + (items.size() * 20.f) };

			//filled gradient
			render.FillRoundedRectangle_GradientLinear(start, sz,
				{ 0.14f, 0.14f, 0.14f, 0.65f },
				{ 159.f / 255.f, 40.f / 255.f, 29.f / 255.f, 0.65f },
				2);

			std::wstring name_str(player_name); name_str = name_str + _(L"'s items");
			render.String({ start.x + 10, start.y + 5 }, name_str.c_str(), { 0.71, 0.71, 0.71, 1 });

			//seperator
			render.Line({ start.x, start.y + 25 }, { start.x + sz.x, start.y + 25 }, { 0.11, 0.11, 0.11, 1 }, 1);

			//outline
			render.RoundedRectangle(start, sz, { 0.11, 0.11, 0.11, 1 }, 2, 1);

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

	static int cases = 0;
	switch (cases) {
	case 0: { r -= 0.003f; if (r <= 0) cases += 1; break; }
	case 1: { g += 0.003f; b -= 0.003f; if (g >= 1) cases += 1; break; }
	case 2: { r += 0.003f; if (r >= 1) cases += 1; break; }
	case 3: { b += 0.003f; g -= 0.003f; if (b >= 1) cases = 0; break; }
	default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
	}

	auto list_value = entity_list->get_value<uintptr_t>();
	if (!list_value) {
		//get_client_entities();
		esp::local_player = nullptr;
		return;
	}

	auto size = entity_list->get_size();
	if (!size) {
		//get_client_entities();
		esp::local_player = nullptr;
		return;
	}

	auto buffer = entity_list->get_buffer<uintptr_t>();
	if (!buffer) {
		//get_client_entities();
		esp::local_player = nullptr;
		return;
	}

	bool found_lp = false;
	Vector4 esp_color(1, 0, 1, 1);

	wchar_t* esp_name = _(L"");

	vars->player_name_list.clear();
	bool flp = false;

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


		auto ent_net = *reinterpret_cast<Networkable**>(ent + 0x58);
		auto ent_id = ent_net->get_id();

		if (tag == 6)
		{
			auto entity = (BasePlayer*)ent;

			//check valid
			if (!entity->is_alive()
				|| (entity->is_sleeping() && !vars->visual.sleeper_esp))
				continue;
			//
			bool npc = entity->playerModel()->isnpc();
			////check npc
			if (npc && !vars->visual.npc_esp) continue;

			if (!map_contains_key(player_map, ent_id)) {
				player_map.insert(std::make_pair(ent_id, entity));

				gplayer* p = new gplayer(entity->get_player_name(), entity->userID(), 0, 0, 0, 0);
				vars->gui_player_map.insert(std::make_pair(entity->userID(), p));
				vars->chams_player_map.insert(std::make_pair(entity->userID(), 0));
				vars->handchams_player_map.insert(std::make_pair(entity->userID(), 0));
			}

			bool is_friend = false, follow = false, block = false;
			if (map_contains_key(vars->gui_player_map, entity->userID())) {
				is_friend = vars->gui_player_map[entity->userID()]->is_friend;
				follow = vars->gui_player_map[entity->userID()]->follow;
				block = vars->gui_player_map[entity->userID()]->block;
			}

			//local player chams, player average velocity
			if (entity->is_local_player()) {
			//if (entity->userID() == LocalPlayer::Entity()->userID()) {
				flp = true;
				esp::local_player = entity;
			}
			else {
				if (esp::local_player)
				{
					if (vars->visual.targetted)
					{
						auto pl = ((BasePlayer*)ent);
						auto playerpos = pl->model()->boneTransforms()->get(48)->position();
						auto localpos = esp::local_player->model()->boneTransforms()->get(48)->position();
						auto distance = playerpos.distance(localpos);
						auto fwd = playerpos + (pl->eyes()->body_forward() *  distance);

						auto dist = fwd.distance(localpos);
						auto r = (distance / 50.f); r = (r < 1.f ? 1.f : r);
						//Line(playerpos, fwd, { 1, 1, 1, 1 }, 0.05f, true, true);
						//Sphere(fwd, r, { 1, 1, 1, 1 }, 0.05f, 100.f);
						if (dist < r)
						{
							auto ws = string::wformat(_(L"%s is looking at you!"), ((BasePlayer*)ent)->get_player_name());
							vars->targetted = true;
							//render.StringCenter({ vars->ScreenX / 2 + 1, (vars->ScreenY / 2) - 30 }, ws, { 0, 0, 0, 1 });
							//render.StringCenter({ vars->ScreenX / 2 - 1, (vars->ScreenY / 2) - 30 }, ws, { 0, 0, 0, 1 });
							//render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 + 1 }, ws, { 0, 0, 0, 1 });
							//render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 - 1 }, ws, { 0, 0, 0, 1 });
							render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 }, ws, { 227 / 255.f, 32 / 255.f, 61 / 255.f, 1 });
						}
					}

					if (vars->visual.showpred
						&& esp::best_target.ent)
					{
						auto pos = esp::local_player->eyes()->position();
						auto targetpos = esp::best_target.pos;
						auto item = esp::local_player->GetActiveItem();
						if (item) {
							auto held = item->GetHeldEntity<BaseProjectile>();
							if (held) {
								auto outvel = Vector3::Zero();
								auto aimdir = Vector3::Zero();
								float travel = 0.f;
								float partialtime = 0.f;
								//float drag = held->get_item_mod_projectile()
							}
						}
					}

					if (vars->visual.grenadeprediction) {
						auto item = entity->GetActiveItem();
						if (item) {
							auto h = item->GetHeldEntity<ThrownWeapon>();
							if (h) {
								GrenadePath* temp = new GrenadePath(entity, h);
								if (!strcmp(h->get_class_name(), _("GrenadeWeapon"))
									|| !strcmp(h->get_class_name(), _("MolotovCocktail"))
									|| !strcmp(h->get_class_name(), _("ThrownWeapon"))
									|| !strcmp(h->get_class_name(), _("SmokeGrenade"))
									|| !strcmp(h->get_class_name(), _("TimedExplosive"))) {
									if (!map_contains_key(grenade_map, entity->userID()))
										grenade_map.insert(std::make_pair(entity->userID(), temp));
									else
										grenade_map[entity->userID()] = temp;
								}
								else {
									if (map_contains_key(grenade_map, entity->userID()))
										grenade_map[entity->userID()] = new GrenadePath();
								}


								if (map_contains_key(grenade_map, entity->userID())) {
									auto current = grenade_map[entity->userID()];
									if (current->ply
										&& current->weapon
										&& !current->endposition.is_empty()) {
										if (current->endposition.distance(esp::local_player->transform()->position()) < 4.f) {
											//std::string on();
											auto ws = string::wformat(_(L"Grenade from %s"), entity->get_player_name());
											render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 50 }, ws, { 227 / 255.f, 32 / 255.f, 61 / 255.f, 1 });
										}
									}
								}
							}
						}
					}

					if (vars->wants_shoot)
						goto draw;

					if (!vars->combat.targetfriends && is_friend)
						goto draw;

					auto target = aim_target();
					switch (vars->combat.aimbone)
					{
					case 0: //Head
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::head)->position();
						break;
					case 1: //Spine 4
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::spine4)->position();
						break;
					case 2: //Pelvis
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::pelvis)->position();
						break;
					case 3: //R arm
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::r_forearm)->position();
						break;
					case 4: //L arm
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::l_forearm)->position();
						break;
					case 5: //R leg
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::r_knee)->position();
						break;
					case 6: //L leg
						target.pos = ent->model()->boneTransforms()->get((int)rust::classes::Bone_List::l_knee)->position();
						break;
					}
					auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos); //crashes bc non game thread
					target.distance = distance;

					auto fov = unity::get_fov(target.pos);
					target.fov = fov;

					target.ent = (BaseCombatEntity*)ent;

					target.network_id = ent_id;

					//auto visible = ent->is_visible(esp::local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
					auto visible = entity->visible();

					target.visible = visible;

					//if (distance < 300.0f)
					//{
					//	visibleplayerswithin300m.push_back(target);
					//}

					if (vars->combat.locktarget)
					{
						if (unity::GetKey(vars->keybinds.locktarget))
							goto choosetarget;
						else if (((BasePlayer*)entity)->userID() == ((BasePlayer*)esp::best_target.ent)->userID())
							goto setandrecordtarget;
						else
							goto draw;
					}

				choosetarget:
					if (target < esp::best_target
						|| !esp::best_target.ent->is_alive()
						|| (target.ent && ((BasePlayer*)target.ent)->userID() == ((BasePlayer*)esp::best_target.ent)->userID()))
					{
					setandrecordtarget:
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
							if (esp::best_target.velocity_list.size() < 30) //0.03125 * 30 = 0.9 seconds
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

					if (esp::best_target.distance > 400.f)
						esp::best_target = aim_target();
					if (target < esp::best_target
						&& target.fov < vars->combat.aimbotfov)
						esp::best_target = target;
					if (esp::best_target.fov > vars->combat.aimbotfov)
						esp::best_target = aim_target();
				}

			draw:
				if (vars->visual.playeresp && esp::local_player)
				{
					//DrawPlayer(entity, npc);
					//offscreen indicator?
					//silent melee?
				}
			}
		}

		if (esp::local_player)
		{
			//select entity			
			if (esp::selected_entity_id == ent_id) {
			
				Vector2 w2s_position = {};
				if (esp::out_w2s(world_position, w2s_position))
				{
					esp_color = Vector4(54, 116, 186, 255);
					w2s_position.y += 10;
					if (esp::selected_entity_id == ent_id) {
						render.StringCenter(w2s_position, _(L"selected"), { 54 / 255.f, 116 / 255.f, 186 / 255.f });
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(("[selected]")), esp_color);
						w2s_position.y += 10;
						render.StringCenter(w2s_position, string::wformat(_(L"[%d]"), (int)ent_id), { 0.f, 1.f, 0.f });
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[%d]"), (int)ent_id)), esp_color);
					}
				}
			}

			auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
			
			//buildingblock stuff
			if (vars->visual.block_chams > 0 || vars->misc.auto_upgrade) {
				if (!strcmp(entity_class_name, _("BuildingBlock"))) {
					if (vars->visual.block_chams > 0) {
						if (unity::bundle && unity::bundle2)
						{
							Shader* shader = 0;
							Material* mat = 0;
							switch (vars->visual.block_chams)
							{
							case 1:
								shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"Chams"), unity::GetType(_("UnityEngine"), _("Shader")));
								break;
							case 2:
								mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/2dmat.mat"), unity::GetType(_("UnityEngine"), _("Material")));
								break;
							case 3:
								mat = (Material*)unity::LoadAsset(unity::galaxy_bundle, string::wformat(vars->visual.galaxymatblock < 10 ? _(L"GalaxyMaterial_0%d") : _(L"GalaxyMaterial_%d"), vars->visual.galaxymatblock), unity::GetType(_("UnityEngine"), _("Material")));
								break;
							}
							entchams(ent, shader, mat);
						}
					}
					if (vars->misc.auto_upgrade) {
						auto block = (BuildingBlock*)ent;
						rust::classes::BuildingGrade upgrade_tier = (rust::classes::BuildingGrade)(vars->misc.upgrade_tier + 1);
						auto distance = esp::local_player->eyes()->position().distance(world_position);
						if (distance < 4.2f) {
							if (!esp::closest_building_block)
								esp::closest_building_block = (uintptr_t)block;
							else
							{
								if (block->grade() != upgrade_tier) {
									auto tranny = ((BuildingBlock*)esp::closest_building_block)->transform();
									auto pos = tranny->position();
									auto lastdist = esp::local_player->eyes()->position().distance(pos);
									if (lastdist > distance)
										esp::closest_building_block = (uintptr_t)block;
								}
							}
						}
					}
				}
			}

			//collectibleitem stuff
			if (vars->misc.pickup_collectibles) {
				if (!strcmp(entity_class_name, _("CollectibleEntity"))) {
					auto dist = world_position.distance(esp::local_player->transform()->position());
					if (dist < 4.f)
						ent->ServerRPC(_(L"Pickup"));
				}
			}

			//misc esp
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
				vars->visual.tc_esp ||
				vars->visual.collectibles ||
				vars->visual.dropped_items ||
				vars->visual.corpses ||
				vars->visual.block_chams > 0 ||
				vars->visual.rock_chams > 0)
			{
				auto dist = world_position.distance(esp::local_player->transform()->position());
				if (vars->visual.dist_on_items < dist) continue;
				esp_name = _(L"");
				auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				if (!object_name.zpad)
					continue;

				auto m = esp::local_player->model();
				if (m)
				{
					auto trans = m->boneTransforms()->get(48);
					if (esp::local_player && vars->visual.distance && trans)
						dist = trans->position().distance(world_position);
				}

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
					
					//	esp::draw_weapon_icon(item, w2s_position);
					//esp::draw_item(w2s_position, 0, esp_color, item_name);
					if (esp::out_w2s(world_position, w2s_position)) {
						if (vars->visual.distance)
						{
							if (item->GetAmount() > 1)
								render.StringCenter(w2s_position, string::wformat(_(L"%s [x%d]"), item_name, item->GetAmount()), FLOAT4TOD3DCOLOR(vars->colors.items.dropped));
							else render.StringCenter(w2s_position, item_name, FLOAT4TOD3DCOLOR(vars->colors.items.dropped));
							auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
							w2s_position.y += 12;
							render.StringCenter(w2s_position, nstr, FLOAT4TOD3DCOLOR(vars->colors.players.details.distance.visible));
							//draw_text(w2s_position, const_cast<wchar_t*>(nstr), Vector4(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1));
						}
					}
					continue;
				}

				//ladder
				if (!strcmp(entity_class_name, _("BaseLadder")))
				{
					if (vars->visual.ladder) {
						esp_name = _(L"Ladder");
						float col[3] = { 0, 219 / 255.f, 58 / 255.f };

						world_position.y += 1.f;
						Vector2 w2s_position = {};
						if (esp::out_w2s(world_position, w2s_position))
						{
							render.StringCenter(w2s_position, esp_name, FLOAT4TOD3DCOLOR(col));
							//esp::draw_item(w2s_position, esp_name, esp_color);
						}
						continue;
					}
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
					world_position.y += 1.f;
					if (esp::out_w2s(world_position, w2s_position))
					{
						world_position.y += 1.5f;
						//if(esp::local_player->bones()->head->position.distance(world_position) < 50.f)
						if(esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 50.f)
							DrawToolcupboard(w2s_position, authorizedPlayers_list);
						//esp::draw_tool_cupboard(w2s_position, il2cpp::methods::new_string(_("Tool Cupboard")), Vector4(255, 0, 0, 255), authorizedPlayers_list);
					}
					continue;
				}

				//heli
				if (std::string(object_name.zpad).find(_("patrol")) != std::string::npos && vars->visual.heli_esp) {
					auto base_heli = reinterpret_cast<BaseHelicopter*>(ent);
					if (!base_heli) continue;
					Vector2 rearrotor, beam, mainrotor;
					esp::out_w2s(base_heli->model()->boneTransforms()->get(22)->position(), rearrotor);
					esp::out_w2s(base_heli->model()->boneTransforms()->get(19)->position(), mainrotor);
					esp::out_w2s(base_heli->model()->boneTransforms()->get(56)->position(), beam);
					esp_name = _(L"Patrol-heli");
					esp_color = Vector4(232, 232, 232, 255);


					const Vector2 diff = { (beam.x + rearrotor.x) / 2, (beam.y + rearrotor.y) / 2 };

					const float h = max(beam.y, rearrotor.y) - min(beam.y, rearrotor.y);
					const float y = diff.y;
					const float w = (max(beam.x, rearrotor.x) - min(beam.x, rearrotor.x));
					float x = diff.x - h / 4;

					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{
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

								//auto visible = esp::local_player->is_visible(esp::local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
								//target.visible = visible;

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

						if (vars->visual.heli_esp
							&& base_heli->is_alive())
						{
							render.StringCenter({ x, y - 5 }, _(L"Patrol-heli"), { 1, 1, 1, 1 });
							render.Rectangle({ x - w, y - h }, { w * 2, h }, { 0, 0, 0, 0 }, 2);
							render.Rectangle({ x - w, y - h }, { w * 2, h }, FLOAT4TOD3DCOLOR(vars->colors.players.boxes.visible), 1);
						}
					}
					continue;
				}

				//stash
				if (vars->visual.stash && *(int*)(object_name.zpad + 46) == '_hsa') {
					esp_name = _(L"Stash");
				}

				//vars->visual.collectibles
				if (vars->visual.collectibles) {
					if (!strcmp(entity_class_name, _("CollectibleEntity"))
						|| std::string(object_name.zpad).find(_("collectablecandy")) != std::string::npos) {
						if (std::string(object_name.zpad).find(_("wood")) != std::string::npos)
							esp_name = _(L"Wood [collectible]");
						else if (std::string(object_name.zpad).find(_("metal")) != std::string::npos)
							esp_name = _(L"Metal [collectible]");
						else if (std::string(object_name.zpad).find(_("sulfur")) != std::string::npos)
							esp_name = _(L"Sulfur [collectible]");
						else if (std::string(object_name.zpad).find(_("stone")) != std::string::npos)
							esp_name = _(L"Stone [collectible]");
						else if (std::string(object_name.zpad).find(_("shroom")) != std::string::npos)
							esp_name = _(L"Mushroom [collectible]");
						else if (std::string(object_name.zpad).find(_("collectablecandy")) != std::string::npos)
							esp_name = _(L"Halloween Candy");
						else if (std::string(object_name.zpad).find(_("cloth")) != std::string::npos && !vars->visual.cloth)
							esp_name = _(L"Cloth [collectible]");
						esp_color = Vector4(vars->colors.items.collectibles[0] * 255.f,
							vars->colors.items.collectibles[1] * 255.f,
							vars->colors.items.collectibles[2] * 255.f,
							1);
					}
				}

				//recycler
				else if (vars->misc.norecycler && *(int*)(entity_class_name) == 'yceR' && get_fixedTime() > esp::last_recycler + 0.35f) {
					esp_name = _(L"Recycler");
					esp_color = Vector4(232, 232, 232, 255);
					if (esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 4.5f)
					{
						ent->ServerRPC(_(L"SVSwitch"));
						esp::last_recycler = get_fixedTime();
					}
					world_position.y += 1.f;
				}

				if (std::string(object_name.zpad).find(_("-ore")) != std::string::npos) {
				//if(!strcmp(entity_class_name, _("OreResourceEntity"))) {
					if (unity::bundle && unity::bundle2)
					{
						Shader* shader = 0;
						Material* mat = 0;
						switch (vars->visual.rock_chams)
						{
						case 1:
							shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"Chams"), unity::GetType(_("UnityEngine"), _("Shader")));
							break;
						case 2:
							mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/2dmat.mat"), unity::GetType(_("UnityEngine"), _("Material")));
							break;
						case 3:
							mat = (Material*)unity::LoadAsset(unity::galaxy_bundle, string::wformat(vars->visual.galaxymatrock < 10 ? _(L"GalaxyMaterial_0%d") : _(L"GalaxyMaterial_%d"), vars->visual.galaxymatrock), unity::GetType(_("UnityEngine"), _("Material")));
							break;
						}

						if (vars->visual.rock_chams > 0)
						{
							entchams(ent, shader, mat);
						}
					}
				}

				//stone
				if (vars->visual.stone_ore && (*(int*)(object_name.zpad + 52) == 'nots' || *(int*)(object_name.zpad + 47) == 'nots')) {
					esp_name = _(L"Stone Ore");
					esp_color = Vector4(232, 232, 232, 255);
					world_position.y += 1.f;
				}

				//sulfur
				else if (vars->visual.sulfur_ore && (*(int*)(object_name.zpad + 52) == 'flus' || *(int*)(object_name.zpad + 47) == 'flus')) {
					esp_name = _(L"Sulfur Ore");
					esp_color = Vector4(203, 207, 0, 255);
					world_position.y += 1.f;
				}

				//metal
				else if (vars->visual.metal_ore && (*(int*)(object_name.zpad + 52) == 'atem' || *(int*)(object_name.zpad + 47) == 'atem')) {
					esp_name = _(L"Metal Ore");
					esp_color = Vector4(145, 145, 145, 255);
					world_position.y += 1.f;
				}

				//hackable crate
				if (*(int*)(entity_class_name) == 'kcaH' && *(int*)(entity_class_name + 14) == 'tarC') {
					auto flag = *reinterpret_cast<int*>(ent + 0x128);
					if (flag != 128 && flag != 256)
						continue;

					world_position.y += 1.f;
					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{
						auto timer = *reinterpret_cast<uintptr_t*>((uintptr_t)ent + 0x440);
						if (!timer) continue;
						auto text = (str)(*reinterpret_cast<uintptr_t*>(timer + 0xD0));
						if (!text) continue;
						auto str = text->str;
						if (!str) continue;
						render.StringCenter(w2s_position, string::wformat(_(L"Hackable crate: %ss"), str), { 130/255.f, 200/255.f, 230/255.f });
					}
				}

				//traps
				else if (vars->visual.traps && (*(int*)(object_name.zpad + 36) == 'terr' || *(int*)(object_name.zpad + 43) == 'tnug' || *(int*)(object_name.zpad + 38) == 'rtra')) {
					if (*(int*)(object_name.zpad + 36) == 'terr')
						esp_name = _(L"Auto Turret*");
					else if (*(int*)(object_name.zpad + 43) == 'tnug')
						esp_name = _(L"Shotgun Trap*");
					else if (*(int*)(object_name.zpad + 38) == 'rtra')
						esp_name = _(L"Bear Trap*");

					esp_color = Vector4(255, 166, 0, 255);
				}

				//vehicles
				else if (vars->visual.vehicles && *(int*)(entity_class_name + 4) == 'iheV') {
					if (std::string(object_name.zpad).find(_("standing")) != std::string::npos)
						esp_name = _(L"Rhib");
					else if(std::string(object_name.zpad).find(_("small")) != std::string::npos)
						esp_name = _(L"Small boat");
					else if(std::string(object_name.zpad).find(_("mini")) != std::string::npos)
						esp_name = _(L"Minicopter");
					else if(std::string(object_name.zpad).find(_("scrap")) != std::string::npos)
						esp_name = _(L"Scrap helicopter");
					else if(std::string(object_name.zpad).find(_("snow")) != std::string::npos)
						esp_name = _(L"Snowmobile");
					else if(std::string(object_name.zpad).find(_("saddle")) != std::string::npos)
						esp_name = _(L"Horse");

					esp_color = Vector4(0, 161, 219, 255);
					world_position.y += 1.f;
				}

				//airdrop
				else if (vars->visual.airdrops && *(int*)(object_name.zpad + 39) == 'pord') {
					world_position.y += 1.f;
					esp_name = _(L"Airdrop");
					esp_color = Vector4(0, 161, 219, 255);
				}

				//cloth
				else if (vars->visual.cloth && *(int*)(object_name.zpad + 52) == 'c-pm') {
					world_position.y += 1.f;
					esp_name = _(L"Cloth");
					esp_color = Vector4(0, 219, 58, 255);
				}

				//corpse
				else if (vars->visual.corpses && *(int*)(object_name.zpad + 29) == 'proc') {
					esp_name = _(L"Player Corpse");
					esp_color = Vector4(230, 230, 230, 255);
				}

				//stash open?
				if (tag != 6
					&& wcscmp(esp_name, L"")) {
					if (*(int*)(entity_class_name) == 'satS') {
						auto flag = *reinterpret_cast<int*>(ent + 0x138);
						if (flag != 2048)
							continue;
					}
					//auto s = il2cpp::methods::new_string(string::format(_("[%dm]"), (int)local_player->eyes()->get_position().distance(world_position)));
					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{
						float color[3] = { esp_color.x / 255.f, esp_color.w / 255.f, esp_color.z / 255.f };
						render.StringCenter(w2s_position, esp_name, FLOAT4TOD3DCOLOR(color));
						//esp::draw_item(w2s_position, esp_name, esp_color);

						if (vars->visual.distance)
						{
							auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
							w2s_position.y += 11;
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
		&& vars->visual.hotbar_esp
		&& esp::local_player)
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
	if (vars->desyncTime < 0) return;
	render.StringCenter({ 100.f, y }, _(L"Desync"), FLOAT4TOD3DCOLOR(vars->accent_color), true, 20.f);
	render.ProgressBar({ 40, y + 10.f }, { 120, 8 }, vars->desyncTime, 1.0f);
}
void IndicatorReload() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	if (vars->time_since_last_shot < vars->reload) {
		render.StringCenter({ 100.f, (y - 30.f) }, _(L"Reload"), FLOAT4TOD3DCOLOR(vars->accent_color), true, 20.f);
		render.ProgressBar({ 40, y - 20.f }, { 120, 8 }, vars->time_since_last_shot, (vars->reload - 0.2f));
	}
}
void IndicatorFlyhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;

}
void IndicatorSpeedhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	if (vars->speedhack <= 0) return;
	render.StringCenter({ 100.f, (y - 60.f) }, _(L"Speedhack"), FLOAT4TOD3DCOLOR(vars->accent_color), true, 20.f);
	render.ProgressBar({ 40, y - 50.f }, { 120, 8 }, vars->speedhack, 1.0f);
}
void IndicatorTp() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 50 }, { 120, 4 }, vars->last_teleport_time - get_fixedTime(), 10.0f);
}

void TargettedIndicator() {
	if (!vars->visual.targetted) return;
	if (vars->targetted) {
		//lol
		vars->targetted = false;
	}
}

void DrawSnapline() {
	Vector2 start = vars->visual.snapline == 1 ? Vector2(vars->ScreenX / 2, 0) :
		vars->visual.snapline == 2 ? Vector2(vars->ScreenX / 2, vars->ScreenY / 2) :
		vars->visual.snapline == 3 ? Vector2(vars->ScreenX / 2, vars->ScreenY) :
		Vector2(vars->ScreenX / 2, vars->ScreenY);
}

void DrawFov() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(esp::best_target.visible ? vars->colors.ui.fov.visible : vars->colors.ui.fov.invisible), vars->combat.aimbotfov, 0.7f);
}

void Watermark() {
	render.FillRectangle(Vector2(7, 7), Vector2(40, 16), FLOAT4TOD3DCOLOR(vars->accent_color_opaque));
	render.FillRectangle(Vector2(6, 6), Vector2(42, 18), D2D1::ColorF(0.11f, 0.11f, 0.11f, 0.6));
	render.FillRectangle(Vector2(7, 21), Vector2(40, 2), FLOAT4TOD3DCOLOR(vars->accent_color_opaque));
	render.String({ 14,7 }, _(L"Matrix"), FLOAT4TOD3DCOLOR(vars->accent_color));
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
	if (vars->visual.speedhack_indicator)
		IndicatorSpeedhack();
	if (vars->misc.tp)
		IndicatorTp();

	if (vars->visual.flyhack_indicator)
		IndicatorFlyhack();


	//Draw FOV
	if (vars->visual.show_fov)
		DrawFov();

	//Draw watermark
	Watermark();
	//if (esp::local_player
	//	&& vars->visual.hitpoint) {
	//	int rindex = -1;
	//	for (size_t i = 0; i < hitpoints.size(); i++)
	//	{
	//		auto h = hitpoints[i];
	//		auto w2s = WorldToScreen(h.position);
	//		render.StringCenter({ w2s.x - 1, w2s.y }, _(L"hit"), { 0, 0, 0, 1 });
	//		render.StringCenter({ w2s.x + 1, w2s.y }, _(L"hit"), { 0, 0, 0, 1 });
	//		render.StringCenter({ w2s.x, w2s.y - 1 }, _(L"hit"), { 0, 0, 0, 1 });
	//		render.StringCenter({ w2s.x, w2s.y + 1 }, _(L"hit"), { 0, 0, 0, 1 });
	//		render.StringCenter({ w2s.x, w2s.y }, _(L"hit"), FLOAT4TOD3DCOLOR(vars->colors.ui.hitpoints));
	//		if (h.time + 5.f < get_fixedTime()) rindex = i;
	//	}
	//	if (rindex != -1) hitpoints.erase(hitpoints.begin() + rindex);
	//}

	//iterate_entities();
	if (!esp::local_player) return;

	//does it make the object 3 times? it lags and flickers lots.

	for (auto o : vars->RenderList) {
		switch (o->type) {
		case RenderObject::RenderType::Entity:
		{
			auto obj = (RenderEntity*)o;

			if (obj->Name.size() > 0) {
				render.StringCenter(obj->NamePos, obj->Name.c_str(), FLOAT4TOD3DCOLOR(obj->NameColor));
			}
			if (obj->Dist > 0.f) {
				render.StringCenter(obj->DistPos, string::wformat(_(L"[%dm]"), (int)(obj->Dist)), FLOAT4TOD3DCOLOR(obj->DistColor));
			}
			if (obj->IsPlayer) {
				if (obj->BoxLines.size() > 0) {
					for (auto l : obj->BoxLines) {
						render.Line(l->Start, l->End, FLOAT4TOD3DCOLOR(l->Color), l->Thickness);
						//delete l;
					}
					obj->BoxLines.clear();
				}
				if (obj->Skeleton.size() > 0) {
					for (auto l : obj->Skeleton) {
						render.Line(l->Start, l->End, FLOAT4TOD3DCOLOR(l->Color), l->Thickness);
					}
					obj->Skeleton.clear();
				}
				if (obj->Flags.size() > 0) {
					for (auto f : obj->Flags) {
						render.String(f->pos, f->Name.c_str(), FLOAT4TOD3DCOLOR(f->Color));
						delete f;
					}
					obj->Flags.clear();
				}
				if (!obj->Hp->BarStart.empty() && !obj->Hp->Sz.empty()
					&& !obj->Hp->Start.empty() && !obj->Hp->BarSz.empty()) {
					render.FillRectangle(obj->Hp->Start, obj->Hp->Sz, FLOAT4TOD3DCOLOR(vars->bl));
					render.FillRectangle(obj->Hp->BarStart, obj->Hp->BarSz, FLOAT4TOD3DCOLOR(obj->Hp->Color));
					//delete obj->Hp;
				}
			}

			o->HasBeenDrawn = true;
		}
		case RenderObject::RenderType::Hotbar:
		{
			auto obj = (RenderHotbar*)o;

			o->HasBeenDrawn = true;
		}
		case RenderObject::RenderType::TC:
		{
			auto obj = (RenderToolCupboard*)o;

			o->HasBeenDrawn = true;
		}
		}
		vars->RenderList.erase(std::remove(vars->RenderList.begin(), vars->RenderList.end(), o), vars->RenderList.end());
	}
}