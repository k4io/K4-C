#pragma once
#include "memory/il2cpp.hpp"
#include "settings.hpp"
#include "offsets.h"
#include <math.h>
#include "Keybind.h"

#include "spherearray.h"

#include <Windows.h>
#include <chrono>
#include "settings.hpp"
#include "rust/rust.hpp"
#include "rust/unity.hpp"
#include "rust/classes.hpp"
#include "projectile.hpp"
//#include <vector>

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
	rust::list<projectileshoot_projectile*>* projectiles; //0x18
};

struct TimeAverageValueData
{
public:
	int Calculate()
	{
		float realtimeSinceStartup = unity::get_realtimesincestartup();
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

struct fired_projectile {
	Projectile* original;
	Projectile* fake;
	float fired_at;
	int updates;
	aim_target player;
};

struct NodeTarget {
	Vector3 pos;
	int steps;
	std::vector<Vector3> path;
	uintptr_t ent;
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

	TickInterpolator ticks;
	TimeAverageValueData ticksPerSecond = {};
	TickHistory tickHistory;
	antihacktype lastViolationType = antihacktype::None;
	std::vector<Vector3> eye_history = {};
	projectileshoot emulated_shot;
	NodeTarget node;

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

	bool isInAir = false;
	bool manual = false;
	bool autoshot = false;
	bool manipulate_vis = false;
	bool emulated = false;

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

	void AddViolation(base_player* ply, antihacktype type, float amount) {
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
			ply->console_echo(string::wformat(
				_(L"[trap] Anti-AntiHack - Added violation [%s] of %d in frame %d (now has %d)"),
				(type == antihacktype::AttackHack ? _("AttackHack") :
					type == antihacktype::CooldownHack ? _("CooldownHack") :
					type == antihacktype::EyeHack ? _("EyeHack") :
					type == antihacktype::FlyHack ? _("FlyHack") :
					type == antihacktype::InsideTerrain ? _("InsideTerrain") :
					type == antihacktype::MeleeHack ? _("MeleeHack") :
					type == antihacktype::NoClip ? _("NoClip") :
					type == antihacktype::ProjectileHack ? _("ProjectileHack") :
					type == antihacktype::ReloadHack ? _("ReloadHack") :
					type == antihacktype::SpeedHack ? _("SpeedHack") : _("none")),
				(int)amount,
				(int)get_frameCount(),
				(int)violationLevel));
		}
	}

	void FadeViolations(base_player* ply, float deltaTime) {
		if (unity::get_realtimesincestartup() - lastViolationTime > 10.0f)
		{
			violationLevel = max(0.0f, violationLevel - 0.1f * deltaTime);
		}
	}

	bool TestNoClipping(base_player* ply = esp::local_player,
		Vector3 oldPos = Vector3(0, 0, 0),
		Vector3 newPos = Vector3(0, 0, 0),
		float radius = 0.01f,
		float backtracking = 0.01f)
	{
		Vector3 normalized = (newPos - oldPos).Normalized();
		Vector3 vector = oldPos - normalized * backtracking;
		float magnitude = (newPos - vector).Length();
		Ray z = Ray(vector, normalized);
		bool flag = unity::Raycast(z, magnitude + radius, 429990145);

		if (!flag)
		{
			typedef bool (*AAA)(Ray, float, float, int);
			//real rust 0x2273840
			//alkad rust 0x2258180
			flag = ((AAA)(mem::game_assembly_base + 0x2273840))(z, radius, magnitude, 429990145);
		}
		return flag;
	}

	bool ValidateEyePos(Vector3 pos,
		Vector3 offset = Vector3(0, 0, 0))
	{
		bool flag = false;
		auto loco = esp::local_player;
		auto eyepos = loco->get_player_eyes()->get_position() + offset;
		float num = 1.5f;
		float num2 = 2.f / 60.f;
		float deltatime = get_deltaTime();
		//float smooth_deltatime = get_smoothdeltaTime();
		//float fixed_deltatime = get_fixeddeltaTime();
		float num3 = 2.f * deltatime;
		float num4 = (1.f + num2 + num3) * num;
		float num5 = loco->max_velocity() + GetParentVelocity(loco).Length();
		float num6 = loco->boundspadding() + num4 * num5;
		float num7 = eyepos.distance(pos);
		float num8 = fabs(GetParentVelocity(loco).y);
		float num9 = loco->boundspadding() + num4 + num8 + loco->get_jump_height();
		float num10 = fabs(eyepos.y - pos.y);

		if (num10 > num9)
		{
			flag = true;
		}

		auto t = get_transform(loco);
		Vector3 position2 = t->get_bone_position();
		Vector3 actual_eye_pos = loco->get_player_eyes()->get_position();

		if (position2.distance(loco->get_player_eyes()->get_position()) > 0.06f
			&& TestNoClipping(loco, cLastTickPos, position2))
			//&& TestNoClipping(loco, cLastTickEyePos, position2))
		{
			flag = true;
		}
		else if (position2.distance(loco->get_player_eyes()->get_position()) > 0.01f
			&& TestNoClipping(loco, actual_eye_pos, eyepos)) {
			flag = true;
		}

		if (flag)
		{
			AddViolation(loco, 
				antihacktype::EyeHack, 
				protections::eye_penalty);
		}
		else if(protections::eye_protection >= 5 && 
			loco->get_model_state()->has_flag(rust::classes::ModelState_Flag::Mounted)) {
			eye_history.push_back(pos);
		}

		return flag;
	}

	bool OnProjectileAttack(Projectile* p,
		base_player* ply,
		rust::classes::PlayerProjectileAttack* ppa)
	{
		auto playerAttack = ppa->playerAttack;




		return true;
	}

	bool can_manipulate(base_player* ply,
		Vector3 pos,
		float mm_eye = 7.f) //7m only check rn
	{
		Vector3 v = *reinterpret_cast<Vector3*>((uintptr_t)ply + eyes);
		Vector3 re_p = ply->get_bone_transform(47)->get_bone_position() + ply->get_bone_transform(47)->up() * (ply->get_player_eyes()->get_view_offset().y + v.y);

		if (ply->is_visible(re_p, pos)) {
			misc::best_lean = Vector3(0, 0, 0);
			return true;
		}

		auto do_check = [&](Vector3 a) {
			Vector3 p = re_p + a;

			if (!ply->is_visible(p, re_p))
				return false;

			//if (!ply->is_visible(p - Vector3(0, 0.3, 0), re_p)) //double check not too low as likes to shoot from just under the ground
			//	return false;

			if (vars->visual.angles)
				Sphere(p, 0.05f, col(10, 30, 90, 1), 0.02f, 10);

			if (!ply->is_visible(p, pos))
			{
				if (!vars->combat.thick_bullet || !vars->combat.shoot_at_fatbullet) return false;
				bool t = false;
				Vector3 z = pos;
				//pos.y -= -0.2f; //imagine aiming at chest?????????
				std::array<Vector3, 5> positions = {
					z + Vector3(0, .3f, 0),
					z + Vector3(.9f, 0, 0),
					z + Vector3(-.9f, 0, 0),
					z + Vector3(0, 0, .9f),
					z + Vector3(0, 0, -.9f)
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

			if (ValidateEyePos(p))
				return false;

			misc::best_lean = a;
			return true;
		};

		for (float y = 1.5f; y > -1.5f; y -= vars->combat.thick_bullet ? 0.3f : 0.1f) {
			int points = vars->combat.thick_bullet ? 7 : 20;
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

	bool TestFlying2(base_player* ply,
		Vector3 oldPos = Vector3(0, 0, 0),
		Vector3 newPos = Vector3(0, 0, 0),
		bool verifyGrounded = true)
	{
		if (verifyGrounded)
		{
			auto extrusion = 2.f;
			Vector3 vec = (oldPos + newPos) * 0.5f;
			auto margin = 0.05f;
			float radius = GetRadius(ply);
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
					float num5 = GetJumpHeight(ply) + num4;
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

	bool IsSpeeding(base_player* ply,
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
			auto trans = get_transform(esp::local_player);
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
				bool flag2 = ply->get_model_state()->has_flag(rust::classes::ModelState_Flag::Sprinting);
				bool flag3 = ply->get_model_state()->has_flag(rust::classes::ModelState_Flag::Ducked);
				bool flag4 = IsSwimming(ply);
				bool flag5 = ply->get_model_state()->has_flag(rust::classes::ModelState_Flag::Crawling);
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
					//real rust 52698608
					uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + 52698608);
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
		auto trans = get_transform(esp::local_player);

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

				if (TestFlying2(lp, oldPos, vector, true))
					return true;
				oldPos = vector;
			}
		}
		return true;
	}

	bool ValidateMove(base_player* ply, TickInterpolator ticks, float deltaTime)
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

	void ValidateEyeHistory(base_player* ply) {
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
		bool flag = ticks.startPoint != ticks.endPoint;
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
			settings::vert_flyhack = flyhackDistanceVertical;
			settings::hor_flyhack = flyhackDistanceHorizontal;
			settings::speedhack = speedhackDistance;
		}
		ticks.Reset(get_transform(esp::local_player)->get_bone_position());
		//ValidateEyeHistory(lp);
		//ticks.Reset(esp::local_player->get_player_eyes()->get_position());
	}

	void ServerUpdate(float deltaTime,
		base_player* ply) {
		desyncTimeRaw = max(ply->get_last_sent_tick_time() - deltaTime, 0.f);
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

	namespace autobot {
		bool needs_to_jump = false;
		int psteps = 0;
		Vector3 last_pos = Vector3(0, 0, 0);
		float last_pos_time = 0.f;

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
				if (lp->is_visible(current, next))
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
			while (point.distance(node.pos) > 1.5f)
			{
				if (iterations++ > 500)
					break;

				path.push_back(point);
				//Vector3 new_point = lowest_pos(Vector3::move_towards(point, node.pos, 1.0f));
				Vector3 new_point = Vector3::move_towards(point, node.pos, 1.0f);

				if (esp::local_player->is_visible(point, new_point, 1.5f))
				{
					esp::local_player->console_echo(string::wformat(_(L"[trap]: CreatePath - visibility straight ahead (%d)"), path.size()));
					old_point = point;
					point = lowest_pos(new_point);
				}
				else
				{
					esp::local_player->console_echo(string::wformat(_(L"[trap]: CreatePath - no visibility straight ahead, creating sphere (%d)"), path.size()));

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
						esp::local_player->console_echo(string::wformat(_(L"[trap]: CreatePath could not create another node, path size: %i"), path.size()));
						break;
					}

					esp::local_player->console_echo(string::wformat(_(L"[trap]: CreatePath - potentials: %d"), ps.size()));

					for (auto e : ps)
						if (e.distance(node.pos) < best.distance(node.pos)
							&& dist_from_ground(e) < 1.6f)
							best = e;
					esp::local_player->console_echo(string::wformat(_(L"[trap]: CreatePath - closest (best chosen) (%d, %d, %d) (%d)"), (int)best.x, (int)best.y, (int)best.z, path.size()));
					old_point = point;
					point = best;
				}
			}
			node.path = path;
			//if (!node.path.empty())
			//	PathSmooth(node.path);
		}

		void do_jump(playerwalkmovement* pwm,
			modelstate* state,
			float force = 10.f) {
			if (!pwm || !state) return;
			state->set_flag(rust::classes::ModelState_Flag::OnGround);
			state->setjumped(true);
			pwm->set_jump_time(get_time());
			auto vel = pwm->get_body_velocity();
			pwm->set_body_velocity(Vector3(vel.x, force, vel.z));
		}

		void auto_farm(playerwalkmovement* pwm) {
			auto lp = esp::local_player;
			if (!lp || !pwm) return;

			Vector3 vel = pwm->get_TargetMovement();
			vel = Vector3(vel.x / vel.length() * 5.5f, vel.y, vel.z / vel.length() * 5.5f);
			auto eyepos = lp->get_player_eyes()->get_position();

			auto transform = get_transform((base_player*)node.ent);
			auto hp = *reinterpret_cast<float*>(node.ent + 0x178); //detect if broken with this fuck knows why
			if (transform && hp > 60) {
				auto marker_pos = get_position((uintptr_t)transform);
				Sphere(marker_pos, 1.f, col(1, 1, 1, 1), 0.02f, 100.f);
				if (node.steps > 0
					&& eyepos.distance(node.pos) < 1.f)
				{
					node.path.clear();
					node.pos = Vector3(0, 0, 0);
					node.steps = 0;
					vel = Vector3(0, 0, 0);
				}

				if (eyepos.distance(node.pos) >= 1.f)
				{
					if (node.path.empty() && (node.pos.is_empty() || node.pos == Vector3(0, 0, 0))
						&& eyepos.distance(node.pos) > 1.f)
						CreatePath(marker_pos, eyepos);

					Vector3 current_step = node.path[node.steps];

					psteps = node.steps;
					if (current_step.distance(node.pos) <= 0.5f)
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
								Line(node.path[i - 1], node.path[i], col(12, 150, 100, 50), 0.02f, false, true);
							else
								Line(node.path[i - 1], node.path[i], col(12, 150, 100, 50), 0.02f, false, true);
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
					pwm->set_TargetMovement(vel);

					last_pos = eyepos;
					last_pos_time = get_fixedTime();

					if (node.path[node.steps].y - eyepos.y > 1.f)
					{
						needs_to_jump = true;
						pwm->set_TargetMovement({ vel.x, vel.y += 10, vel.z });


						//auto state = lp->get_model_state();
						//if (settings::vert_flyhack < 2.5f && settings::hor_flyhack < 6.f)
						//{
						//	pwm->force_jump(state, true);
						//	do_jump(pwm, state, 3.f);
						//}
					}
				}
			}
			else
			{
				misc::node.ent = (uintptr_t)lp->find_closest(_("OreResourceEntity"), (networkable*)lp, 200.f);

				misc::node.path.clear();
				misc::node.pos = Vector3(0, 0, 0);
				misc::node.steps = 0;
			}
		}
	}

	void get_prediction(aim_target target,
		Vector3 rpc_position,
		Vector3 target_pos,
		Vector3 original_vel,
		Vector3& aimbot_velocity,
		Vector3& _aimdir,
		Projectile* p,
		bool skip_draw = false) {

		float travel_t;
		Vector3 player_velocity = Vector3(0, 0, 0);
		std::vector<Vector3> path = {};
		int simulations = 0;

		target_pos.y -= 0.1f;

		if (target.player) {
			auto travel = 0.f;
			auto vel = (getmodifiedaimcone(0, rpc_position - target_pos, true)).Normalized() * original_vel.length();
			auto drag = p->drag();
			auto grav = p->gravityModifier();
			auto gravity = get_gravity();
			auto deltatime = get_deltaTime();
			auto timescale = get_timeScale();
			auto offset = 0.1f;
			simulations = 0;

			//auto wv = GetWorldVelocity(target.player);
			//player_velocity = Vector3(wv.x, 0, wv.z);


			while (simulations < 300) {
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
						//Line(origin, pos, col(0, 1, 0, 1), 10.f, false, true);
						aimbot_velocity = (_aimdir).Normalized() * original_vel.length();
						//emulate 1 tick has already passed
						aimbot_velocity += gravity * grav * num;
						aimbot_velocity -= aimbot_velocity * drag * num;
						break;
					}
					else
					{
						//Line(origin, pos, col(1, 1, 1, 1), 10.f, false, true);
					}
				}
				offset += 0.1f;
				simulations++;
				if (!aimbot_velocity.is_empty())
					break;
			}
		}

		if (travel_t > 0.f) {
			//movement prediction
			aimbot_velocity = Vector3(0, 0, 0);
			if (target.player) {
				auto wv = target.avg_vel;

				if (wv.is_empty() || wv.is_nan())
					wv = GetWorldVelocity(target.player);

				if (target.is_heli)
					wv = GetWorldVelocity(target.player);

				Vector3 player_velocity = Vector3(wv.x, 0, wv.z) * (target.is_heli ? 0.75f : 0.9f);

				Vector3 final_vel = player_velocity * travel_t;
				Vector3 actual = target_pos += final_vel;

				auto partialTime = p->partialTime();
				auto travel = 0.f;
				auto vel = (getmodifiedaimcone(0, rpc_position - actual, true)).Normalized() * original_vel.length();
				auto drag = p->drag();
				auto grav = p->gravityModifier();
				auto gravity = get_gravity();
				auto deltatime = get_deltaTime();
				auto timescale = get_timeScale();
				auto offset = -.5f;
				simulations = 0;


				travel_t = 0.f;

				while (simulations < 1200) {
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
							//Line(origin, pos, col(0, 1, 0, 1), 10.f, false, true);
							aimbot_velocity = (_aimdir).Normalized() * original_vel.length();
							//emulate 1 tick has already passed
							aimbot_velocity += gravity * grav * num;
							aimbot_velocity -= aimbot_velocity * drag * num;

							if (!skip_draw && vars->visual.tracers)
							{
								path.push_back(target_pos);
								Vector3 va = path[0];
								for (auto v : path) {
									Line(va, v, col(12, 150, 100, 50), 10.f, false, true);
									va = v;
								}
							}
							break;
						}
						else
						{
							//Line(origin, pos, col(1, 1, 1, 1), 10.f, false, true);
						}
					}
					offset += 0.05f;
					simulations++;
					if (!aimbot_velocity.is_empty())
						break;
				}
			}
		}
	}

	bool just_shot = false;
	bool did_reload = false;
	float time_since_last_shot = 0.0f;
	float fixed_time_last_shot = 0.0f;
}