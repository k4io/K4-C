#include "RenderClass.h"
#include "vars.hpp"
#include "skinstr.h"
#include "rust/features/player_esp.hpp"
#include "inttypes.h"

#define d2c(col) D2D1::ColorF(col.r, col.g, col.b, col.a)

//lua wrapper for functions
namespace lw {
	std::vector<BasePlayer*> playerlist{};
	std::vector<BaseEntity*> entitylist{};

	class Entity {
	public:
		BaseEntity* self;

		Vector3 GetPosition() {
			if (!self) return {};
			return self->transform()->position();
		}

		void Rpc(std::string s) {
			if (!self) return;
			return self->ServerRPCs(s.c_str());
		}

		std::string GetClassname() {
			if (!self) return "";
			return self->get_class_name();
		}

		Entity(BaseEntity* e) 
			: self(e) { }

		Entity(int i) :
			self(lw::entitylist[i]) { }
	};

	class Eyes {
	public:
		PlayerEyes* self;

		Vector3 GetPosition() {
			if (!self) return {};
			return self->position();
		}

		Vector4 GetRotation() {
			if (!self) return {};
			return self->rotation();
		}

		Vector3 BodyForward() {
			if (!self) return {};
			return self->body_forward();
		}

		Vector3 BodyRight() {
			if (!self) return {};
			return self->body_right();
		}

		Vector3 GetViewOffset() {
			if (!self) return {};
			return self->get_view_offset();
		}

		void SetViewOffset(Vector3 v) {
			if (!self) return;
			return self->set_view_offset(v);
		}

		Eyes(PlayerEyes* e) 
			: self(e) { }
	};

	class Player {
	public:
		BasePlayer* self;

		void SetTargetMovement(Vector3 target) {
			if (!self) return;
			self->movement()->set_TargetMovement(target);
		}

		Vector3 GetTargetMovement() {
			if (!self) return {};
			return self->movement()->get_TargetMovement();
		}

		bool IsNpc() {
			if (!self) return false;
			return self->playerModel()->isnpc();
		}

		bool IsAlive() {
			if (!self) return false;
			return self->is_alive();
		}

		int GetHealth() {
			if (!self) return 0.f;
			return (int)self->health();
		}

		int GetUserId() {
			if (!self) return -1;
			return self->userID();
		}

		void Rpc(std::string s) {
			if (!self) return;
			return self->ServerRPCs(s.c_str());
		}

		std::string GetName() {
			if (!self) return "";
			auto w = std::wstring(self->get_player_name());
			return std::string(w.begin(), w.end());
		}

		Vector3 GetBonePos(int id) {
			if (!self) return Vector3::Zero();
			return self->get_bone_transform(id)->position();
		}

		bool IsLocalPlayer() {
			if (!self) return false;
			return self->is_local_player();
		}

		bool IsFriend() {
			if (!self) return false;
			if (map_contains_key(vars->gui_player_map, self->userID()))
				return vars->gui_player_map[self->userID()]->is_friend;
			return false;
		}

		bool IsTeammate(int lp) {
			auto lpc = lw::playerlist[lp];
			if (!self || !lpc) return false;
			return self->is_teammate(lpc);
		}

		void SetPlayerFlag(int f) {
			if (!self) return;
			return self->set_player_flag((rust::classes::PlayerFlags)f);
		}

		//Eyes GetEyes() {
		//	if (!self) return Eyes();
		//	return Eyes(self->eyes());
		//}

		Player(BasePlayer* e)
			: self(e) { }

		Player(int i)
			: self(lw::playerlist[i]) {}
	};

	struct color {
		float r, g, b, a;
	};

	namespace draw {
		void Rect(Vector2 a, Vector2 b, color c, float thickness) {
			render.Rectangle(a, b, d2c(c), thickness);
		}
		void FillRect(Vector2 a, Vector2 b, color c) {
			render.FillRectangle(a, b, d2c(c));
		}
		void Circle(Vector2 a, float radius, color c, float thickness) {
			render.Circle(a, d2c(c), radius, thickness);
		}
		void FillCircle(Vector2 a, float radius, color c) {
			render.FillCircle(a, d2c(c), radius);
		}
		void Text(Vector2 a, std::string s, color c, bool outline) {
			auto ws = std::wstring(s.begin(), s.end());
			render.String(a, ws.c_str(), d2c(c), outline);
		}
		void TextCentered(Vector2 a, std::string s, color c, bool outline) {
			auto ws = std::wstring(s.begin(), s.end());
			render.StringCenter(a, ws.c_str(), d2c(c), outline);
		}
		void Line(Vector2 a, Vector2 b, color c, float t) {
			render.Line(a, b, d2c(c), t);
		}
		void Line3d(Vector3 a, Vector3 b, color c, float t) {
			::Line(a, b, { c.r, c.g, c.b, c.a }, t, false, false);
		}
		void Sphere3d(Vector3 p, float f, color c, float t) {
			::Sphere(p, f, { c.r, c.g, c.b, c.a }, t, false);
		}
	}

	namespace entities {
		Player GetPlayer(int idx) {
			return Player(lw::playerlist[idx]);
		}

		Entity GetEntity(int idx) {
			return Entity(idx);
		}

		static int PlayerListSize() {
			return playerlist.size();
		}

		static int EntityListSize() {
			return entitylist.size();
		}
	}

	namespace misc {
		Vector2 w2s(Vector3 position) {
			Vector2 out{};
			esp::out_w2s(position, out);
			return out;
		}

		bool LineOfSight(Vector3 a, Vector3 b, float radius) {
			return unity::LineOfSightRadius(a, b, 0, radius);
		}

		bool IsKeyPressed(int vk) {
			return unity::GetKey(vk);
		}

		float TimeSinceStartup() {
			return unity::get_realtimesincestartup();
		}

		float FixedTime() {
			return get_fixedTime();
		}
	}
}