#pragma once
#include "gui.h"
#include "vars.hpp"

#include "memory/lazy_importer.hpp"
#include "utils/xorstr.hpp"
#include <vector>
#include <filesystem>
#include <fstream>

#include "snake.hpp"

#if CUSTOM_ICONS
#include "icons.h"
#endif
#include "utils/vector.hpp"

#include <filesystem>

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

float _r = 1.00f, _g = 0.00f, _b = 1.00f;


namespace Gui
{
	sg::Snake mysnake;
	gplayer* selected_player = nullptr;

	void SnakeStuff() {
		im::SetNextWindowSize({ 301, 395 });
		im::Begin(_("Snake game"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		{
			if (im::BeginChild(_("Game"), { 285, 286 }, true))
			{
				auto cpos = im::GetCursorPos(); 
				cpos.x += im::GetWindowPos().x;
				cpos.y += im::GetWindowPos().y;

				mysnake.DoInput();

				for (size_t i = 0; i < sg::gridsize; i++) {
					for (size_t f = 0; f < sg::gridsize; f++) {
						int x = i * 14; x += cpos.x;
						int y = f * 14; y += cpos.y;
						int ex = 2; ex += cpos.x;
						int ey = 2; ey += cpos.y;

						Vector2 vec(i, f);

						bool drew = false;

						for (size_t j = 0; j < mysnake.sp.size(); j++) {
							if (mysnake.sp[j] == vec) {
								if(j == 0) //head
									im::GetForegroundDrawList()->AddRectFilled({ (float)x - 4.f, (float)y - 4.f }, { x + 8.f, y + 8.f }, im::GetColorU32({ 0.24, 0.24, 0.94, 0.95 }), 1.f);
								else
									im::GetForegroundDrawList()->AddRectFilled({ (float)x - 4.f, (float)y - 4.f }, { x + 8.f, y + 8.f }, im::GetColorU32({ 0.24, 0.74, 0.24, 0.95 }), 1.f);
								drew = true;
							}
						}

						if (drew) continue;

						for (size_t j = 0; j < sg::apples.size(); j++) {
							if (sg::apples[j - 1] == vec) {
								im::GetForegroundDrawList()->AddRectFilled({ (float)x - 4.f, (float)y - 4.f }, { x + 8.f, y + 8.f }, im::GetColorU32({ 0.94, 0.24, 0.24, 0.65 }), 1.f);
								drew = true;
							}
						}
						if (drew) continue;
						
						im::GetForegroundDrawList()->AddRect({ (float)x - 4.f, (float)y - 4.f }, { x + 8.f, y + 8.f }, im::GetColorU32({ 0.34, 0.34, 0.34, 0.65 }), 1.f);
					}
				}

				im::EndChild();
			}
			if (im::BeginChild(_("Control"), { 285, 67 }, true))
			{
				im::SetCursorPosX(im::GetCursorPosX() + 66);

				if (GetAsyncKeyState(0x57)) //W
					im::TextColored({ 0.1, 0.8, 0.1, 1 }, _("W"));
				else im::Text(_("W"));

				im::SetCursorPosX(im::GetCursorPosX() + 50);
				if (GetAsyncKeyState(0x41)) //A
					im::TextColored({ 0.1, 0.8, 0.1, 1 }, _("A"));
				else im::Text(_("A"));
				im::SameLine();
				if (GetAsyncKeyState(0x53)) //S
					im::TextColored({ 0.1, 0.8, 0.1, 1 }, _("S"));
				else im::Text(_("S"));
				im::SameLine();
				if (GetAsyncKeyState(0x44)) //D
					im::TextColored({ 0.1, 0.8, 0.1, 1 }, _("D"));
				else im::Text(_("D"));

				im::SetCursorPosX(im::GetCursorPosX() + 40);
				if (!mysnake.alive)
					if (im::Button(_("Start"), { 60, 20 }))
						mysnake = sg::Snake();
				

				im::SetCursorPos({ im::GetCursorPosX() + 150, im::GetCursorPosY() - 40 });
				im::LabelText(_("##score"), _("Score: %d"), sg::currentscore);
				im::SetCursorPosX(im::GetCursorPosX() + 150);
				im::SetCursorPosY(im::GetCursorPosY() - 10);
				im::LabelText(_("##highscore"), _("Highscore: %d"), sg::highscore);
				im::EndChild();
			}
		}
		im::End();
	}

	void PlayersListWindow() {
		im::SetNextWindowSize({ 495, 206 });
		im::Begin(_("##Players"), 0, ImGuiWindowFlags_NoCollapse);
		{
			if (im::BeginChild(_("List"), { 285, 166 }, true))
			{
				im::Text(_("Players"));
				im::Separator();
				if (im::ListBoxHeader(_("##PlayerList"), ImVec2(270, 128)))
				{
					for (auto pair : vars->gui_player_map)
					{
						auto name = pair.second->name;
						if (im::Selectable(std::string(name.begin(), name.end()).c_str()))
						{
							for (auto g : vars->gui_player_map)
								if (g.second->name == name)
									selected_player = g.second;
						}
						if (pair.second->follow)
							vars->follow_player_id = pair.second->userid;
					}
					im::ListBoxFooter();
				}
				im::EndChild();
			}
			im::SameLine();
			if (im::BeginChild(_("Controls"), { 185, 166 }, true))
			{
				if (selected_player) {
					im::Text(_("Options for %s"), std::string(selected_player->name.begin(), selected_player->name.end()).c_str());
					im::Separator();
					im::Text(_("SteamId3: %d"), selected_player->userid);
					im::Checkbox(_("Friend"), &selected_player->is_friend);
					if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
						im::SetTooltip(_("Adds player to cheat friends list"));
					}
					//im::Checkbox(_("Target priority"), &selected_player->priority_target);
					im::Checkbox(_("Follow player"), &selected_player->follow);
					if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
						im::SetTooltip(_("When walk to has player selected the last player followed in player list is chosen, then target"));
					}
					im::Checkbox(_("Block player"), &selected_player->block);
					if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
						im::SetTooltip(_("Walks in front of player"));
					}
				}
				else {
					im::Text(_("Choose player"));
				}
				im::EndChild();
			}
		}
		im::End();
	}

	void SkinChanger() {
		im::SetNextWindowSize({ 495, 206 });
		im::Begin(_("##Items"), 0, ImGuiWindowFlags_NoCollapse);
		{
			if (im::BeginChild(_("List"), { 285, 166 }, true))
			{
				im::Text(_("Items"));
				im::Separator();
				if (im::ListBoxHeader(_("##ItemsList"), ImVec2(270, 128)))
				{
					//for (auto s : weapon_names_list) {
					//	if (im::Selectable(s.c_str())) {
					//
					//	}
					//}
					im::ListBoxFooter();
				}
				im::EndChild();
			}
		}
		im::End();
	}

	class dot
	{
	public:
		dot(Vector3 p, Vector3 v) {
			m_vel = v;
			m_pos = p;
		}

		void update();
		void draw();

		Vector3 m_pos, m_vel;
	};

	std::vector<dot*> dots = { };

	void dot::update() {
		auto opacity = 240 / 255.0f;

		m_pos += m_vel * (opacity);
	}

	void dot::draw() {
		int opacity = 55.0f * (240 / 255.0f);

		im::GetWindowDrawList()->AddRectFilled({ m_pos.x - 2, m_pos.y - 2 }, { m_pos.x + 2, m_pos.y + 2 }, ImColor(70, 130, 180, 100));

		auto t = std::find(dots.begin(), dots.end(), this);
		if (t == dots.end()) {
			return;
		}

		for (auto i = t; i != dots.end(); i++) {
			if ((*i) == this) continue;

			auto dist = (m_pos - (*i)->m_pos).length_2d();

			auto ydist = m_pos.y - (*i)->m_pos.y;

			if (dist < 128
				&& ydist < 50) {
				int alpha = opacity * (dist / 128);
				im::GetWindowDrawList()->AddLine({ m_pos.x - 1, m_pos.y - 2 }, { (*i)->m_pos.x - 2, (*i)->m_pos.y - 1 }, ImColor(192, 192, 192, 100));
			}
		}
	}

	void dotdraw() {
		struct screen_size {
			int x, y;
		}; screen_size sc;

		sc.x = vars->ScreenX, sc.y = vars->ScreenY;

		int s = rand() % 9;
		/*
		if (s == 0) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, -16 - 100, 0), Vector3((rand() % 7) - 3, rand() % 3 + 1, 0)));
		}
		else if (s == 3) {
			dots.push_back(new dot(Vector3((int)sc.x + 16, rand() % (int)sc.y - 100, 0), Vector3(-1 * (rand() % 3 + 1), (rand() % 7) - 3, 0)));
		}
		else if (s == 2) {
			dots.push_back(new dot(Vector3(-16, rand() % (int)sc.y - 100, 0), Vector3(rand() % 3 + 1, (rand() % 7) - 3, 0)));
		}*/
		if (s == 1) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, (int)sc.y + 16 - 100, 0), Vector3((rand() % 7) - 3, -1 * (rand() % 3 + 1), 0)));
		}

		auto alph = 135.0f * (240 / 255.0f);
		auto a_int = (int)(alph);

		im::GetWindowDrawList()->AddRectFilled({ 0, 0 }, { (float)sc.x / 2, (float)sc.y / 2 }, ImColor(a_int, 0, 0, 0));

		for (auto i = dots.begin(); i < dots.end();) {
			if ((*i)->m_pos.y < -20 || (*i)->m_pos.y > sc.y + 20 || (*i)->m_pos.x < -20 || (*i)->m_pos.x > sc.x + 20) {
				delete (*i);
				i = dots.erase(i);
			}
			else {
				(*i)->update();
				i++;
			}
		}

		for (auto i = dots.begin(); i < dots.end(); i++) {
			(*i)->draw();
		}
	}

	bool init = false;
	int selected_tab = 0;
	static char str0[32] = "";
	int selected_cfg = 0;
	std::vector<char*> tempnames;

	void delete_config(char* cfgname) {
		auto s = LI_FIND(getenv)(_("APPDATA"));
		auto p = s + std::string(_("\\trap\\configs\\")) + std::string(cfgname) + _(".t");
		remove(p.c_str());
	}

	void loadcolor(float* col, std::string line)
	{
		auto vals = split(line, _(","));
		col[0] = stof(vals[0]);
		col[1] = stof(vals[1]);
		col[2] = stof(vals[2]);
	}

	std::string savefloat(std::string beginning, float* col) {
		return beginning + "=" + std::to_string(col[0]) + _(",") + std::to_string(col[1]) + _(",") + std::to_string(col[2]) + _("\n");
	}

	void load_config() {
		auto s = LI_FIND(getenv)(_("APPDATA"));
		auto p = s + std::string(_("\\matrix\\configs\\")) + std::string(str0) + _(".m");
		if (!std::filesystem::exists(p)) return;
		std::ifstream cFile(p, std::ios::in);
		

		std::string line;
		while (getline(cFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line[0] == '#' || line.empty()) continue;

			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);

			if (name == _("boxvis")) loadcolor(vars->colors.players.boxes.visible, value);
			else if (name == _("boxinvis")) loadcolor(vars->colors.players.boxes.invisible, value);
			else if (name == _("chamsvis")) loadcolor(vars->colors.players.chams.visible, value);
			else if (name == _("chamsinvis")) loadcolor(vars->colors.players.chams.invisible, value);
			else if (name == _("namevis")) loadcolor(vars->colors.players.details.name.visible, value);
			else if (name == _("nameinvis")) loadcolor(vars->colors.players.details.name.invisible, value);
			else if (name == _("distvis")) loadcolor(vars->colors.players.details.distance.visible, value);
			else if (name == _("distinvis")) loadcolor(vars->colors.players.details.distance.invisible, value);
			else if (name == _("flagvis")) loadcolor(vars->colors.players.details.flags.visible, value);
			else if (name == _("flaginvis")) loadcolor(vars->colors.players.details.flags.invisible, value);
			else if (name == _("skelvis")) loadcolor(vars->colors.players.details.skeleton.visible, value);
			else if (name == _("skelinvis")) loadcolor(vars->colors.players.details.skeleton.invisible, value);
			else if (name == _("snapvis")) loadcolor(vars->colors.ui.snapline.visible, value);
			else if (name == _("snapinvis")) loadcolor(vars->colors.ui.snapline.invisible, value);
			else if (name == _("fovvis")) loadcolor(vars->colors.ui.fov.visible, value);
			else if (name == _("fovinvis")) loadcolor(vars->colors.ui.fov.invisible, value);
			else if (name == _("crosshair")) loadcolor(vars->colors.ui.crosshair, value);
			else if (name == _("hempvis")) loadcolor(vars->colors.items.hemp.visible, value);
			else if (name == _("hempinvis")) loadcolor(vars->colors.items.hemp.invisible, value);
			else if (name == _("stonevis")) loadcolor(vars->colors.items.stone.visible, value);
			else if (name == _("stoneinvis")) loadcolor(vars->colors.items.stone.invisible, value);
			else if (name == _("sulfurvis")) loadcolor(vars->colors.items.sulfur.visible, value);
			else if (name == _("sulfurinvis")) loadcolor(vars->colors.items.sulfur.invisible, value);
			else if (name == _("metalvis")) loadcolor(vars->colors.items.metal.visible, value);
			else if (name == _("metalinvis")) loadcolor(vars->colors.items.metal.invisible, value);
			else if (name == _("barrelvis")) loadcolor(vars->colors.items.barrel.visible, value);
			else if (name == _("barrelinvis")) loadcolor(vars->colors.items.barrel.invisible, value);
			else if (name == _("animalvis")) loadcolor(vars->colors.items.animal.visible, value);
			else if (name == _("animalinvis")) loadcolor(vars->colors.items.animal.invisible, value);
			else if (name == _("stashopen")) loadcolor(vars->colors.items.stash.open, value);
			else if (name == _("stashclosed")) loadcolor(vars->colors.items.stash.closed, value);
			else if (name == _("accent")) loadcolor(vars->accent_color, value);

			if (name == _("psilent")) vars->combat.psilent = std::stoi(value);
			else if (name == _("aimbotfov")) vars->combat.aimbotfov = std::stoi(value);
			else if (name == _("hitbox_override")) vars->combat.hitbox_override = std::stoi(value);
			else if (name == _("hitbox")) vars->combat.hitbox = std::stoi(value);
			else if (name == _("silent_melee")) vars->combat.silent_melee = std::stoi(value);
			else if (name == _("manipulator")) vars->combat.manipulator = std::stoi(value);
			else if (name == _("doubletap")) vars->combat.doubletap = std::stoi(value);
			else if (name == _("always_reload")) vars->combat.always_reload = std::stoi(value);
			else if (name == _("randomize")) vars->combat.randomize = std::stoi(value);
			else if (name == _("autoshoot")) vars->combat.autoshoot = std::stoi(value);
			else if (name == _("bullet_tp")) vars->combat.bullet_tp = std::stoi(value);
			else if (name == _("instaeoka")) vars->combat.instaeoka = std::stoi(value);
			else if (name == _("weakspots")) vars->combat.weakspots = std::stoi(value);
			else if (name == _("locktarget")) vars->combat.locktarget = std::stoi(value);
			else if (name == _("rapidfire")) vars->combat.rapidfire = std::stoi(value);
			else if (name == _("automatic")) vars->combat.automatic = std::stoi(value);
			else if (name == _("thick_bullet")) vars->combat.thick_bullet = std::stoi(value);
			else if (name == _("always_shoot")) vars->combat.always_shoot = std::stoi(value);
			else if (name == _("melee_range")) vars->combat.melee_range = std::stof(value);
			else if (name == _("thickness")) vars->combat.thickness = std::stof(value);
			else if (name == _("norecoil")) vars->combat.norecoil = std::stoi(value);
			else if (name == _("nospread")) vars->combat.nospread = std::stoi(value);
			else if (name == _("fast_bullet")) vars->combat.fast_bullet = std::stoi(value);

			else if (name == _("playeresp")) vars->visual.playeresp = std::stoi(value);
			else if (name == _("boxtype")) vars->visual.boxtype = std::stoi(value);
			else if (name == _("hpbar")) vars->visual.hpbar = std::stoi(value);
			else if (name == _("crosshair1")) vars->visual.crosshair1 = std::stoi(value);
			else if (name == _("crosshair2")) vars->visual.crosshair2 = std::stoi(value);
			else if (name == _("crosshair3")) vars->visual.crosshair3 = std::stoi(value);
			else if (name == _("playerfov")) vars->visual.playerfov = std::stof(value);
			else if (name == _("zoomtoggle")) vars->visual.zoomtoggle = std::stoi(value);
			else if (name == _("staramount")) vars->visual.staramount = std::stof(value);
			else if (name == _("hitpoint")) vars->visual.hitpoint = std::stoi(value);
			else if (name == _("targetted")) vars->visual.targetted = std::stoi(value);
			else if (name == _("always_day")) vars->visual.always_day = std::stoi(value);
			else if (name == _("zoomfov")) vars->visual.zoomfov = std::stof(value);
			else if (name == _("berry")) vars->visual.berry = std::stoi(value);
			else if (name == _("offscreen_indicator")) vars->visual.offscreen_indicator = std::stoi(value);
			else if (name == _("food")) vars->visual.food = std::stoi(value);
			else if (name == _("animal")) vars->visual.animal = std::stoi(value);
			else if (name == _("corpses")) vars->visual.corpses = std::stoi(value);
			else if (name == _("draw_fov")) vars->visual.draw_fov = std::stoi(value);
			else if (name == _("rainbow_chams")) vars->visual.rainbow_chams = std::stoi(value);
			else if (name == _("hotbar_esp")) vars->visual.hotbar_esp = std::stoi(value);
			else if (name == _("hand_chams")) vars->visual.hand_chams = std::stoi(value);
			else if (name == _("chams")) vars->visual.shader = std::stoi(value);
			else if (name == _("boxesp")) vars->visual.boxesp = std::stoi(value);
			else if (name == _("spriteitem")) vars->visual.spriteitem = std::stoi(value);
			else if (name == _("snaplines")) vars->visual.snaplines = std::stoi(value);
			else if (name == _("rainbowname")) vars->visual.rainbowname = std::stoi(value);
			else if (name == _("rainbowbox")) vars->visual.rainbowbox = std::stoi(value);
			else if (name == _("rainbowhpbar")) vars->visual.rainbowhpbar = std::stoi(value);
			else if (name == _("rainbowskeleton")) vars->visual.rainbowskeleton = std::stoi(value);
			else if (name == _("rainbowflags")) vars->visual.rainbowflags = std::stoi(value);
			else if (name == _("rainbowdist")) vars->visual.rainbowdist = std::stoi(value);
			else if (name == _("show_fov")) vars->visual.show_fov = std::stoi(value);
			else if (name == _("misc_esp")) vars->visual.misc_esp = std::stoi(value);
			else if (name == _("sleeper_esp")) vars->visual.sleeper_esp = std::stoi(value);
			else if (name == _("heli_esp")) vars->visual.heli_esp = std::stoi(value);
			else if (name == _("npc_esp")) vars->visual.npc_esp = std::stoi(value);
			else if (name == _("dropped_items")) vars->visual.dropped_items = std::stoi(value);
			else if (name == _("stash")) vars->visual.stash = std::stoi(value);
			else if (name == _("sulfur_ore")) vars->visual.sulfur_ore = std::stoi(value);
			else if (name == _("stone_ore")) vars->visual.stone_ore = std::stoi(value);
			else if (name == _("metal_ore")) vars->visual.metal_ore = std::stoi(value);
			else if (name == _("weapon")) vars->visual.Item = std::stoi(value);
			else if (name == _("midhealth")) vars->visual.midhealth = std::stoi(value);
			else if (name == _("midname")) vars->visual.midname = std::stoi(value);
			else if (name == _("traps")) vars->visual.traps = std::stoi(value);
			else if (name == _("vehicles")) vars->visual.vehicles = std::stoi(value);
			else if (name == _("airdrops")) vars->visual.airdrops = std::stoi(value);
			else if (name == _("cloth")) vars->visual.cloth = std::stoi(value);
			else if (name == _("barrels")) vars->visual.barrels = std::stoi(value);
			else if (name == _("tc_esp")) vars->visual.tc_esp = std::stoi(value);
			else if (name == _("angles")) vars->visual.angles = std::stoi(value);
			else if (name == _("hackable_crate_esp")) vars->visual.hackable_crate_esp = std::stoi(value);
			else if (name == _("full_box")) vars->visual.full_box = std::stoi(value);
			else if (name == _("corner_box")) vars->visual.corner_box = std::stoi(value);
			else if (name == _("bottomhealth")) vars->visual.bottomhealth = std::stoi(value);
			else if (name == _("snapline")) vars->visual.snapline = std::stoi(value);
			else if (name == _("sidehealth")) vars->visual.sidehealth = std::stoi(value);
			else if (name == _("skeleton")) vars->visual.skeleton = std::stoi(value);
			else if (name == _("nameesp")) vars->visual.nameesp = std::stoi(value);
			else if (name == _("woundedflag")) vars->visual.woundedflag = std::stoi(value);
			else if (name == _("distance")) vars->visual.distance = std::stoi(value);
			else if (name == _("weaponesp")) vars->visual.weaponesp = std::stoi(value);
			else if (name == _("desync_indicator")) vars->visual.desync_indicator = std::stoi(value);
			else if (name == _("flyhack_indicator")) vars->visual.flyhack_indicator = std::stoi(value);
			else if (name == _("shader")) vars->visual.shader = std::stoi(value);

			else if (name == _("day")) vars->visual.day = std::stof(value);
			else if (name == _("night")) vars->visual.night = std::stof(value);

			else if (name == _("auto_upgrade")) vars->misc.auto_upgrade = std::stoi(value);
			else if (name == _("flywall")) vars->misc.flywall = std::stoi(value);
			else if (name == _("force_privlidge")) vars->misc.force_privlidge = std::stoi(value);
			else if (name == _("upgrade_tier")) vars->misc.upgrade_tier = std::stoi(value);
			else if (name == _("flyhack_stop")) vars->misc.flyhack_stop = std::stoi(value);
			else if (name == _("playerfovtoggle")) vars->misc.playerfovtoggle = std::stoi(value);
			else if (name == _("playerfov")) vars->misc.playerfov = std::stof(value);
			else if (name == _("desync")) vars->misc.desync = std::stoi(value);
			else if (name == _("Movement")) vars->misc.Movement = std::stoi(value);
			else if (name == _("eyeoffset")) vars->misc.eyeoffset = std::stoi(value);
			else if (name == _("PlayerEyes")) vars->misc.PlayerEyes = std::stof(value);
			else if (name == _("spinbot")) vars->misc.spinbot = std::stoi(value);
			else if (name == _("always_shoot")) vars->misc.always_shoot = std::stoi(value);
			else if (name == _("attack_on_mountables")) vars->misc.attack_on_mountables = std::stoi(value);
			else if (name == _("speedhack")) vars->misc.speedhack = std::stoi(value);
			else if (name == _("hitsound")) vars->misc.hitsound = std::stoi(value);
			else if (name == _("speedhackspeed")) vars->misc.speedhackspeed = std::stof(value);
			else if (name == _("antispeed")) vars->misc.antispeed = std::stoi(value);
			if (name == _("tpcamdist")) vars->misc.tpcamdist = std::stof(value);
			else if (name == _("tpcamfov")) vars->misc.tpcamfov = std::stof(value);
			else if (name == _("TakeFallDamage")) vars->misc.TakeFallDamage = std::stoi(value);
			else if (name == _("silent_farm")) vars->misc.silent_farm = std::stoi(value);
			else if (name == _("auto_lock")) vars->misc.auto_lock = std::stoi(value);
			else if (name == _("always_sprint")) vars->misc.always_sprint = std::stoi(value);
			else if (name == _("gravity")) vars->misc.gravity = std::stoi(value);
			else if (name == _("infinite_jump")) vars->misc.infinite_jump = std::stoi(value);
			else if (name == _("fake_lag")) vars->misc.fake_lag = std::stoi(value);
			else if (name == _("admin_mode")) vars->misc.admin_mode = std::stoi(value);
			else if (name == _("view_offset")) vars->misc.view_offset = std::stoi(value);
			else if (name == _("norecycler")) vars->misc.norecycler = std::stoi(value);
			else if (name == _("instant_med")) vars->misc.instant_med = std::stoi(value);
			else if (name == _("instant_revive")) vars->misc.instant_revive = std::stoi(value);
			else if (name == _("no_playercollision")) vars->misc.no_playercollision = std::stoi(value);
			else if (name == _("spiderman")) vars->misc.spiderman = std::stoi(value);
			else if (name == _("silentwalk")) vars->misc.silentwalk = std::stoi(value);
			else if (name == _("autofarm")) vars->misc.autofarm = std::stoi(value);
			else if (name == _("interactive_debug")) vars->misc.interactive_debug = std::stoi(value);
			//else if (name == _("gesture_spam")) vars->misc.gesture_spam = std::stoi(value);
		}
	}

	void save_config(char* cfgname) {
		//save all settings idfk how yet
		auto s = LI_FIND(getenv)(_("APPDATA"));
		auto p = s + std::string(_("\\matrix\\configs\\")) + std::string(cfgname) + _(".m");
		remove(p.c_str());
		std::ofstream f(p);
		char buffer[4]; 

		auto str = savefloat(_("boxvis"), vars->colors.players.boxes.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("boxinvis"), vars->colors.players.boxes.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("chamsvis"), vars->colors.players.chams.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("chamsinvis"), vars->colors.players.chams.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("namevis"), vars->colors.players.details.name.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("nameinvis"), vars->colors.players.details.name.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("distvis"), vars->colors.players.details.distance.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("distinvis"), vars->colors.players.details.distance.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("flagvis"), vars->colors.players.details.flags.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("flaginvis"), vars->colors.players.details.flags.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("skelvis"), vars->colors.players.details.skeleton.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("skelinvis"), vars->colors.players.details.skeleton.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("snapvis"), vars->colors.ui.snapline.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("snapinvis"), vars->colors.ui.snapline.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("fovvis"), vars->colors.ui.fov.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("fovinvis"), vars->colors.ui.fov.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("crosshair"), vars->colors.ui.crosshair);
		f.write(str.c_str(), str.size());
		str = savefloat(_("hempvis"), vars->colors.items.hemp.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("hempinvis"), vars->colors.items.hemp.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("stonevis"), vars->colors.items.stone.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("stoneinvis"), vars->colors.items.stone.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("sulfurvis"), vars->colors.items.sulfur.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("sulfurinvis"), vars->colors.items.sulfur.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("metalvis"), vars->colors.items.metal.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("metalinvis"), vars->colors.items.metal.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("barrelvis"), vars->colors.items.barrel.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("barrelinvis"), vars->colors.items.barrel.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("animalvis"), vars->colors.items.animal.visible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("animalinvis"), vars->colors.items.animal.invisible);
		f.write(str.c_str(), str.size());
		str = savefloat(_("stashopen"), vars->colors.items.stash.open);
		f.write(str.c_str(), str.size());
		str = savefloat(_("stashclosed"), vars->colors.items.stash.closed);
		f.write(str.c_str(), str.size());
		str = savefloat(_("accent"), vars->accent_color);
		f.write(str.c_str(), str.size());

		itoa(vars->combat.psilent, buffer, 4);
		str = (std::string("psilent=") + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->combat.aimbotfov);
		str = (std::string(_("aimbotfov=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa((int)vars->combat.hitbox_override, buffer, 4);
		str = (std::string(_("hitbox_override=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa((int)vars->combat.randomize, buffer, 4);
		str = (std::string(_("randomize=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.hitbox, buffer, 4);
		str = (std::string(_("hitbox=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.silent_melee, buffer, 4);
		str = (std::string(_("silent_melee=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.manipulator, buffer, 4);
		str = (std::string(_("manipulator=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.doubletap, buffer, 4);
		str = (std::string(_("doubletap=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.weakspots, buffer, 4);
		str = (std::string(_("weakspots=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.locktarget, buffer, 4);
		str = (std::string(_("locktarget=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.instaeoka, buffer, 4);
		str = (std::string(_("instaeoka=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.always_reload, buffer, 4);
		str = (std::string(_("always_reload=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.autoshoot, buffer, 4);
		str = (std::string(_("autoshoot=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.bullet_tp, buffer, 4);
		str = (std::string(_("bullet_tp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.rapidfire, buffer, 4);
		str = (std::string(_("rapidfire=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.automatic, buffer, 4);
		str = (std::string(_("automatic=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.thick_bullet, buffer, 4);
		str = (std::string(_("thick_bullet=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.always_shoot, buffer, 4);
		str = (std::string(_("always_shoot=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->combat.melee_range);
		str = (std::string(_("melee_range=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->combat.thickness);
		str = (std::string(_("thickness=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.norecoil, buffer, 4);
		str = (std::string(_("norecoil=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.nospread, buffer, 4);
		str = (std::string(_("nospread=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->combat.fast_bullet, buffer, 4);
		str = (std::string(_("fast_bullet=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());

		itoa(vars->visual.playeresp, buffer, 4);
		str = (std::string(_("playeresp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		//itoa(vars->visual.boxtype, buffer, 4);
		str = (std::string(_("boxtype=")) + std::to_string(vars->visual.boxtype) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.hitpoint, buffer, 4);
		str = (std::string(_("hitpoint=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.targetted, buffer, 4);
		str = (std::string(_("targetted=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.hpbar, buffer, 4);
		str = (std::string(_("hpbar=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.crosshair1, buffer, 4);
		str = (std::string(_("crosshair1=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.crosshair2, buffer, 4);
		str = (std::string(_("crosshair2=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.crosshair3, buffer, 4);
		str = (std::string(_("crosshair3=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.playerfov);
		str = (std::string(_("playerfov=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.zoomtoggle, buffer, 4);
		str = (std::string(_("zoomtoggle=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.staramount);
		str = (std::string(_("staramount=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.always_day, buffer, 4);
		str = (std::string(_("always_day=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.zoomfov);
		str = (std::string(_("zoomfov=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.berry, buffer, 4);
		str = (std::string(_("berry=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.offscreen_indicator, buffer, 4);
		str = (std::string(_("offscreen_indicator=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.food, buffer, 4);
		str = (std::string(_("food=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.animal, buffer, 4);
		str = (std::string(_("animal=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.corpses, buffer, 4);
		str = (std::string(_("corpses=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.draw_fov, buffer, 4);
		str = (std::string(_("draw_fov=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbow_chams, buffer, 4);
		str = (std::string(_("rainbow_chams=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowbox, buffer, 4);
		str = (std::string(_("rainbowbox=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowdist, buffer, 4);
		str = (std::string(_("rainbowdist=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowname, buffer, 4);
		str = (std::string(_("rainbowname=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowflags, buffer, 4);
		str = (std::string(_("rainbowflags=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowhpbar, buffer, 4);
		str = (std::string(_("rainbowhpbar=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.rainbowskeleton, buffer, 4);
		str = (std::string(_("rainbowskeleton=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.hotbar_esp, buffer, 4);
		str = (std::string(_("hotbar_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.hand_chams, buffer, 4);
		str = (std::string(_("hand_chams=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.shader, buffer, 4);
		str = (std::string(_("chams=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.boxesp, buffer, 4);
		str = (std::string(_("boxesp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.spriteitem, buffer, 4);
		str = (std::string(_("spriteitem=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.snaplines, buffer, 4);
		str = (std::string(_("snaplines=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.show_fov, buffer, 4);
		str = (std::string(_("show_fov=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.misc_esp, buffer, 4);
		str = (std::string(_("misc_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.sleeper_esp, buffer, 4);
		str = (std::string(_("sleeper_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.heli_esp, buffer, 4);
		str = (std::string(_("heli_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.npc_esp, buffer, 4);
		str = (std::string(_("npc_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.dropped_items, buffer, 4);
		str = (std::string(_("dropped_items=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.VisRcolor);
		str = (std::string(_("VisRcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.VisGcolor);
		str = (std::string(_("VisGcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.VisBcolor);
		str = (std::string(_("VisBcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.InvRcolor);
		str = (std::string(_("InvRcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.InvGcolor);
		str = (std::string(_("InvGcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.InvBcolor);
		str = (std::string(_("InvBcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.TeamRcolor);
		str = (std::string(_("TeamRcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.TeamGcolor);
		str = (std::string(_("TeamGcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.TeamBcolor);
		str = (std::string(_("TeamBcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.nameRcolor);
		str = (std::string(_("nameRcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.nameGcolor);
		str = (std::string(_("nameGcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->visual.nameBcolor);
		str = (std::string(_("nameBcolor=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.stash, buffer, 4);
		str = (std::string(_("stash=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.sulfur_ore, buffer, 4);
		str = (std::string(_("sulfur_ore=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.stone_ore, buffer, 4);
		str = (std::string(_("stone_ore=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.metal_ore, buffer, 4);
		str = (std::string(_("metal_ore=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.Item, buffer, 4);
		str = (std::string(_("weapon=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.midhealth, buffer, 4);
		str = (std::string(_("midhealth=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.midname, buffer, 4);
		str = (std::string(_("midname=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.traps, buffer, 4);
		str = (std::string(_("traps=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.vehicles, buffer, 4);
		str = (std::string(_("vehicles=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.airdrops, buffer, 4);
		str = (std::string(_("airdrops=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.cloth, buffer, 4);
		str = (std::string(_("cloth=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.barrels, buffer, 4);
		str = (std::string(_("barrels=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.tc_esp, buffer, 4);
		str = (std::string(_("tc_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.angles, buffer, 4);
		str = (std::string(_("angles=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.hackable_crate_esp, buffer, 4);
		str = (std::string(_("hackable_crate_esp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.full_box, buffer, 4);
		str = (std::string(_("full_box=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.corner_box, buffer, 4);
		str = (std::string(_("corner_box=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.bottomhealth, buffer, 4);
		str = (std::string(_("bottomhealth=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.snapline, buffer, 4);
		str = (std::string(_("snapline=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.sidehealth, buffer, 4);
		str = (std::string(_("sidehealth=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.skeleton, buffer, 4);
		str = (std::string(_("skeleton=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.nameesp, buffer, 4);
		str = (std::string(_("nameesp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.woundedflag, buffer, 4);
		str = (std::string(_("woundedflag=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.distance, buffer, 4);
		str = (std::string(_("distance=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.weaponesp, buffer, 4);
		str = (std::string(_("weaponesp=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.desync_indicator, buffer, 4);
		str = (std::string(_("desync_indicator=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.flyhack_indicator, buffer, 4);
		str = (std::string(_("flyhack_indicator=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->visual.shader, buffer, 4);
		str = (std::string(_("shader=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());

		str = (std::string(_("day=")) + std::to_string(vars->visual.day) + _("\n"));
		f.write(str.c_str(), str.size());
		str = (std::string(_("night=")) + std::to_string(vars->visual.night) + _("\n"));
		f.write(str.c_str(), str.size());

		itoa(vars->misc.auto_upgrade, buffer, 4);
		str = (std::string(_("auto_upgrade=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.flywall, buffer, 4);
		str = (std::string(_("flywall=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.upgrade_tier, buffer, 4);
		str = (std::string(_("upgrade_tier=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.flyhack_stop, buffer, 4);
		str = (std::string(_("flyhack_stop=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.playerfovtoggle, buffer, 4);
		str = (std::string(_("playerfovtoggle=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.desync, buffer, 4);
		str = (std::string(_("desync=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.Movement, buffer, 4);
		str = (std::string(_("Movement=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.eyeoffset, buffer, 4);
		str = (std::string(_("eyeoffset=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->misc.PlayerEyes);
		str = (std::string(_("PlayerEyes=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.spinbot, buffer, 4);
		str = (std::string(_("spinbot=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.always_shoot, buffer, 4);
		str = (std::string(_("always_shoot=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.attack_on_mountables, buffer, 4);
		str = (std::string(_("attack_on_mountables=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.speedhack, buffer, 4);
		str = (std::string(_("speedhack=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.antispeed, buffer, 4);
		str = (std::string(_("antispeed=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.hitsound, buffer, 4);
		str = (std::string(_("hitsound=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		sprintf(buffer, _("%.2f"), vars->misc.speedhackspeed);
		str = (std::string(_("speedhackspeed=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.TakeFallDamage, buffer, 4);
		str = (std::string(_("TakeFallDamage=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.silent_farm, buffer, 4);
		str = (std::string(_("silent_farm=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.auto_lock, buffer, 4);
		str = (std::string(_("auto_lock=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.always_sprint, buffer, 4);
		str = (std::string(_("always_sprint=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.gravity, buffer, 4);
		str = (std::string(_("gravity=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.infinite_jump, buffer, 4);
		str = (std::string(_("infinite_jump=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.fake_lag, buffer, 4);
		str = (std::string(_("fake_lag=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.admin_mode, buffer, 4);
		str = (std::string(_("admin_mode=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.view_offset, buffer, 4);
		str = (std::string(_("view_offset=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.norecycler, buffer, 4);
		str = (std::string(_("norecycler=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.instant_med, buffer, 4);
		str = (std::string(_("instant_med=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.instant_revive, buffer, 4);
		str = (std::string(_("instant_revive=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.no_playercollision, buffer, 4);
		str = (std::string(_("no_playercollision=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.spiderman, buffer, 4);
		str = (std::string(_("spiderman=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.silentwalk, buffer, 4);
		str = (std::string(_("silentwalk=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.autofarm, buffer, 4);
		str = (std::string(_("autofarm=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.interactive_debug, buffer, 4);
		str = (std::string(_("interactive_debug=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		itoa(vars->misc.gesture_spam, buffer, 4);
		str = (std::string(_("gesture_spam=")) + std::string(buffer) + _("\n"));
		f.write(str.c_str(), str.size());
		f.close();
	}

	void combat() {
		if (im::BeginChild(_("Aimbot"), ImVec2(235, 200), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Aimbot"));
			im::Separator();

			im::Checkbox(_("PSilent"), &vars->combat.psilent);
			im::SliderFloat(_("Movement pred"), &vars->combat.movementpred, 0.1f, 1.0f, _("%.2f"), 1.f);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Percent of average velocity to add to position when predicting."));
			}
			im::Combo(_("Aim bone"), &vars->combat.aimbone, _("Head\0Spine 4\0Pelvis\0Right arm\0Left arm\0Right leg\0Left leg"));
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 4);
			im::Hotkey(_("P"), &vars->keybinds.psilent, ImVec2(50, 15));
			im::Checkbox(_("Memory aimbot"), &vars->combat.aimbot);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 4);
			im::Hotkey(_("A"), &vars->keybinds.aimbot, ImVec2(50, 15));
			im::SliderFloat(_("Smoothing"), &vars->combat.aimbot_smooth, .1f, .99f, _("%.2f"), 1.f);


			im::SliderFloat(_("Target fov"), &vars->combat.aimbotfov, 1.f, 900.f, _("%.0f"));
			im::SliderFloat(_("Recoil X"), &vars->combat.recoilx, 0.f, 5.f, _("%.2f"));
			im::SliderFloat(_("Recoil Y"), &vars->combat.recoily, 0.f, 5.f, _("%.2f"));

			im::Checkbox(_("No spread (quick ban)"), &vars->combat.nospread);
			im::Checkbox(_("Remove shoot restrictions"), &vars->combat.always_shoot);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Allows things like shooting while jumping"));
			}

			im::Checkbox(_("Lock target"), &vars->combat.locktarget);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Keep target within fov locked even if other targets are closer to crosshair"));
			}
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 4);
			im::Hotkey(_("L"), &vars->keybinds.locktarget, ImVec2(50, 15));
			im::Checkbox(_("Target friends"), &vars->combat.targetfriends);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Add friends with the player list in misc"));
			}
			im::EndChild();
		}
		im::SameLine();;
		if (im::BeginChild(_("Weapon"), ImVec2(235, 330), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Weapon"));
			im::Separator();
			im::Checkbox(_("Hitbox override"), &vars->combat.hitbox_override);
			im::Combo(_("Hitboxes"), &vars->combat.hitbox,
				_("Head\0Body\0Upperbody\0Penis\0Hands\0Legs\0Feet"));
			im::Checkbox(_("Randomize hitboxes"), &vars->combat.randomize);
			//im::Checkbox(_("Hitscan"), &vars->combat.HitScan);
			im::Checkbox(_("Manipulator"), &vars->combat.manipulator);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Abuses desync (0s-1s) and eye forgiveness to spawn bullets up to 10m away"));
			}
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("M"), &vars->keybinds.manipulator, ImVec2(50, 15));
			//im::Checkbox(_("Target behind wall"), &vars->combat.targetbehindwall);
			//im::Checkbox(_("STW (Ladder)"), &vars->combat.throughladder);
			//im::Checkbox(_("Pierce"), &vars->combat.pierce);
			im::Checkbox(_("Double-tap"), &vars->combat.doubletap);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Shoots up to 9 bullets at once"));
			}
			im::Checkbox(_("Bullet tp"), &vars->combat.bullet_tp);
			im::SliderFloat(_("Tp multiplier"), &vars->combat.tpmultiplier, .1f, 1.f, _("%.2f"), 1.f);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Multiplier for distance to teleport the bullet within desync time"));
			}
			//im::Checkbox(_("Lower velocities"), &vars->combat.lower_vel);
			im::Checkbox(_("Fast bullets"), &vars->combat.fast_bullet);
			im::Checkbox(_("Instant eoka"), &vars->combat.instaeoka);
			//im::Checkbox(_("Fast bow"), &vars->combat.fastbow);
			im::Checkbox(_("Heli-weakspot"), &vars->combat.weakspots);
			im::Checkbox(_("Thick bullets"), &vars->combat.thick_bullet);
			im::SliderFloat(_("Thickness"), &vars->combat.thickness, 0.1f, 2.2f, _("%.2f"), 1.f);
			im::Checkbox(_("Autoshoot"), &vars->combat.autoshoot);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Will automatically shoot players, works with manipulator"));
			}
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("A"), &vars->keybinds.autoshoot, ImVec2(50, 14));
			im::Checkbox(_("Rapid-fire"), &vars->combat.rapidfire);
			im::SliderFloat(_("Fire rate"), &vars->combat.firerate, .01f, .2f, _("%.2f"), 1.f);
			im::Checkbox(_("Automatic"), &vars->combat.automatic);
			im::Checkbox(_("Auto-reload"), &vars->combat.always_reload);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Will silently reload your weapon as soon as you stop firing"));
			}
			im::EndChild();
		}
		im::SetCursorPosY(im::GetCursorPosY() - 126);
		if (im::BeginChild(_("Melee"), ImVec2(235, 122), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Melee"));
			im::Separator();
			im::Checkbox(_("Silent melee"), &vars->combat.silent_melee);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("S"), &vars->keybinds.silentmelee, ImVec2(50, 14));
			im::SliderFloat(_("Range"), &vars->combat.melee_range, 1.f, 4.f, _("%.2f"), 1.f);
			im::EndChild();
		}

	}
	void visual() {
		if (im::BeginChild(_("Indicators"), ImVec2(235, 170), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Indicators"));
			im::Separator();
			im::Checkbox(_("Desync indicator"), &vars->visual.desync_indicator);
			im::Checkbox(_("Flyhack indicator"), &vars->visual.flyhack_indicator);
			im::Checkbox(_("Speedhack indicator"), &vars->visual.speedhack_indicator);
			//im::Checkbox(_("Offscreen indicator"), &vars->visual.offscreen_indicator);
			im::Combo(_("Snapline"), &vars->visual.snapline,
				_("None\0Top\0Center\0Bottom"));
			im::Checkbox(_("Show target fov"), &vars->visual.show_fov);
			im::Checkbox(_("Grenade prediction"), &vars->visual.grenadeprediction);
			im::Checkbox(_("Targetted indicator"), &vars->visual.targetted);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Displays indicator above crosshair to show if you are being looked at"));
			}
			im::Checkbox(_("Hitpoints"), &vars->visual.hitpoint);
			//ImGui::Checkbox(_("Target snapline"), &vars->visual.offscreen_indicator);
			im::EndChild();
		}
		im::SameLine();

		//im::SetCursorPosX(im::GetCursorPosX() + 132);
		if (im::BeginChild(_("Player ESP"), ImVec2(235, 170), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Player ESP"));
			im::Separator();
			im::Checkbox(_("Enabled"), &vars->visual.playeresp);
			im::Checkbox(_("Name"), &vars->visual.nameesp);
			im::Checkbox(_("Rainbow name"), &vars->visual.rainbowname);
			im::Checkbox(_("Distance"), &vars->visual.distance);
			im::Checkbox(_("Rainbow distance"), &vars->visual.rainbowdist);
			im::Combo(_("Health bar"), &vars->visual.hpbar,
				_("None\0Side\0Bottom"));
			im::Checkbox(_("Rainbow health bar"), &vars->visual.rainbowhpbar);
			im::Checkbox(_("Friend flag"), &vars->visual.friendflag);
			im::Checkbox(_("Wounded flag"), &vars->visual.woundedflag);
			im::Checkbox(_("Held item flag"), &vars->visual.weaponesp);
			im::Checkbox(_("Rainbow flags"), &vars->visual.rainbowflags);
			im::Checkbox(_("Hotbar"), &vars->visual.hotbar_esp);
			im::Checkbox(_("Skeleton"), &vars->visual.skeleton);
			im::Checkbox(_("Rainbow skeleton"), &vars->visual.rainbowskeleton);
			im::Combo(_("Text box"), &vars->visual.text_background_box,
				_("None\0Full\0Rounded"));
			im::Combo(_("Box type"), &vars->visual.boxtype,
				_("None\0Full\0Corner\0Cube\0Box"));
			im::Checkbox(_("Rainbow box"), &vars->visual.rainbowbox);
			//im::Checkbox(_("Custom box"), &vars->visual.custombox);
			//im::InputTextWithHint(_("##File"), _("filename"), vars->visual.boxfilename, 32);
			//im::Checkbox(_("Corner box"), &vars->visual.corner_box);
			//im::Checkbox(_("3D Cube"), &vars->visual.cube);
			im::Checkbox(_("Crosshair name"), &vars->visual.midhealth);
			im::Checkbox(_("Crosshair hp"), &vars->visual.midname);
			im::Checkbox(_("Sleeper"), &vars->visual.sleeper_esp);
			im::Checkbox(_("NPC"), &vars->visual.npc_esp);
			//im::Checkbox(_("Held item icon"), &vars->visual.spriteitem);
			im::EndChild();
		}

		im::SetCursorPosY(im::GetCursorPosY() + 5);
		if (im::BeginChild(_("Gameplay"), ImVec2(235, 151), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Gameplay"));
			im::Separator();
			im::SliderFloat(_("Player fov"), &vars->visual.playerfov, 50.f, 179.9f, _("%.1f"));
			im::Checkbox(_("Bullet tracers"), &vars->visual.tracers);
			im::Checkbox(_("Crosshair 1"), &vars->visual.crosshair1);
			im::Checkbox(_("Crosshair 2"), &vars->visual.crosshair2);
			im::Checkbox(_("Crosshair 3"), &vars->visual.crosshair3);
			im::Checkbox(_("Zoom"), &vars->visual.zoomtoggle);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("Z"), &vars->keybinds.zoom, ImVec2(50, 14));
			im::SliderFloat(_("Zoom fov"), &vars->visual.zoomfov, 1.f, 100.f, _("%.1f"));
			im::SliderFloat(_("Stars"), &vars->visual.staramount, 1.f, 1000.f, _("%.0f"));
			im::SliderFloat(_("Brightness"), &vars->visual.day, 0.0f, 10.0f, _("%.0f"));
			im::SliderFloat(_("Nightness"), &vars->visual.night, 0.0f, 10.0f, _("%.0f"));
			im::SliderFloat(_("Rayleigh"), &vars->visual.rayleigh, 0.0f, 30.0f, _("%.1f"));
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Changes color of skybox"));
			}
			//im::Checkbox(_("Follow projectiles"), &vars->visual.followprojectiles);
			im::Checkbox(_("Manipulator angles"), &vars->visual.angles);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Visualizes the positions the manipulator will check to see if it can shoot from"));
			}
			im::EndChild();
		}
		im::SetCursorPosY(im::GetCursorPosY() - 155);
		im::SetCursorPosX(im::GetCursorPosX() + 243);
		if (im::BeginChild(_("Chams"), ImVec2(235, 151), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Chams"));
			im::Separator();
			im::Checkbox(_("Rainbow chams"), &vars->visual.rainbow_chams);
			im::Combo(_("Players"), &vars->visual.shader,
				_("None\0Normal\0Seethrough\0Wireframe\0Lit\0Force field\0Distort rim\0Layered\0Galaxy"));
			if (vars->visual.shader == 8)
				im::SliderInt(_("Material"), &vars->visual.galaxymat, 1, 16);
			im::Combo(_("Hands"), &vars->visual.hand_chams,
				_("None\0Seethrough\0Normal\0Force field\0Distort rim\0Layered\0Galaxy"));
			if (vars->visual.hand_chams == 6)
				im::SliderInt(_("Hand Material"), &vars->visual.galaxymathand, 1, 16);
			im::Combo(_("Rocks"), &vars->visual.rock_chams,
				_("None\0Normal\0Layered\0Galaxy"));
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("You must pick the chams on players before anything else, known crash unless done on players first"));
			}
			if (vars->visual.rock_chams == 3)
				im::SliderInt(_("Rock Material"), &vars->visual.galaxymatrock, 1, 16);
			im::Combo(_("Buildings"), &vars->visual.block_chams,
				_("None\0Normal\0Layered\0Galaxy"));
			if (vars->visual.block_chams == 3)
				im::SliderInt(_("Block Material"), &vars->visual.galaxymatblock, 1, 16);
			im::EndChild();
		}
	}
	void esp_items() {
		if (im::BeginChild(_("Resources"), ImVec2(235, 330), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Resources"));
			im::Separator();
			im::Checkbox(_("Sulfur ore"), &vars->visual.sulfur_ore);
			im::Checkbox(_("Stone ore"), &vars->visual.stone_ore);
			im::Checkbox(_("Metal ore"), &vars->visual.metal_ore);
			im::Checkbox(_("Food"), &vars->visual.food);
			im::Checkbox(_("Barrels"), &vars->visual.barrels);
			im::Checkbox(_("Cloth"), &vars->visual.cloth);
			im::Checkbox(_("Collectibles"), &vars->visual.collectibles);
			im::SliderFloat(_("Distance"), &vars->visual.dist_on_items, 1.f, 400.f, _("%.0f"));
			im::EndChild();
		}
		im::SameLine();
		//im::SetCursorPosX(im::GetCursorPosX() + 132);
		if (im::BeginChild(_("World"), ImVec2(235, 330), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("World"));
			im::Separator();
			im::Checkbox(_("Corpses"), &vars->visual.corpses);
			im::Checkbox(_("Patrol-heli"), &vars->visual.heli_esp);
			im::Checkbox(_("Vehicles"), &vars->visual.vehicles);
			im::Checkbox(_("Ladders"), &vars->visual.ladder);
			im::Checkbox(_("Stashes"), &vars->visual.stash);
			im::Checkbox(_("Airdrops"), &vars->visual.airdrops);
			im::Checkbox(_("Hackable crates"), &vars->visual.hackable_crate_esp);
			im::Checkbox(_("Animals"), &vars->visual.animal);
			im::Checkbox(_("Traps"), &vars->visual.traps);
			im::Checkbox(_("Tool cupboard"), &vars->visual.tc_esp);
			im::Checkbox(_("Dropped items"), &vars->visual.dropped_items);
			im::EndChild();
		}
	}
	void misc() {
		if (im::BeginChild(_("Movement"), ImVec2(235, 160), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Movement"));
			im::Separator();
			im::Checkbox(_("Anti-flyhack"), &vars->misc.flyhack_stop);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Stops player before they get kicked for flyhacking"));
			}
			im::Checkbox(_("Anti-speedhack"), &vars->misc.antispeed);
			im::Checkbox(_("Anti-deathbarrier"), &vars->misc.antideathbarrier);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Automatic cooldown and multiplier that enables you to move 2x faster"));
			}
			im::Checkbox(_("Omnisprint"), &vars->misc.always_sprint);
			im::Checkbox(_("Spiderman"), &vars->misc.spiderman);
			im::Checkbox(_("Big jump"), &vars->misc.gravity);
			im::Checkbox(_("Infinite jump"), &vars->misc.infinite_jump);
			im::Checkbox(_("Fly-wall"), &vars->misc.flywall);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("F"), &vars->keybinds.flywall, ImVec2(50, 14));
			im::Checkbox(_("No collisions"), &vars->misc.no_playercollision);
			im::Combo(_("Anti-aim"), &vars->misc.antiaim,
				_("none\0 backwards\0 backwards (down)\0 backwards (up)\0 left\0 left (down)\0 left (up)\0 right\0 right (down)\0 right (up)\0 jitter\0 jitter (down)\0 jitter (up)\0 spin\0 spin (down)\0 spin (up)\0 random"));
			//im::Checkbox(_("Teleport (5m)"), &vars->misc.tp);
			//im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 4);
			//im::Hotkey(_("T"), &vars->keybinds.tp, ImVec2(50, 15));
			im::SliderFloat(_("Spin speed"), &vars->misc.spinspeed, 1.f, 50.f, _("%.0f"));
			im::EndChild();
		}
		im::SetCursorPosY(im::GetCursorPosY() + 2);
		if (im::BeginChild(_("Automation"), ImVec2(235, 164), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Automation"));
			im::Separator();
			im::Combo(_("Walk to"), &vars->misc.walkto,
				_("None\0Map Marker\0Stone ore\0Sulfur ore\0Metal ore\0Player"));
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Pathfinds to map marker, ores, players from player list in misc"));
			}
			im::Checkbox(_("Auto-farm"), &vars->misc.silent_farm);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Essentially silent farm, works well with walk to ores"));
			}
			im::Checkbox(_("Auto-attack"), &vars->misc.autoattack);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Auto switch held item to weapon/tool when walking/farming and shoot at all visible players"));
			}
			im::SliderFloat(_("Max dist"), &vars->misc.autoattackdist, 0.1f, 400.f, _("%.0f"));
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Distance player has to be to get shot"));
			}
			im::Checkbox(_("Auto-med"), &vars->misc.automed);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Auto switch held item to med/bandage based off hp"));
			}
			im::Combo(_("Targetting mode"), &vars->misc.targetting_mode, 
				_("Closest (distance)\0Lowest hp"));
			//vars->playersnamesstr.clear();
			im::EndChild();
		}
		//im::SetCursorPosX(im::GetCursorPosX() + 245);
		im::SameLine();
		im::SetCursorPosY(im::GetCursorPosY() - 166);
		if (im::BeginChild(_("Gameplay"), ImVec2(235, 330), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
		{
			im::Text(_("Gameplay"));
			im::Separator();
			im::Checkbox(_("Anti-recycler"), &vars->misc.norecycler);
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Turn off recyclers"));
			}
			im::Checkbox(_("Pickup collectibles"), &vars->misc.pickup_collectibles);
			im::Checkbox(_("Interactive debug"), &vars->misc.interactive_debug);
			im::Checkbox(_("Instant med"), &vars->misc.instant_med);
			im::Checkbox(_("Instant revive"), &vars->misc.instant_revive);
			//im::Checkbox(_("Third-person"), &vars->misc.thirdperson);
			//im::Checkbox(_("Debug camera"), &vars->misc.debugcam);
			//im::Checkbox(_("Culling"), &vars->misc.culling);
			im::Checkbox(_("Suicide"), &vars->misc.TakeFallDamage);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("S"), &vars->keybinds.suicide, ImVec2(50, 14));
			im::Checkbox(_("Longneck"), &vars->misc.eyeoffset);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("L"), &vars->keybinds.neck, ImVec2(50, 14));
			im::SliderFloat(_("Size"), &vars->misc.PlayerEyes, 1.f, 1.5f, _("%.2f"));
			im::Checkbox(_("Auto upgrade"), &vars->misc.auto_upgrade);
			im::Combo(_("Upgrade tier"), &vars->misc.upgrade_tier,
				_("Wood\0Stone\0Metal\0Armored"));
			//im::Checkbox(_("Auto refill"), &vars->misc.autorefill);
			im::Checkbox(_("Fakelag"), &vars->misc.fake_lag);
			im::Checkbox(_("Desync on key"), &vars->misc.desync);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("D"), &vars->keybinds.desync_ok, ImVec2(50, 14));
			im::Checkbox(_("Fake admin"), &vars->misc.admin_mode);
			im::Checkbox(_("Timescale"), &vars->misc.speedhack);
			im::SameLine(); im::SetCursorPosY(im::GetCursorPosY() + 2);
			im::Hotkey(_("T"), &vars->keybinds.timescale, ImVec2(50, 14));
			im::SliderFloat(_("Speed"), &vars->misc.speedhackspeed, 0.1f, 10.f, _("%.1f"));
			if (im::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
				im::SetTooltip(_("Set timescale key as hover loot key and set to 10.f for fast loot"));
			}
			im::Checkbox(_("Console logs"), &vars->misc.logs);
			im::Checkbox(_("Rainbow accent"), &vars->rainbow_accent);
			im::Combo(_("Gesture spam"), &vars->misc.gesture_spam, _(" None\x00 Clap\x00 Friendly\x00 Thumbsdown\x00 Thumbsup\x00 Ok\x00 Point\x00 Shrug\x00 Victory\x00 Wave"));
			
			//im::Checkbox(_("Skin changer"), &vars->misc.skinchanger);
			//if(vars->misc.skinchanger)
			//{
			//	SkinChanger();
			//}

			im::Checkbox(_("Player list"), &vars->misc.playerlist);
			im::Checkbox(_("Snake"), &vars->misc.snake);
			im::EndChild();
		}
	}
	void configs() {
		//do buttons on left side
		if (im::BeginChild(_("Listbox"), ImVec2(480, 330), true))
		{
			if (im::ListBoxHeader(_("Configs"), ImVec2(475, 130)))
			{
				int i = 0;
				auto s = LI_FIND(getenv)(_("APPDATA"));
				auto p = s + std::string(_("\\matrix\\configs\\"));
				for (auto& p : std::filesystem::recursive_directory_iterator(p))
					if (p.path().extension() == _(".m"))
					{
						i++;
						if (im::Selectable(p.path().stem().string().c_str(), ImGuiSelectableFlags_AllowDoubleClick))
						{
							selected_cfg = i;
							LI_FIND(strcpy)(str0, const_cast<char*>(p.path().stem().string().c_str()));
						}
					}
				im::ListBoxFooter();
			}
			if (im::Button(_("Save config"))) { save_config(str0); }
			im::SameLine();
			if (im::Button(_("Load config"))) { load_config(); }
			im::InputText(_("Config name"), str0, IM_ARRAYSIZE(str0));

			im::EndChild();
		}
		
		//im::ListBox(_("Configs"), &selected_config, configs_name_list.data(), configs_name_list.size());
	}
	void colors() {
		if (im::BeginChild(_("Players"), ImVec2(235, 330), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
			im::Text(_("Players"));
			im::Separator();
			if (im::BeginChild(_("Player"), ImVec2(217, 90), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
				im::Text(_("Player"));
				im::Separator();
				im::ColorEdit4(_("Boxes"), vars->colors.players.boxes.visible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Invisible"), vars->colors.players.boxes.invisible, ImGuiColorEditFlags_NoInputs);
				im::ColorEdit4(_("Chams visible"), vars->colors.players.chams.visible, ImGuiColorEditFlags_NoInputs);
				im::ColorEdit4(_("Chams invisible"), vars->colors.players.chams.invisible, ImGuiColorEditFlags_NoInputs);
				im::EndChild();
			}
			if (im::BeginChild(_("Details"), ImVec2(217, 200), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
				im::Text(_("Details"));
				im::Separator();
				im::ColorEdit4(_("Name"), vars->colors.players.details.name.visible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Name invisible"), vars->colors.players.details.name.invisible, ImGuiColorEditFlags_NoInputs);
				im::ColorEdit4(_("Distance"),	vars->colors.players.details.distance.visible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Distance invisible"), vars->colors.players.details.distance.invisible, ImGuiColorEditFlags_NoInputs);
				im::ColorEdit4(_("Flags"), vars->colors.players.details.flags.visible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Flags invisible"), vars->colors.players.details.flags.invisible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Skeleton"),	vars->colors.players.details.skeleton.visible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Skeleton invisible"), vars->colors.players.details.skeleton.invisible, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Background box"), vars->colors.players.details.boxbackground, ImGuiColorEditFlags_NoInputs);
				//im::ColorEdit4(_("Background outline"), vars->colors.players.details.backgroundoutline, ImGuiColorEditFlags_NoInputs);
				im::EndChild();
			}
			im::EndChild();
		}
		im::SameLine();
		if (im::BeginChild(_("UI"), ImVec2(235, 142), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
			im::Text(_("UI"));
			im::Separator();
			im::ColorEdit4(_("Snapline visible"), vars->colors.ui.snapline.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Snapline invisible"), vars->colors.ui.snapline.invisible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Fov visible"), vars->colors.ui.fov.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Fov invisible"), vars->colors.ui.fov.invisible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Hitpoints"), vars->colors.ui.hitpoints, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Menu accent"), vars->accent_color, ImGuiColorEditFlags_NoInputs);
			//im::ColorEdit4(_("Sun"), vars->colors.sun, ImGuiColorEditFlags_NoInputs);
			//im::ColorEdit4(_("Moon"), vars->colors.moon, ImGuiColorEditFlags_NoInputs);
			vars->accent_color_opaque[0] = vars->accent_color[0];
			vars->accent_color_opaque[1] = vars->accent_color[1];
			vars->accent_color_opaque[2] = vars->accent_color[2];
			im::EndChild();
		}
		im::SetCursorPos(ImVec2(im::GetCursorPosX() + 243, im::GetCursorPosY() - 184));
		if (im::BeginChild(_("ESP Items"), ImVec2(235, 180), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove)) {
			im::Text(_("ESP Items"));
			im::Separator();
			im::ColorEdit4(_("Hemp"),		vars->colors.items.hemp.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Stone"),		vars->colors.items.stone.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Sulfur"),		vars->colors.items.sulfur.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Metal"),		vars->colors.items.metal.visible, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Stash (closed)"),		vars->colors.items.stash.closed, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Dropped items"),		vars->colors.items.dropped, ImGuiColorEditFlags_NoInputs);
			im::ColorEdit4(_("Collectible"),		vars->colors.items.collectibles, ImGuiColorEditFlags_NoInputs);
			im::EndChild();
		}
	}

	void Render()
	{
		if (!init)
		{
			mysnake = sg::Snake();
			init = true;

			//load 'default' config by default
			strcpy(str0, _("default"));
			load_config();
		}

#ifdef DOTS
		im::SetNextWindowPos({ 0,0 });
		im::SetNextWindowSize(im::GetIO().DisplaySize);
		im::PushStyleColor(ImGuiCol_WindowBg, { 0,0,0,0 });
		im::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		im::Begin("##DOTSDOTS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove);
		{
			dotdraw();
		}
		im::End();
		im::PopStyleColor();
		im::PopStyleVar();
#endif
		if (vars->rainbow_accent)
		{
			static int cases = 0;
			switch (cases) {
			case 0: { _r -= 0.003f; if (_r <= 0) cases += 1; break; }
			case 1: { _g += 0.003f; _b -= 0.003f; if (_g >= 1) cases += 1; break; }
			case 2: { _r += 0.003f; if (_r >= 1) cases += 1; break; }
			case 3: { _b += 0.003f; _g -= 0.003f; if (_b >= 1) cases = 0; break; }
			default: { _r = 1.00f; _g = 0.00f; _b = 1.00f; break; }
			}
			vars->accent_color[0] = _r;
			vars->accent_color[1] = _g;
			vars->accent_color[2] = _b;
			vars->accent_color_opaque[0] = _r;
			vars->accent_color_opaque[1] = _g;
			vars->accent_color_opaque[2] = _b;
		}

		im::PushStyleColor(ImGuiCol_CheckMark, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
		im::PushStyleColor(ImGuiCol_SliderGrab, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
		im::PushStyleColor(ImGuiCol_SeparatorActive, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
		im::PushStyleColor(ImGuiCol_ResizeGripActive, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
		im::PushStyleColor(ImGuiCol_TextSelectedBg, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
		im::PushStyleColor(ImGuiCol_Separator, { 0.43f, 0.43f, 0.50f, 0.65f });

		//im::SetNextWindowPos(ImVec2(x + 170, y + 60));
		//im::SetNextWindowSize(ImVec2(520, plus2));
		im::Begin(GUI_NAME, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

			im::Separator();
			if (im::Button(_("Combat"), ImVec2(73, 20))) { selected_tab = 0; }  im::SameLine();
			if (im::Button(_("Visuals"), ImVec2(73, 20))) { selected_tab = 1; }  im::SameLine();
			if (im::Button(_("ESP Items"), ImVec2(73, 20))) { selected_tab = 2; }  im::SameLine();
			if (im::Button(_("Misc"), ImVec2(73, 20))) { selected_tab = 3; }  im::SameLine();
			if (im::Button(_("Configs"), ImVec2(73, 20))) { selected_tab = 4; }  im::SameLine();
			if (im::Button(_("Colors"), ImVec2(73, 20))) { selected_tab = 5; }
			im::Separator();
			im::PushStyleColor(ImGuiCol_Separator, { vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], vars->accent_color[3] });
			switch (selected_tab)
			{
			case 0:
				combat();
				break;
			case 1:
				visual();
				break;
			case 2:
				esp_items();
				break;
			case 3:
				misc();
				break;
			case 4:
				configs();
				break;
			case 5:
				colors();
				break;
			}
		}
		im::End();

		if (vars->misc.playerlist)
			PlayersListWindow();
		if (vars->misc.snake)
			SnakeStuff();
	}
}
