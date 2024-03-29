#pragma once
#include "utils/vector.hpp"
#include "utils/xorstr.hpp"
//#include "../sol/sol.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include "rust/features/shplayerdata.h"

#define GUI_NAME "Matrix"
#define CUSTOM_ICONS FALSE

#include <map>

class gplayer {
private:
	static bool CheckFriend(int uid) {
		auto s = getenv(_("APPDATA"));
		auto p = s + std::string(_("\\matrix\\friends.lst"));

		if (!std::filesystem::exists(p)) {
			CloseHandle(CreateFileA(p.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));
			return false;
		}
		std::ifstream cFile(p, std::ios::in);
		std::string line;
		bool exists = false;
		while (getline(cFile, line))
			if (line.find(std::to_string(uid)) != std::string::npos)
				exists = true;
		cFile.close();
		return exists;
	}
public:
	std::wstring name;
	ULONG userid;
	bool is_friend, priority_target, follow, block;
	gplayer(std::wstring n, ULONG i, bool f, bool p, bool f2, bool b) : 
		name(n), userid(i), is_friend(CheckFriend(i)), priority_target(p), follow(f2), block(b) { }
};

struct gskin {
public:
	std::wstring DisplayName;
	int SkinId;
};

struct guiskin {
public:
	std::wstring ShortName = _(L"");
	std::wstring ItemName = _(L"");
	std::vector<gskin> skins = {};
};

class RenderFlag {
public:
	Vector2 pos;
	std::wstring Name;
	float* Color;
	RenderFlag(Vector2 position, std::wstring name, float* Col)
		: pos(position), Name(name), Color(Col) { }
};
class RenderObject {
public:
	enum RenderType {
		Entity,
		Hotbar,
		TC
	};
	RenderType type;
	bool HasBeenDrawn = false;
};
class RLine {
public:
	Vector2 Start, End;
	float *Color;
	float Thickness;
	RLine(Vector2 s, Vector2 e, float* c, float t = 1.5f) : Start(s), End(e), Color(c), Thickness(t) { }
};
class RHealthBar {
public:
	Vector2 Start, Sz, BarStart, BarSz;
	float *Color;
	RHealthBar() { }
};
class RenderEntity : public RenderObject {
public:
	Vector2 NamePos, DistPos;
	float *LineColor, *NameColor, *DistColor;
	float Dist;
	bool IsPlayer = false;
	std::wstring Name;
	RHealthBar* Hp;
	std::vector<RLine*> BoxLines{};
	std::vector<RLine*> Skeleton{};
	std::vector<RenderFlag*> Flags{};
	RenderEntity() { type = RenderType::Entity; }
	//RenderEntity(Vector2 namePos, Vector2 distPos, float* LineCol, float* NameCol, float* DistCol, float dist, std::wstring name, std::vector<RenderFlag> Flag, std::vector<RLine> lines, std::vector<RLine> Skel, RHealthBar hpbar)
	//	: NamePos(namePos), DistPos(distPos), LineColor(LineCol), NameColor(NameCol), DistColor(DistCol), Dist(dist), Name(name), Flags(Flag), BoxLines(lines), Skeleton(Skel), Hp(hpbar) {
	//	type = RenderType::Entity;
	//}
};
class RenderHotbar : public RenderObject {
public:
	std::string Name;
	std::vector<std::string> Items;
	RenderHotbar(std::string n, std::vector<std::string> i)
		: Name(n), Items(i) {
		type = RenderType::Hotbar;
	}
};
class RenderToolCupboard : public RenderObject {
public:
	Vector2 Start;
	std::wstring Name = _(L"Tool cupboard");
	std::vector<std::wstring> AuthNames;
	RenderToolCupboard(Vector2 pos, std::vector<std::wstring> n) 
		: Start(pos), AuthNames(n) {
		type = RenderType::TC;
	}
};

struct _item {
	wchar_t* name;
	int count;
};

enum FlyMode {
	None = 0,
	Hover = 1,
	ToTarget = 2
};

class BasePlayer;
class BaseCombatEntity;

class aim_target {
public:
	Vector3 pos;

	BaseCombatEntity* ent = 0;

	float distance = 5000;
	float fov = 1;
	float maxfov = 0;

	int network_id;

	bool is_heli = false;
	bool visible = false;
	bool sleeping = false;
	bool teammate = false;
	bool found = false;

	/*Velocity related shit*/
	Vector3 avg_vel = Vector3(0, 0, 0);
	std::vector<Vector3> velocity_list = {};
	float last_frame = 0.f; // overwrite every fixedtime + deltatime

	bool operator<(const aim_target& b) {
		if (fov == maxfov) {
			return distance < b.distance;
		}
		else {
			return fov < b.fov;
		}
	}
	void Reset() {
		visible = false;
		sleeping = false;
		is_heli = false;
		teammate = false;
		found = false;
		avg_vel = {};
		velocity_list.clear();
		last_frame = 0.f;
		network_id = -1;
		distance = 5000;
		fov = 0;
		ent = 0;
		pos = {};
	}
};


struct Vars
{
	std::string data_dir = _("");
	std::string customboxpath = _("");
	std::string target_name = _("");
	std::vector<std::wstring> player_name_list{};
	std::vector<_item*> target_hotbar_list{};

	std::map<std::wstring, guiskin> gui_skin_map{};

	std::map<ULONG, gplayer*> gui_player_map{};
	std::map<ULONG, int> chams_player_map{};
	std::map<ULONG, int> handchams_player_map{};
	std::map<std::wstring, gskin> applied_skins_map{};
	std::map<std::string, bool*> loaded_lua_list{};
	std::map<ULONG, shPlayerData*> shared_player_map{};

	std::vector<RenderObject*> RenderList{};
	IDXGISwapChain* pSwapChain;
	aim_target best_target;
	shPlayerData* currentPlayerData;

	int follow_player_id = -1;

	unsigned int selected_entity_id;
	float time_last_upgrade = 0.f;
	float rl_time = 0.f;
	uintptr_t client_entities;
	BasePlayer* local_player;
	VMatrix matrix;
	//aim_target best_target = aim_target();
	uintptr_t closest_building_block = 0;

	bool open = false;
	float rainbow[4] = { 0, 0, 0, 1 };
	float bl[4] = { 0, 0, 0, 1 };
	float accent_color[4] = { 28 / 255.f, 232 / 255.f, 89 / 255.f, 1.f };
	float accent_color_opaque[4] = { 28 / 255.f, 232 / 255.f, 89 / 255.f, 0.6f };
	bool wants_shoot = false;
	bool rainbow_accent = false;
	float rainbow_speed = 1.f;
	bool targetted = false;
	bool grenadeclose = false;

	float last_refill_time = 0.f;

	struct combat {
		bool aimbot = false;
		bool vischeck = false;
		float firerate = 0.133f;
		float tpmultiplier = 0.75f;
		bool psilent = false;
		bool targetfriends = false;
		int aimbone = 0;
		bool bodyaim = false;
		float movementpred = 0.8f;
		float aimbotfov = 500.f;
		float aimbot_smooth = 0.5f;
		bool hitbox_override = false;
		bool randomize = false;
		int hitbox = 0;
		bool silent_melee = false;
		bool manipulator = false;
		bool manipulator2 = false;
		bool doubletap = false;
		bool always_reload = false;
		bool autoshoot = false;
		bool bullet_tp = false;
		bool lower_vel = false;
		bool rapidfire = false;
		bool automatic = false;
		bool thick_bullet = false;
		bool always_shoot = false;
		bool locktarget = false;
		float melee_range = 1.f;
		float thickness = 1.f;
		bool HitScan = false;
		bool norecoil = false;
		bool nospread = false;
		bool fast_bullets = false;
		float fast_bullet = 1.f;
		bool bestvelocity = false;
		bool instaeoka = false;
		bool pierce = false;
		bool fastbow = false;
		bool throughwall = false;
		bool psilentmelee = false;
		bool weakspots = false;
		bool targetbehindwall = false;
		float recoilx = 1.f;
		float recoily = 1.f;
		float spread = .5f;

		bool shoot_at_fatbullet = false;
		
		struct hitboxes {
			bool Head;
			bool Body;
			bool Upperbody;
			bool Penis;
			bool Hands;
			bool Legs;
			bool Feet;
		}; hitboxes hitboxes;
	}; combat combat;

	struct visual {
		bool playeresp = false;
		bool grenadeprediction = false;
		bool rocketprediction = false;
		bool crosshair1 = false;
		bool crosshair2 = false;
		bool crosshair3 = false;
		bool customtime = false;
		int text_background_box = 0;
		float ambient = 1.f;
		float ambcol[4] = { 1, 1, 1, 1 };
		float time = 12.f;
		float day = 1.f;
		float night = 1.f;
		float dist_on_items = 100.f;
		float playerfov = 90.f;
		float rayleigh = 1.f;
		bool zoom = false;
		float staramount = 350;
		bool always_day = true;
		float zoomfov = 10.f;
		bool berry = false;
		bool offscreen_indicator = false;
		bool food = false;
		bool animal = false;
		bool crates = false;
		bool corpses = false;
		bool draw_fov = false;
		bool rainbow_chams = false;
		bool hotbar_esp = false;
		int hand_chams = 0;
		int rock_chams = 0;
		int block_chams = 0;
		int ladder_chams = 0;
		int boxtype = 0;
		bool boxesp = false;
		bool custombox = false;
		char* boxfilename = _("");
		bool spriteitem = false;
		bool snaplines = true;
		bool show_fov = false;
		bool showpred = false;
		bool radar = false;
		bool targetted = false;
		bool misc_esp = true;
		bool sleeper_esp = false;
		bool heli_esp = false;
		bool npc_esp = false;
		bool dropped_items = false;
		float VisRcolor = 1;
		float VisGcolor = 0.6;
		float VisBcolor = 0;
		float InvRcolor = 249.f / 255.f;
		float InvGcolor = 130.f / 255.f;
		float InvBcolor = 109.f / 255.f;
		float TeamRcolor = 249.f / 255.f;
		float TeamGcolor = 130.f / 255.f;
		float TeamBcolor = 109.f / 255.f;
		float nameRcolor = 249.f / 255.f;
		float nameGcolor = 130.f / 255.f;
		float nameBcolor = 109.f / 255.f;
		bool stash = false;
		bool collectibles = false;
		bool ladder = false;
		bool sulfur_ore = false;
		bool stone_ore = false;
		bool metal_ore = false;
		bool Item = false;
		bool midhealth = false;
		bool midname = false;
		bool traps = false;
		bool vehicles = false;
		bool airdrops = false;
		bool cloth = false;
		bool barrels = false;
		bool tc_esp = false;
		bool angles = false;
		bool followprojectiles = false;
		bool hackable_crate_esp = false;
		bool full_box = false;
		bool corner_box = false;
		bool cube = false;
		bool box3d = false;
		bool bottomhealth = false;
		bool rainbowname = false;
		bool rainbowbox = false;
		bool rainbowhpbar = false;
		bool drawfriends = true;
		bool rainbowskeleton = false;
		bool rainbowflags = false;
		bool rainbowdist = false;
		bool movementline = false;
		bool showcapsule = false;
		int hpbar = 0;
		int snapline = 0;
		bool sidehealth = false;
		bool skeleton = false;
		bool nameesp = false;
		int galaxymat = 15;
		int galaxymathand = 15;
		int galaxymatrock = 15;
		int galaxymatblock = 15;
		bool woundedflag = false;
		bool distance = false;
		bool weaponesp = false;
		bool turretradius = false;
		bool friendflag = false;
		bool targettedflag = false;
		bool desync_indicator = false;
		bool speedhack_indicator = false;
		bool hitpoint = false;
		bool flyhack_indicator = false;
		bool tracers = false;
		bool visthick = false;
		bool radarname = false;
		float visthickness = 1.f;
		float radarsize = 150.f;
		float radarrange = 150.f;

		int radarx = 1670;
		int radary = 80;

		int shader = 0;
	}; visual visual;

	struct misc {
		bool tooltips = true;
		bool automini = false;
		struct minicfg {
			float speed = 1.f;
			Vector3 target = Vector3::Zero();
			int mode = 0;
			float maxheight = 150;
			bool visualize = false;
		}; minicfg minicfg;
		bool antideathbarrier = false;
		bool weaponspam = false;
		bool auto_upgrade = false;
		bool emulate_p = false;
		bool flywall = false;
		bool force_privlidge = false;
		int upgrade_tier = 2;
		bool flyhack_stop = false;
		bool tp = false;
		bool revivefriendsonly = false;
		bool openstash = false;
		bool antispeed = false;
		bool freezeondesync = false;
		bool autofarmbarrel = false;
		float    m_idebugcam_speed = 1.f;
		float code_lock_code = 1000;
		bool playerfovtoggle = false;
		float playerfov = 90;
		bool desync = false;
		bool Movement = false;
		bool eyeoffset = false;
		float PlayerEyes = 1.4;
		bool panic = false;
		bool spinbot = false;
		bool always_shoot = false;
		bool attack_on_mountables = false;
		bool speedhack = false;
		bool hitsound = false;
		float speedhackspeed = 2.5f;

		float tpcamdist = 5.f;
		float tpcamfov = 70.f;

		bool pickup_collectibles = false;
		bool autoknock = false;
		float spinspeed = 50.f;
		bool TakeFallDamage = false;
		bool silent_farm = false;
		bool auto_lock = false;
		bool always_sprint = false;
		bool debugcam = false;
		bool thirdperson = false;
		bool culling = false;
		int antiaim = 0;
		bool gravity = true;
		bool infinite_jump = false;
		bool fake_lag = false;
		bool admin_mode = false;
		bool view_offset = false;
		bool norecycler = false;
		bool instant_med = false;
		bool instant_revive = false;
		bool fastloot = false;
		bool skinchanger = false;
		bool hammerspam = false;
		bool no_playercollision = false;
		char* playerselected = _("~");
		//int playerselected = 0;
		bool spiderman = false;
		bool silentwalk = false;
		bool walktomarker = false;
		int walkto = 0;
		bool autofarm = false;
		bool autoattack = false;
		bool automed = false;
		float autoattackdist = 0.f;
		int targetting_mode = 0;
		bool autorefill = false;
		bool logs = false;
		bool snake = false;
		bool playerlist = false;
		bool interactive_debug = false;
		bool wireplay = false;

		float capsuleHeight = 0.5f;
		float capsuleCenter = 0.5f;
		float capsuleHeightDucked = 0.5f;
		float capsuleCenterDucked = 0.5f;
		float capsuleradius = 0.5f;
		char playernamesearch[32];

		int gesture_spam = 0;
	}; misc misc;

	struct keybinds {
		int psilent = 0;
		int aimbot = 0;
		int flywall = 0;
		int silentmelee = 0;
		int autoshoot = 0;
		int manipulator = 0;
		int manipulator2 = 0;
		int desync_ok = 0;
		int timescale = 0;
		int silentwalk = 0;
		int wireplay = 0;
		int suicide = 0;
		int weaponspam = 0;
		int neck = 0;
		int zoom = 0;
		int bullettp = 0;
		int fakelag = 0;
		int tp = 0;
		int locktarget = 0;
		int hammerspam = 0;
		int stash = 0;
		int capsule = 0;
	}; keybinds keybinds;

	struct colors {
		struct players {
			struct boxes {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; boxes boxes;
			struct chams {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; chams chams;

			struct details {
				float boxbackground[4] = { 1, 1, 1, 1 };
				float backgroundoutline[4] = { 1, 1, 1, 1 };
				struct name {
					float visible[4] = { 1, 1, 1, 1 };
					float invisible[4] = { 1, 1, 1, 1 };
				}; name name;
				struct distance {
					float visible[4] = { 1, 1, 1, 1 };
					float invisible[4] = { 1, 1, 1, 1 };
				}; distance distance;
				struct flags {
					float visible[4] = { 1, 1, 1, 1 };
					float invisible[4] = { 1, 1, 1, 1 };
				}; flags flags;
				struct skeleton {
					float visible[4] = { 1, 1, 1, 1 };
					float invisible[4] = { 1, 1, 1, 1 };
				}; skeleton skeleton;
			}; details details;
		}; players players;

		struct ui {
			struct snapline {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; snapline snapline;
			struct fov {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; fov fov;
			float crosshair[4] = { 1, 1, 1, 1 };
			float hitpoints[4] = { 1, 1, 1, 1 };
		}; ui ui;

		struct items {
			float dropped[4] = { 1, 1, 1, 1 };
			struct hemp {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; hemp hemp;
			struct stone {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; stone stone;
			struct sulfur {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; sulfur sulfur;
			struct metal {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; metal metal;
			struct stash {
				float open[4] = { 1, 1, 1, 1 };
				float closed[4] = { 1, 1, 1, 1 };
			}; stash stash;
			struct barrel {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; barrel barrel;
			struct vehicle {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; vehicle vehicle;
			float collectibles[4] = { 1, 1, 1, 1 };
			float crate[4] = { 1, 1, 1, 1 };
			float animal[4] = { 1, 1, 1, 1 };
		}; items items;

		float sun[4] = { 1, 1, 1, 1 };
		float moon[4] = { 1, 1, 1, 1 };
	}; colors colors;

	struct menu {
		float TabRounding = 0.f;
		float GrabRounding = 0.f;
		float ScrollbarRounding = 0.f;
		float FrameRounding = 0.f;
		float PopupRounding = 0.f;
		float ChildRounding = 0.f;
		float WindowRounding = 0.f;
	}; menu menu;

	int tab = 0;

	std::wstring config_name = L"default";


	float last_teleport_time = 0;
	float ScreenX = 1920;
	float ScreenY = 1080;
	float tick_time_when_called;

	float desyncTime;
	float speedhack;
	float reload;
	float time_since_last_shot;
};
extern Vars* vars;