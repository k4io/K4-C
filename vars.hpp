#pragma once
#include "utils/xorstr.hpp"
#include <string>
#include <vector>
#define GUI_NAME "Matrix"
#define CUSTOM_ICONS FALSE

#include <map>

struct gplayer {
	std::wstring name;
	ULONG userid;
	bool is_friend, priority_target, follow, block;
	gplayer(std::wstring n, ULONG i, bool f, bool p, bool f2, bool b) : 
		name(n), userid(i), is_friend(f), priority_target(p), follow(f2), block(b) { }
};

struct Vars
{
	std::string data_dir = _("");
	std::string customboxpath = _("");
	std::vector<std::wstring> player_name_list{};
	
	std::map<std::string, int> weaponskin_map;
	std::map<int, std::string> allskins_map;
	std::map<ULONG, gplayer*> gui_player_map{};
	std::map<ULONG, int> chams_player_map{};
	std::map<ULONG, int> handchams_player_map{};



	int follow_player_id = -1;

	bool open = false;
	float accent_color[4] = { 28 / 255.f, 232 / 255.f, 89 / 255.f, 1.f };
	float accent_color_opaque[4] = { 28 / 255.f, 232 / 255.f, 89 / 255.f, 0.6f };
	bool wants_shoot = false;
	bool rainbow_accent = false;
	bool targetted = false;

	float last_refill_time = 0.f;

	struct combat {
		bool aimbot = false;
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
		bool fast_bullet = false;
		bool instaeoka = false;
		bool pierce = false;
		bool fastbow = false;
		bool throughladder = false;
		bool weakspots = false;
		bool targetbehindwall = false;
		float recoilx = 1.f;
		float recoily = 1.f;

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
		bool crosshair1 = false;
		bool crosshair2 = false;
		bool crosshair3 = false;
		int text_background_box = 0;
		float day = 1.f;
		float night = 1.f;
		float dist_on_items = 100.f;
		float playerfov = 90.f;
		float rayleigh = 1.f;
		bool zoomtoggle = false;
		float staramount = 350;
		bool always_day = true;
		float zoomfov = 10.f;
		bool berry = false;
		bool offscreen_indicator = false;
		bool food = false;
		bool animal = false;
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
		bool rainbowskeleton = false;
		bool rainbowflags = false;
		bool rainbowdist = false;
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
		bool friendflag = false;
		bool desync_indicator = false;
		bool speedhack_indicator = false;
		bool hitpoint = false;
		bool flyhack_indicator = false;
		bool tracers = false;

		int shader = 0;
	}; visual visual;

	struct misc {
		bool antideathbarrier = false;
		bool auto_upgrade = false;
		bool emulate_p = false;
		bool flywall = false;
		bool force_privlidge = false;
		int upgrade_tier = 2;
		bool flyhack_stop = false;
		bool tp = false;
		bool antispeed = false;
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

		int gesture_spam = 0;
	}; misc misc;

	struct keybinds {
		int psilent = 0;
		int aimbot = 0;
		int flywall = 0;
		int silentmelee = 0;
		int autoshoot = 0;
		int manipulator = 0;
		int desync_ok = 0;
		int timescale = 0;
		int silentwalk = 0;
		int suicide = 0;
		int neck = 0;
		int zoom = 0;
		int bullettp = 0;
		int fakelag = 0;
		int tp = 0;
		int locktarget = 0;
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
			struct animal {
				float visible[4] = { 1, 1, 1, 1 };
				float invisible[4] = { 1, 1, 1, 1 };
			}; animal animal;
			float collectibles[4] = { 1, 1, 1, 1 };
		}; items items;

		float sun[4] = { 1, 1, 1, 1 };
		float moon[4] = { 1, 1, 1, 1 };
	}; colors colors;

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