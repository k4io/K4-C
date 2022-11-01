#pragma once
#include "../../mmisc.hpp"
#include <array>
#include <cstdint>

#include "../../memory/il2cpp.hpp"
#include "../../settings.hpp"

#include "../classes.hpp"
#include "../rust.hpp"
#include <map>

float r = 1.00f, g = 0.00f, b = 1.00f;

void ColorConvertHSVtoRGB2(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = my_fmod(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

D2D1::ColorF HSVD(float h, float s, float v, float a = 1.0f)
{
	float r, g, b; ColorConvertHSVtoRGB2(h, s, v, r, g, b); return D2D1::ColorF(r, g, b, a);
}

std::vector<BasePlayer*> player_list = {};
std::map<int32_t, BasePlayer*> player_map = {};

struct hit {
	Vector3 position;
	float time;
};
std::vector<hit> hitpoints = {};


void entchams(BaseEntity* ent, Shader* s = 0, Material* m = 0) {
	if (!ent) return;
	if (!m && !s) return;
	__try {
		auto renderers = ((Networkable*)ent)->GetComponentsInChildren(unity::GetType(_("UnityEngine"), _("Renderer")));
		if (!renderers) return;
		auto sz = renderers->get_size();
		for (size_t i = 0; i < sz; i++)
		{
			auto renderer = (Renderer*)renderers->get(i);
			if (!renderer) continue;
			if (m) {
				auto cm = renderer->GetMaterial();
				auto shader1 = cm->GetShader();
				auto shader2 = m->GetShader();
				SetInt((uintptr_t)m, _(L"_ZTest"), 8);
				if (shader1 != shader2)
					renderer->SetMaterial(m);
			}
			else {
				auto mat = renderer->GetMaterial();
				SetInt((uintptr_t)mat, _(L"_ZTest"), 8);
				if (mat) {
					auto shader = mat->GetShader();
					if (shader != s)
						mat->SetShader(s);
				}
			}
		}
	}
	__except (true) { return; }
}

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


	void DrawPlayer(BasePlayer* ply, bool npc)
	{
		if (!ply) return;
		if (!esp::local_player) return;
		if (npc && !vars->visual.npc_esp) return;

		auto player_id = ply->userID();

		//esp::do_chams(ply);

		bounds_t bounds;

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


		const auto get_bounds = [&](bounds_t& out, float expand = 0) -> bool {
			bounds = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };

			for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {

				auto Transform = ply->model()->boneTransforms()->get(bone_idx);

				world_position = Transform->position();

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
			RenderEntity* en = new RenderEntity();

			//is_visible = unity::is_visible(camera_position, bones[48].world_position, (uintptr_t)esp::local_player);
			//is_visible = ply->visible();
			is_visible = true;
			if (vars->combat.vischeck) {
				__try {
					for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {
						if (is_visible) break;
						is_visible = unity::is_visible(camera_position, world_position, (uintptr_t)esp::local_player);
					}
				}
				__except (true) {}
			}

			float box_width = bounds.right - bounds.left;
			float box_height = bounds.bottom - bounds.top;
			auto name = ply->_displayName()->str;
			auto activeitem = ply->GetActiveItem();

			const auto rainbowcolor = D2D1::ColorF(r, g, b, 1);
			vars->rainbow[0] = r; vars->rainbow[1] = g; vars->rainbow[2] = b;

			//health bar
			const auto cur_health = ply->health();
			const auto max_health = (npc ? ply->maxHealth() : 100);
			const auto health_pc = min(cur_health / max_health, 1);
			const auto health_color =
				HSVD((health_pc * .25f), 1, .875f * 1);
			const auto h = (bounds.bottom - bounds.top) * health_pc;

			float* hp = new float[4]{ health_color.r, health_color.g, health_color.b, 1 };

			//boxes
			auto box_color = is_visible ? vars->colors.players.boxes.visible : vars->colors.players.boxes.invisible;
			if (vars->visual.rainbowbox) { box_color[0] = r; box_color[1] = g; box_color[2] = b; }

			if (vars->visual.custombox && strlen(vars->visual.boxfilename) > 0)
			{
				std::string ap(vars->visual.boxfilename);
				std::string path = vars->data_dir + _("images\\") + ap;
				if (!ap.empty()) {
					if (std::filesystem::exists(path)) {
						if (vars->customboxpath != path) {
							vars->customboxpath = path;
							//set custom box
							//render->SetCustomBox(vars->customboxpath);
						}
						//else render->DrawCustomBox({ bounds.left, bounds.top }, { box_width, box_height });//draw custom box
					}
				}
			}
			else {
				switch (vars->visual.boxtype)
				{
				case 1: //full box
					//render->Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, { 0,0,0,1 }, 4.f);
					//render->Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, FLOAT4TOD3DCOLOR(box_color), 2.f);
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left + box_width, bounds.top }, vars->bl));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left, bounds.top + box_height }, vars->bl));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left + box_width, bounds.bottom }, vars->bl));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right, bounds.top + box_height }, vars->bl));

					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left + box_width, bounds.top }, box_color));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left, bounds.top + box_height }, box_color));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left + box_width, bounds.bottom }, box_color));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right, bounds.top + box_height }, box_color));
					break;
				case 2: //corner box

					//top left
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, box_color, 2.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, vars->bl, 4.f));
					en->BoxLines.push_back(new RLine({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, box_color, 2.f));
					break;
				case 3: //3d cube
				{
					auto wid = 4;
					Bounds cbounds = Bounds();

					auto lfoott = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::l_foot);
					auto lfootp = lfoott->position();
					auto rfoott = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::r_foot);
					auto rfootp = lfoott->position();
					auto spine3 = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::spine3)->position();
					auto spine4 = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::spine4)->position();
					auto eyepos = spine3.midPoint(spine4);
					Vector3 mp = Vector3(eyepos.x, eyepos.y - 1.2f, eyepos.z);//lfootp.midPoint(rfootp);

					if (ply->modelState()->has_flag(rust::classes::ModelState_Flag::Ducked)) {
						//auto midpoint = ent->FindBone(_(L""))
						cbounds.center = mp + Vector3(0.0f, 0.55f, 0.0f);
						cbounds.extents = Vector3(0.4f, 0.65f, 0.4f);
					}
					else {
						if (ply->modelState()->has_flag(rust::classes::ModelState_Flag::Crawling) || ply->modelState()->has_flag(rust::classes::ModelState_Flag::Sleeping)) {
							cbounds.center = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::pelvis)->position();
							cbounds.extents = Vector3(0.9f, 0.2f, 0.4f);
						}
						else {
							cbounds.center = mp + Vector3(0.0f, 0.85f, 0.0f);
							cbounds.extents = Vector3(0.4f, 0.9f, 0.4f);
						}
					}

					auto rotate_point = [&](Vector3 center, Vector3 origin, float angle) {
						float num = angle * 0.0174532924f;
						float num2 = -std::sin(num);
						float num3 = std::cos(num);
						origin.x -= center.x;
						origin.z -= center.z;
						float num4 = origin.x * num3 - origin.z * num2;
						float num5 = origin.x * num2 + origin.z * num3;
						float num6 = num4 + center.x;
						num5 += center.z;
						return Vector3(num6, origin.y, num5);
					};

					float y = EulerAngles(ply->eyes()->rotation()).y;
					Vector3 center = cbounds.center;
					Vector3 extents = cbounds.extents;
					Vector3 frontTopLeft = rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontTopRight = rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontBottomLeft = rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 frontBottomRight = rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 backTopLeft = rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z + extents.z), y);
					Vector3 backTopRight = rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z + extents.z), y);
					Vector3 backBottomLeft = rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z + extents.z), y);
					Vector3 backBottomRight = rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z + extents.z), y);

					Vector2 frontTopLeft_2d, frontTopRight_2d, frontBottomLeft_2d, frontBottomRight_2d, backTopLeft_2d, backTopRight_2d, backBottomLeft_2d, backBottomRight_2d;

					auto xy = WorldToScreen(frontTopLeft);
					frontTopLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(frontTopRight);
					frontTopRight_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(frontBottomLeft);
					frontBottomLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(frontBottomRight);
					frontBottomRight_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(backTopLeft);
					backTopLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(backTopRight);
					backTopRight_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(backBottomLeft);
					backBottomLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(backBottomRight);
					backBottomRight_2d = Vector2(xy.x, xy.y);

					if (!frontTopLeft_2d.empty() &&
						!frontTopRight_2d.empty() &&
						!frontBottomLeft_2d.empty() &&
						!frontBottomRight_2d.empty() &&
						!backTopLeft_2d.empty() &&
						!backTopRight_2d.empty() &&
						!backBottomLeft_2d.empty() &&
						!backBottomRight_2d.empty()) {
						en->BoxLines.push_back(new RLine(frontTopLeft_2d, frontTopRight_2d, box_color));
						en->BoxLines.push_back(new RLine(frontTopRight_2d, frontBottomRight_2d, box_color));
						en->BoxLines.push_back(new RLine(frontBottomRight_2d, frontBottomLeft_2d, box_color));
						en->BoxLines.push_back(new RLine(frontBottomLeft_2d, frontTopLeft_2d, box_color));
						en->BoxLines.push_back(new RLine(backTopLeft_2d, backTopRight_2d, box_color));
						en->BoxLines.push_back(new RLine(backTopRight_2d, backBottomRight_2d, box_color));
						en->BoxLines.push_back(new RLine(backBottomRight_2d, backBottomLeft_2d, box_color));
						en->BoxLines.push_back(new RLine(backBottomLeft_2d, backTopLeft_2d, box_color));
						en->BoxLines.push_back(new RLine(frontTopLeft_2d, backTopLeft_2d, box_color));
						en->BoxLines.push_back(new RLine(frontTopRight_2d, backTopRight_2d, box_color));
						en->BoxLines.push_back(new RLine(frontBottomRight_2d, backBottomRight_2d, box_color));
						en->BoxLines.push_back(new RLine(frontBottomLeft_2d, backBottomLeft_2d, box_color));
					}
					break;
				}
				case 4: //3d box
				{
					auto wid = 4;
					Bounds cbounds = Bounds();

					auto lfoott = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::l_foot);
					auto lfootp = lfoott->position();
					auto rfoott = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::r_foot);
					auto rfootp = lfoott->position();
					auto spine3 = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::spine3)->position();
					auto spine4 = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::spine4)->position();
					auto eyepos = spine3.midPoint(spine4);
					Vector3 mp = Vector3(eyepos.x, eyepos.y - 1.25f, eyepos.z);//lfootp.midPoint(rfootp);

					if (ply->modelState()->has_flag(rust::classes::ModelState_Flag::Ducked)) {
						//auto midpoint = ent->FindBone(_(L""))
						cbounds.center = mp + Vector3(0.0f, 0.55f, 0.0f);
						cbounds.extents = Vector3(0.4f, 0.65f, 0.4f);
					}
					else {
						if (ply->modelState()->has_flag(rust::classes::ModelState_Flag::Crawling) || ply->modelState()->has_flag(rust::classes::ModelState_Flag::Sleeping)) {
							cbounds.center = ply->model()->boneTransforms()->get((int)rust::classes::Bone_List::pelvis)->position();
							cbounds.extents = Vector3(0.9f, 0.2f, 0.4f);
						}
						else {
							cbounds.center = mp + Vector3(0.0f, 0.8f, 0.0f);
							cbounds.extents = Vector3(0.4f, 0.9f, 0.4f);
						}
					}

					auto rotate_point = [&](Vector3 center, Vector3 origin, float angle) {
						float num = angle * 0.0174532924f;
						float num2 = -std::sin(num);
						float num3 = std::cos(num);
						origin.x -= center.x;
						origin.z -= center.z;
						float num4 = origin.x * num3 - origin.z * num2;
						float num5 = origin.x * num2 + origin.z * num3;
						float num6 = num4 + center.x;
						num5 += center.z;
						return Vector3(num6, origin.y, num5);
					};

					float y = EulerAngles(ply->eyes()->rotation()).y;
					Vector3 center = cbounds.center;
					Vector3 extents = cbounds.extents;
					Vector3 frontTopLeft = rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontTopRight = rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z - extents.z), y);
					Vector3 frontBottomLeft = rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 frontBottomRight = rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z - extents.z), y);
					Vector3 backTopLeft = rotate_point(center, Vector3(center.x - extents.x, center.y + extents.y, center.z + extents.z), y);
					Vector3 backTopRight = rotate_point(center, Vector3(center.x + extents.x, center.y + extents.y, center.z + extents.z), y);
					Vector3 backBottomLeft = rotate_point(center, Vector3(center.x - extents.x, center.y - extents.y, center.z + extents.z), y);
					Vector3 backBottomRight = rotate_point(center, Vector3(center.x + extents.x, center.y - extents.y, center.z + extents.z), y);

					Vector3 middleBottomLeft = backBottomLeft.midPoint(frontBottomLeft);
					Vector3 middleBottomRight = backBottomRight.midPoint(frontBottomRight);
					Vector3 middleTopLeft = backTopLeft.midPoint(frontTopLeft);
					Vector3 middleTopRight = backTopRight.midPoint(frontTopRight);

					Vector2 middleBottomLeft_2d, middleBottomRight_2d, middleTopLeft_2d, middleTopRight_2d;

					auto xy = WorldToScreen(middleBottomLeft);
					middleBottomLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(middleBottomRight);
					middleBottomRight_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(middleTopLeft);
					middleTopLeft_2d = Vector2(xy.x, xy.y);
					xy = WorldToScreen(middleTopRight);
					middleTopRight_2d = Vector2(xy.x, xy.y);

					if (!middleBottomLeft_2d.empty() &&
						!middleBottomRight_2d.empty() &&
						!middleTopLeft_2d.empty() &&
						!middleTopRight_2d.empty()) {
						en->BoxLines.push_back(new RLine(middleBottomLeft_2d, middleBottomRight_2d, vars->bl, 3));
						en->BoxLines.push_back(new RLine(middleBottomLeft_2d, middleBottomRight_2d, box_color, 2));
						en->BoxLines.push_back(new RLine(middleBottomLeft_2d, middleTopLeft_2d, vars->bl, 3));
						en->BoxLines.push_back(new RLine(middleBottomLeft_2d, middleTopLeft_2d, box_color, 2));
						en->BoxLines.push_back(new RLine(middleBottomRight_2d, middleTopRight_2d, vars->bl, 3));
						en->BoxLines.push_back(new RLine(middleBottomRight_2d, middleTopRight_2d, box_color, 2));
						en->BoxLines.push_back(new RLine(middleTopLeft_2d, middleTopRight_2d, vars->bl, 3));
						en->BoxLines.push_back(new RLine(middleTopLeft_2d, middleTopRight_2d, box_color, 2));
					}
					break;
				}
				}
			}

			RHealthBar *hpbar = new RHealthBar();
			switch (vars->visual.hpbar)
			{
			case 1:
				hpbar->Start = { bounds.left - 7, bounds.top - 1 };
				hpbar->Sz = { 4, box_height + 3 };
				hpbar->BarStart = { bounds.left - 6, bounds.bottom };
				hpbar->BarSz = { 2, -h - 1 };
				hpbar->Color = vars->visual.rainbowhpbar ? vars->rainbow : hp;
				//render->FillRectangle({ bounds.left - 7, bounds.top - 1 }, { 4, box_height + 3 }, { 0,0,0,1 });
				//render->FillRectangle({ bounds.left - 6, bounds.bottom }, { 2, -h - 1 }, vars->visual.rainbowhpbar ? rainbowcolor : health_color);
				break;
			case 2:
				hpbar->Start = { bounds.left, bounds.bottom + 6 };
				hpbar->Sz = { box_width + 1, 4 };
				hpbar->BarStart = { bounds.left + 1, bounds.bottom + 7 };
				hpbar->BarSz = { ((box_width / max_health) * cur_health) - 1, 2 };
				hpbar->Color = vars->visual.rainbowhpbar ? vars->rainbow : hp;
				//render->FillRectangle({ bounds.left, bounds.bottom + 6 }, { box_width + 1, 4 }, { 0,0,0,1 });
				//render->FillRectangle({ bounds.left + 1, bounds.bottom + 7 }, { ((box_width / max_health) * cur_health) - 1, 2 }, vars->visual.rainbowhpbar ? rainbowcolor : health_color);
				break;
			}
			en->Hp = hpbar;
			if (vars->visual.rainbowhpbar)
				delete[] hp;

			//skeleton
			if (vars->visual.skeleton)
			{
				//jaw
				auto Transform = ply->model()->boneTransforms()->get(48);
				if (!Transform) return;
				Vector3 world_position = Transform->position();
				Vector3 jaw = WorldToScreen(world_position);

				//spine4
				Transform = ply->model()->boneTransforms()->get(22);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 spine4 = WorldToScreen(world_position);

				//spine3
				Transform = ply->model()->boneTransforms()->get(21);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 spine3 = WorldToScreen(world_position);

				//pelvis
				Transform = ply->model()->boneTransforms()->get(7);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 pelvis = WorldToScreen(world_position);

				//l_hip
				Transform = ply->model()->boneTransforms()->get(3);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_hip = WorldToScreen(world_position);

				//r_knee
				Transform = ply->model()->boneTransforms()->get(14);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_knee = WorldToScreen(world_position);

				//l_ankle_scale
				Transform = ply->model()->boneTransforms()->get(6);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_ankle_scale = WorldToScreen(world_position);

				//r_ankle_scale
				Transform = ply->model()->boneTransforms()->get(17);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_ankle_scale = WorldToScreen(world_position);

				//r_foot
				Transform = ply->model()->boneTransforms()->get(15);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_foot = WorldToScreen(world_position);

				//l_foot
				Transform = ply->model()->boneTransforms()->get(4);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_foot = WorldToScreen(world_position);

				//r_upperarm
				Transform = ply->model()->boneTransforms()->get(55);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_upperarm = WorldToScreen(world_position);

				//l_upperarm
				Transform = ply->model()->boneTransforms()->get(24);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_upperarm = WorldToScreen(world_position);

				//r_forearm
				Transform = ply->model()->boneTransforms()->get(56);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_forearm = WorldToScreen(world_position);

				//l_forearm
				Transform = ply->model()->boneTransforms()->get(25);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_forearm = WorldToScreen(world_position);

				//r_hip
				Transform = ply->model()->boneTransforms()->get(13);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 r_hip = WorldToScreen(world_position);

				//l_knee
				Transform = ply->model()->boneTransforms()->get(2);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_knee = WorldToScreen(world_position);

				//l_hand
				Transform = ply->model()->boneTransforms()->get(26);
				if (!Transform) return;
				world_position = Transform->position();
				Vector3 l_hand = WorldToScreen(world_position);

				//r_hand
				Transform = ply->model()->boneTransforms()->get(57);
				if (!Transform) return;
				world_position = Transform->position();
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

				en->Skeleton.push_back(new RLine(Vector2(jaw.x, jaw.y), Vector2(spine4.x, spine4.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(spine4.x, spine4.y), Vector2(spine3.x, spine3.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(spine4.x, spine4.y), Vector2(l_upperarm.x, l_upperarm.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(spine4.x, spine4.y), Vector2(r_upperarm.x, r_upperarm.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(l_upperarm.x, l_upperarm.y), Vector2(l_forearm.x, l_forearm.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(r_upperarm.x, r_upperarm.y), Vector2(r_forearm.x, r_forearm.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(l_forearm.x, l_forearm.y), Vector2(l_hand.x, l_hand.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(r_forearm.x, r_forearm.y), Vector2(r_hand.x, r_hand.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(spine3.x, spine3.y), Vector2(pelvis.x, pelvis.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(l_hip.x, l_hip.y), Vector2(l_knee.x, l_knee.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(pelvis.x, pelvis.y), Vector2(r_knee.x, r_knee.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(l_knee.x, l_knee.y), Vector2(l_ankle_scale.x, l_ankle_scale.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(r_knee.x, r_knee.y), Vector2(r_ankle_scale.x, r_ankle_scale.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				en->Skeleton.push_back(new RLine(Vector2(r_ankle_scale.x, r_ankle_scale.y), Vector2(r_foot.x, r_foot.y), vars->visual.rainbowskeleton ? vars->rainbow : hp));
				//HANDS??
			}

			//name, flags and distance
			if (name)
			{
				auto Transform = ply->model()->boneTransforms()->get(48);
				if (!Transform) return;

				auto position = Transform->position();

				auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().distance(position);

				auto name_color = is_visible ? vars->colors.players.details.name.visible : vars->colors.players.details.name.invisible;
				auto half = (bounds.right - bounds.left) / 2;

				//held item
				if (vars->visual.weaponesp) {
					auto player_weapon = ply->GetActiveItem();
					if (player_weapon)
						en->Flags.push_back(
							new RenderFlag(
								{ bounds.center, bounds.bottom + 19 }, 
								player_weapon->get_weapon_name(), 
								vars->visual.rainbowflags 
									? vars->rainbow 
									: is_visible 
										? vars->colors.players.details.flags.visible 
											: vars->colors.players.details.flags.invisible));
						//render->StringCenter({ bounds.center, bounds.bottom + 19 }, player_weapon->get_weapon_name(), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
				}

				//cheat friend
				if (vars->visual.friendflag) {
					if (map_contains_key(vars->gui_player_map, ply->userID()))
					{
						auto gp = vars->gui_player_map[ply->userID()];
						if (gp->is_friend)
						{
							en->Flags.push_back(
								new RenderFlag(
									{ bounds.right + 9, bounds.bottom - 5 },
									_(L"[Friend]"),
									vars->visual.rainbowflags
									? vars->rainbow
									: is_visible
									? vars->colors.players.details.flags.visible
									: vars->colors.players.details.flags.invisible));
							//render->String({ bounds.right + 9, bounds.bottom - 5 }, _(L"[Friend]"), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
						}
					}
				}

				//wounded
				if (vars->visual.woundedflag) {
					if (HasPlayerFlag(ply, rust::classes::PlayerFlags::Wounded)) {
						en->Flags.push_back(
							new RenderFlag(
								{ bounds.right + 9, bounds.top + 5 },
								_(L"[Wounded]"),
								vars->visual.rainbowflags
								? vars->rainbow
								: is_visible
								? vars->colors.players.details.flags.visible
								: vars->colors.players.details.flags.invisible));
						//render->String({ bounds.right + 9, bounds.top + 5 }, _(L"[Wounded]"), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
					}
				}

				//distance
				if (vars->visual.distance)
				{
					auto dist_color = is_visible ? vars->colors.players.details.distance.visible : vars->colors.players.details.distance.invisible;
					en->Dist = (int)distance;
					en->DistColor = vars->visual.rainbowdist ? vars->rainbow : dist_color;
					en->DistPos = { bounds.center, bounds.bottom + 9 };
				}

				//name
				if (vars->visual.nameesp) {
					en->Name = std::wstring(name);
					en->NameColor = vars->visual.rainbowname ? vars->rainbow : name_color;
					en->NamePos = { bounds.center, bounds.top - 8 };
				}
			}

			en->IsPlayer = true;
			vars->RenderList.push_back(en);
		}
	}

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

	void iterate_players_new() {
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
#pragma region current
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
#pragma endregion

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
							auto fwd = playerpos + (pl->eyes()->body_forward() * distance);
							auto dist = fwd.distance(localpos);
							auto r = (distance / 50.f); r = (r < 1.f ? 1.f : r);
							if (dist < r)
								vars->targetted = true;
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
											if (current->endposition.distance(esp::local_player->transform()->position()) < 4.f)
												vars->grenadeclose = true;
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
						DrawPlayer(entity, npc);
						//offscreen indicator?
						//silent melee?
					}
				}
			}

			if (esp::local_player)
			{
				//select entity			
				//if (esp::selected_entity_id == ent_id) {
				//	Vector2 w2s_position = {};
				//	if (esp::out_w2s(world_position, w2s_position))
				//	{
				//		esp_color = Vector4(54, 116, 186, 255);
				//		w2s_position.y += 10;
				//		if (esp::selected_entity_id == ent_id) {
				//			render->StringCenter(w2s_position, _(L"selected"), { 54 / 255.f, 116 / 255.f, 186 / 255.f });
				//			//esp::draw_item(w2s_position, il2cpp::methods::new_string(("[selected]")), esp_color);
				//			w2s_position.y += 10;
				//			render->StringCenter(w2s_position, string::wformat(_(L"[%d]"), (int)ent_id), { 0.f, 1.f, 0.f });
				//			//esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[%d]"), (int)ent_id)), esp_color);
				//		}
				//	}
				//}

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
					auto visible = esp::local_player->is_visible(esp::local_player->transform()->position(), world_position);
					if (vars->visual.dist_on_items < dist) continue;
					esp_name = _(L"");
					float* ncol = vars->accent_color;
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

						//esp_color = Vector4(196, 124, 0, 255);
						ncol = vars->colors.items.dropped;

						Vector2 w2s_position = {};

						//	esp::draw_weapon_icon(item, w2s_position);
						//esp::draw_item(w2s_position, 0, esp_color, item_name);
						if (esp::out_w2s(world_position, w2s_position)) {
							if (item->GetAmount() > 1)
								esp_name = const_cast<wchar_t*>(string::wformat(_(L"%s [x%d]"), item_name, item->GetAmount()));
							else
								esp_name = item_name;
						}
						continue;
					}

					//ladder
					if (!strcmp(entity_class_name, _("BaseLadder")))
					{
						if (vars->visual.ladder) {
							esp_name = _(L"Ladder");
							ncol = vars->accent_color;
							//float col[3] = { 0, 219 / 255.f, 58 / 255.f };
							//world_position.y += 1.f;
							//Vector2 w2s_position = {};
							//if (esp::out_w2s(world_position, w2s_position))
							//{
							//	render->StringCenter(w2s_position, esp_name, FLOAT4TOD3DCOLOR(col));
							//	//esp::draw_item(w2s_position, esp_name, esp_color);
							//}
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
							//if (esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 50.f)
							//	DrawToolcupboard(w2s_position, authorizedPlayers_list);
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
						//esp_color = Vector4(232, 232, 232, 255);


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

							//if (vars->visual.heli_esp
							//	&& base_heli->is_alive())
							//{
							//	render->StringCenter({ x, y - 5 }, _(L"Patrol-heli"), { 1, 1, 1, 1 });
							//	render->Rectangle({ x - w, y - h }, { w * 2, h }, { 0, 0, 0, 0 }, 2);
							//	render->Rectangle({ x - w, y - h }, { w * 2, h }, FLOAT4TOD3DCOLOR(vars->colors.players.boxes.visible), 1);
							//}
						}
						continue;
					}

					//stash
					if (vars->visual.stash && *(int*)(object_name.zpad + 46) == '_hsa') {
						esp_name = _(L"Stash");
						ncol = vars->colors.items.stash.closed;
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
							ncol = vars->colors.items.collectibles;
						}
					}

					//recycler
					else if (vars->misc.norecycler && *(int*)(entity_class_name) == 'yceR') {
						esp_name = _(L"Recycler");
						esp_color = Vector4(232, 232, 232, 255);
						if (esp::local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 4.5f
							&& get_fixedTime() > esp::last_recycler + 0.35f)
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
						ncol = visible ? vars->colors.items.stone.visible : vars->colors.items.stone.invisible;
						esp_color = Vector4(232, 232, 232, 255);
						world_position.y += 1.f;
					}

					//sulfur
					else if (vars->visual.sulfur_ore && (*(int*)(object_name.zpad + 52) == 'flus' || *(int*)(object_name.zpad + 47) == 'flus')) {
						esp_name = _(L"Sulfur Ore");
						ncol = visible ? vars->colors.items.sulfur.visible : vars->colors.items.sulfur.invisible;
						esp_color = Vector4(203, 207, 0, 255);
						world_position.y += 1.f;
					}

					//metal
					else if (vars->visual.metal_ore && (*(int*)(object_name.zpad + 52) == 'atem' || *(int*)(object_name.zpad + 47) == 'atem')) {
						esp_name = _(L"Metal Ore");
						ncol = visible ? vars->colors.items.metal.visible : vars->colors.items.metal.invisible;
						esp_color = Vector4(145, 145, 145, 255);
						world_position.y += 1.f;
					}

					//hackable crate
					//if (*(int*)(entity_class_name) == 'kcaH' && *(int*)(entity_class_name + 14) == 'tarC') {
					//	auto flag = *reinterpret_cast<int*>(ent + 0x128);
					//	if (flag != 128 && flag != 256)
					//		continue;
					//
					//	world_position.y += 1.f;
					//	Vector2 w2s_position = {};
					//	if (esp::out_w2s(world_position, w2s_position))
					//	{
					//		auto timer = *reinterpret_cast<uintptr_t*>((uintptr_t)ent + 0x440);
					//		if (!timer) continue;
					//		auto text = (str)(*reinterpret_cast<uintptr_t*>(timer + 0xD0));
					//		if (!text) continue;
					//		auto str = text->str;
					//		if (!str) continue;
					//		render->StringCenter(w2s_position, string::wformat(_(L"Hackable crate: %ss"), str), { 130 / 255.f, 200 / 255.f, 230 / 255.f });
					//	}
					//}

					//traps
					else if (vars->visual.traps && (*(int*)(object_name.zpad + 36) == 'terr' || *(int*)(object_name.zpad + 43) == 'tnug' || *(int*)(object_name.zpad + 38) == 'rtra')) {
						if (*(int*)(object_name.zpad + 36) == 'terr')
							esp_name = _(L"Auto Turret*");
						else if (*(int*)(object_name.zpad + 43) == 'tnug')
							esp_name = _(L"Shotgun Trap*");
						else if (*(int*)(object_name.zpad + 38) == 'rtra')
							esp_name = _(L"Bear Trap*");

						ncol = vars->accent_color;
						//esp_color = Vector4(255, 166, 0, 255);
					}

					//vehicles
					else if (vars->visual.vehicles && *(int*)(entity_class_name + 4) == 'iheV') {
						if (std::string(object_name.zpad).find(_("standing")) != std::string::npos)
							esp_name = _(L"Rhib");
						else if (std::string(object_name.zpad).find(_("small")) != std::string::npos)
							esp_name = _(L"Small boat");
						else if (std::string(object_name.zpad).find(_("mini")) != std::string::npos)
							esp_name = _(L"Minicopter");
						else if (std::string(object_name.zpad).find(_("scrap")) != std::string::npos)
							esp_name = _(L"Scrap helicopter");
						else if (std::string(object_name.zpad).find(_("snow")) != std::string::npos)
							esp_name = _(L"Snowmobile");
						else if (std::string(object_name.zpad).find(_("saddle")) != std::string::npos)
							esp_name = _(L"Horse");

						//esp_color = Vector4(0, 161, 219, 255);
						ncol = visible ? vars->colors.items.vehicle.visible : vars->colors.items.vehicle.invisible;
						world_position.y += 1.f;
					}

					//airdrop
					else if (vars->visual.airdrops && *(int*)(object_name.zpad + 39) == 'pord') {
						world_position.y += 1.f;
						esp_name = _(L"Airdrop");
						//esp_color = Vector4(0, 161, 219, 255);
					}

					//cloth
					else if (vars->visual.cloth && *(int*)(object_name.zpad + 52) == 'c-pm') {
						world_position.y += 1.f;
						esp_name = _(L"Hemp");
						ncol = visible ? vars->colors.items.hemp.visible : vars->colors.items.hemp.invisible;
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

						//???
						if (*(int*)(entity_class_name) == 'satS') {
							auto flag = *reinterpret_cast<int*>(ent + 0x138);
							if (flag != 2048)
								continue;
						}
						//auto s = il2cpp::methods::new_string(string::format(_("[%dm]"), (int)local_player->eyes()->get_position().distance(world_position)));
						Vector2 w2s_position = {};
						if (esp::out_w2s(world_position, w2s_position))
						{
							RenderEntity* en = new RenderEntity();
							if (vars->visual.distance) {
								en->Dist = dist;
								en->DistColor = visible ? vars->colors.players.details.distance.visible : vars->colors.players.details.distance.invisible;
								en->DistPos = Vector2(w2s_position.x, w2s_position.y + 10);
							}

							en->Name = std::wstring(esp_name);
							en->NameColor = ncol;
							en->NamePos = w2s_position;

							vars->RenderList.push_back(en);
							//float color[3] = { esp_color.x / 255.f, esp_color.w / 255.f, esp_color.z / 255.f };
							//render->StringCenter(w2s_position, esp_name, FLOAT4TOD3DCOLOR(color));
							//esp::draw_item(w2s_position, esp_name, esp_color);

							//if (vars->visual.distance)
							//{
							//	auto nstr = string::wformat(_(L"[%dm]"), (int)dist);
							//	w2s_position.y += 11;
							//	render->StringCenter(w2s_position, nstr, FLOAT4TOD3DCOLOR(vars->colors.players.details.distance.visible));
							//}
							//if (vars->visual.distance
							//	&& local_player)
							//	esp::draw_item(Vector2(w2s_position.x, w2s_position.y += 10), s, esp_color);
						}

						continue;
					}
				}
			}
		}
		//if (esp::best_target.ent
		//	&& vars->visual.hotbar_esp
		//	&& esp::local_player)
		//	DrawPlayerHotbar(esp::best_target);
	}

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
		//	local_player->console_echo(string::wformat(_(L"[matrix]: OnGui - clientEntities size: %d"), size));

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
								auto local = esp::local_player->ClosestPoint(world_position);
								auto camera = esp::local_player->model()->boneTransforms()->get(48)->position();

								if (camera.get_3d_dist(world_position) >= 4.2f)
									return;

								aim_target target = esp::best_target;

								attack_melee(target, baseprojectile, esp::local_player, true);
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
		//iterate_players();
		iterate_players_new();
	}
}