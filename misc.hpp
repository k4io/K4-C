#pragma once
#include "memory/il2cpp.hpp"
#include "settings.hpp"
#include <math.h>
#include "esp.hpp"
#include "spherearray.h"

#include <Windows.h>
#include <chrono>

//#include "d3d.hpp"

#include "settings.hpp"
#include "rust/rust.hpp"
#include "rust/classes.hpp"
#include "projectile.hpp"

#include <algorithm>

//#include "projectile.hpp"
//#include <vector>

float vprojectile_desync1 = 0.55f;

int projectileProtection1 = 6;

UINT64 g_UpdateReusable = NULL;

struct TraceResult1 {
public:
	bool didHit;
	bool silentCat;
	BaseEntity* hitEntity;
	Vector3 hitPosition;
	Vector3 outVelocity;
	float hitTime;
	float hitDist;
	bool canHit;
	bool HasLOS;
};

enum class MessageType : BYTE
{
	Welcome = 1,
	Auth = 2,
	Approved = 3,
	Ready = 4,
	Entities = 5,
	EntityDestroy = 6,
	GroupChange = 7,
	GroupDestroy = 8,
	RPCMessage = 9,
	EntityPosition = 10,
	ConsoleMessage = 11,
	ConsoleCommand = 12,
	Effect = 13,
	DisconnectReason = 14,
	Tick = 15,
	Message = 16,
	RequestUserInformation = 17,
	GiveUserInformation = 18,
	GroupEnter = 19,
	GroupLeave = 20,
	VoiceData = 21,
	EAC = 22,
	EntityFlags = 23,
	World = 24,
	ConsoleReplicatedVars = 25,
};

float GetMountedVelocity(BasePlayer* target)
{
	if (!target)
		return 0.f;

	BaseMountable* mounted = (BaseMountable*)get_mounted((uintptr_t)target);

	if (!mounted)
		return 0.f;


	auto realM = get_parent_entity((uintptr_t)mounted);

	if (!realM)
		return 0.f;

	//bool Ten = !strcmp(name, ("minicopter.entity")) || !strcmp(name, ("scraptransporthelicopter"));
	//bool Six = std::string(name).find("car") != std::string::npos || !strcmp(name, ("testridablehorse")) || !strcmp(name, ("rowboat")) || !strcmp(name, ("rhib"));
	const wchar_t* name = get_short_prefab_name(realM).str;

	if (!wcscmp(name, L"minicopter.entity") || !wcscmp(name, L"scraptransporthelicopter"))  return 50.f;
	else if (!wcscmp(name, L"rowboat") || !wcscmp(name, L"rhib")) return 25.f;
	else if (std::wstring(name).find(L"car") != std::wstring::npos) {

		//static auto GetMaxForwardSpeed = *reinterpret_cast<float(**)(BaseEntity*)>(Il2CppWrapper::GetClassFromName(_(""), _("ModularCar"))->GetMethodFromName(_("GetMaxForwardSpeed")));
		float speed = get_max_fwd_speed(realM) * 1.3f;
		return max(speed, 30.f);
	}
	else if (!wcscmp(name, L"testridablehorse")) {
		//static unsigned long long offset = Il2CppWrapper::GetClassFromName("", "BaseRidableAnimal")->GetFieldFromName("maxSpeed")->offset - 0;
		return *reinterpret_cast<float*>(realM + 0x684) * 1.5f; //public float maxSpeed; // 0x684
	}

	return 0.f;
}

struct TimeAverageValueData
{
public:
	int Calculate()
	{
		float realtimeSinceStartup = unity::get_realtimesincestartup();//UnityEngine::Time::get_realtimeSinceStartup();
		float num = realtimeSinceStartup - refreshTime;
		if (num >= 1.0)
		{
			counterPrev = (int)(counterNext / num + 0.5);
			counterNext = 0;
			refreshTime = realtimeSinceStartup;
			num = 0;
		}
		return (int)(counterPrev * (1.0 - num)) + counterNext;
	}

	void Increment()
	{
		this->Calculate();
		counterNext += 1;
	}

	void Reset()
	{
		counterPrev = 0;
		counterNext = 0;
	}

	float refreshTime;

	int counterPrev;

	int counterNext;
};

TimeAverageValueData Total_Counter = { 0, 0, 0 };
TimeAverageValueData RPC_Counter = { 0, 0, 0 };
TimeAverageValueData Signal_Counter = { 0, 0, 0 };

float Clamp(float value, float min, float max)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
	return value;
}

float Dot(const Vector3& Vec1, const Vector3& Vec2)
{
	return Vec1.x * Vec2.x + Vec1.y * Vec2.y + Vec1.z * Vec2.z;
}

class Line1111 {
public:
	Vector3 start;
	Vector3 end;
	Line1111(Vector3 s, Vector3 e) {
		start = s; end = e;
	}
	Line1111() { }
	Vector3 ClosestPoint(Vector3 pos)
	{
		Vector3 a = end - start;
		float magnitude = a.Length();
		if (magnitude == 0.f) return start;
		Vector3 vector = a / magnitude;
		return start + vector * Clamp(Dot(pos - start, vector), 0.f, magnitude);
	}
};

bool PLOS(Vector3 a, Vector3 b, int layerMask = 10551296) {
	return unity::is_visible(a, b, (uintptr_t)esp::local_player);
}

struct projectileshoot_projectile {
	bool ShouldPool; // 0x10
	bool _disposed; // 0x11
	int projectileID; // 0x14
	Vector3 startPos; // 0x18
	Vector3 startVel; // 0x24
	int seed; // 0x30
};

struct projectileshoot {
	bool ShouldPool; // 0x10
	bool _disposed; // 0x11
	int ammoType; // 0x14
	//std::array<projectileshoot_projectile, 8> projectiles; //0x18
	//projectileshoot_projectile projectiles[];
	//std::vector<projectileshoot_projectile> projectiles;
	System::list<projectileshoot_projectile*>* projectiles; //0x18
};

struct fired_projectile {
	Projectile* original;
	Projectile* fake;
	float fired_at;
	int updates;
	aim_target ent;
};

struct NodeTarget {
	Vector3 pos;
	int steps;
	std::vector<Vector3> path;
	BaseEntity* ent;
};

namespace misc
{
	enum antihacktype {
		None,
		NoClip,
		SpeedHack,
		FlyHack,
		ProjectileHack,
		MeleeHack,
		EyeHack,
		AttackHack,
		ReloadHack,
		CooldownHack,
		InsideTerrain
	};

	namespace protections {
		int flyhack_protection = 3;
		int speedhack_protection = 2;
		int eye_protection = 4;
		int projectile_protection = 6;

		float speedhack_slopespeed = 10.0f;
		float speedhack_forgiveness_inertia = 10.0f;
		float speedhack_forgiveness = 2.0f;
		float speedhack_penalty = 0.0f;
		bool speedhack_reject = true;

		float flyhack_penalty = 100.0f;
		bool flyhack_reject = true;

		float eyehack_penalty = 100.0f;
		float eye_penalty = 0.0f;

		int debuglevel = 4;
	}

	Projectile* lastFiredProjectile = nullptr;

	TickInterpolator ticks;
	TimeAverageValueData ticksPerSecond = {};
	TickHistory tickHistory;
	antihacktype lastViolationType = antihacktype::None;
	std::vector<Vector3> eye_history = {};
	projectileshoot emulated_shot;
	NodeTarget node;

	bool automation_wants_shoot = false;
	bool started_reload = false;

	float speedhackCooldownEnd = 0.f;
	float lastSpeedhackReset = 0.f;

	float speedhackDistance = 0.f;
	float speedhackPauseTime = 0.f;
	float flyhackDistanceVertical = 0.f;
	float flyhackDistanceHorizontal = 0.f;
	float flyhackPauseTime = 0.f;
	float desyncTimeRaw = 0.f;
	float desyncTimeClamped = 0.f;
	float tickDeltaTime = 0.f;
	float lastViolationTime = 0.f;
	float violationLevel = 0.f;
	float time_since_last_shot = 0.0f;
	float fixed_time_last_shot = 0.0f;
	float last_teleport_time = 0.f; //cooldown 10s?
	float last_reload_time = 0.f;

	bool hasNavigator = false;

	bool isInAir = false;
	bool manual = false;
	bool autoshot = false;
	bool manipulate_vis = false;
	bool emulated = false;
	bool just_shot = false;
	bool did_reload = false;

	Vector3 cLastTickPos{};
	Vector3 cLastTickEyePos{};
	Vector3 best_lean{};
	Vector3 best_target{};

	std::array<fired_projectile, 32> fired_projectiles;
	// ADD PROJECTILES TO THIS LIST WHEN FIRED FROM RPC, MAYBE STRUCT THAT CONTAINS SHOOT TIME AND AMOUNT OF UPDATES ALREADY?
	// LOOP LIST IN CLIENTINPUT, ONLY CALL UPDATE WHENEVER TIME MORE THAN 0.03125f FROM PREVIOUS?
	// MIGHT NOT EVEN WORK, MIGHT BE TOO FAST, MAYBE WE JUST CALL IT ONCE PER 0.03125f?
	//

	double fabs(double x)
	{
		union { double f; uint64_t i; } u = { x };
		u.i &= -1ULL / 2;
		return u.f;
	}

	void AddViolation(BasePlayer* ply, 
		antihacktype type, 
		float amount) {
		//if (Interface.CallHook("OnPlayerViolation", ply, type, amount) != null)
		//
		// this code would call the hooks from plugins etc

		lastViolationType = type;
		lastViolationTime = unity::get_realtimesincestartup();
		violationLevel += amount;
		if ((protections::debuglevel >= 2 && amount > 0.f)
			|| (protections::debuglevel >= 3 && type != antihacktype::NoClip)
			|| protections::debuglevel >= 4)
		{
			if (!vars->misc.logs) return;
			ply->console_echo(string::wformat(
				_(L"[matrix] Anti-AntiHack - Added violation [%s] of %d in frame %d (now has %d)"),
				(type == antihacktype::AttackHack ? _(L"AttackHack") :
					type == antihacktype::CooldownHack ? _(L"CooldownHack") :
					type == antihacktype::EyeHack ? _(L"EyeHack") :
					type == antihacktype::FlyHack ? _(L"FlyHack") :
					type == antihacktype::InsideTerrain ? _(L"InsideTerrain") :
					type == antihacktype::MeleeHack ? _(L"MeleeHack") :
					type == antihacktype::NoClip ? _(L"NoClip") :
					type == antihacktype::ProjectileHack ? _(L"ProjectileHack") :
					type == antihacktype::ReloadHack ? _(L"ReloadHack") :
					type == antihacktype::SpeedHack ? _(L"SpeedHack") : _(L"none")),
				(int)amount,
				(int)get_frameCount(),
				(int)violationLevel));
		}
	}

	void FadeViolations(BasePlayer* ply, 
		float deltaTime) {
		if (unity::get_realtimesincestartup() - lastViolationTime > 10.0f)
		{
			violationLevel = max(0.0f, violationLevel - 0.1f * deltaTime);
		}
	}

	bool TestInsideTerrain(Vector3 pos) {
		uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oTerrainMeta_TypeInfo); //52690304 alkad
		uintptr_t fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);


		auto terrain = *reinterpret_cast<Terrain**>(fieldz + 0x8);
		if (!terrain) return false;
		auto collision = *reinterpret_cast<TerrainCollision**>(fieldz + 0x70);
		if (!collision) return false;
		auto heightmap = *reinterpret_cast<TerrainHeightMap**>(fieldz + 0xB0);
		if (!heightmap) return false;
		auto position = *reinterpret_cast<Vector3*>(fieldz + 0x18);
		if (position.is_empty()) return false;

		float height = heightmap->GetHeight(pos);
		
		if (pos.y > height - .3f)
			return false;
		float f = position.x + terrain->SampleHeight(pos);
		return pos.y <= f - .3f && collision->GetIgnore(pos, .01f);
	}

	bool IsInsideTerrain(BasePlayer* ply, bool prevent = false) {
		bool result = TestInsideTerrain(ply->transform()->position());
		if (prevent && result)
			ply->ForcePositionTo(cLastTickPos);
		return result;
	}

	bool TestNoClipping(BasePlayer* ply = esp::local_player,
		Vector3 oldPos = Vector3(0, 0, 0),
		Vector3 newPos = Vector3(0, 0, 0),
		float radius = 0.01f,
		float backtracking = 0.01f)
	{
		Vector3 normalized = (newPos - oldPos).Normalized();
		Vector3 vector = oldPos - normalized * backtracking;
		float magnitude = (newPos - vector).Length();
		Ray z = Ray(vector, normalized);
		
		bool flag = false; //unity::Raycast(z, magnitude + radius, 429990145);


		typedef bool (*AAA)(Ray, float, float, int);
		//real rust 0x2298A50
		//alkad rust 0x2271FB0
		flag = ((AAA)(mem::game_assembly_base + oSphereCast))(z, radius, magnitude, 429990145);
		return flag;
	}

	bool ValidateEyePos(Vector3 pos,
		Vector3 offset = Vector3(0, 0, 0))
	{
		bool flag = true;
		auto loco = esp::local_player;
		auto eyepos = loco->eyes()->position() + offset;
		float num = 1.5f;
		float num2 = 2.f / 60.f;
		float deltatime = get_deltaTime();
		//float smooth_deltatime = get_smoothdeltaTime();
		//float fixed_deltatime = get_fixeddeltaTime();
		float num3 = 2.f * deltatime;
		float num4 = (1.f + num2 + num3) * num;
		float num5 = loco->max_velocity() + loco->GetParentVelocity().Length();
		float num6 = loco->BoundsPadding() + num4 * num5;
		float num7 = eyepos.distance(pos);
		float num8 = fabs(loco->GetParentVelocity().y);
		float num9 = loco->BoundsPadding() + num4 + num8 + loco->GetJumpHeight();
		float num10 = fabs(eyepos.y - pos.y);

		if (num10 > num9)
		{
			flag = false;
		}

		Vector3 v = *reinterpret_cast<Vector3*>((uintptr_t)loco + eyes);
		Vector3 re_p = loco->model()->boneTransforms()->get(47)->position() + loco->model()->boneTransforms()->get(47)->up() * (loco->eyes()->get_view_offset().y + v.y);

		auto t = loco->transform();
		Vector3 loco_position = t->position();
		Vector3 actual_eye_pos = loco->eyes()->position();
			
		//LOS from eyes.center to eyes.position on server, i think eyes.position is lastSentTick.pos
		//if(loco->is_visible(, pos))
		
		if (!unity::LineOfSightRadius(cLastTickPos, eyepos, (uintptr_t)esp::local_player, .2f))
			flag = false;
		//if (!loco->is_visible(cLastTickPos, eyepos, 0.18f)) {
		//	esp::local_player->console_echo(_(L"[matrix]: ValidateEyePos - eye_los caught"));
		//	flag = true;
		//}

		if (loco_position.distance(loco->eyes()->position()) > 0.06f
			&& TestNoClipping(loco, cLastTickPos, loco_position, .3f))
			//&& TestNoClipping(loco, cLastTickEyePos, position2))
		{
			flag = false;
		}
		else if (loco_position.distance(loco->eyes()->position()) > 0.01f
			&& TestNoClipping(loco, actual_eye_pos, eyepos, 0.02f)) {
			flag = false;
		}

		if (!flag)
		{
			//AddViolation(loco, 
			//	antihacktype::EyeHack, 
			//	protections::eye_penalty); 
		}
		else if(protections::eye_protection >= 5 && 
			loco->modelState()->has_flag(ModelState_Flag::Mounted)) {
			eye_history.push_back(pos);
		}

		return flag;
	}

	bool can_manipulate(BasePlayer* ply,
		Vector3 pos,
		float mm_eye = 10.f) //7m only check rn
	{
		Vector3 v = *reinterpret_cast<Vector3*>((uintptr_t)ply + eyes);
		Vector3 re_p = ply->model()->boneTransforms()->get(47)->position() + ply->model()->boneTransforms()->get(47)->up() * (ply->eyes()->get_view_offset().y + v.y);
		//Vector3 re_p = ply->transform()->position() + ply->transform()->up() * (ply->eyes()->get_view_offset().y + v.y);

		if (ply->is_visible(re_p, pos)) {
			misc::best_lean = Vector3(0, 0, 0);
			return true;
		}
		auto layermask = 1218519041;
		auto HitScan = [&](Vector3 from, bool do_ = true, int val = 0) {
			Vector3 head_pos_ = esp::best_target.pos;//player->model()->boneTransforms()->get(48)->get_bone_position();

			if (vars->combat.targetbehindwall && val) {
				if ((val % 20) == 0) {
					Vector3 RealTargetPosition = Vector3(pos.x, pos.y + 1.15f, pos.z);
					bool LOS = PLOS(from, RealTargetPosition, layermask) && PLOS(pos, RealTargetPosition, layermask);
					//draw_line_(trg_pos, RealTargetPosition, LOS ? Vector4(0, 255, 0, 255) : ImColor(255, 0, 0, 255));
					if (LOS)
						return std::make_pair(LOS, RealTargetPosition);
				}

				float DegreeEnemyx = cos(val) * 1.7f;
				float DegreeEnemyz = sin(val) * 1.7f;
				Vector3 RealTargetPosition = Vector3(pos.x + DegreeEnemyx, pos.y, pos.z + DegreeEnemyz);
				bool LOS = PLOS(from, RealTargetPosition, layermask) && PLOS(pos, RealTargetPosition, layermask);

				return std::make_pair(LOS, RealTargetPosition);
			}

			if (!do_)
				return std::make_pair(false, head_pos_);

			if (vars->combat.HitScan) {
				if (!esp::best_target.ent) return std::make_pair(false, head_pos_);
				for (auto bone : { 48, 3, 4, 15, 14, 26, 57 }) {
					Vector3 TargetPosition;
					if (bone == 48) TargetPosition = head_pos_;
					else TargetPosition = esp::best_target.ent->model()->boneTransforms()->get(bone)->position();
					if (PLOS(from, TargetPosition, layermask)) {
						settings::HitScanBone = bone;
						return std::make_pair(true, TargetPosition);
					}
				}
				return std::make_pair(false, head_pos_);
			}
			else
				return std::make_pair(PLOS(from, head_pos_, layermask), head_pos_);
		};

		//HitScan returns pair { lineofsight, targetposition }

		auto do_check = [&](Vector3 a) {
			Vector3 p = re_p + a;

			//check between manip pos and eye pos
			if (!ply->is_visible(p, re_p, 0.18f))
				return false;

			//if (!ply->is_visible(p - Vector3(0, 0.3, 0), re_p)) //double check not too low as likes to shoot from just under the ground
			//	return false;

			if (vars->visual.angles)
				Sphere(p, 0.05f, col(10, 30, 90, 1), 0.01f, 10);

			//check between manip pos and player pos
			if (!ply->is_visible(p, pos, .18f))
			{
				if (!vars->combat.thick_bullet || !vars->combat.shoot_at_fatbullet) return false;
				bool t = false;
				Vector3 z = pos;
				//pos.y -= -0.2f; //imagine aiming at chest?????????
				std::array<Vector3, 9> positions = {
					z + Vector3(0, .3f, 0),
					z + Vector3(vars->combat.thickness, 0, 0),
					z + Vector3(-vars->combat.thickness, 0, 0),
					z + Vector3(0, 0, vars->combat.thickness),
					z + Vector3(0, 0, -vars->combat.thickness),
					z + Vector3(vars->combat.thickness / 2, 0, 0),
					z + Vector3(-vars->combat.thickness / 2, 0, 0),
					z + Vector3(0, 0, vars->combat.thickness / 2),
					z + Vector3(0, 0, -vars->combat.thickness / 2)
				};
				////Line(p, pos + Vector3(0, 1, 0), col(1,1,1,1), 10.f, true, false);
				for (auto v : positions) {
					if (vars->visual.angles)
						Sphere(v, 0.05f, col(10, 30, 90, 1), 0.02f, 10);
					if (ply->is_visible(p, v))
					{
						if (ply->is_visible(z, v))
						{
							t = true;
							misc::best_target = v;
							break;
						}
					}
				}

				if (!t) return false;
			}

			//fuck you
			if (!ValidateEyePos(p))
				return false;

			if (vars->visual.angles)
				Line(p, pos, { r * 100, g * 100, b * 100, 1 }, (vars->desyncTime * 5.f), true, true);

			misc::best_lean = p;
			misc::best_target = pos;
			return true;
		};

		int points = vars->combat.thick_bullet ? 7 : 20;
		for (float y = 0; y < 1.5f; y += vars->combat.targetbehindwall ? 0.3f : 0.1f) {
			float step = (M_PI_2) / points;
			float x, z, current = 0;
			for (size_t i = 0; i < points; i++)
			{
				x = Vector3::my_sin(current) * mm_eye;
				z = Vector3::my_cos(current) * mm_eye;

				Vector3 p1 = Vector3(x, y, z);
				Vector3 p2 = Vector3(-x, y, z);
				Vector3 p3 = Vector3(x, y, -z);
				Vector3 p4 = Vector3(-x, y, -z);

				if (do_check(p1)) return true;
				if (do_check(p2)) return true;
				if (do_check(p3)) return true;
				if (do_check(p4)) return true;

				current += step;
			}
		}
		for (float y = 0; y > -1.5f; y -= vars->combat.targetbehindwall ? 0.3f : 0.1f) {
			float step = (M_PI_2) / points;
			float x, z, current = 0;
			for (size_t i = 0; i < points; i++)
			{
				x = Vector3::my_sin(current) * mm_eye;
				z = Vector3::my_cos(current) * mm_eye;

				Vector3 p1 = Vector3(x, y, z);
				Vector3 p2 = Vector3(-x, y, z);
				Vector3 p3 = Vector3(x, y, -z);
				Vector3 p4 = Vector3(-x, y, -z);

				if (do_check(p1)) return true;
				if (do_check(p2)) return true;
				if (do_check(p3)) return true;
				if (do_check(p4)) return true;

				current += step;
			}
		}

		misc::best_lean = Vector3(0, 0, 0);
		return false;
	}

	bool TestFlying2(BasePlayer* ply,
		Vector3 oldPos = Vector3(0, 0, 0),
		Vector3 newPos = Vector3(0, 0, 0),
		bool verifyGrounded = true)
	{
		if (verifyGrounded)
		{
			auto extrusion = 2.f;
			Vector3 vec = (oldPos + newPos) * 0.5f;
			auto margin = 0.05f;
			float radius = _GetRadius(ply);
			float height = GetHeight(ply);
			Vector3 vec2 = vec + Vector3(0.f, radius - extrusion, 0.f);
			Vector3 vec3 = vec + Vector3(0.f, height - radius, 0.f);
			float radius2 = radius - margin;
			isInAir = !unity::CheckCapsule(vec2, vec3, radius2, 1503731969, 1);

			if (isInAir)
			{
				bool flag = false;
				Vector3 vec4 = newPos - oldPos;
				float num2 = std::fabs(vec4.y);
				float num3 = vec4.length_2d();

				if (vec4.y >= 0.f)
				{
					flag = true;
					flyhackDistanceVertical += vec4.y;
				}

				if (num2 < num3)
				{
					flag = true;
					flyhackDistanceHorizontal += num3;
				}

				if (flag)
				{
					float num4 = max((flyhackPauseTime > 0.f ? 10.f : 1.5f), 0.f);
					float num5 = _getjumpheight(ply) + num4;
					if (flyhackDistanceVertical > num5)
						return true;

					float num6 = num4;
					float num7 = 5.f + num6;
					if (flyhackDistanceHorizontal > num7)
						return true;
				}
			}
			else
			{
				flyhackDistanceVertical = 0.0f;
				flyhackDistanceHorizontal = 0.0f;
			}
		}
		return false;
	}

	bool IsSpeeding(BasePlayer* ply,
		TickInterpolator ticks,
		float deltaTime) {
		bool result;
		//using (TimeWarning.New("AntiHack.IsSpeeding", 0))
		speedhackPauseTime = max(0.f, speedhackPauseTime - deltaTime);
		if (protections::speedhack_protection <= 0)
		{
			return false;
		}
		else
		{
			auto trans = esp::local_player->transform();
			bool flag = trans ? !(!trans) : false;
			VMatrix _mv; _mv.matrix_identity();


			VMatrix matrix4x = flag ? _mv : get_localToWorldMatrix(trans);

			Vector3 vector = flag ? ticks.startPoint :
				matrix4x.MultiplyPoint3x4(ticks.startPoint);

			Vector3 a = flag ? ticks.endPoint :
				matrix4x.MultiplyPoint3x4(ticks.endPoint);

			float running = 1.0f;
			float ducking = 0.f;
			float crawling = 0.f;
			if (protections::speedhack_protection >= 2)
			{
				bool flag2 = ply->modelState()->has_flag(ModelState_Flag::Sprinting);
				bool flag3 = ply->modelState()->has_flag(ModelState_Flag::Ducked);
				bool flag4 = IsSwimming(ply);
				bool flag5 = ply->modelState()->has_flag(ModelState_Flag::Crawling);
				running = (flag2 ? 1.0f : 0.f);
				ducking = ((flag3 || flag4) ? 1.0f : 0.f);
				crawling = (flag5 ? 1.0f : 0.f);
			}
			float speed = GetSpeed(ply, running, ducking, crawling);
			Vector3 v = a - vector;
			float num = v.length_2d();
			float num2 = deltaTime * speed;
			if (num > num2)
			{
				auto getheightmap = [&]() {
					//real rust 0x1146750
					//"TerrainMeta_TypeInfo",
					uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oTerrainMeta_TypeInfo); //52690304 alkad
					uintptr_t fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
					uintptr_t heightmap = *reinterpret_cast<uintptr_t*>(fieldz + 0xB0);
					return heightmap;
				};

				uintptr_t heightmap = getheightmap();
				Vector3 v2 = GetNormal(heightmap, vector);
				Vector3 lhs = Vector3(v2.x, v2.y, v2.z);
				float num3 = max(0.f, lhs.dot(lhs)) * protections::speedhack_slopespeed * deltaTime;
				num = max(0.f, num - num3);
			}
			float num4 = max((speedhackPauseTime > 0.f) ? protections::speedhack_forgiveness_inertia : protections::speedhack_forgiveness, 0.1f);
			float num5 = num4 + max(protections::speedhack_forgiveness, 0.1f);
			speedhackDistance = std::clamp(speedhackDistance, -num5, num5);
			speedhackDistance = std::clamp(speedhackDistance - num2, -num5, num5);

			//esp::local_player->console_echo(string::wformat(_(L"[matrix]: IsSpeeding - speedhackDistance: %d, num4: %d, speedhackPauseTime: %d"),(int)((speedhackDistance + 4) * 100), (int)(num4 * 100), (int)(speedhackPauseTime * 100)));

			if (speedhackDistance > num4) {
				result = true;
			}
			else
			{
				speedhackDistance = std::clamp(speedhackDistance + num, -num5, num5);
				if (speedhackDistance > num4)
				{
					result = true;
				}
				else
					result = false;
			}
		}
		return result;
	}

	bool IsFlying(float deltaTime) {
		auto lp = esp::local_player;
		bool result;
		bool flag = deltaTime > 1.f;

		flyhackPauseTime = max(0.f, flyhackPauseTime - deltaTime);
		ticks.Reset();
		auto trans = esp::local_player->transform();


		if (ticks.HasNext()) {
			bool flag = trans ? !(!trans) : false;
			VMatrix v; v.matrix_identity();
			VMatrix matrix4x = flag ? v : get_localToWorldMatrix(trans);

			Vector3 oldPos = flag ? ticks.startPoint :
				matrix4x.MultiplyPoint3x4(ticks.startPoint);
			Vector3 vector = flag ? ticks.startPoint :
				matrix4x.MultiplyPoint3x4(ticks.endPoint);
			float num = 0.1f;
			float num2 = 15.0f;
			num = max(ticks.len / num2, num);
			while (ticks.MoveNext(num))
			{
				vector = (flag ? ticks.currentPoint
					: matrix4x.MultiplyPoint3x4(ticks.currentPoint));

				if (esp::local_player->modelState()->has_flag(ModelState_Flag::OnLadder))
					return false;
				if (TestFlying2(lp, oldPos, vector, true))
					return true;
				oldPos = vector;
			}
		}
		return false;
	}

	bool ValidateMove(BasePlayer* ply, 
		TickInterpolator ticks, 
		float deltaTime)
	{
		bool flag = deltaTime > 1.0f;
		//IsNoClipping

		if (IsFlying(deltaTime))
		{
			if (flag)
				return false;
			auto penalty = protections::flyhack_penalty * ticks.len;
			if (penalty > 0)
			{
				AddViolation(ply, antihacktype::FlyHack, penalty);
				if (protections::flyhack_reject)
					return false;
			}
		}
		if (IsSpeeding(ply, ticks, deltaTime))
		{
			if (flag)
				return false;
			auto penalty = protections::speedhack_penalty * ticks.len;
			if (penalty > 0)
			{
				AddViolation(ply, antihacktype::SpeedHack, penalty);
				if (protections::speedhack_reject)
					return false;
			}
		}
		return true;
	}

	void ValidateEyeHistory(BasePlayer* ply) {
		for (size_t i = 0; i < eye_history.size(); i++)
		{
			Vector3 point = eye_history[i];
			if (tickHistory.Distance(ply, point) > 0.1f)
			{
				AddViolation(ply, antihacktype::EyeHack, protections::eyehack_penalty);
			}
		}
		eye_history.clear();
	}

	void FinalizeTick(float deltatime) {
		if (esp::local_player->is_sleeping())
			return;
		auto lp = esp::local_player;
		tickDeltaTime += deltatime;
		//bool flag = ticks.startPoint != ticks.endPoint;
		bool flag = true;
		if (flag) {
			if (ValidateMove(lp, ticks, deltatime))
			{
				ticksPerSecond.Increment();
				int tickHistoryCapacity = max(1, (int)(ticksPerSecond.Calculate() * 0.5f)); //tickhistorytime = 0.5f;
				tickHistory.AddPoint(ticks.endPoint, tickHistoryCapacity);
				FadeViolations(lp, tickDeltaTime);
			}
			else {
				flag = false;
				//if (ConVar.AntiHack.forceposition)
				//{
				//	base.ClientRPCPlayer<UnityEngine.Vector3, uint>(null, this, "ForcePositionToParentOffset", base.transform.localPosition, this.parentEntity.uid);
				//}
				//bad
			}
			
			if (IsInsideTerrain(lp, vars->misc.antideathbarrier)) {
				//do sumn
			}

			settings::vert_flyhack = flyhackDistanceVertical;
			settings::hor_flyhack = flyhackDistanceHorizontal;
			//settings::speedhack = speedhackDistance + 4.0f;
			vars->speedhack = speedhackDistance + 3.9f;
		}
		ticks.Reset(esp::local_player->transform()->position());
		ValidateEyeHistory(lp);
		//ticks.Reset(esp::local_player->eyes()->get_position());
	}

	void ServerUpdate(float deltaTime,
		BasePlayer* ply) {
		desyncTimeRaw = max(ply->lastSentTickTime() - deltaTime, 0.f);
		desyncTimeClamped = max(desyncTimeRaw, 1.f);
		FinalizeTick(deltaTime);
	}

	bool LineCircleIntersection(Vector3 center,
		float radius,
		Vector3 rayStart,
		Vector3 rayEnd,
		float& offset)
	{
		Vector2 P(rayStart.x, rayStart.z);
		Vector2 Q(rayEnd.x, rayEnd.z);

		float a = Q.y - P.y;
		float b = P.x - Q.x;
		float c = (a * (P.x) + b * (P.y)) * -1.f;

		float x = center.x;
		float y = center.z;

		float c_x = (b * ((b * x) - (a * y)) - a * c) / (std::pow(a, 2) + std::pow(b, 2));
		float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (std::pow(a, 2) + std::pow(b, 2));

		Vector2 closestPoint(c_x, c_y);

		float distance = P.Distance(Q);

		if (P.Distance(closestPoint) > distance || Q.Distance(closestPoint) > distance)
		{
			return false;
		}

		if (radius > closestPoint.Distance(Vector2(center.x, center.z)))
		{
			Vector2 P(rayStart.x, rayStart.y);
			Vector2 Q(rayEnd.x, rayEnd.y);

			float a = Q.y - P.y;
			float b = P.x - Q.x;
			float c = (a * (P.x) + b * (P.y)) * -1.f;

			float x = center.x;
			float y = center.y;

			float c_x = (b * ((b * x) - (a * y)) - a * c) / (std::pow(a, 2) + std::pow(b, 2));
			float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (std::pow(a, 2) + std::pow(b, 2));

			Vector2 closestPoint(c_x, c_y);
			if (radius > closestPoint.Distance(Vector2(center.x, center.y)))
			{
				return true;
			}
			else
			{
				offset += std::fabs(center.y - closestPoint.y);
				return false;
			}
		}

		return false;
	};

	//AUTO BOT STUFF LOLLLL
	//
	// AIMS:
	// AUTO FARM { 
	//	FORCE LOOKING TO WALKING DIRECTION
	//	CONSTANTLY CHECK FOR PLAYERS, 
	//   -HAVE THEY ALREADY LOOKED AT ME MORE THAN TWICE? CHEATER?
	//   -ARE THEY VISIBLE?
	//	 -SHOOT AT PLAYER?
	// 
	//	RETURN TO BASE?
	//	AUTO DEPOT????
	// }
	// 
	// 
	// AUTO BOT? {
	//	IF NOT FARMING LOOK FOR PLACE TO PEEK WITH MANIP ??
	//	HVH MODE?
	//  ....??????
	// }
	//

	Vector3 lowest_pos(Vector3 in)
	{
		Vector3 current = in;
		for (size_t i = 0; i < 100; i++)
		{
			if (esp::local_player->is_visible(in, current))
			{
				current = Vector3(current.x, current.y -= 1.f, current.z);
				continue;
			}
			else break;
		}
		return Vector3(current.x, current.y += 1.6f, current.z);
	}

	float dist_from_ground(Vector3 v)
	{
		Vector3 p = v;
		int t = 0;
		while (t++ < 100)
		{
			if (!esp::local_player->is_visible(v, p))
				return v.distance(p);
			p.y -= 0.1;
		}
	}

	int FindFromHotbar(BasePlayer* lp, std::wstring shortname) {
		System::list<Item*>* belt = lp->get_belt_items();
		auto Belt = lp->Belt();
		int id = -1;
		if (belt) {
			auto checkitem = [&](Item* item, int idx) {
				if (!item) return false;
				if (std::wstring(item->get_weapon_name()).find(shortname.c_str()) != std::wstring::npos)
					return true;
				return false;
			};

			for (size_t i = 0; i < 5; i++)
				if(checkitem(Belt->GetItemInSlot(i), i))
					id = i;
		}
		return id;
	}

	std::pair<int, bool> FindToolFromHotbar(BasePlayer* lp, bool findtool = true) {
		System::list<Item*>* belt = lp->get_belt_items();
		auto Belt = lp->Belt();
		int id = -1;
		int ied = -2;
		int h_id = -3;
		auto h = lp->GetActiveItem();
		if (h) 
			h_id = h->get_item_definition_id();
		if (belt) {
			//hotbar esp
			auto checkitem = [&](Item* item, int idx) {
				if (!item) return;
				auto name = item->get_weapon_name();
				if (!name) return;

				if (findtool &&
					(std::wstring(name).find(_(L"Jackhammer")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Chainsaw")) != std::wstring::npos
						|| std::wstring(name).find(_(L"ickaxe")) != std::wstring::npos
						|| std::wstring(name).find(_(L"cepick")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Rock")) != std::wstring::npos
						|| std::wstring(name).find(_(L"atchet")) != std::wstring::npos))
				{
					ied = item->get_item_definition_id();
					id = idx;
					return;
				}
				else if (!findtool &&
					(std::wstring(name).find(_(L"Rifle")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Pistol")) != std::wstring::npos
						|| std::wstring(name).find(_(L"SMG")) != std::wstring::npos
						|| std::wstring(name).find(_(L"gun")) != std::wstring::npos
						|| std::wstring(name).find(_(L"M249")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Bow")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Crossbow")) != std::wstring::npos
						|| std::wstring(name).find(_(L"MP5")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Thompson")) != std::wstring::npos
						|| std::wstring(name).find(_(L"Revolver")) != std::wstring::npos))
				{
					ied = item->get_item_definition_id();
					id = idx;
					return;
				}
			};
			for (size_t i = 0; i < 5; i++)
				checkitem(Belt->GetItemInSlot(i), i);
				//checkitem(belt->get(i), i);
		}
		return std::make_pair(id, ied == h_id);
	}

	namespace anticheater {

	};

	namespace autobot {
		bool needs_to_jump = false;
		bool wants_shoot = false;
		int psteps = 0;
		Vector3 last_pos = Vector3(0, 0, 0);
		float last_pos_time = 0.f;
		float time_at_node = 0.f;
		float last_player_follow_update = 0.f;
		float time_within_1m = 0.f;
		Vector3 last_pos_until_jump = Vector3::Zero();

		void PathSmooth(std::vector<Vector3>& ref) {
			std::vector<int> indexes = { };
			if (ref.size() < 3) return;
			std::vector<Vector3> new_path = { ref[0], ref[1] };
			auto lp = esp::local_player;
			Vector3 current = ref[0];
			Vector3 next = ref[1];
			int fi = 0;
			for (size_t i = 2; i < ref.size(); i++)
			{
				next = ref[i];
				if (lp->is_visible(current, next)
					&& dist_from_ground(next) <= 1.5f)
					continue;
				else
					new_path.push_back(next);
			}
			if (!new_path.empty() && new_path.size() < ref.size())
				ref = new_path;
		}

		void CreatePath(Vector3 marker_pos,
			Vector3 point)
		{
			node.pos = marker_pos;
			node.steps = 1;
			//create path
			std::vector<Vector3> path;
			Vector3 original = point;
			bool failed = false;
			Vector3 old_point = point;
			float control = 0.f;
			int iterations = 0;
			while (point.distance(node.pos) > 1.0f)
			{
				if (iterations++ > 500)
					break;

				path.push_back(point);
				//Vector3 new_point = lowest_pos(Vector3::move_towards(point, node.pos, 1.0f));
				Vector3 new_point = Vector3::move_towards(point, node.pos, 1.0f);

				if (esp::local_player->is_visible(point, new_point, 1.0f))
				{
					esp::local_player->console_echo(string::wformat(_(L"[matrix]: CreatePath - visibility straight ahead (%d)"), path.size()));
					old_point = point;
					point = lowest_pos(new_point);
				}
				else
				{
					esp::local_player->console_echo(string::wformat(_(L"[matrix]: CreatePath - no visibility straight ahead, creating sphere (%d)"), path.size()));

					std::vector<Vector3> ps = {};

					for (auto e : sphere1m) //create sphere if cannot find LOS straight ahead
						if (esp::local_player->is_visible(point, point + e, 1.5f)
							&& (point + e).distance(node.pos) < point.distance(node.pos)
							&& (point + e).distance(point) > 0.7f)
						{
							ps.push_back(lowest_pos(point + e));
						}

					Vector3 best = Vector3(0, 0, 0);
					if (ps.size() == 0) {
						esp::local_player->console_echo(string::wformat(_(L"[matrix]: CreatePath could not create another node, path size: %i"), path.size()));
						break;
					}

					esp::local_player->console_echo(string::wformat(_(L"[matrix]: CreatePath - potentials: %d"), ps.size()));

					for (auto e : ps)
						if (e.distance(node.pos) < best.distance(node.pos)
							&& dist_from_ground(e) < 1.6f)
							best = e;
					esp::local_player->console_echo(string::wformat(_(L"[matrix]: CreatePath - closest (best chosen) (%d, %d, %d) (%d)"), (int)best.x, (int)best.y, (int)best.z, path.size()));
					old_point = point;
					point = best;
				}
			}

			if (node.path.size() > 5)
			{
				int within_2m = 0;
				int within_idx = 0;
				for (size_t i = 4; i < node.path.size(); i++)
				{
					if (node.path[i].distance(node.path[i - 4]) < 2.f)
						within_2m++;
					else within_idx = i;
				}
				if (within_idx != 0
					&& within_2m != 0) {
					node.path.erase(node.path.begin() + within_idx, node.path.end());
				}
			}

			if (!node.path.empty())
				PathSmooth(node.path);
			node.path = path;
		}

		void do_jump(PlayerWalkMovement* pwm,
			ModelState* state,
			float force = 10.f) {
			if (!pwm || !state) return;
			state->set_flag(ModelState_Flag::OnGround);
			state->setjumped(true);
			pwm->set_jump_time(get_time());
			auto vel = pwm->get_body_velocity();
			pwm->set_body_velocity(Vector3(vel.x, force, vel.z));
		}

		void checkandshoot(PlayerWalkMovement* pwm) {
			__try {
				auto lp = esp::local_player;
				if (!pwm || !lp) return;
				std::vector<BasePlayer*> internal_playerlist = {};
				auto lppos = lp->model()->boneTransforms()->get(48)->position();
				//maybe check for manipulator
				for (auto p : player_map) {
					__try {
						if (p.second && (p.second->is_alive() && (!p.second->is_sleeping())))
						{
							auto model = p.second->model();
							if (model) {
								//auto trans = model->boneTransforms()->get(48);
								auto bonetransforms = *reinterpret_cast<System::Array<Transform*>**>(model + 0x48);
								if (bonetransforms)
								{
									auto trans = bonetransforms->get(48);
									if (trans) {
										auto pos = trans->position();
										auto visible = lp->is_visible(pos, lppos);
										if (visible
											&& pos.distance(lppos) < vars->misc.autoattackdist
											&& !p.second->is_local_player())
											internal_playerlist.push_back(p.second);
									}
								}
							}
						}
					}
					__except (true) {
						//player_map.erase(std::remove(player_map.begin(), player_map.end(), p), player_map.end());
					}
				}
				BasePlayer* ply = nullptr;
				for (auto p : internal_playerlist)
				{
					auto pos = p->transform()->position();
					switch (vars->misc.targetting_mode)
					{
					case 0: //closest
					{
						if (!ply) {
							ply = p;
							continue;
						}
						if (ply->transform()->position().distance(lppos) > pos.distance(lppos))
						{
							ply = p;
							continue;
						}
						break;
					}
					case 1: //lowest hp
					{
						if (!ply) {
							ply = p;
							continue;
						}
						if (ply->health() > p->health())
						{
							ply = p;
							continue;
						}
						break;
					}
					}
				}
				auto target = aim_target();

				if (ply == nullptr)
				{
					vars->wants_shoot = false;
					wants_shoot = false;
					return;
				}

				switch (vars->combat.aimbone)
				{
				case 0: //Head
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::head)->position();
					break;
				case 1: //Spine 4
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::spine4)->position();
					break;
				case 2: //Pelvis
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::pelvis)->position();
					break;
				case 3: //R arm
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::r_forearm)->position();
					break;
				case 4: //L arm
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::l_forearm)->position();
					break;
				case 5: //R leg
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::r_knee)->position();
					break;
				case 6: //L leg
					target.pos = ply->model()->boneTransforms()->get((int)Bone_List::l_knee)->position();
					break;
				}
				//Sphere(target.pos, 1.f, { 1, 1, 1, 1 }, 20.f, 100.f);
				auto distance = esp::local_player->model()->boneTransforms()->get(48)->position().get_3d_dist(target.pos); //crashes bc non game thread
				target.distance = distance;
				auto fov = unity::get_fov(target.pos);
				target.fov = fov;
				target.ent = (BaseCombatEntity*)ply;
				target.network_id = ply->userID();
				target.avg_vel = ply->GetWorldVelocity();
				//only shoots at visible players anyway
				auto visible = true;

				esp::best_target = target;

				auto item = lp->GetActiveItem();
				if (item)
				{
					auto held = item->GetHeldEntity<BaseProjectile>();
					if (held)
					{
						auto mag_ammo = held->ammo_left();
						float nextshot = misc::fixed_time_last_shot + held->repeatDelay();
						if (get_fixedTime() > nextshot
							&& held->timeSinceDeploy() > held->deployDelay()
							&& mag_ammo > 0)
						{
							wants_shoot = true;
							vars->wants_shoot = true;
							autoshot = true;
						}
						else
						{
							//updateammodisplay((uintptr_t)held);
							//shot_fired((uintptr_t)held);
							//did_attack_client_side((uintptr_t)held);
						}
					}
					else {
						//switch to weapon
					}
				}
			}
			__except (true) {
				return;
			}
		}

		void pathfind(PlayerWalkMovement* pwm,
			Vector3 marker_pos) {
			Vector3 vel = pwm->get_TargetMovement();
			vel = Vector3(vel.x / vel.length() * 5.5f, vel.y, vel.z / vel.length() * 5.5f);
			auto eyepos = esp::local_player->transform()->position();

			if (vars->misc.autoattack)
				checkandshoot(pwm);

			auto threshold = vars->misc.walkto == 5 ? 0.1f : 1.f;

			if (node.steps > 0
				&& eyepos.distance(node.pos) < threshold)
			{
				node.path.clear();
				node.pos = Vector3(0, 0, 0);
				node.steps = 0;
				vel = Vector3(0, 0, 0);
				time_at_node += get_deltaTime();
			}

			if (eyepos.distance(node.pos) >= threshold)
			{
				if (node.path.empty() || ((node.pos.is_empty() || node.pos == Vector3(0, 0, 0))
					&& eyepos.distance(node.pos)) > 1.f
					|| last_player_follow_update + 5.f < get_fixedTime())
				{
					CreatePath(marker_pos, eyepos);
					last_player_follow_update = get_fixedTime();
				}

				Vector3 current_step = node.path[node.steps];

				if (current_step.is_empty())
					CreatePath(marker_pos, eyepos);

				psteps = node.steps;
				if (current_step.distance(node.pos) <= threshold / 2.f)
				{
					vel = Vector3(0, 0, 0);
					node.path.clear();
					node.pos = Vector3(0, 0, 0);
					node.steps = 0;
					return;
				}

				//draw path
				if (!node.path.empty())
				{
					for (size_t i = 1; i < node.path.size(); i++)
					{
						if (node.path[i] == current_step)
							Line(node.path[i - 1], node.path[i], col(190, 20, 20, 50), 0.02f, false, true);
						else
							Line(node.path[i - 1], node.path[i], col(190, 190, 190, 50), 0.02f, false, true);
					}
				}

				if (eyepos.distance(current_step) < 1.6f)
					node.steps += 1;

				if (node.steps >= node.path.size() - 1)
				{
					vel = Vector3(0, 0, 0);
					node.path.clear();
					node.pos = Vector3(0, 0, 0);
					node.steps = 0;
					return;
				}

				Vector3 dir = ((Vector3(current_step.x, current_step.y - dist_from_ground(current_step) + 0.1f, current_step.z)) - eyepos).Normalized();
				vel = { (dir.x / dir.length() * 5.5f), vel.y, (dir.z / dir.length() * 5.5f) };
				if(!node.pos.is_empty())
					pwm->set_TargetMovement(vel);


				if (node.path[node.steps].y - eyepos.y > 1.4f)
				{
					needs_to_jump = true;
					pwm->set_TargetMovement({ vel.x, vel.y += 10, vel.z });
				}

				if (time_within_1m + 5.f < get_fixedTime()
					&& eyepos.distance(last_pos_until_jump) <= 1.f)
				{
					needs_to_jump = true;
					pwm->set_TargetMovement({ vel.x, vel.y += 5, vel.z });
					last_pos_until_jump = last_pos;
				}
				else
					time_within_1m = get_fixedTime();

			}
		}

		void walktoplayer(PlayerWalkMovement* pwm,
			char* selected) {
			auto lp = esp::local_player;
			BasePlayer* ply = (BasePlayer*)esp::best_target.ent;
			bool block = false;
			for (auto p : player_map)
				if (p.second->userID() == vars->follow_player_id)
					ply = p.second;
			if (!ply) return;
			auto position = ply->transform()->position();
			if (vars->gui_player_map[ply->userID()]->block)
				position += (ply->eyes()->body_forward() * .5f);
			node.pos = position;
			pathfind(pwm, position);
			if(node.pos.is_empty()) node.pos = position;
		}

		void auto_farm(PlayerWalkMovement* pwm,
			std::string classname,
			std::string name) {
			auto lp = esp::local_player;
			if (!lp || !pwm) return;

			if(!misc::node.ent)
				misc::node.ent = (BaseEntity*)lp->find_closest((Networkable*)lp, 200.f, classname.c_str(), name.c_str());

			Vector3 vel = pwm->get_TargetMovement();
			vel = Vector3(vel.x / vel.length() * 5.5f, vel.y, vel.z / vel.length() * 5.5f);
			auto eyepos = lp->transform()->position();//lp->eyes()->get_position();

			auto Transform = node.ent->transform();
			auto hp = *reinterpret_cast<float*>(node.ent + 0x178); //detect if broken with this fuck knows why
			if (Transform && hp > 60 && time_at_node < 5.f) {
				auto marker_pos = Transform->position();
				node.pos = marker_pos;
				if (node.pos.distance(eyepos) < 5.f) { //equip tool
					auto toolslot = FindToolFromHotbar(lp);
					if (!toolslot.second && toolslot.first > -1)
						lp->Belt()->SetSelectedSlot(toolslot.first);
				}
				else { //equip weapon
					auto toolslot = FindToolFromHotbar(lp, false);
					if (!toolslot.second && toolslot.first > -1)
						lp->Belt()->SetSelectedSlot(toolslot.first);
				}
				pathfind(pwm, marker_pos);
			}
			else
			{
				misc::node.ent = (BaseEntity*)lp->find_closest((Networkable*)lp, 200.f, classname.c_str(), name.c_str());
				time_at_node = 0.f;
				misc::node.path.clear();
				misc::node.pos = Vector3(0, 0, 0);
				misc::node.steps = 0;
			}
		}
	}

	bool get_prediction(aim_target target,
		Vector3 rpc_position,
		Vector3 target_pos,
		Vector3 original_vel,
		Vector3& aimbot_velocity,
		Vector3& _aimdir,
		float& travel_t,
		//Projectile* p,
		float partialTime,
		float drag,
		float gravityModifier,
		Vector3& actualposition,
		bool skip_draw = false,
		int simulationsmax = 100,
		float offset = -.5f,
		float offsetstep = .05f) 
	{

		Vector3 player_velocity = Vector3(0, 0, 0);
		std::vector<Vector3> path = {};
		int simulations = 0;

		target_pos.y -= 0.1f;
		float o = offset;
		if (target.ent) {
			auto travel = 0.f;
			auto vel = (getmodifiedaimcone(0, rpc_position - target_pos, true)).Normalized() * original_vel.length();
			auto grav = gravityModifier;
			auto gravity = get_gravity();
			auto deltatime = get_deltaTime();
			auto timescale = get_timeScale();
			simulations = 0;

			//auto wv = GetWorldVelocity(target.player);
			//player_velocity = Vector3(wv.x, 0, wv.z);


			while (simulations < simulationsmax) {
				travel_t = 0.f;
				auto pos = rpc_position;
				auto origin = pos;
				float num = deltatime * timescale;
				float travelTime = 0.f;
				int num3 = (int)(8.f / num);

				Vector3 vt = target_pos + Vector3(0, offset, 0);
				_aimdir = getmodifiedaimcone(0.f, vt - rpc_position, true);
				auto velocity = _aimdir.Normalized() * original_vel.length();

				//predict movement?

				for (size_t i = 0; i < num3; i++)
				{
					origin = pos;
					pos += velocity * num;
					velocity += gravity * grav * num;
					velocity -= velocity * drag * num;
					travel_t += num;
					if (misc::LineCircleIntersection(target_pos, 0.1f, origin, pos, offset))
					{
						aimbot_velocity = (_aimdir).Normalized() * original_vel.length();
						//emulate 1 tick has already passed
						//aimbot_velocity += gravity * grav * num;
						//aimbot_velocity -= aimbot_velocity * drag * num;
						break;
					}
					else
					{
						//Line(origin, pos, col(1, 1, 1, 1), 2.f, false, true);
					}
				}
				offset += offsetstep;
				simulations++;
				if (!aimbot_velocity.is_empty())
					break;
			}
		}
		offset = o;
		if (travel_t > 0.f) {
			//movement prediction

			if (vars->desyncTime > 0.f
				&& vars->combat.bullet_tp)
				travel_t -= vars->desyncTime;

			aimbot_velocity = Vector3(0, 0, 0);
			if (target.ent) {
				auto wv = target.avg_vel;

				auto worldvel = target.ent->GetWorldVelocity();

				if (wv.is_empty() || wv.is_nan())
					wv = worldvel;

				if (target.is_heli)
					wv = worldvel;

				if (worldvel.is_empty()) //player has stopped moving so override average velocity
					wv = Vector3(0, 0, 0);

				Vector3 player_velocity = Vector3(wv.x, 0, wv.z) * (target.is_heli ? 0.75f : vars->combat.movementpred);


				Vector3 final_vel = player_velocity * travel_t;
				Vector3 actual = target_pos += final_vel;
				Sphere(actual, 0.1f, { 1, 1, 1, 1 }, 10.f, 0);

				auto travel = 0.f;
				auto vel = (getmodifiedaimcone(0, rpc_position - actual, true)).Normalized() * original_vel.length();
				auto grav = gravityModifier;
				auto gravity = get_gravity();
				auto deltatime = get_deltaTime();
				auto timescale = get_timeScale();
				simulations = 0;


				travel_t = 0.f;

				while (simulations < simulationsmax) {
					path.clear();
					auto pos = rpc_position;
					auto origin = pos;
					float num = deltatime * timescale;
					float travelTime = 0.f;
					int num3 = (int)(8.f / num);

					Vector3 vt = actual + Vector3(0, offset, 0);
					_aimdir = getmodifiedaimcone(0.f, vt - rpc_position, true);
					auto velocity = _aimdir.Normalized() * original_vel.length();

					//predict movement?

					for (size_t i = 0; i < num3; i++)
					{
						path.push_back(origin);
						origin = pos;
						pos += velocity * num;
						velocity += gravity * grav * num;
						velocity -= velocity * drag * num;
						travel_t += num;

						if (misc::LineCircleIntersection(actual, 0.1f, origin, pos, offset))
						{
							//Line(actual, pos, col(0, 1, 0, 1), 10.f, false, true);
							aimbot_velocity = (_aimdir).Normalized() * original_vel.length();
							////emulate 1 tick has already passed
							//aimbot_velocity += gravity * grav * 0;
							//aimbot_velocity -= aimbot_velocity * drag * 0;

							//aimbot_velocity.y += 1.f;

							actualposition = pos;

							if (!skip_draw && vars->visual.tracers)
							{
								path.push_back(target_pos);
								Vector3 va = path[0];
								for (auto v : path) {
									Line(va, v, col(12, 150, 100, 50), 10.f, false, true);
									va = v;
								}
							}
							return true;
						}
						else
						{
							//Line(origin, pos, col(1, 1, 1, 1), 10.f, false, true);
						}
					}
					offset += offsetstep;
					simulations++;
					if (!aimbot_velocity.is_empty())
						break;
				}
			}
		}

		return false;
	}

	void lower_velocity(aim_target target,
		Vector3 rpc_position,
		Vector3 target_pos,
		Vector3 original_vel,
		float original_velmax,
		Vector3& aimbot_velocity,
		Vector3& _aimdir,
		float& travel_t,
		float partialTime,
		float drag,
		float gravityModifier,
		Projectile* p,
		Vector3 actualposition,
		bool skip_draw = false,
		int simulations = 100) {

		auto velocitymin = 0.9f;
		auto velocitymax = 1.5f;

		for (size_t i = velocitymax; i > velocitymin; i -= 0.02f)
		{
			if (get_prediction(target,
				rpc_position,
				target_pos,
				(original_vel * i),
				aimbot_velocity,
				_aimdir,
				travel_t,
				partialTime,
				drag,
				gravityModifier,
				actualposition,
				skip_draw,
				50))
				return;
		}

	}
}