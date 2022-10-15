#pragma once
#include "RenderClass.h"
#include "vars.hpp"

#include "rust/features/player_esp.hpp"
//#include "rust/classes.hpp"

float r = 1.00f, g = 0.00f, b = 1.00f;

std::vector<BasePlayer*> player_list = {};
std::map<int32_t, BasePlayer*> player_map = {};

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
	
	//if () {
	//bounds = ply->bones()->bounds;
	//if (!bounds.empty()) {
	if (get_bounds(bounds, 4)) {
		//is_visible = unity::is_visible(camera_position, bones[48].world_position, (uintptr_t)esp::local_player);
		//for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {
		//	if (is_visible) break;
		//	is_visible = unity::is_visible(camera_position, world_position, (uintptr_t)esp::local_player);
		//}
		//is_visible = ply->visible();
		is_visible = true;

		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto name = ply->_displayName()->str;
		auto activeitem = ply->GetActiveItem();

		//chams
		esp::do_chams(ply);


		//health bar
		const auto cur_health = ply->health();
		const auto max_health = (npc ? ply->maxHealth() : 100);
		const auto health_pc = min(cur_health / max_health, 1);
		const auto health_color =
			HSVD((health_pc * .25f), 1, .875f * 1);
		const auto h = (bounds.bottom - bounds.top) * health_pc;

		//boxes
		auto box_color = is_visible ? vars->colors.players.boxes.visible : vars->colors.players.boxes.invisible;
		if (vars->visual.rainbowbox) { box_color[0] = r; box_color[1] = g; box_color[2] = b; }
		switch (vars->visual.boxtype)
		{
		case 1: //full box
			render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, { 0,0,0,1 }, 4.f);
			render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			break;
		case 2: //corner box

			//top left
			render.Line({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left + (box_width / 4), bounds.top }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.left, bounds.top }, { bounds.left, bounds.top + (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			//top right
			render.Line({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right - (box_width / 4), bounds.top }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.right, bounds.top }, { bounds.right, bounds.top + (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			//bottom left
			render.Line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left + (box_width / 4), bounds.bottom }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.left, bounds.bottom }, { bounds.left, bounds.bottom - (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			//bottom right
			render.Line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right - (box_width / 4), bounds.bottom }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, { 0,0,0,1 }, 4.f);
			render.Line({ bounds.right, bounds.bottom }, { bounds.right, bounds.bottom - (box_height / 4) }, FLOAT4TOD3DCOLOR(box_color), 2.f);
			break;
		case 3: //3d cube
		{
			//LMAO I WILL DO THIS LATER

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
				render.Line(frontTopLeft_2d, frontTopRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontTopRight_2d, frontBottomRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontBottomRight_2d, frontBottomLeft_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontBottomLeft_2d, frontTopLeft_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(backTopLeft_2d, backTopRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(backTopRight_2d, backBottomRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(backBottomRight_2d, backBottomLeft_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(backBottomLeft_2d, backTopLeft_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontTopLeft_2d, backTopLeft_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontTopRight_2d, backTopRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontBottomRight_2d, backBottomRight_2d, FLOAT4TOD3DCOLOR(box_color));
				render.Line(frontBottomLeft_2d, backBottomLeft_2d, FLOAT4TOD3DCOLOR(box_color));
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

				render.Line(middleBottomLeft_2d, middleBottomRight_2d, { 0, 0, 0, 1 }, 3);
				render.Line(middleBottomLeft_2d, middleBottomRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);
				render.Line(middleBottomLeft_2d, middleTopLeft_2d, { 0, 0, 0, 1 }, 3);
				render.Line(middleBottomLeft_2d, middleTopLeft_2d, FLOAT4TOD3DCOLOR(box_color), 2);
				render.Line(middleBottomRight_2d, middleTopRight_2d, { 0, 0, 0, 1 }, 3);
				render.Line(middleBottomRight_2d, middleTopRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);
				render.Line(middleTopLeft_2d, middleTopRight_2d, { 0, 0, 0, 1 }, 3);
				render.Line(middleTopLeft_2d, middleTopRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);
			}
			break;
		}
		}

		switch (vars->visual.hpbar)
		{
		case 1:
			render.FillRectangle({ bounds.left - 7, bounds.top - 1 }, { 4, box_height + 3 }, { 0,0,0,1 });
			render.FillRectangle({ bounds.left - 6, bounds.bottom }, { 2, -h - 1 }, health_color);
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

			auto position = Transform->position();

			auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().distance(position);
			//const char* new_name = ;
			// PLAYER NAME

			auto name_color = is_visible ? vars->colors.players.details.name.visible : vars->colors.players.details.name.invisible;
			auto half = (bounds.right - bounds.left) / 2;

			//selected flag for automation following
			//std::wstring ws(name);
			//std::string s(ws.begin(), ws.end());
			//if (!vars->misc.playerselected)
			//	vars->misc.playerselected = _("~");
			//if (!strcmp(vars->misc.playerselected, s.c_str()))
			//{
			//	render.String({ bounds.right + 5, bounds.top }, _(L"[Selected]"), { 0, 0, 0, 1 });
			//	render.String({ bounds.right + 5, bounds.top + 1 }, _(L"[Selected]"), FLOAT4TOD3DCOLOR(name_color));
			//}
			//wounded
			if (vars->visual.woundedflag) {
				if (HasPlayerFlag(ply, rust::classes::PlayerFlags::Wounded)) {
					render.String({ bounds.right + 5, bounds.top }, _(L"[Wounded]"), { 0, 0, 0, 1 });
					render.String({ bounds.right + 5, bounds.top + 1 }, _(L"[Wounded]"), FLOAT4TOD3DCOLOR(name_color));
				}
			}

			//distance
			if (vars->visual.distance)
			{
				auto dist_color = is_visible ? vars->colors.players.details.distance.visible : vars->colors.players.details.distance.invisible;
				auto nstr = string::wformat(_(L"[%dm]"), (int)distance);
				render.StringCenter({ bounds.center - 1, bounds.bottom + 10 }, nstr, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center + 1, bounds.bottom + 10 }, nstr, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center - 1, bounds.bottom + 8 }, nstr, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center + 1, bounds.bottom + 8 }, nstr, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center, bounds.bottom + 9 }, nstr, FLOAT4TOD3DCOLOR(dist_color));
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.bottom + 1.f, 79.f + half * 2 + 80.f , 30 }, nstr, gui::Color(0, 0, 0, 120), true, 10.5);
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.bottom, 80.f + half * 2 + 80.f , 30 }, nstr, gui::Color(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1), true, 10);
			}



			//name
			if (vars->visual.nameesp) {

				render.StringCenter({ bounds.center - 1, bounds.top - 9 }, name, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center + 1, bounds.top - 9 }, name, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center - 1, bounds.top - 7 }, name, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center + 1, bounds.top - 7 }, name, { 0, 0, 0, 1 });
				render.StringCenter({ bounds.center, bounds.top - 8 }, name, vars->visual.rainbowname ? D2D1::ColorF{ r, g, b, 1 } : FLOAT4TOD3DCOLOR(name_color));
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.top - 23.f, 79.f + half * 2 + 80.f , 30 }, name, gui::Color(0, 0, 0, 120), true, 10.5);
				//gui::Label(rust::classes::Rect{ bounds.left - 75.f  , bounds.top - 24.f, 80.f + half * 2 + 80.f , 30 }, name, gui::Color(vars->visual.nameRcolor, vars->visual.nameGcolor, vars->visual.nameBcolor, 1), true, 10);
			}
			// PLAYER NAME
		}
	}
}

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

	wchar_t* esp_name = _(L"");

	vars->player_name_list.clear();
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
			//cache::CacheBones(entity, esp::local_player);
			bool exists_in_list = false;
			auto fff = 0;
			auto sizebefore = player_list.size();
			for (BasePlayer* p : player_list)
			{
				if (!p->is_alive()
					|| (entity->is_sleeping() && !vars->visual.sleeper_esp)
					|| !p)
				{
					//vars->player_id_name.erase(fff);
					player_list.erase(std::remove(player_list.begin(), player_list.end(), p), player_list.end());
				}
				if (entity->userID() == p->userID())
					exists_in_list = true;
			}
			if (!map_contains_key(player_map, ent_id))
				player_map.insert(std::make_pair(ent_id, entity));
			//	//std::wstring ws(entity->get_player_name());
				//std::string s(ws.begin(), ws.end());
				//vars->player_name_list.push_back(s);
				//if(!map_contains_key(vars->player_id_name, fff))
				//	vars->player_id_name.insert(std::make_pair(fff, entity->get_player_name()));
			//	fff++;
			//}
			if (!exists_in_list)
			{
				player_list.push_back(entity);
			}
			//hit player for silent melee, but not here as may crash due to not being run from a game thread

			//check valid
			if (!entity->is_alive()
				|| (entity->is_sleeping() && !vars->visual.sleeper_esp))
				continue;

			bool npc = entity->playerModel()->isnpc();
			//check npc
			if (npc && !vars->visual.npc_esp) continue;

			//local player chams, player average velocity
			if (entity->is_local_player())
				esp::local_player = entity;
			else {
				if (esp::local_player)
				{
					if (vars->visual.targetted)
					{
						auto pl = ((BasePlayer*)ent);
						//project bodyforward distance between ent and local player, if within 1,2m indicate?
						auto playerpos = pl->model()->boneTransforms()->get(48)->position();
						auto localpos = esp::local_player->model()->boneTransforms()->get(48)->position();
						auto distance = playerpos.distance(localpos);
						auto fwd = pl->eyes()->body_forward() *  distance;

						auto w2 = WorldToScreen(fwd);
						auto w1 = WorldToScreen(playerpos);
						Vector2 w1s = { w1.x, w1.y };
						Vector2 w2s = { w2.x, w2.y };
						if (!w2s.empty())
						{
							render.Line(w1s, w2s, { 1, 1, 1, 1 }, 2.f);
						}

						//Sphere(fwd, 0.05, { 1, 1, 1, 1 }, 0.05f, 10.f);
						if (fwd.distance(localpos) < 1.f)
						{
							auto ws = std::wstring(string::wformat(_(L"[%s is looking at you]"), ((BasePlayer*)ent)->get_player_name()));
							render.StringCenter({ vars->ScreenX / 2 + 1, (vars->ScreenY / 2) - 30 }, ws.c_str(), { 0, 0, 0, 1 });
							render.StringCenter({ vars->ScreenX / 2 - 1, (vars->ScreenY / 2) - 30 }, ws.c_str(), { 0, 0, 0, 1 });
							render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 + 1 }, ws.c_str(), { 0, 0, 0, 1 });
							render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 - 1 }, ws.c_str(), { 0, 0, 0, 1 });
							render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 }, ws.c_str(), { 227 / 255.f, 32 / 255.f, 61 / 255.f, 1 });
						}
					}

					if (GetAsyncKeyState(0x37))
					{
						auto look = esp::local_player->_lookingAtEntity();
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

								esp::selected_entity_id = ent_id;
							}
						}
					}

					if (vars->wants_shoot)
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
						else if (target.network_id == esp::best_target.network_id) {
							esp::best_target.pos = target.pos;
							esp::best_target.distance = target.distance;
							esp::best_target.fov = target.fov;
							esp::best_target.ent = target.ent;
							esp::best_target.visible = visible;
						}
						else
							goto draw;
					}

				choosetarget:
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
										auto world_position = ent->model()->boneTransforms()->get(48)->position();
										auto local = ClosestPoint(esp::local_player, world_position);
										auto camera = esp::local_player->model()->boneTransforms()->get(48)->position();

										if (camera.get_3d_dist(world_position) >= 4.2f)
											return;

										aim_target target = esp::best_target;

										target.visible = esp::local_player->is_visible(camera, local);

										attack_melee(target, melee, true);
									}
								}
							}
						};
						hit_player();
					}
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
			if (vars->misc.auto_upgrade) {
				if (!strcmp(entity_class_name, _("BuildingBlock"))) {
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
				vars->visual.corpses
				&& world_position.distance(esp::local_player->transform()->position()) < vars->visual.dist_on_items)
			{
				esp_name = _(L"");
				auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				if (!object_name.zpad)
					continue;

				float dist = 10.f;
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
					esp_name = _(L"Ladder");
					float col[3] = { 0, 219/255.f, 58/255.f };

					world_position.y += 1.f;
					Vector2 w2s_position = {};
					if (esp::out_w2s(world_position, w2s_position))
					{
						render.StringCenter(w2s_position, esp_name, FLOAT4TOD3DCOLOR(col));
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

				//stone
				else if (vars->visual.stone_ore && (*(int*)(object_name.zpad + 52) == 'nots' || *(int*)(object_name.zpad + 47) == 'nots')) {
					esp_name = _(L"Stone Ore");
					esp_color = Vector4(232, 232, 232, 255);
					if (vars->visual.rock_chams >= 1)
						esp::rock_chams(ent);
					world_position.y += 1.f;
				}

				//sulfur
				else if (vars->visual.sulfur_ore && (*(int*)(object_name.zpad + 52) == 'flus' || *(int*)(object_name.zpad + 47) == 'flus')) {
					esp_name = _(L"Sulfur Ore");
					esp_color = Vector4(203, 207, 0, 255);
					if (vars->visual.rock_chams >= 1)
						esp::rock_chams(ent);
					world_position.y += 1.f;
				}

				//metal
				else if (vars->visual.metal_ore && (*(int*)(object_name.zpad + 52) == 'atem' || *(int*)(object_name.zpad + 47) == 'atem')) {
					esp_name = _(L"Metal Ore");
					esp_color = Vector4(145, 145, 145, 255);
					if (vars->visual.rock_chams >= 1)
						esp::rock_chams(ent);
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
void IndicatorTp() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 50 }, { 120, 4 }, vars->last_teleport_time - get_fixedTime(), 10.0f);
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
	if (vars->misc.tp)
		IndicatorTp();

	//Draw FOV
	if (vars->visual.show_fov)
		DrawFov();

	//Draw watermark
	Watermark();

	iterate_entities();
}