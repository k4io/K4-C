#include "misc.hpp"

uintptr_t pools_offset = 0xBCA990;
uintptr_t poollist_offset = 0x6E9DF0;
/*
 /* GenericInstMethod :
	|
	|-RVA: 0xBC69B0 Offset: 0xCD8920 VA: 0x180CD9520
	|-Pool.Get<BaseMission.MissionInstance>
	|-Pool.Get<BaseVehicle.Enumerator.Box>
	|-Pool.Get<BuildingManager.Building>
	|-Pool.Get<EffectData>
	|-Pool.Get<EngineAudioClip.Grain>
	|-Pool.Get<EntityLink>
	|-Pool.Get<FoliageGroup>
	|-Pool.Get<GranularAudioClip.Grain>
	|-Pool.Get<ImpostorBatch>
	|-Pool.Get<InputMessage>
	|-Pool.Get<LODCell>
	|-Pool.Get<ListDictionary<FoliageKey, FoliageGroup>>
	|-Pool.Get<ModelState>
	|-Pool.Get<MusicClipLoader.LoadedAudioClip>
	|-Pool.Get<MusicManager.ClipPlaybackData>
	|-Pool.Get<NativeList<uint>>
	|-Pool.Get<NativeList<Vector4>>
	|-Pool.Get<Message>
	|-Pool.Get<Networkable>
	|-Pool.Get<Subscriber>
	|-Pool.Get<PlayerTick>
	|-Pool.Get<PositionLerp>
	|-Pool.Get<AIDesign>
*/
uintptr_t Method$System_Collections_Generic_List_Projectile__Clear__ = 15617184; //Method$System.Collections.Generic.List<Projectile>.Clear() (METHOD ADDRESS)

static auto _DoHit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("DoHit"), -1, _(""), _(""))));

class Projectile1;
class Projectile1 : public BaseMonoBehaviour
{
public:
	void UpdateBulletTracer(Vector3 OldPos, Vector3 NewPos, bool Update = true) {
		//Line(OldPos, NewPos, { r * 255, g * 255, b * 255, 100 }, 10.f, false, false);
		//Sphere(((Projectile*)this)->currentPosition(), .5f, { 1, 1, 1, 1 }, 10.f, 10.f);
		/*
		if (!settings.BulletTracers)
			return;

		auto _this = (Projectile*)this;
		if (!memory::IsAddressValid(_this))
			return;

		bool found = false;
		static byte ratio = 0;
		static std::vector<int> used_ids;

		for (auto& projectile : Render::GetInstance()->m_projectiles)
			if (projectile.id == _this->projectileID() && IsAlive()) {
				if (projectile.start_pos.IsEmpty())
					projectile.start_pos = CheatCore::m_cheat->LocalPlayerEyePosition;

				projectile.positions.push_back(std::make_pair(OldPos, NewPos));

				if (Update) {
					projectile.colors.push_back(RAINBOW(ratio));

					static float last_changed_color = UnityEngine::Time::get_time();
					if (UnityEngine::Time::get_time() - last_changed_color > 0.1f) {
						ratio++; if (ratio > 255) ratio = 0;
						last_changed_color = UnityEngine::Time::get_time();
					}
					found = true;
				}
				else
					projectile.colors.push_back(settings.BulletTracerColor.color);
			}

		int ID = _this->projectileID();
		if (!found && !std::count(used_ids.begin(), used_ids.end(), ID)) {
			Render::Projectile proj;
			proj.id = ID;
			used_ids.push_back(ID);
			Render::GetInstance()->m_projectiles.push_back(proj);
		}
		*/
	}

	bool IsAlive() {
		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return false;

		vars->local_player->console_echo(string::wformat(_(L"[matrix]: IsAlive - integrity: %d, traveledtime * 100: %d"), (int)(_this->integrity() * 100), (int)(_this->traveledTime() * 100)));
		return (_this->integrity() > 0.f && _this->traveledTime() < 8.f);
	}

	TraceResult1 Trace(Vector3 startDistposition, Vector3 position, Vector3 velocity, Vector3 gravity, float drag, float maxTime, float traveledTime, float startVelocityLen)
	{
		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return {};

		constexpr float num = 0.03125f;
		constexpr float projectile_forgiviness = 1.5f;
		float projectile_desync = vprojectile_desync1;
		constexpr float underground_forgiviness = 3.f;
		constexpr float projectile_bounds_padding = 0.1f;

		const Vector3 startVel = velocity;
		const Vector3 startPos = position;
		Vector3 prevPosition = position;
		float travelTime = traveledTime;

		float updateDist = 2.3f;
		TraceResult1 result;

		result.didHit = result.canHit = false;
		result.silentCat = false;
		result.hitEntity = NULL;

		if (maxTime >= 7.95f || travelTime >= 7.95f) {
			return result;
		}


		bool canHit = false;

		auto getheightmap = [&]() {
			//real rust 52698608
			uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oTerrainMeta_TypeInfo);
			uintptr_t fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
			uintptr_t heightmap = *reinterpret_cast<uintptr_t*>(fieldz + 0xB0);
			return heightmap;
		};

		auto heightMap = getheightmap();
		if (heightMap) {
			//auto RealTarget = reinterpret_cast<BasePlayerSDK*>(CheatCore::m_cheat->AimbotTarget.entity);
			auto RealTarget = vars->best_target;
			//if (memory::IsAddressValid(RealTarget))
			if (RealTarget.ent)
			{
				if (!RealTarget.ent)
					return {};
				for (int i = 0; i <= 0; i++)
				{
				START:
					position = prevPosition = startPos;
					velocity = startVel;
					travelTime = traveledTime;

					//Bounds bnds = RealTarget->GetBounds1111();
					Bounds bnds = GetBounds((uintptr_t)RealTarget.ent);
					
					//Vector3 pos = bnds._m_Center;
					Vector3 pos = bnds.m_center;

					float prevDist = position.distance(pos);

					float dst = prevDist;
					bool wantsExit = false;

					float maxHitDist = GetHitDist(min(travelTime, maxTime), (BasePlayer*)RealTarget.ent, true);
					float maxTravelDst = _this->initialDistance() + startVelocityLen * min(travelTime + num, maxTime) * projectile_forgiviness;
					float absoluteMaxDst = _this->initialDistance() + startVelocityLen * min((travelTime + (dst - maxHitDist) / 0.98f * (num * 2)), 7.95f) * projectile_forgiviness;
					//Sphere(position, maxHitDist, { 1, 0, 0, 1 }, 5.f, 100.f);
					//Sphere(position, maxTravelDst, { 0, 1, 0, 1 }, 5.f, 100.f);
					//Sphere(position, absoluteMaxDst, { 0, 0, 1, 1 }, 5.f, 100.f);
					updateDist = min(absoluteMaxDst, updateDist);
				FOR:
					bool shouldWait = false;
					for (; travelTime <= 7.95f; )
					{
						prevPosition = position;
						position += velocity * num;

						Line1111 line = Line1111(prevPosition, position);
						Vector3 nearest = line.ClosestPoint(pos);

						dst = nearest.distance(pos);

						if (projectileProtection1 >= 1 && (dst > prevDist || travelTime > maxTime + projectile_desync))
						{
							wantsExit = true;
						}
						if (wantsExit && !shouldWait && dst > maxHitDist && dst > updateDist) {
							break;
						}

						prevDist = dst;

						if (dst < maxHitDist && abs(maxTime - travelTime) < projectile_desync) {
							float hDst = startDistposition.distance(nearest);
							if (projectileProtection1 < 1.f || hDst < maxTravelDst) {
								shouldWait = false;

								if (projectileProtection1 >= 6) {
									//if (bnds.Distance(nearest) > projectile_bounds_padding) {
									if (bnds.m_center.Distance(nearest) > projectile_bounds_padding) {
										goto NOHIT;
									}
								}
								if (projectileProtection1 >= 3) {
									bool flag1 = false;
									if (heightMap && velocity.y <= 0.f) {
										//float height = heightMap->GetHeight(nearest);
										float height = terrainheightmap_GetHeight(heightMap);
										if (nearest.y + underground_forgiviness <= height) {
											if (!PLOS(startPos, nearest)) {
												break;
											}
											flag1 = true;
										}
									}

									if (!PLOS(nearest, pos)) {
										goto CONTINUE;
									}
									if (!flag1 && !PLOS(startPos, nearest)) {
										break;
									}
								}

								result.didHit = true;
								result.hitDist = hDst;
								result.hitEntity = RealTarget.ent;
								result.hitPosition = nearest;
								result.hitTime = travelTime;
								return result;
							}
							else
								shouldWait = true;
							canHit = true;
						}
						else if (dst < maxHitDist)
							canHit = true;

					NOHIT:
						//printf("dst: %.2f, updateDist: %.2f\n", dst, updateDist);
						if (dst < updateDist) {
							if (projectileProtection1 >= 3) {
								bool flag1 = false;
								//printf("hm: %s, velocity.y: %.2f\n", heightMap ? "true" : "false", velocity.y);
								if (heightMap && velocity.y <= 0.f) {
									//float height = heightMap->GetHeight(nearest);
									float height = terrainheightmap_GetHeight(heightMap);
									//printf("height: %.2f, nearest.y + underground_forgiviness: %.2f, \n", height, nearest.y + underground_forgiviness);
									if (nearest.y + underground_forgiviness <= height) {
										if (!PLOS(startPos, nearest))
										{
											//printf("no los\n");
											break;
										}
										//printf("los flag1 = true\n");
										flag1 = true;
									}
								}

								if (!PLOS(nearest, pos)) {
									//printf("no los nearest - pos\n");
									goto CONTINUE;
								}
								if (!flag1 && !PLOS(startPos, nearest)) {
									//printf("!flag1 & no los startpos - nearest\n");
									break;
								}
							}

							result.silentCat = true;
							result.hitDist = dst;
							result.hitPosition = nearest;
							result.hitEntity = RealTarget.ent;
							result.hitTime = travelTime;
							result.outVelocity = velocity;
							updateDist = dst;
						}

					CONTINUE:
						velocity += gravity * num;
						velocity -= velocity * drag * num;
						travelTime += num;
						maxHitDist = GetHitDist(min(travelTime, maxTime), (BasePlayer*)RealTarget.ent, true);
						//Sphere(position, maxHitDist, { 1, 1, 1, 1 }, 10.f, 100.f);
						maxTravelDst = _this->initialDistance() + startVelocityLen * min(travelTime, maxTime) * projectile_forgiviness;
					}
				}
			}
		}

		//		}
		//	}
		//}
		result.silentCat = result.silentCat && !canHit && result.hitEntity;
		return result;
	}

	bool DoHit(BaseEntity* at, Vector3 point, float travelTime) {
		//if (!memory::IsAddressValid(at))
		if (!at)
			return false;

		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return false;

		auto ht = (HitTest2*)_this->hitTest();
		//if (!memory::IsAddressValid(ht))
		if (!ht)
			return false;

		vars->local_player->console_echo(string::wformat(_(L"[matrix]: DoHit - Called")));
		if (ht && at) {
			//ht->DidHit() = true;
			ht->set_did_hit(true);
			//ht->HitEntity() = at;
			ht->set_hit_entity((BasePlayer*)at);

			if (at) {
				auto trans = at->model()->boneTransforms()->get(48);
				//if (!memory::IsAddressValid(ht))
				if (!ht)
					return false;

				//ht->HitTransform() = Transform;
				ht->set_hit_transform(trans);
				//ht->HitPoint() = Transform->InverseTransformPoint(point);
				//auto v = trans->InverseTransformPoint(point);
				auto v = _InverseTransformPoint(trans, point);
				vars->local_player->console_echo(string::wformat(_(L"[matrix]: DoHit - Point: (%d, %d, %d)"), v.x, v.y, v.z));
				if(!v.is_empty() && !v.is_nan())
					ht->set_hit_point(v);
				//ht->HitMaterial() = Il2CppString::create(_("flesh"));

				ht->set_hit_material(il2cpp::methods::new_string(_("flesh")));
				//ht->HitMaterial() = _(L"flesh");
			}
			else
				ht->set_hit_point(point);
			//ht->HitPoint() = point;

			//ht->AttackRay() = Ray(point, Vector3(0, 0, 0));
			ht->set_attack_ray(Ray(point, Vector3(0, 0, 0)));

			float real_travel_time = _this->traveledTime();
			//_this->traveledTime() = travelTime
			_this->traveledTime(travelTime);

			Vector3 vel = _this->currentVelocity();
			_this->currentVelocity(Vector3(0, 0, 0));


			//private bool DoHit(HitTest test, Vector3 point, Vector3 normal) { }
			//0x7A9420
			//typedef bool(*dh)(Projectile*, HitTest*, Vector3, Vector3);
			//if (((dh)(mem::game_assembly_base + 0x8ABF20))(_this, (HitTest*)ht, point, Vector3())) {
			//Sphere(point, 0.2f, { 1, 1, 1, 1 }, 10.f, 100.f);
			//static auto DoHit = *reinterpret_cast<bool(**)(Projectile * _instance, HitTest * test, Vector3 point, Vector3 normal)>(Il2CppWrapper::GetClassFromName(_(""), _("Projectile"))->GetMethodFromName(_("DoHit")));
			//__try {

			if (_DoHit(_this, (HitTest*)ht, point, Vector3::Zero())) {
				//if (Do_Hit(_this, (uintptr_t)ht, point, Vector3::Zero())) {
				_this->currentVelocity(vel);
				_this->traveledTime(real_travel_time);
				return true;
			}
			//} __except(true) { }
			return false;
		}
	}

	void CreateHT() {
		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return;

		auto hitTest = _this->hitTest();

		if (!hitTest) {
			/*HitTest_TypeInfo*/
			//DWORD64 htstatic = il2cpp::init_class(_("HitTest"), _(""));

			DWORD64 HitTest = il2cpp::methods::object_new(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + 56969464));
			//DWORD64 HitTest = il2cpp::methods::object_new(htstatic);
			_this->hitTest((DWORD64)HitTest);
			//*reinterpret_cast<uintptr_t*>((uintptr_t)_this + 0x198) = HitTest; //hittest

			//auto HitTest_TypeInfo = reinterpret_cast<uint64_t>(HitTest::StaticClass());
			//if (!HitTest_TypeInfo)
			//	return;
			//
			//auto HitTest_ = utils::CallExportByName<HitTest*>(_("il2cpp_object_new"), HitTest_TypeInfo);
			//if (!memory::IsAddressValid(HitTest_))
			//	return;
			//
			//_this->hitTest() = HitTest_;
		}
	}

	struct SendInfo
	{
		int method; // 0x10
		BYTE channel; // 0x14
		int priority; // 0x18
		void* connections; // 0x20
		uint64_t connection; // 0x28
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

	static uint64_t CreatePlayerProjectileUpdate()
	{
		vars->local_player->console_echo(_(L"[matrix]: CreatePlayerProjectileUpdate - Called"));
		typedef uint64_t(__stdcall* PoolGet)(uint64_t);
		uint64_t update = *reinterpret_cast<uint64_t*>(mem::game_assembly_base + 56727856); //"Method$Facepunch.Pool.Get<PlayerProjectileUpdate>()",

		if (!update)
			return 0;

		//public static T Get<T>() { }
		uint64_t projectileShoot = ((PoolGet)(mem::game_assembly_base + pools_offset))(update);

		if (!projectileShoot)
			return 0;

		return projectileShoot;
	}

	void SendPlayerProjectileUpdate(uint64_t update) {
		//if (!memory::IsAddressValid(update))
		if (!update)
			return;

		typedef uintptr_t(*AAA)(); // Client get_cl() { }
		auto cl = ((AAA)(mem::game_assembly_base + 0x1340260))();
		if (!cl)
			return;
		vars->local_player->console_echo(_(L"[matrix]: SendPlayerProjectileUpdate - Called"));
		
		//public NetWrite get_write() { }
		typedef uintptr_t(*AA)(uintptr_t);
		auto write = ((AA)(mem::game_assembly_base + 0x58E6F0))(cl);
		//uint64_t tod_sky = mem::read<uint64_t>(p1 + 0x28);

		//auto write = cl->write();
		//if (!memory::IsAddressValid(write))
		//	return;

		typedef bool(*netwrite_start)(uintptr_t);

		//public bool Start() { }
		//if (write->Start()) {
		if (((netwrite_start)(mem::game_assembly_base + 0x24B53E0))(write)) {

			typedef void(*netwrite_packetid)(uintptr_t, UINT);
			((netwrite_packetid)(mem::game_assembly_base + 0x24B5010))(write, (UINT)MessageType::RPCMessage);
			//write->PacketID((UINT)MessageType::RPCMessage);

			auto net = *reinterpret_cast<uintptr_t*>((uintptr_t)vars->local_player + 0x58);
			if (!net) return;
			//auto Network = CheatCore::m_cheat->LocalPlayer->net();
			//if (!memory::IsAddressValid(Network))
			//	return;

			UINT id = *reinterpret_cast<UINT*>(net + 0x10);
			//UINT id = Network->ID();

			typedef void(*netwrite_uint32)(uintptr_t, UINT);

			((netwrite_uint32)(mem::game_assembly_base + 0x24B56E0))(write, id);
			//write->UInt32(id);

			((netwrite_uint32)(mem::game_assembly_base + 0x24B56E0))(write, 2324190493); //projectile update id 2324190493
			//write->UInt32(2324190493); //projectile update

			typedef void(*serialize)(uintptr_t, uint64_t);
			((serialize)(mem::game_assembly_base + 0x20AB3D0))(write, update);
			//ProtoBuf::PlayerProjectileUpdate::Serialize(write, (ProtoBuf::PlayerProjectileUpdate*)update);

			auto conn = *reinterpret_cast<uintptr_t*>(cl + 0x28);
			if (!conn) return;
			//auto conn = ((Network::Client*)cl)->__Connection_k__BackingField();
			//if (!memory::IsAddressValid(conn))
			//	return;

			DWORD64 sistat = il2cpp::init_class(_("SendInfo"), _("Network"));
			DWORD64 si = il2cpp::methods::object_new(sistat);
			if (!si) return;
			//static auto SendInfo_ = Network::SendInfo::StaticClass();
			//if (!memory::IsAddressValid(SendInfo_))
			//	return;
			//
			//auto sendInfo = utils::CallExportByName< Network::SendInfo* >(_("il2cpp_object_new"), SendInfo_);
			//if (!memory::IsAddressValid(sendInfo))
			//	return;

			*reinterpret_cast<uintptr_t*>(si + 0x0) = 0; //method
			*reinterpret_cast<uintptr_t*>(si + 0x8) = 0; //priority
			*reinterpret_cast<uintptr_t*>(si + 0x10) = 0; //connections
			*reinterpret_cast<uintptr_t*>(si + 0x18) = conn; //connection
			//sendInfo->priority() = 0;
			//sendInfo->method() = 0;
			//sendInfo->connection() = conn;
			//sendInfo->connections() = 0;

			//public void Send(SendInfo info) { }
			typedef void(*netwrite_send)(uintptr_t, uintptr_t);
			((netwrite_send)(mem::game_assembly_base + 0x24B52C0))(write, si);
			vars->local_player->console_echo(_(L"[matrix]: SendPlayerProjectileUpdate - Finished"));
			//write->Send(sendInfo);
		}
	}

	bool SilentCat(TraceResult1& data, Vector3 LOSPoint)
	{
		vars->local_player->console_echo(_(L"[matrix]: SilentCat - Called"));
		auto _this = (Projectile*)this;
		float projectile_desync = vprojectile_desync1 - 0.05f;
		Vector3 from = data.hitPosition;
		Vector3 target = vars->best_target.pos;//GetBounds((uintptr_t)data.hitEntity).m_center;
		Vector3 initialTarget = target;
		Vector3 velocity = data.outVelocity;
		float desiredTime = data.hitTime;

		float stepSize = 0.98f;
		float needDist = from.distance(target);
		int allowedUpdates = min((int)3.f, needDist / stepSize);

		Vector3 dir = (target - from).Normalized() * stepSize;
		Vector3 movPos = from;
		Vector3 prevPos = movPos;

		float time = get_fixedTime();//UnityEngine::Time::get_realtimeSinceStartup();
		float maxTime = time - _this->launchTime();

		if (maxTime >= 7.95f) {
			_this->integrity(0.f);
			return false;
		}

		if (!g_UpdateReusable) {
			g_UpdateReusable = CreatePlayerProjectileUpdate();
		}


		((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->projectileID = _this->projectileID();
		/*
			member(int, projectileID, 0x14);
			member(Vector3, position, 0x18);
			member(Vector3, velocity, 0x24);
			member(float, traveltime, 0x30);
			*/
		//((ProtoBuf::PlayerProjectileUpdate*)g_UpdateReusable)->projectileID() = _this->projectileID();

		constexpr float num = 0.03125f;
		const float drag = _this->drag();
		Vector3 gravity = Vector3(0.f, -9.81f * _this->gravityModifier(), 0.f);

		bool first = true;
		int updates = 0;

		while (updates < allowedUpdates && movPos.distance(target) > stepSize) {
			if (RPC_Counter.Calculate() >= 190)
			{
				break;
			}
			bool velocityChanged = false;
			if (desiredTime + 0.0625f < min(maxTime + projectile_desync, 7.f))
			{
				desiredTime = desiredTime + 0.0625f;      //BRRRRRRRRRRRRRRRRR
				velocityChanged = true;
			}

			float reduceLen = 0.f;
			if (projectileProtection1 < 6)
			{
				Vector3 dir2 = (initialTarget - movPos).Normalized();
				reduceLen = GetHitDist(min(maxTime, desiredTime), (BasePlayer*)data.hitEntity, true);
				reduceLen = min(reduceLen, (initialTarget - from).Length());
				target = initialTarget - dir2 * reduceLen;
			}

			bool done = false;
			float pdst = movPos.distance(target);
			if (pdst == 0.f) {
				break;
			}
			else if (pdst < stepSize) {
				movPos = target + (initialTarget - target).Normalized() * min(stepSize - pdst, reduceLen);
				done = true;
			}
			else {
				movPos = movPos + dir;
			}

			if (first) {
				if (!PLOS(LOSPoint, movPos)) {
					movPos = from;
				}
			}
			first = false;

			if (velocityChanged) {
				Vector3 position = prevPos;
				Vector3 prevPosition, prevVelocity;
				for (int i = 0; i < 2; i++)
				{
					prevPosition = position;
					prevVelocity = velocity;
					position += velocity * num;
					velocity += gravity * num;
					velocity -= velocity * drag * num;
				}
				Vector3 b = prevVelocity * num;
				Line1111 line = Line1111(prevPosition - b, position + b);
				Vector3 closest = line.ClosestPoint(movPos);
				Vector3 c = movPos - closest;
				float len = c.Length();
				if (len == 0.f)
					movPos = closest;
				else
					movPos = closest + (c / len) * min(len, stepSize);
			}

			//Sphere(movPos, 0.5f, { 1, 0, 0, 1 }, 10.f, 1);
			//Line(movPos, prevPos, { 0, 1, 0, 1 }, 10.f, 1, 1);
			prevPos = movPos;

			((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->position = movPos;
			((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->velocity = velocity;
			((protobuf::PlayerProjectileUpdate*)g_UpdateReusable)->traveltime = desiredTime;

			//((ProtoBuf::PlayerProjectileUpdate*)g_UpdateReusable)->curPosition() = movPos;
			//((ProtoBuf::PlayerProjectileUpdate*)g_UpdateReusable)->curVelocity() = velocity;
			//((ProtoBuf::PlayerProjectileUpdate*)g_UpdateReusable)->travelTime() = desiredTime;

			SendPlayerProjectileUpdate(g_UpdateReusable);
			RPC_Counter.Increment();

			_this->sentPosition(movPos);
			_this->previoustraveledTime(desiredTime);
			//_this->previousTraveledTime() = desiredTime;
			_this->initialVelocity(velocity);
			//_this->initialVelocity() = velocity;

			data.hitTime = desiredTime;

			_this->sentTraveledTime(desiredTime);
			//_this->closeFlybyDistance() = desiredTime;
			++updates;
			if (done)
				break;
		}
	END:
		bool flag = false;
		float leftDist = min(movPos.distance(target), (initialTarget - movPos).Length());
		if (leftDist < stepSize) {
			data.hitPosition = movPos = movPos + (initialTarget - movPos).Normalized() * leftDist;
			flag = true;
		}
		return flag;
	}

	void DoRealMovement()
	{
		vars->local_player->console_echo(_(L"[matrix]: DoRealMovement - Called"));
		auto _this = (Projectile*)this;
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		if (!_this) return;
		constexpr float num = 0.03125f;
		constexpr float projectile_forgiviness = 1.5f;
		constexpr float bulletUpdateRate = 0.0625f;
		float projectile_desync = vprojectile_desync1;

		Vector3 hitPosition = Vector3(-1, -1, -1);
		Vector3 position = _this->sentPosition();
		Vector3 startLOSPosition = position;
		//Vector3 startPosition = _this->pre();
		Vector3 startPosition = _this->tumbleAxis();
		float startVelocityLen = *reinterpret_cast<float*>((uintptr_t)_this + 0x90); //flybySoundDistance
		//float startVelocityLen = _this->flybySoundDistance();
		Vector3 velocity = _this->initialVelocity();
		Vector3 gravity = Vector3(0.f, -9.81f * _this->gravityModifier(), 0.f);
		Vector3 prevPosition = position;
		const float drag = _this->drag();

		Vector3 CurrentProjectilePosition = _this->currentPosition();

		//Line(CurrentProjectilePosition, prevPosition, { 1, 1, 1, 1 }, 10.f, true, true);

		_this->traveledTime(_this->traveledTime() + num);
		 
		float travelTime = _this->previoustraveledTime();

		bool flag = false;
		float time = get_fixedTime();//UnityEngine::Time::get_realtimeSinceStartup();
		float maxTime = time - _this->launchTime();

		//printf("result\ndidHit: %s\nprevPosition: (%.2f, %.2f, %.2f)\nposition: (%.2f, %.2f, %.2f)\nvelocity: (%.2f, %.2f, %.2f)\ngravity: (%.2f, %.2f, %.2f)\ndrag: %.2f\nmaxTime: %.2f\ntravelTime: %.2f\nstartVelLen: %.3f\n",
		//	false ? "true" : "false",
		//	prevPosition.x, prevPosition.y, prevPosition.z,
		//	position.x, position.y, position.z,
		//	velocity.x, velocity.y, velocity.z,
		//	gravity.x, gravity.y, gravity.z,
		//	drag, maxTime, travelTime, startVelocityLen);
		//printf("realtime: %.2f\ntime: %.2f\nlaunchTime: %.2f\n", get_fixedTime(), time, _this->launchTime());
		if (maxTime >= 7.95f)
		{
			vars->local_player->console_echo(string::wformat(_(L"[matrix]: UpdateVelocity - MaxTime > 7.95f, Integrity: %d"), (int)(_this->integrity() * 100)));
			_this->integrity(0.f);
			return;
		}

		//TraceResult1 result = this->Trace(startPosition, position, velocity, gravity, drag, maxTime, travelTime, startVelocityLen);
		TraceResult1 result = this->Trace(prevPosition, position, velocity, gravity, drag, maxTime, travelTime, startVelocityLen);

		if (result.didHit)
		{
		HIT:
			vars->local_player->console_echo(_(L"[matrix]: DoRealMovement - didHit"));
			_this->traveledDistance(result.hitDist);

			//result.hitPosition = result.hitEntity->model()->boneTransforms()->get(48)->position();
			result.hitPosition = result.hitEntity->model()->boneTransforms()->get(48)->position();
			//if (settings.AimbotCircle)
			//	result.hitPosition = result.hitEntity->model()->boneTransforms()->get(47)->_get_position();

			UpdateBulletTracer(_this->currentPosition(), result.hitPosition);

			flag = DoHit(result.hitEntity, result.hitPosition, result.hitTime);

			auto Transform = result.hitEntity->transform();

			hitPosition = Transform->position();//transform->get_bone_position();
		}
		else if (result.silentCat && result.hitEntity)
		{
			bool canHit = SilentCat(result, position);
			//Line(result.hitPosition, position, { 0, 1, 0, 100 }, 10.f, 1, 1);
			//Line(result.hitPosition, startPosition, { 1, 1, 0, 100 }, 10.f, 1, 1);
			//DDraw::Text(_(L"SilentCat"), result.hitPosition, { r, g, b, 255 }, 10.f);
			vars->local_player->console_echo(string::wformat(_(L"[matrix]: DoRealMovement - SilentCat: %d"), (int)canHit));
			if (canHit && abs(result.hitTime - maxTime) <= projectile_desync) {
				float maxTravelDst = _this->initialDistance() + startVelocityLen * min(result.hitTime, maxTime) * projectile_forgiviness;
				if ((result.hitDist = startPosition.distance(result.hitPosition)) < maxTravelDst) {
					result.didHit = true;

					goto HIT;
				}
			}
		}

		Vector3 prev = _this->previousPosition();
		Vector3 vel = _this->previousVelocity();

		Vector3 newPos = prev + vel * num;
		//DDraw::Text(_(L"DoRealMovement"), newPos, { r, g, b, 255 }, 10.f);
		vars->local_player->console_echo(string::wformat(_(L"[matrix]: DoRealMovement - Pos: (%d, %d, %d), newPos: (%d, %d, %d)"),
			(int)prev.x,
			(int)prev.y,
			(int)prev.z,
			(int)newPos.x,
			(int)newPos.y,
			(int)newPos.z
		));

		//Line(prev, newPos, { 1, 1, 1, 1 }, 10.f, 1, 1);

		vel += gravity * num;
		vel -= vel * drag * num;
		_this->previousPosition(newPos);
		_this->previousVelocity(vel);
		position = newPos;

		_this->currentVelocity(_this->previousVelocity());

		if (flag && !(hitPosition.x == hitPosition.y && hitPosition.x == hitPosition.z && hitPosition.x == -1)) {
			vars->local_player->console_echo(_(L"[matrix]: DoRealMovement - Hit 1"));
			_this->currentPosition(hitPosition);
		}
		else {
			_this->currentPosition(position);
		}

		if (flag) {
			vars->local_player->console_echo(_(L"[matrix]: DoRealMovement - Hit 2"));
			_this->integrity(0.f);
		}
	}

	void UpdateVelocity()
	{
		vars->local_player->console_echo(_(L"[matrix]: UpdateVelocity - Called"));
		auto _this = (Projectile*)this;
		auto Transform = _this->transform();

		if (Transform) {
			
			Vector3 pos = Transform->position();
			//Sphere(pos, .3f, { 1, 1, 0, 1 }, 10.f, false);
			//Line(pos, _this->currentPosition(), { r, g, b, 100 }, 10.f, 1, 1);
			_this->currentPosition(pos);
			//Sphere(_this->currentPosition(), 1.f, { 1, 1, 0, 1 }, 10.f, false);

			if (_this->traveledTime() == 0.f)
			{
				//i handle manipulation from projectileshoot rpc, maybe change?
				//if (misc::best_lean != Vector3(0, 0, 0))
				//	pos = misc::best_lean;
					//pos = CheatCore::m_cheat->RealGangstaShit;

				//that was a lie this manipulator is different fuck me sideways
				if (settings::RealGangstaShit != Vector3(0, 0, 0))
					pos = settings::RealGangstaShit;

				//_this->currentPosition() = pos;
				_this->currentPosition(pos);

				float time1 = get_fixedTime();//get_fixedTime();//UnityEngine::Time::get_realtime SinceStartup();
				_this->launchTime(time1);
				_this->sentTraveledTime(0.f);
				_this->sentPosition(pos);
				_this->prevSentVelocity(pos);
				_this->previousPosition(pos);
				_this->previousVelocity(_this->initialVelocity());

				Vector3 InitialVel = _this->initialVelocity();
				*reinterpret_cast<float*>((uintptr_t)_this + 0x90) = InitialVel.Length();
				//_this->flybySoundDistance() = InitialVel.Length();
				_this->previoustraveledTime(0.f);
				_this->needsLOS(false);
				*reinterpret_cast<bool*>((uintptr_t)_this + 0xf8) = false;
				//_this->clientsideEffect() = false;
				//printf("projectile has been reset to 0 from update velocity\n");
				CreateHT();
			}

			_this->SetEffectScale(0.f); //??

			DoRealMovement();

			Vector3 curPos = _this->currentPosition();

			UpdateBulletTracer(pos, curPos);

			set_position(Transform, curPos);
			//Transform->_set_position(curPos);

			Vector4 rotation = LookRotation(_this->currentVelocity(), Vector3(0, 1, 0));
			//Quaternion rotation = UnityEngine::Quaternion::LookRotation(_this->currentVelocity());
			set_rotation(Transform, rotation);
			//Transform->_set_rotation(rotation);

			Vector3 SentPos = _this->sentPosition();

			if (SentPos.distance(curPos) > 10.f && !_this->needsLOS())
			{
				_this->SetEffectScale(1.f);
			}
		}
	}

	void Launch1() {
		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return;

		auto LocalPlayer = vars->local_player;
		//auto LocalPlayer = CheatCore::m_cheat->LocalPlayer;
		if (!LocalPlayer)
		//if (!memory::IsAddressValid(_this))
			return;

		//if (!_this->authoritative())
		//	return _this->Launch();
		LocalPlayer->console_echo(_(L"[matrix]: Launch1 - Called"));
		if (vars->combat.targetbehindwall) {
			while (_this->IsAlive() && _this->traveledTime() < 0.09f)
				this->UpdateVelocity();
			LocalPlayer->console_echo(_(L"[matrix]: Launch1 - Finished launch"));
		}
		else {	
			_this->ricochetChance(0.f);
			return _this->Launch();
		}
	}

	void Update() {
		do
		{	
			vars->local_player->console_echo(_(L"[matrix]: Update - Called"));
			auto _this = (Projectile*)this;
			if (!_this)
				break;

			BasePlayer* owner = _this->owner();
			if (!owner)
				break;

			auto LocalPlayer = vars->local_player;
			if (!LocalPlayer)
				break;

			if (!this->IsAlive() || _this->projectileID() == 0 || !owner) {
				_this->Retire();
				break;
			}

			float bulletUpdateRate = 0.01f;
			//float bulletUpdateRate = 1.01f;
			if (_this->authoritative() && vars->combat.targetbehindwall) {
				while (this->IsAlive()) {
					float time = get_fixedTime();//get_fixedTime();//UnityEngine::Time::get_realtimeSinceStartup();
					vars->local_player->console_echo(_(L"[matrix]: Update - updating"));
					if (time - _this->launchTime() < _this->traveledTime() + bulletUpdateRate) {
						//vars->local_player->console_echo(string::wformat(_(L"[matrix]: Update - breaking, time: %d, launchTime: %d, traveled: %d"), (int)(time * 100), (int)(_this->launchTime() * 100), (int)(_this->traveledTime() * 100)));
						break;
					}
					UpdateVelocity();
				}
				vars->local_player->console_echo(string::wformat(_(L"[matrix]: Update - Not alive, integrity: %d"), (int)_this->integrity()));
			}
			else
				return _update((Projectile*)this);
			//return Hooks::ProjectileUpdateHk.get_original<decltype(&Hooks::_Update)>()(this);

		} while (0);

		vars->local_player->console_echo(_(L"[matrix]: Update - Sending normal update"));
		return _update((Projectile*)this);//hooks::orig::_update((Projectile*)this);//Hooks::ProjectileUpdateHk.get_original<decltype(&Hooks::_Update)>()(this);
	}

	static float GetHitDist(float travel, BasePlayer* target, bool ent) {
		if (!target)
			return 0.f;

		auto LocalPlayerBase = vars->local_player;//CheatCore::m_cheat->LocalPlayer;
		if (!LocalPlayerBase)
			return 0.f;

		float time = unity::get_realtimesincestartup();//UnityEngine::Time::get_realtimeSinceStartup();
		float timeSinceLastTick = time - LocalPlayerBase->lastSentTickTime();
		float timeSinceLastTickClamped = max(0.f, min(timeSinceLastTick, 1.f));

		bool mountedplayer = target->mounted() ? 1 : 0;

		float Velocity = 0.f;
		if (mountedplayer && target && !ent)
			Velocity = GetMountedVelocity(target);

		else if (!ent && !mountedplayer)
			Velocity = 95.f;

		float maxDist = 0.1f + (timeSinceLastTickClamped + 2.f / 60.f) * 1.5f * Velocity;
		//Sphere(((Projectile*)this)->currentPosition(), )

		if (projectileProtection1 >= 6) {
			if (ent && !mountedplayer)
				return min(1.4f, 1.4f);

			else if (mountedplayer && target) {

				BaseMountable* mounted = target->mounted();
				if (mounted) {

					auto realM = get_parent_entity((uintptr_t)mounted);

					if (!realM)
						return 0.f;

					auto name = get_short_prefab_name((uintptr_t)realM).str;//realM->_get_ShortPrefabName()->str();

					bool Ten = !wcscmp(name, _(L"minicopter.entity")) || !wcscmp(name, _(L"scraptransporthelicopter"));
					bool Six = std::wstring(name).find(L"car") != std::wstring::npos || !wcscmp(name, _(L"testridablehorse")) || !wcscmp(name, _(L"rowboat")) || !wcscmp(name, _(L"rhib"));

					if (Ten)
						if (maxDist > 10.f)
							return maxDist;
						else
							return 10.f;

					else if (Six) {
						if (maxDist > 6.f)
							return maxDist;
						else
							return 6.f;
					}
				}
				else {
					if (maxDist > 15.f)
						return maxDist;
					else
						return 15.f;
				}
			}
		}
	};
};
