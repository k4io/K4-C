#pragma once
#include "lua_wrapper.hpp"

void DrawOnRadar(Vector3 worldpos, wchar_t* entityname, bool visible = false) {
	if (!vars->local_player) return;
	auto local = vars->local_player->transform()->position();
	float dist = local.distance(worldpos);
	const float x = local.z - worldpos.z, y = local.x - worldpos.x;
	auto euler = EulerAngles(vars->local_player->eyes()->rotation());
	const float n = Vector3::my_atan2(y, x) * 57.29578f - 270.f - euler.y;
	float px = dist * Vector3::my_cos(n * 0.0174532924f);
	float py = dist * Vector3::my_sin(n * 0.0174532924f);

	px = px * (vars->visual.radarsize / vars->visual.radarrange) / 2.f;
	py = py * (vars->visual.radarsize / vars->visual.radarrange) / 2.f;

	if (dist <= vars->visual.radarrange)
	{
		render.FillRectangle({ 
			vars->visual.radarx + vars->visual.radarsize / 2.f + px - 3.f, 
			vars->visual.radary + vars->visual.radarsize / 2.f + py - 3.f }, 
			{ 5, 5 }, 
			FLOAT4TOD3DCOLOR(visible ? vars->colors.players.boxes.visible : vars->colors.players.boxes.invisible));
		if (vars->visual.radarname)
			render.StringCenter({
			vars->visual.radarx + vars->visual.radarsize / 2.f + px - 3.f,
			(vars->visual.radary + vars->visual.radarsize / 2.f + py - 3.f) + 5 },
			entityname,
			FLOAT4TOD3DCOLOR(visible ? vars->colors.players.details.name.visible : vars->colors.players.details.name.invisible));
		//gui::FullRect({ settings::Radar::Pos_X + settings::Radar::Radar_Size / 2.f + PointPos_X - 3.f, settings::Radar::Pos_Y + settings::Radar::Radar_Size / 2.f + PointPos_Y - 3.f, 5, 5 }, { 40,212,101,255 });
	}
}

void DrawPlayer(BasePlayer* ply, bool npc)
{
	if (!ply) return;
	if (!vars->local_player) return;
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

	bool is_visible = false, is_teammate = ply->is_teammate(vars->local_player);
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


	if (vars->visual.radar)
		DrawOnRadar(ply->transform()->position(), ply->get_player_name(), ply->visible());

	if (get_bounds(bounds, 4)) {
		//is_visible = unity::is_visible(camera_position, bones[48].world_position, (uintptr_t)vars->local_player);
		is_visible = ply->visible();
		//if (vars->combat.vischeck) {
		//	__try {
		//		for (auto& [bone_screen, bone_idx, on_screen, world_position, visible] : bones) {
		//			if (is_visible) break;
		//			is_visible = unity::is_visible(camera_position, world_position, (uintptr_t)vars->local_player);
		//		}
		//	}
		//	__except (true) {}
		//} else is_visible = true;

		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto name = ply->_displayName()->str;
		auto activeitem = ply->GetActiveItem();

		const auto rainbowcolor = D2D1::ColorF(r, g, b, 1);

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

		if (vars->visual.boxtype == 5)
		{
			//__try {
			//	std::string ap(vars->visual.boxfilename);
			//	if (!ap.empty()) {
			//		std::string path = vars->data_dir + _("images\\") + ap;
			//		if (std::filesystem::exists(path)) {
			//			if (vars->customboxpath != path) {
			//				vars->customboxpath = path;
			//				//set custom box
			//				render.SetCustomBox(vars->customboxpath);
			//			}
			//			else render.DrawCustomBox({ bounds.left, bounds.top }, { box_width, box_height });//draw custom box
			//		}
			//	}
			//}
			//__except (true) {}
		}
		else {
			switch (vars->visual.boxtype)
			{
			case 1: //full box
				render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, { 0,0,0,1 }, 4.f);
				render.Rectangle({ bounds.left, bounds.top }, { box_width, box_height }, FLOAT4TOD3DCOLOR(box_color), 2.f);
				break;
			case 2: //corner box
			{
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
			}
			case 3: //3d cube
			{
				//LMAO I WILL DO THIS LATER

				auto wid = 4;
				Bounds cbounds = Bounds();

				auto lfoott = ply->model()->boneTransforms()->get((int)Bone_List::l_foot);
				auto lfootp = lfoott->position();
				auto rfoott = ply->model()->boneTransforms()->get((int)Bone_List::r_foot);
				auto rfootp = lfoott->position();
				auto spine3 = ply->model()->boneTransforms()->get((int)Bone_List::spine3)->position();
				auto spine4 = ply->model()->boneTransforms()->get((int)Bone_List::spine4)->position();
				auto eyepos = spine3.midPoint(spine4);
				Vector3 mp = Vector3(eyepos.x, eyepos.y - 1.2f, eyepos.z);//lfootp.midPoint(rfootp);

				if (ply->modelState()->has_flag(ModelState_Flag::Ducked)) {
					//auto midpoint = ent->FindBone(_(L""))
					cbounds.center = mp + Vector3(0.0f, 0.55f, 0.0f);
					cbounds.extents = Vector3(0.4f, 0.65f, 0.4f);
				}
				else {
					if (ply->modelState()->has_flag(ModelState_Flag::Crawling) || ply->modelState()->has_flag(ModelState_Flag::Sleeping)) {
						cbounds.center = ply->model()->boneTransforms()->get((int)Bone_List::pelvis)->position();
						cbounds.extents = Vector3(0.9f, 0.2f, 0.4f);
					}
					else if (ply->modelState()->has_flag(ModelState_Flag::Ducked)) {
						cbounds.center = ply->model()->boneTransforms()->get((int)Bone_List::pelvis)->position();
						cbounds.size.y *= .5f;
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

					if (vars->visual.hpbar == 3) {
						Vector2 points[4];
						auto c = vars->visual.rainbowhpbar ? rainbowcolor : health_color;
						c.a = 0.5f;
						points[0] = frontBottomLeft_2d;
						points[1] = frontBottomRight_2d;

						int leftsz = frontTopLeft_2d.y - frontBottomLeft_2d.y;
						int rightsz = frontTopRight_2d.y - frontBottomRight_2d.y;

						frontTopLeft_2d.y -= leftsz;
						frontTopRight_2d.y -= rightsz;

						leftsz = ((leftsz / max_health) * cur_health);
						rightsz = ((rightsz / max_health) * cur_health);

						frontTopLeft_2d.y += leftsz;
						frontTopRight_2d.y += rightsz;

						points[2] = frontTopRight_2d;
						points[3] = frontTopLeft_2d;
						render.FillPolygon(points, 4, c);
					}
				}


				break;
			}
			case 4: //3d box
			{
				auto wid = 4;
				Bounds cbounds = Bounds();

				auto lfoott = ply->model()->boneTransforms()->get((int)Bone_List::l_foot);
				auto lfootp = lfoott->position();
				auto rfoott = ply->model()->boneTransforms()->get((int)Bone_List::r_foot);
				auto rfootp = lfoott->position();
				auto spine3 = ply->model()->boneTransforms()->get((int)Bone_List::spine3)->position();
				auto spine4 = ply->model()->boneTransforms()->get((int)Bone_List::spine4)->position();
				auto eyepos = spine3.midPoint(spine4);
				Vector3 mp = Vector3(eyepos.x, eyepos.y - 1.25f, eyepos.z);//lfootp.midPoint(rfootp);

				if (ply->modelState()->has_flag(ModelState_Flag::Ducked)) {
					//auto midpoint = ent->FindBone(_(L""))
					cbounds.center = mp + Vector3(0.0f, 0.55f, 0.0f);
					cbounds.extents = Vector3(0.4f, 0.65f, 0.4f);
				}
				else {
					if (ply->modelState()->has_flag(ModelState_Flag::Crawling) || ply->modelState()->has_flag(ModelState_Flag::Sleeping)) {
						cbounds.center = ply->model()->boneTransforms()->get((int)Bone_List::pelvis)->position();
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

					render.Line(middleBottomLeft_2d, middleBottomRight_2d, { 0, 0, 0, 1 }, 3);
					render.Line(middleBottomLeft_2d, middleBottomRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);
					render.Line(middleBottomLeft_2d, middleTopLeft_2d, { 0, 0, 0, 1 }, 3);
					render.Line(middleBottomLeft_2d, middleTopLeft_2d, FLOAT4TOD3DCOLOR(box_color), 2);
					render.Line(middleBottomRight_2d, middleTopRight_2d, { 0, 0, 0, 1 }, 3);
					render.Line(middleBottomRight_2d, middleTopRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);
					render.Line(middleTopLeft_2d, middleTopRight_2d, { 0, 0, 0, 1 }, 3);
					render.Line(middleTopLeft_2d, middleTopRight_2d, FLOAT4TOD3DCOLOR(box_color), 2);


					if (vars->visual.hpbar == 3) {
						Vector2 points[4];
						auto c = vars->visual.rainbowhpbar ? rainbowcolor : health_color;
						c.a = 0.5f;
						points[0] = middleBottomLeft_2d;
						points[1] = middleBottomRight_2d;

						int leftsz = middleTopLeft_2d.y - middleBottomLeft_2d.y;
						int rightsz = middleTopRight_2d.y - middleBottomRight_2d.y;

						middleTopLeft_2d.y -= leftsz;
						middleTopRight_2d.y -= rightsz;

						leftsz = ((leftsz / max_health) * cur_health);
						rightsz = ((rightsz / max_health) * cur_health);

						middleTopLeft_2d.y += leftsz;
						middleTopRight_2d.y += rightsz;

						points[2] = middleTopRight_2d;
						points[3] = middleTopLeft_2d;
						render.FillPolygon(points, 4, c);
					}
				}

				break;
			}
			}
		}


		switch (vars->visual.hpbar)
		{
		case 1:
			render.FillRectangle({ bounds.left - 7, bounds.top - 1 }, { 4, box_height + 3 }, { 0,0,0,1 });
			render.FillRectangle({ bounds.left - 6, bounds.bottom }, { 2, -h - 1 }, vars->visual.rainbowhpbar ? rainbowcolor : health_color);
			break;
		case 2:
			render.FillRectangle({ bounds.left, bounds.bottom + 6 }, { box_width + 1, 4 }, { 0,0,0,1 });
			render.FillRectangle({ bounds.left + 1, bounds.bottom + 7 }, { ((box_width / max_health) * cur_health) - 1, 2 }, vars->visual.rainbowhpbar ? rainbowcolor : health_color);
			break;
		case 3:
		{
			if (vars->visual.boxtype != 3
				&& vars->visual.boxtype != 4)
			{
				auto c = vars->visual.rainbowhpbar ? rainbowcolor : health_color;
				c.a = 0.5;
				render.FillRectangle({ bounds.left, bounds.bottom }, { box_width, -h - 1 }, c);
			}
			break;
		}
		}

		//skeleton
		if (vars->visual.skeleton)
		{
			//jaw
			auto Transform = ply->model()->boneTransforms()->get(48);
			if (!Transform) return;
			Vector3 world_position = Transform->position();
			Vector3 jaw = WorldToScreen(world_position);

			//smiley face
			//eyes
			//auto r = ply->eyes()->position() + (ply->eyes()->body_right() * .035f) + (ply->eyes()->body_forward() * 0.23f);
			//auto left = ply->eyes()->position() - (ply->eyes()->body_right() * .035f) + (ply->eyes()->body_forward() * 0.23f);
			////Vector3 eye1 = WorldToScreen(left);
			////Vector3 eye2 = WorldToScreen(r);
			//auto doeye = [&](Vector3 e) {
			//	std::vector<Vector2> vl{};
			//	float step = (M_PI_2) / 10;
			//	float x, z, y, current = 0;
			//	for (size_t i = 0; i < 10; i++)
			//	{
			//		x = Vector3::my_sin(current) * .02f;
			//		y = Vector3::my_cos(current) * .02f;
			//
			//		Vector3 p1 = WorldToScreen(e + Vector3(x, y + .03f, x));
			//		Vector3 p2 = WorldToScreen(e + Vector3(-x, y + .03f, -x));
			//		Vector3 p3 = WorldToScreen(e + Vector3(x, -y + .03f, x));
			//		Vector3 p4 = WorldToScreen(e + Vector3(-x, -y + .03f, -x));
			//
			//		vl.push_back(Vector2(p1.x, p1.y));
			//		vl.push_back(Vector2(p2.x, p2.y));
			//		vl.push_back(Vector2(p3.x, p3.y));
			//		vl.push_back(Vector2(p4.x, p4.y));
			//
			//		//if (!p1.is_empty()
			//		//	&& !l.is_empty()) {
			//		//	render.Line(Vector2(p1.x, p1.y), Vector2(l.x, l.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//		//}
			//		//
			//		//render.Line(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//		//render.Line(Vector2(p2.x, p2.y), Vector2(p3.x, p3.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//		//render.Line(Vector2(p3.x, p3.y), Vector2(p4.x, p4.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//
			//		current += step;
			//	}
			//	render.FillPolygon(vl.data(), vl.size(), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			//};
			//doeye(r);
			//doeye(left);

			//smile
			//Vector3 l = Vector3::Zero();
			//float step = (M_PI_2) / 10;
			//float x, z, y, current = 0;
			//Vector3 e = ply->eyes()->position() + (ply->eyes()->head_forward() * 0.23f);
			//e.y -= 0.08f;
			//for (size_t i = 0; i < 10; i++)
			//{
			//	x = Vector3::my_sin(current) * .05f;
			//	y = Vector3::my_cos(current) * .05f;
			//
			//	//Vector3 p1 = WorldToScreen(e + Vector3(x, y + .03f, 0));
				//Vector3 p2 = WorldToScreen(e + Vector3(-x, y + .03f, 0));
			//	Vector3 p3 = WorldToScreen(e + Vector3(x, -y + .04f, 0));
			//	Vector3 p4 = WorldToScreen(e + Vector3(-x, -y + .04f, 0));
			//
			//	if (!p3.is_empty()
			//		&& !l.is_empty()) {
			//		render.Line(Vector2(p3.x, p3.y), Vector2(l.x, l.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//	}
			//
			//	render.Line(Vector2(p3.x, p3.y), Vector2(p4.x, p4.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//	//render.Line(Vector2(p2.x, p2.y), Vector2(p3.x, p3.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
				//render.Line(Vector2(p3.x, p3.y), Vector2(p4.x, p4.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 2.f);
			//
			//	l = p3;
			//	current += step;
			//}
			//l = Vector3::Zero();

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

			//render.Line(Vector2(jaw.x, jaw.y), Vector2(spine4.x, spine4.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(spine3.x, spine3.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(l_upperarm.x, l_upperarm.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(spine4.x, spine4.y), Vector2(r_upperarm.x, r_upperarm.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(l_upperarm.x, l_upperarm.y), Vector2(l_forearm.x, l_forearm.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(r_upperarm.x, r_upperarm.y), Vector2(r_forearm.x, r_forearm.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(l_forearm.x, l_forearm.y), Vector2(l_hand.x, l_hand.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(r_forearm.x, r_forearm.y), Vector2(r_hand.x, r_hand.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(spine3.x, spine3.y), Vector2(pelvis.x, pelvis.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(l_hip.x, l_hip.y), Vector2(l_knee.x, l_knee.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(pelvis.x, pelvis.y), Vector2(r_knee.x, r_knee.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(l_knee.x, l_knee.y), Vector2(l_ankle_scale.x, l_ankle_scale.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(r_knee.x, r_knee.y), Vector2(r_ankle_scale.x, r_ankle_scale.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			render.Line(Vector2(r_ankle_scale.x, r_ankle_scale.y), Vector2(r_foot.x, r_foot.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);
			
			//render.Circle(Vector2(eye1.x, eye1.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 10, 3);
			//render.Circle(Vector2(eye2.x, eye2.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color, 10, 3);
			//render.Line(Vector2(r_ankle_scale.x, r_ankle_scale.y), Vector2(r_foot.x, r_foot.y), vars->visual.rainbowskeleton ? rainbowcolor : health_color);

			//HANDS??
		}

		//name, flags and distance
		if (name)
		{
			auto Transform = ply->model()->boneTransforms()->get(48);
			if (!Transform) return;

			auto position = Transform->position();

			auto distance = vars->local_player->model()->boneTransforms()->get(48)->position().distance(position);

			auto name_color = is_visible ? vars->colors.players.details.name.visible : vars->colors.players.details.name.invisible;
			auto half = (bounds.right - bounds.left) / 2;

			if (vars->visual.weaponesp) {
				auto player_weapon = ply->GetActiveItem();
				if (player_weapon)
					render.StringCenter({ bounds.center, bounds.bottom + 19 }, player_weapon->get_weapon_name(), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
			}

			if (vars->visual.targettedflag) {
				if (vars->best_target.ent) {
					if (ply->userID() == ((BasePlayer*)vars->best_target.ent)->userID()) {
						render.String({ bounds.right + 9, bounds.bottom - (box_height / 2) }, _(L"[Target]"), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
					}
				}
			}

			if (vars->visual.friendflag) {
				if (map_contains_key(vars->gui_player_map, ply->userID()))
				{
					auto gp = vars->gui_player_map[ply->userID()];
					if (gp->is_friend)
					{
						render.String({ bounds.right + 9, bounds.bottom - 5 }, _(L"[Friend]"), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
					}
				}
			}

			if (vars->visual.woundedflag) {
				if (HasPlayerFlag(ply, PlayerFlags::Wounded)) {
					render.String({ bounds.right + 9, bounds.top + 5 }, _(L"[Wounded]"), vars->visual.rainbowflags ? rainbowcolor : FLOAT4TOD3DCOLOR(vars->colors.players.details.flags.visible));
				}
			}

			//distance
			if (vars->visual.distance)
			{
				auto dist_color = is_visible ? vars->colors.players.details.distance.visible : vars->colors.players.details.distance.invisible;
				auto nstr = string::wformat(_(L"[%dm]"), (int)distance);
				render.StringCenter({ bounds.center, bounds.bottom + 9 + (vars->visual.hpbar == 3 ? 0 : 10) }, nstr, vars->visual.rainbowdist ? rainbowcolor : FLOAT4TOD3DCOLOR(dist_color));
			}

			//name
			if (vars->visual.nameesp) {
				render.StringCenter({ bounds.center, bounds.top - 8 }, name, vars->visual.rainbowname ? D2D1::ColorF{ r, g, b, 1 } : FLOAT4TOD3DCOLOR(name_color));
			}
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
		render.Rectangle({ start.x, start.y }, { 190, 40.f + names.size() * 20.f }, { 0.21, 0.21, 0.21, 1 });

		render.String({ start.x + 10, start.y + 5 }, _(L"Tool cupboard"), { 0.71, 0.71, 0.71, 1 });

		//seperator
		render.Line({ start.x, start.y + 25 }, { start.x + sz.x, start.y + 25 }, { 0.11, 0.11, 0.11, 1 }, 1);

		//outline
		render.Rectangle(start, sz, { 0.11, 0.11, 0.11, 1 }, 2);

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
	std::vector<_item*> items = {};

	if (target.ent && !target.is_heli && !target.teammate)
	{
		System::list<Item*>* belt = ((BasePlayer*)target.ent)->get_belt_items();
		if (!belt) return;
		Item* result = nullptr;
		belt->for_each([&](Item* item, int32_t idx) {
			{
				if (!item) return;
				_item* cur = new _item();
				const auto name = item->get_weapon_name();
				if (name)
				{
					cur->name = name;
					const auto amt = item->GetAmount();
					cur->count = amt;
					items.push_back(cur);
				}
			}
			});

		auto player_name = ((BasePlayer*)target.ent)->_displayName()->str;
		auto ws = std::wstring(player_name);
		auto s = std::string(ws.begin(), ws.end());

		vars->target_hotbar_list = items;
		vars->target_name = s;

		return;

		//check for draw wearable?
		if (items.size() > 0)
		{


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
			render.Rectangle(start, sz,
				{ 0.14f, 0.14f, 0.14f, 1 },
				2);

			std::wstring name_str(player_name); name_str = name_str + _(L"'s items");
			render.String({ start.x + 10, start.y + 5 }, name_str.c_str(), { 0.71, 0.71, 0.71, 1 });

			//seperator
			render.Line({ start.x, start.y + 25 }, { start.x + sz.x, start.y + 25 }, { 0.11, 0.11, 0.11, 1 }, 1);

			//outline
			render.Rectangle(start, sz, { 0.11, 0.11, 0.11, 1 }, 2);

			start.x += 10; //left-side margin
			start.y += 30; //margin from seperator

			int k = 0;
			for (auto i : items)
			{
				auto name = i->name;
				auto amount = i->count;

				std::wstring ws(name);
				auto astr = std::to_string(amount);
				std::wstring wastr(astr.begin(), astr.end());
				ws = ws + _(L" - x") + wastr;

				render.String({ start.x, start.y + (k++ * 20) }, ws.c_str(), { 0.71, 0.71, 0.71, 1 });
			}
		}
	}
	else {
		vars->target_hotbar_list = {};
		vars->target_name = _("");
	}
}

void DrawRadarBackground() {
	float r = 2;
	float x = vars->visual.radarx;
	float y = vars->visual.radary;
	auto acol = FLOAT4TOD3DCOLOR(vars->accent_color);
	render.FillCircle({ (x + r / 2) + (vars->visual.radarsize / 2), (y + r / 2) + (vars->visual.radarsize / 2) }, acol, r);
	render.FillRectangle({ x, y }, { vars->visual.radarsize, vars->visual.radarsize }, { .2f, .2f, .2f, .6f });
	render.Rectangle({ x, y }, { vars->visual.radarsize, vars->visual.radarsize }, acol, 1.f);
	float c[4] = { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], .2f };
	acol = FLOAT4TOD3DCOLOR(c);
	render.Line({ (x + r / 2) + (vars->visual.radarsize / 2), (y + r / 2) },
		{ (x + r / 2) + (vars->visual.radarsize / 2), (y + r / 2) + vars->visual.radarsize },
		acol, 1.f);
	render.Line({ (x + r / 2), (y + r / 2) + (vars->visual.radarsize / 2) },
		{ (x + r / 2) + vars->visual.radarsize, (y + r / 2) + (vars->visual.radarsize / 2) },
		acol, 1.f);
}

void iterate_entities() {
	auto get_client_entities = [&]() {
		vars->client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
	};
	if (!vars->client_entities)
		get_client_entities();

	rust::list* entity_list = (rust::list*)vars->client_entities;

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
		vars->local_player = nullptr;
		return;
	}

	auto size = entity_list->get_size();
	if (!size) {
		//get_client_entities();
		vars->local_player = nullptr;
		return;
	}

	auto buffer = entity_list->get_buffer<uintptr_t>();
	if (!buffer) {
		//get_client_entities();
		vars->local_player = nullptr;
		return;
	}

	bool found_lp = false;
	Vector4 esp_color(1, 0, 1, 1);

	wchar_t* esp_name = _(L"");

	vars->player_name_list.clear();
	bool flp = false;
	bool found_a_target = false;
	auto temptarget = aim_target();
	//printf("before ent loop\n");
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

			bool npc = entity->playerModel()->isnpc();
			//check npc
			if (npc && !vars->visual.npc_esp) continue;

			if (!map_contains_key(player_map, ent_id)) {
				player_map.insert(std::make_pair(ent_id, entity));
			
				gplayer* p = new gplayer(entity->get_player_name(), entity->userID(), 0, 0, 0, 0);

				if(!map_contains_key(vars->gui_player_map, entity->userID()))
					vars->gui_player_map.insert(std::make_pair(entity->userID(), p));
			}
			if (!map_contains_key(vars->chams_player_map, entity->userID()))
				vars->chams_player_map.insert(std::make_pair(entity->userID(), 0));
			if (!map_contains_key(vars->handchams_player_map, entity->userID()))
				vars->handchams_player_map.insert(std::make_pair(entity->userID(), 0));
			

			bool is_friend = false, follow = false, block = false;
			if (map_contains_key(vars->gui_player_map, entity->userID())) {
				is_friend = vars->gui_player_map[entity->userID()]->is_friend || entity->is_teammate(vars->local_player);
				follow = vars->gui_player_map[entity->userID()]->follow;
				block = vars->gui_player_map[entity->userID()]->block;
			}

			//local player chams, player average velocity
			if (entity->is_local_player()) {
				flp = true;
				vars->local_player = entity;
			} else {
				if (vars->local_player)
				{
					//if (std::count(lw::playerlist.begin(), lw::playerlist.end(), entity) < 1)
					//	lw::playerlist.push_back(entity);
					if (vars->visual.targetted)
					{
						auto pl = ((BasePlayer*)ent);
						auto playerpos = pl->model()->boneTransforms()->get(48)->position();
						auto localpos = vars->local_player->model()->boneTransforms()->get(48)->position();
						auto distance = playerpos.distance(localpos);
						auto fwd = playerpos + (pl->eyes()->body_forward() * distance);

						auto dist = fwd.distance(localpos);
						auto r = (distance / 50.f); r = (r < 1.f ? 1.f : r);
						if (dist < r)
						{
							auto ws = string::wformat(_(L"%s is looking at you!"), ((BasePlayer*)ent)->get_player_name());
							vars->targetted = true;
							render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 30 }, ws, { 227 / 255.f, 32 / 255.f, 61 / 255.f, 1 });
						}
					}

					if (vars->visual.showpred
						&& vars->best_target.ent)
					{
						auto pos = vars->local_player->eyes()->position();
						auto targetpos = vars->best_target.pos;
						auto item = vars->local_player->GetActiveItem();
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

					if (vars->visual.grenadeprediction
						&& vars->local_player->is_alive()) {
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
										if (current->endposition.distance(vars->local_player->transform()->position()) < 4.f) {
											//std::string on();
											auto ws = string::wformat(_(L"Grenade from %s"), entity->get_player_name());
											render.StringCenter({ vars->ScreenX / 2, (vars->ScreenY / 2) - 50 }, ws, { 227 / 255.f, 32 / 255.f, 61 / 255.f, 1 });
										}
									}
								}
							}
						}
					}

					if (is_friend && !vars->visual.drawfriends)
						continue;

					if (vars->wants_shoot)
						goto draw;

					if (!vars->combat.targetfriends && is_friend)
						goto draw;

					auto target = aim_target();
					switch (vars->combat.aimbone)
					{
					case 0: //Head
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::head)->position();
						break;
					case 1: //Spine 4
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::spine4)->position();
						break;
					case 2: //Pelvis
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::pelvis)->position();
						break;
					case 3: //R arm
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::r_forearm)->position();
						break;
					case 4: //L arm
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::l_forearm)->position();
						break;
					case 5: //R leg
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::r_knee)->position();
						break;
					case 6: //L leg
						target.pos = ent->model()->boneTransforms()->get((int)Bone_List::l_knee)->position();
						break;
					}
					auto distance = vars->local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos); //crashes bc non game thread
					target.distance = distance;

					auto fov = unity::get_fov(target.pos);
					target.fov = fov;

					target.ent = (BaseCombatEntity*)ent;

					target.network_id = ent_id;

					//auto visible = ent->is_visible(vars->local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
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
						else if (((BasePlayer*)entity)->userID() == ((BasePlayer*)vars->best_target.ent)->userID())
							goto setandrecordtarget;
						else
							goto draw;
					}

				choosetarget:
					if (target < vars->best_target
						|| (!vars->best_target.ent->is_alive() && entity->is_alive())
						|| (target.ent && ((BasePlayer*)target.ent)->userID() == ((BasePlayer*)vars->best_target.ent)->userID()))
					{
					setandrecordtarget:
						vars->best_target.pos = target.pos;
						vars->best_target.distance = target.distance;
						vars->best_target.fov = target.fov;
						vars->best_target.ent = target.ent;
						vars->best_target.visible = visible;

						auto vel = target.ent->GetWorldVelocity();
						
						float next_frame = vars->best_target.last_frame + get_deltaTime();
						if (get_fixedTime() > next_frame)
						{
							//new frame, record velocity, record frame
							vars->best_target.last_frame = get_fixedTime();
							if (vars->best_target.velocity_list.size() < 30) //0.03125 * 30 = 0.9 seconds
								vars->best_target.velocity_list.push_back(vel);
							else
							{
								vars->best_target.velocity_list.pop_back();
								vars->best_target.velocity_list.insert(vars->best_target.velocity_list.begin(), 1, vel);
							}
							float avgx = 0.f;
							float avgy = 0.f;
							float avgz = 0.f;
							int count = 0;
							for (auto v : vars->best_target.velocity_list)
							{
								if (v.is_empty()) break;
								avgx += v.x;
								avgy += v.y;
								avgz += v.z;
								count += 1;
							}
							avgx /= count; avgy /= count; avgz /= count;
							vars->best_target.avg_vel = Vector3(avgx, avgy, avgz);
						}
					}

					if (vars->best_target.distance > 400.f)
						vars->best_target.Reset();
					if (target < vars->best_target
						&& target.fov < vars->combat.aimbotfov) {
						vars->best_target = target;
						found_a_target = true;
					}
					if (vars->best_target.fov > vars->combat.aimbotfov)
						vars->best_target.Reset();
				}

			draw:
				if (vars->visual.playeresp && vars->local_player)
				{
					DrawPlayer(entity, npc);
					//offscreen indicator?
					//silent melee?
				}
			}
		}

		if (vars->local_player)
		{
			//if (std::count(lw::entitylist.begin(), lw::entitylist.end(), ent) < 1)
			//	lw::entitylist.push_back(ent);

			//select entity			
			if (vars->selected_entity_id == ent_id) {

				Vector2 w2s_position = {};
				if (esp::out_w2s(world_position, w2s_position))
				{
					esp_color = Vector4(54, 116, 186, 255);
					w2s_position.y += 10;
					if (vars->selected_entity_id == ent_id) {
						render.StringCenter(w2s_position, _(L"selected"), { 54 / 255.f, 116 / 255.f, 186 / 255.f });
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(("[selected]")), esp_color);
						w2s_position.y += 10;
						render.StringCenter(w2s_position, string::wformat(_(L"[%d]"), (int)ent_id), { 0.f, 1.f, 0.f });
						//esp::draw_item(w2s_position, il2cpp::methods::new_string(string::format(_("[%d]"), (int)ent_id)), esp_color);
					}
				}
			}

			//auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);

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
						BuildingGrade upgrade_tier = (BuildingGrade)(vars->misc.upgrade_tier + 1);
						auto distance = vars->local_player->eyes()->position().distance(world_position);
						if (distance < 4.2f) {
							if (!vars->closest_building_block)
								vars->closest_building_block = (uintptr_t)block;
							else
							{
								if (block->grade() != upgrade_tier) {
									auto tranny = ((BuildingBlock*)vars->closest_building_block)->transform();
									auto pos = tranny->position();
									auto lastdist = vars->local_player->eyes()->position().distance(pos);
									if (lastdist > distance)
										vars->closest_building_block = (uintptr_t)block;
								}
							}
						}
					}
				}
			}

			//collectibleitem stuff
			if (vars->misc.pickup_collectibles) {
				if (!strcmp(entity_class_name, _("CollectibleEntity"))) {
					auto dist = world_position.distance(vars->local_player->transform()->position());
					if (dist < 4.f)
						ent->ServerRPC(_(L"Pickup"));
				}
			}

			//silent stash stuff
			if (vars->misc.openstash) {
				if (!strcmp(ent->get_class_name(), _("StashContainer"))) {
					if (unity::GetKey(vars->keybinds.stash)) {
						if (ent->transform()->position().distance(
							vars->local_player->transform()->position()) < .9f) {
							ent->ServerRPC(_(L"RPC_OpenLoot"));
						}
					}
				}
			}

			if (vars->visual.turretradius) {
				auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				if (!object_name.zpad)
					continue;
				if (vars->visual.traps && (*(int*)(object_name.zpad + 36) == 'terr' || *(int*)(object_name.zpad + 43) == 'tnug' || *(int*)(object_name.zpad + 38) == 'rtra'))
					if (*(int*)(object_name.zpad + 36) == 'terr')
						Sphere(world_position, 30.f, col(r, g, b, 1), 0.01f, false);
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
				auto dist = world_position.distance(vars->local_player->transform()->position());
				if (vars->visual.dist_on_items < dist) continue;
				esp_name = _(L"");
				auto object_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				if (!object_name.zpad)
					continue;

				auto m = vars->local_player->model();
				if (m)
				{
					auto trans = m->boneTransforms()->get(48);
					if (vars->local_player && vars->visual.distance && trans)
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
						//if(vars->local_player->bones()->head->position.distance(world_position) < 50.f)
						if (vars->local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 50.f)
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

							auto distance = vars->local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos);
							target.distance = distance;

							auto fov = unity::get_fov(target.pos);
							target.fov = fov;

							target.network_id = ent_id;

							if (fov < vars->combat.aimbotfov)
							{
								target.ent = base_heli;

								//auto visible = vars->local_player->is_visible(vars->local_player->model()->boneTransforms()->get(48)->get_position(), target.pos);
								//target.visible = visible;

								if (target < vars->best_target)
								{
									vars->best_target = target;
									vars->best_target.is_heli = true;
								}
								else if (vars->best_target.is_heli)
									vars->best_target.pos = target.pos;
								else vars->best_target.is_heli = false;
							}
							else
							{
								vars->best_target.Reset();
							}
						}
						else vars->best_target.is_heli = false;

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
					if (vars->local_player->model()->boneTransforms()->get(48)->position().distance(world_position) < 4.5f)
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
						render.StringCenter(w2s_position, string::wformat(_(L"Hackable crate: %ss"), str), { 130 / 255.f, 200 / 255.f, 230 / 255.f });
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

				else if (vars->visual.barrels && std::string(object_name.zpad).find(_("barrel")) != std::string::npos) {
					world_position.y += 1.f;
					esp_name = _(L"Barrel");
					esp_color = Vector4(vars->colors.items.crate[0] * 255.f,
						vars->colors.items.crate[1] * 255.f,
						vars->colors.items.crate[2] * 255.f,
						1);
				}

				//food crates
				else if (vars->visual.food && std::string(object_name.zpad).find(_("crate_normal_2_food")) != std::string::npos) {
					world_position.y += 0.3f;
					esp_name = _(L"Food crate");
					esp_color = Vector4(vars->colors.items.crate[0] * 255.f,
						vars->colors.items.crate[1] * 255.f,
						vars->colors.items.crate[2] * 255.f,
						1);
				}

				//mil crates
				else if (vars->visual.crates && std::string(object_name.zpad).find(_("crate_normal")) != std::string::npos) {
					world_position.y += 0.3f;
					esp_name = _(L"Military crate");
					esp_color = Vector4(vars->colors.items.crate[0] * 255.f,
						vars->colors.items.crate[1] * 255.f,
						vars->colors.items.crate[2] * 255.f,
						1);
				}
				

				//mil crates
				else if (vars->visual.crates && std::string(object_name.zpad).find(_("crate_tools")) != std::string::npos) {
					world_position.y += 0.3f;
					esp_name = _(L"Tool crate");
					esp_color = Vector4(vars->colors.items.crate[0] * 255.f,
						vars->colors.items.crate[1] * 255.f,
						vars->colors.items.crate[2] * 255.f,
						1);
				}

				//elite crates
				else if (vars->visual.crates && std::string(object_name.zpad).find(_("crate_elite")) != std::string::npos) {
					world_position.y += 0.3f;
					esp_name = _(L"Elite crate");
					esp_color = Vector4(vars->colors.items.crate[0] * 255.f,
						vars->colors.items.crate[1] * 255.f,
						vars->colors.items.crate[2] * 255.f,
						1);
				}

				//animals
				else if (vars->visual.animal) {
					if (std::string(object_name.zpad) == _("bear"))
						esp_name = _(L"Bear");
					else if (std::string(object_name.zpad) == _("polarbear"))
						esp_name = _(L"Polar bear");
					else if (std::string(object_name.zpad) == _("reindeer"))
						esp_name = _(L"Reindeer");
					else if (std::string(object_name.zpad) == _("reindeer1"))
						esp_name = _(L"Reindeer");
					else if (std::string(object_name.zpad) == _("wolf"))
						esp_name = _(L"Wolf");
					else if (std::string(object_name.zpad) == _("chicken"))
						esp_name = _(L"Chicken");
					else if (std::string(object_name.zpad) == _("horse"))
						esp_name = _(L"Horse");

					world_position.y += 0.5f;
					esp_color = Vector4(vars->colors.items.animal[0] * 255.f,
						vars->colors.items.animal[1] * 255.f,
						vars->colors.items.animal[2] * 255.f,
						1);
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

	//printf("after ent loop\n");
	System::list<Item*>* belt = ((BasePlayer*)vars->best_target.ent)->get_belt_items();
	if (!belt)
		vars->best_target.Reset();

	if (vars->best_target.ent
		&& vars->visual.hotbar_esp
		&& vars->local_player) {
		DrawPlayerHotbar(vars->best_target);
	}
	else {
		vars->target_hotbar_list = {};
		vars->target_name = _("");
	}
	//printf("iterate entities return\n");
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
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 1.3f);
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
	if (vars->best_target.pos.is_empty()
		|| !vars->best_target.ent) return;
	Vector2 start = vars->visual.snapline == 1 ? Vector2(vars->ScreenX / 2, 0) :
		vars->visual.snapline == 2 ? Vector2(vars->ScreenX / 2, vars->ScreenY / 2) :
		vars->visual.snapline == 3 ? Vector2(vars->ScreenX / 2, vars->ScreenY) :
		Vector2(vars->ScreenX / 2, vars->ScreenY);
	Vector3 o = WorldToScreen(vars->best_target.pos);
	if (o.x != 0 && o.y != 0)
		render.Line(start, Vector2(o.x, o.y), FLOAT4TOD3DCOLOR(vars->best_target.visible ? vars->colors.ui.snapline.visible : vars->colors.ui.snapline.invisible), 1.f, true);
}

void DrawFov() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->best_target.visible ? vars->colors.ui.fov.visible : vars->colors.ui.fov.invisible), vars->combat.aimbotfov, 0.7f);
}

void Watermark() {
	render.FillRectangle(Vector2(7, 7), Vector2(40, 16), FLOAT4TOD3DCOLOR(vars->accent_color_opaque));
	render.FillRectangle(Vector2(6, 6), Vector2(42, 18), D2D1::ColorF(0.11f, 0.11f, 0.11f, 0.6));
	render.FillRectangle(Vector2(7, 21), Vector2(40, 2), FLOAT4TOD3DCOLOR(vars->accent_color_opaque));
	render.String({ 14,7 }, _(L"Matrix"), FLOAT4TOD3DCOLOR(vars->accent_color));
}

void LoadGuiSkinmap() {
	//uiskin item = {};
	//or(int i = 0; i < SkinFileLines.size(); i++) {
	//	auto line = SkinFileLines[i];
	//	if (line.find(_(L"(")) != std::wstring::npos) { //is new item name
	//		if (wcslen(item.ItemName.c_str()) > 1) { //will not run first time
	//			if (!map_contains_key(vars->gui_skin_map, item.ItemName)) {
	//				vars->gui_skin_map.insert(std::make_pair(item.ItemName, item));
	//				//item = new guiskin();
	//				item = {};
	//			}
	//		}
	//		auto fpos = line.find(L"(") + 1;
	//		auto zpos = line.find(L")") + 1;
	//		auto ns1 = line.substr(0, fpos - 2).c_str();
	//		item.ShortName = ns1;
	//		item.ItemName = line.substr(fpos, (zpos - fpos) - 1);
	//	}
	//	else if (line.find(_(L"Skin ID")) == std::string::npos
	//		&& line.find(_(L"Skin Display Name")) == std::string::npos) {
	//		//gskin* ns = new gskin();
	//		gskin ns;
	//		auto idws = line.substr(0, line.find_first_of(_(L" ")));
	//		auto ids = std::string(idws.begin(), idws.end());
	//		auto id = atoi(ids.c_str());
	//		ns.SkinId = id;
	//		ns.DisplayName = line.substr(line.find_first_of(_(L" ")) + 1, line.size());
	//		item.skins.push_back(ns);
	//	}
	//
}

bool finit = false;

void new_frame() {
	if (!finit) {
		//printf("unity base: %" PRIxPTR "\n", mem::unity_player_base);
		//printf("game assembly base: %" PRIxPTR "\n", mem::game_assembly_base);
		//float Y = GetSystemMetrics(SM_CYSCREEN);
		//float X = GetSystemMetrics(SM_CXSCREEN);
		Vector2 vScreen = render.CanvasSize();
		vars->ScreenX = vScreen.x; 
		vars->ScreenY = vScreen.y;
		finit = true;

		//lw::entities::playerlist = new std::vector<BasePlayer*>();
		//InitLua();
	}
	//lw::playerlist.clear();
	//lw::entitylist.clear();
	//printf("crosshairs\n");
	//Draw crosshairs

	if (vars->local_player) {
		if (vars->visual.crosshair1)
			Crosshair1();
		if (vars->visual.crosshair2)
			Crosshair2();
		if (vars->visual.crosshair3)
			Crosshair3();

		//printf("indicators\n");
		//Draw indicators
		if (vars->visual.desync_indicator)
			IndicatorDesync();
		if (vars->combat.always_reload)
			IndicatorReload();
		if (vars->visual.speedhack_indicator)
			IndicatorSpeedhack();
		if (vars->misc.tp)
			IndicatorTp();
		if (vars->visual.snapline > 0)
			DrawSnapline();

		if (vars->visual.flyhack_indicator)
			IndicatorFlyhack();

		//printf("fov\n");
		//Draw FOV
		if (vars->visual.show_fov)
			DrawFov();

		if (vars->visual.radar)
			DrawRadarBackground();
	}
	//printf("watermark\n");
	//Draw watermark
	Watermark();
	if (vars->local_player
		&& vars->visual.hitpoint) {
		int rindex = -1;
		for (size_t i = 0; i < hitpoints.size(); i++)
		{
			auto h = hitpoints[i];
			auto w2s = WorldToScreen(h.position);
			if(!w2s.is_empty())
				render.StringCenter({ w2s.x, w2s.y }, _(L"hit"), FLOAT4TOD3DCOLOR(vars->colors.ui.hitpoints));
			if (h.time + 5.f < get_fixedTime()) rindex = i;
		}
		if (rindex != -1) hitpoints.erase(hitpoints.begin() + rindex);
	}

	//iterate_entities();
	//printf("iterate entities\n");
	iterate_entities();
	return;
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
	
				sol::protected_function func = lua[_("entityloop")];
				//func.set_error_handler(lua[_("errorhandler")]);
				if (func.valid())
					auto f = (std::function<void()>)func();
			}
		}
	}
	__except (true) {}
	*/

	//if(lw::playerlist.size() > 0)
	//	if (!vars->local_player) {
	//		lw::playerlist.clear();
	//		lw::entitylist.clear();
	//	}
	//for (auto v : lw::playerlist)
	//	if (!v)
	//		lw::playerlist.erase(
	//			std::remove(
	//				lw::playerlist.begin(),
	//				lw::playerlist.end(),
	//				v),
	//			lw::playerlist.end());
	//for (auto v : lw::entitylist)
	//	if (!v)
	//		lw::entitylist.erase(
	//			std::remove(
	//				lw::entitylist.begin(),
	//				lw::entitylist.end(),
	//				v),
	//			lw::entitylist.end());
	
	//printf("new_frame return\n");
	//does it make the object 3 times? it lags and flickers lots.
	for (auto o : vars->RenderList) {
		if (o->HasBeenDrawn) {
			vars->RenderList.erase(std::remove(vars->RenderList.begin(), vars->RenderList.end(), o), vars->RenderList.end());
			continue;
		}
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
	}
}