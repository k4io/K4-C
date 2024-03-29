#pragma once
#include <stdio.h>
#include <vector>
#include <map>

#include <d2d1.h>
#include "rust.hpp"
#include "../utils/xor_float.hpp"
#include "../utils/string_format.h"
#include "../offsets.h"
#include "unity.hpp"
#include "../vars.hpp"
//#define pent printf("entered function: " __FUNCTION__ "!\n");
#define pent ;

#define safe_read(Addr, Type) mem::read<Type>((DWORD64)Addr)
#define safe_write(Addr, Data, Type) mem::write<Type>((DWORD64)Addr, Data); 


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

float r, g, b;

inline float NormalizeAngle(float angle) {
	while (angle > 360.0f) {
		angle -= 360.0f;
	}
	while (angle < 0.0f) {
		angle += 360.0f;
	}
	return angle;
}

inline Vector3 NormalizeAngles(Vector3 angles) {
	angles.x = NormalizeAngle(angles.x);
	angles.y = NormalizeAngle(angles.y);
	angles.z = NormalizeAngle(angles.z);
	return angles;
}

inline Vector3 EulerAngles(Vector4 q1) {
	float num = q1.w * q1.w;
	float num2 = q1.x * q1.x;
	float num3 = q1.y * q1.y;
	float num4 = q1.z * q1.z;
	float num5 = num2 + num3 + num4 + num;
	float num6 = q1.x * q1.w - q1.y * q1.z;
	Vector3 vector;
	if (num6 > 0.4995f * num5) {
		vector.y = 2.0f * std::atan2f(q1.y, q1.x);
		vector.x = 1.57079637f;
		vector.z = 0.0f;
		return NormalizeAngles(vector * 57.2958f);
	}
	if (num6 < -0.4995f * num5) {
		vector.y = -2.0f * std::atan2f(q1.y, q1.x);
		vector.x = -1.57079637f;
		vector.z = 0.0f;
		return NormalizeAngles(vector * 57.2958f);
	}
	Vector4 quaternion = Vector4(q1.w, q1.z, q1.x, q1.y);
	vector.y = std::atan2f(2.0f * quaternion.x * quaternion.w + 2.0f * quaternion.y * quaternion.z, 1.0f - 2.0f * (quaternion.z * quaternion.z + quaternion.w * quaternion.w));
	vector.x = std::asin(2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y));
	vector.z = std::atan2f(2.0f * quaternion.x * quaternion.y + 2.0f * quaternion.z * quaternion.w, 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z));
	return NormalizeAngles(vector * 57.2958f);
}

//PASTE XD
struct BulletDropPredictionData
{
	float distCoeff;
	float startY;
	float yCoeff;
};
BulletDropPredictionData bulletDropData[51];

struct rust_str
{
	char zpad[128];
};

class default_t
{
public:
	template<typename T>
	operator T() const { return T(); }
};
default_t const defaultt = default_t();
#define NP(type) type nonptr = defaultt; if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nonptr;
//#define FIELD(klass,tr,name,type) type& name() { \
//	NP(type) \
//	static auto off = il2cpp::value(klass, tr); \
//	return *reinterpret_cast<type*>(this + off); }
#define FIELD(offset,name,type) type& name() { \
	NP(type) \
	return *reinterpret_cast<type*>((uintptr_t)this + offset); }
	//return (mem::read<type>((uintptr_t)this + offset)); }	

template<typename T>
void pop_front(std::vector<T>& vec)
{
	if (vec.empty()) return;
	vec.front() = std::move(vec.back());
	vec.pop_back();
}

struct bounds_t {
	float left, right, top, bottom, center;
	bool empty() {
		if (left == FLT_MAX
			|| right == FLT_MIN
			|| top == FLT_MAX
			|| bottom == FLT_MIN)
			return true;
		return false;
	}
};

class Bounds {
public:
	Vector3 m_center;
	Vector3 m_extents;
	Vector3 center;
	Vector3 extents;
	Vector3 max;
	Vector3 min;
	Vector3 size;
};

enum BaseCombatEntity_LifeState {
	Alive = 0,
	Dead = 1
};


#pragma region il2
uintptr_t planner_rotationoffset = il2cpp::value(_("Planner"), _("rotationOffset"));
uintptr_t planner_currentconstruction = il2cpp::value(_("Planner"), _("currentConstruction"));
uintptr_t planner_guide = il2cpp::value(_("Planner"), _("guide"));

uintptr_t _displayName = il2cpp::value(_("BasePlayer"), _("_displayName"));
uintptr_t woundedlol = il2cpp::value(_("BasePlayer"), _("Wounded"));
uintptr_t movement = il2cpp::value(_("BasePlayer"), _("movement"));
uintptr_t player_inventory = il2cpp::value(_("BasePlayer"), _("inventory"));
uintptr_t clActiveItem = il2cpp::value(_("BasePlayer"), _("clActiveItem"));
uintptr_t playerModel = il2cpp::value(_("BasePlayer"), _("playerModel"));
uintptr_t clientTeam = il2cpp::value(_("BasePlayer"), _("clientTeam"));
uintptr_t playerFlags = il2cpp::value(_("BasePlayer"), _("playerFlags"));
uintptr_t eyes = il2cpp::value(_("BasePlayer"), _("eyes"));
uintptr_t _lastSentTickTime = il2cpp::value(_("BasePlayer"), _("lastSentTickTime"));
uintptr_t modelstate = il2cpp::value(_("BasePlayer"), _("ModelState"));
uintptr_t input = il2cpp::value(_("BasePlayer"), _("input"));
uintptr_t containerBelt = il2cpp::value(_("PlayerInventory"), _("containerBelt"));
uintptr_t containerWear = il2cpp::value(_("PlayerInventory"), _("containerWear"));
uintptr_t itemList = il2cpp::value(_("ItemContainer"), _("itemList"));

uintptr_t building_grade = il2cpp::value(_("BuildingBlock"), _("grade"));

uintptr_t numprojectiles = il2cpp::value(_("ItemModProjectile"), _("numProjectiles"));

uintptr_t damageProperties = il2cpp::value(_("BaseMelee"), _("damageProperties"));
uintptr_t newVelocity = il2cpp::value(_("PlayerModel"), _("newVelocity"));
uintptr_t isLocalPlayer = il2cpp::value(_("PlayerModel"), _("isLocalPlayer"));

uintptr_t _health = il2cpp::value(_("BaseCombatEntity"), _("_health"));
uintptr_t _maxHealth = il2cpp::value(_("BaseCombatEntity"), _("_maxHealth"));
uintptr_t lifestate = il2cpp::value(_("BaseCombatEntity"), _("lifestate"));

uintptr_t net = il2cpp::value(_("BaseNetworkable"), _("net"));

uintptr_t heldEntity = il2cpp::value(_("Item"), _("heldEntity"));

uintptr_t canWieldItems = il2cpp::value(_("BaseMountable"), _("canWieldItems"));

uintptr_t successFraction = il2cpp::value(_("FlintStrikeWeapon"), _("successFraction"));
uintptr_t didSparkThisFrame = il2cpp::value(_("FlintStrikeWeapon"), _("_didSparkThisFrame"));

uintptr_t currentVelocity = il2cpp::value(_("Projectile"), _("currentVelocity"));
uintptr_t currentPosition = il2cpp::value(_("Projectile"), _("currentPosition"));
uintptr_t thickness_addr = il2cpp::value(_("Projectile"), _("thickness"));
uintptr_t traveledTime = il2cpp::value(_("Projectile"), _("traveledTime"));
uintptr_t isAuthoritative = il2cpp::value(_("Projectile"), _("isAuthoritative"));

uintptr_t mod = il2cpp::value(_("Projectile"), _("mod"));
uintptr_t hitTest = il2cpp::value(_("Projectile"), _("hitTest"));

uintptr_t weakspots = il2cpp::value(_("BaseHelicopter"), _("weakspots"));

uintptr_t info = il2cpp::value(_("Item"), _("info"));

uintptr_t iconSprite = il2cpp::value(_("ItemDefinition"), _("iconSprite"));
uintptr_t steamItem = il2cpp::value(_("ItemDefinition"), _("steamItem"));

uintptr_t category = il2cpp::value(_("ItemDefinition"), _("category"));
uintptr_t itemid = il2cpp::value(_("ItemDefinition"), _("itemid"));
uintptr_t displayName = il2cpp::value(_("ItemDefinition"), _("displayName"));

uintptr_t viewOffset = il2cpp::value(_("PlayerEyes"), _("viewOffset"));
uintptr_t eyepos = il2cpp::value(_("PlayerEyes"), _("position"));

uintptr_t swimming = il2cpp::value(_("PlayerWalkMovement"), _("swimming"));

uintptr_t ducking = il2cpp::value(_("BaseMovement"), _("<Ducking>k__BackingField"));

uintptr_t TargetMovement = il2cpp::value(_("BaseMovement"), _("<TargetMovement>k__BackingField"));

uintptr_t lastHitTime = il2cpp::value(_("Chainsaw"), _("lastHitTime"));
uintptr_t lastHitMaterial = il2cpp::value(_("Chainsaw"), _("lastHitMaterial"));

uintptr_t integrity = il2cpp::value(_("Projectile"), _("integrity"));

uintptr_t groundAngleNew = il2cpp::value(_("PlayerWalkMovement"), _("groundAngleNew"));
uintptr_t landTime = il2cpp::value(_("PlayerWalkMovement"), _("landTime"));
uintptr_t jumpTime = il2cpp::value(_("PlayerWalkMovement"), _("jumpTime"));
uintptr_t groundTime = il2cpp::value(_("PlayerWalkMovement"), _("groundTime"));
uintptr_t gravityMultiplier = il2cpp::value(_("PlayerWalkMovement"), _("gravityMultiplier"));
uintptr_t flying = il2cpp::value(_("PlayerWalkMovement"), _("flying"));

uintptr_t model = il2cpp::value(_("BaseEntity"), _("model"));
#pragma endregion

class Object;
class Component;
class Behaviour;
class FacepunchBehaviour;
class BaseMonoBehaviour;
class MonoBehaviour;
class Networkable;
class BaseNetworkable;
class GameObject;
class Transform;
class BaseEntity;
class Item;
class AttackEntity;
class BaseMelee;
class BaseProjectile;
class Bone;
class BoneCache;
class Model;
class ModelState;
class BasePlayer;
class HitTest;
class Shader;
class Material;
class Renderer;
class PlayerBelt;
class Terrain;
class Projectile;
class PlayerInventory;
class Magazine;
class TerrainHeightMap;
class TerrainCollision;
class TerrainMeta;
class InputState;
class ViewModel;
class SkinnedMultiMesh;
class Collider;
class CapsuleCollider;
class PlayerVoiceRecorder;
class BaseMovement;
class RaycastHit;
class BaseMountable;
class BaseVehicle;
class TimeWarning;
class col;

std::map<uint64_t, BoneCache*> cachedBones = std::map<uint64_t, BoneCache*>();
std::vector<BasePlayer*> player_list = {};
std::map<int32_t, BasePlayer*> player_map = {};
//struct hit {
//	Vector3 position;
//	float time;
//};
//std::vector<hit> hitpoints = {};


typedef struct Str
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *str;

#pragma region il2func
//static auto ServerRPC_intstring = reinterpret_cast<void (*)(BaseEntity*, System::string, unsigned int, System::string, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uintstring_);

//static auto setrayleigh = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Weather"), _("set_atmosphere_rayleigh"), 0, _(""), _(""))));
static auto viewmodelplay = reinterpret_cast<void(*)(uintptr_t, System::string, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ViewModel"), _("Play"), 2, _(""), _(""))));

static auto set_ambientintensity = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("RenderSettings"), _("set_ambientIntensity"), 1, _(""), _("UnityEngine"))));

static auto set_ambientlight = reinterpret_cast<void(*)(uintptr_t, col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Sky"), _("set_AmbientColor"), 1, _(""), _(""))));

static auto getmountedvehicle = reinterpret_cast<BaseVehicle * (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMountedVehicle"), 0, _(""), _(""))));

static auto getmounted = reinterpret_cast<BaseMountable * (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMounted"), 0, _(""), _(""))));

static auto raycasthit_get_transform = reinterpret_cast<Transform * (*)(RaycastHit*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("RaycastHit"), _("get_transform"), 0, _(""), _("UnityEngine"))));

static auto bmdoattack = reinterpret_cast<void(*)(BaseMelee*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("DoAttack"), 0, _(""), _(""))));

static auto dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("DontDestroyOnLoad"), 0, _(""), _("UnityEngine"))));

static auto create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_CreateGameObject"), 0, _(""), _("UnityEngine"))));

static auto add_component = reinterpret_cast<Component * (*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_AddComponentWithType"), 0, _(""), _("UnityEngine"))));

static auto hasammo = reinterpret_cast<bool(*)(PlayerInventory*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerInventory"), _("HasAmmo"), 0, _(""), _(""))));

static auto getrenderers = reinterpret_cast<System::list<Renderer*>*(*)(SkinnedMultiMesh*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinnedMultiMesh"), _("get_Renderers"), 0, _(""), _(""))));

static auto setmaterial = reinterpret_cast<void(*)(Renderer*, Material*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("set_material"), 0, _(""), _("UnityEngine"))));

static auto getmaterial = reinterpret_cast<Material * (*)(Renderer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("get_material"), 0, _(""), _("UnityEngine"))));

static auto setfloat = reinterpret_cast<void(*)(Material*, System::string, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetFloat"), 2, _("name"), _("UnityEngine"), 1)));

static auto setcolor = reinterpret_cast<void(*)(Material*, System::string, col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetColor"), 2, _("name"), _("UnityEngine"), 1)));

static auto setshader = reinterpret_cast<void(*)(Material*, Shader*)>(il2cpp::methods::resolve_icall(_("UnityEngine.Material::set_shader()")));

static auto getshader = reinterpret_cast<Shader * (*)(Material*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("get_shader"), 0, _(""), _("UnityEngine"))));

static auto canhit = reinterpret_cast<bool (*)(BaseMelee*, HitTest*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("CanHit"), 1, _(""), _(""))));

static auto serverrpc = reinterpret_cast<void (*)(BaseEntity*, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("ServerRPC"), 1, _("funcName"), _(""), 1)));

static auto getparentvel = reinterpret_cast<Vector3(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("GetParentVelocity"), 0, _(""), _(""))));

static auto sendprojupdate = reinterpret_cast<void(*)(BasePlayer*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("SendProjectileUpdate"), 0, _(""), _(""))));

static auto gamephystrace = reinterpret_cast<bool(*)(Ray r, float f, RaycastHit * r2, float f2, int layer, int querytriggerinteraction)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GamePhysics"), _("Trace"), 0, _(""), _(""))));

static auto get_localscale = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_localScale"), 0, _(""), _("UnityEngine"))));

static auto set_localscale = reinterpret_cast<void(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_localScale"), 1, _(""), _("UnityEngine"))));

static auto launchproj = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("LaunchProjectile"), 0, _(""), _(""))));

static auto capgetheight = reinterpret_cast<float(*)(CapsuleCollider*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("get_height"), 0, _(""), _("UnityEngine"))));

static auto capsetheight = reinterpret_cast<void(*)(CapsuleCollider*, float f)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("set_height"), 1, _(""), _("UnityEngine"))));

static auto capgetrad = reinterpret_cast<float(*)(CapsuleCollider*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("get_radius"), 0, _(""), _("UnityEngine"))));

static auto capsetrad = reinterpret_cast<void(*)(CapsuleCollider*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("set_radius"), 1, _(""), _("UnityEngine"))));

static auto basemeldothrow = reinterpret_cast<void(*)(BaseMelee*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("DoThrow"), 0, _(""), _(""))));

static auto baseentgetitem = reinterpret_cast<Item * (*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("GetItem"), 0, _(""), _(""))));

static auto item_cmd = reinterpret_cast<void(*)(unsigned int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("LocalPlayer"), _("ItemCommand"), 2, _(""), _(""))));

static auto get_activemodel = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseViewModel"), _("get_ActiveModel"), 0, _(""), _(""))));

static auto useaction = reinterpret_cast<void(*)(BasePlayer*, InputState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("UseAction"), 1, _(""), _(""))));

static auto bmclientinput = reinterpret_cast<void(*)(BaseMovement*, InputState*, ModelState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), 2, _(""), _(""))));

static auto timewarnnew = reinterpret_cast<TimeWarning * (*)(System::string, int)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TimeWarning"), _("New"), 2, _(""), _(""))));

static auto heldentinput = reinterpret_cast<void(*)(BasePlayer*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HeldEntityInput"), 0, _(""), _(""))));

static auto mapinterfacesetopen = reinterpret_cast<void(*)(bool)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("MapInterface"), _("SetOpen"), 1, _(""), _(""))));

static auto uiinvclose = reinterpret_cast<void(*)()> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("UIInventory"), _("Close"), -1, _(""), _(""))));

static auto pbclientinput = reinterpret_cast<void(*)(PlayerBelt*, InputState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerBelt"), _("ClientInput"), 1, _(""), _(""))));

static auto anyactive = reinterpret_cast<bool(*)(int, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("NeedsKeyboard"), _("AnyActive"), 2, _(""), _(""))));

static auto haslocalcontrols = reinterpret_cast<bool(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HasLocalControls"), 0, _(""), _(""))));

static auto voicerecclientinput = reinterpret_cast<void(*)(PlayerVoiceRecorder*, InputState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerVoiceRecorder"), _("ClientInput"), 0, _(""), _(""))));

static auto waterfactor = reinterpret_cast<float(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("WaterFactor"), 0, _(""), _(""))));

static auto objgetname = reinterpret_cast<System::string * (*)(Object*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("get_name"), 0, _(""), _("UnityEngine"))));

static auto lpgetent = reinterpret_cast<BasePlayer * (*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("LocalPlayer"), _("get_Entity"), 0, _(""), _(""))));

static auto closestpoint = reinterpret_cast<Vector3(*)(BaseEntity*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("ClosestPoint"), 1, _(""), _(""))));

static auto thrownwpn_inheritedvel = reinterpret_cast<Vector3(*)(AttackEntity*, BasePlayer*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ThrownWeapon"), _("GetInheritedVelocity"), 2, _(""), _(""))));

static auto forceposto = reinterpret_cast<void(*)(BasePlayer*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ForcePositionTo"), 1, _(""), _(""))));

static auto getignore = reinterpret_cast<bool(*)(TerrainCollision*, Vector3, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainCollision"), _("GetIgnore"), 2, _(""), _(""))));

static auto sampleheight = reinterpret_cast<float(*)(Terrain*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Terrain"), _("SampleHeight"), 1, _(""), _("UnityEngine"))));

static auto thmgetheight = reinterpret_cast<float(*)(TerrainHeightMap*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetHeight"), 1, _(""), _(""))));

static auto transgetpos = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_position"), 0, _(""), _("UnityEngine"))));

static auto transgetrot = reinterpret_cast<Vector4(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_rotation"), 0, _(""), _("UnityEngine"))));

static auto transsetrot = reinterpret_cast<void(*)(Transform*, Vector4)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_rotation"), 0, _(""), _("UnityEngine"))));

static auto transsetpos = reinterpret_cast<void(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_position"), 0, _(""), _("UnityEngine"))));

static auto Sphere = reinterpret_cast<void (*)(Vector3 vPos, float fRadius, col color, float fDuration, bool distanceFade)>(0);

static auto Capsule = reinterpret_cast<void (*)(Vector3, Vector4, float, float, col, float, bool)>(0);

static auto GetNormal = reinterpret_cast<Vector3(*)(uintptr_t, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetNormal"), 1, _(""), _(""))));

static auto get_visplayerlist = reinterpret_cast<System::Array<BasePlayer*>*(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("get_VisiblePlayerList"), -1, _(""), _(""))));

static auto GetSpeed = reinterpret_cast<float (*)(BasePlayer * baseplayer, float running, float ducking, float crawling)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetSpeed"), 3, _(""), _(""))));

static auto IsSwimming = reinterpret_cast<bool (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("IsSwimming"), 0, _(""), _(""))));

static auto Line = reinterpret_cast<void (*)(Vector3, Vector3, col, float, bool, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Line"), 6, _(""), _("UnityEngine"))));

static auto GetIndex = reinterpret_cast<int(*)(uintptr_t SkinSetCollection, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinSetCollection"), _("GetIndex"), 1, _(""), _(""))));

static auto set_onLadder = reinterpret_cast<void(*)(ModelState*, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_onLadder"), 1, _(""), _(""))));

static auto HasPlayerFlag = reinterpret_cast<bool(*)(BasePlayer*, PlayerFlags)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HasPlayerFlag"), 1, _(""), _(""))));

static auto get_iconSprite = reinterpret_cast<uintptr_t(*)(Item * Item)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Item"), _("get_iconSprite"), 0, _(""), _(""))));

static auto get_texture = reinterpret_cast<uintptr_t(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Sprite"), _("get_texture"), 0, _(""), _("UnityEngine"))));

static auto get_rect = reinterpret_cast<Rect(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Sprite"), _("get_rect"), 0, _(""), _("UnityEngine"))));

static auto StringPool_Get = reinterpret_cast<unsigned int(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("StringPool"), _("Get"), 1, _("str"), _(""))));

static auto set_timeScale = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("set_timeScale"), 1, _(""), _("UnityEngine"))));

static auto get_frameCount = reinterpret_cast<int(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_frameCount"), 0, _(""), _("UnityEngine"))));

static auto get_timeScale = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_timeScale"), 0, _(""), _("UnityEngine"))));

static auto get_mousePosition = reinterpret_cast<Vector3(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Input"), _("get_mousePosition"), 0, _(""), _("UnityEngine"))));

static auto FloorToInt = reinterpret_cast<int (*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Mathf"), _("FloorToInt"), 1, _(""), _("UnityEngine"))));

static auto get_gravity = reinterpret_cast<Vector3(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Physics"), _("get_gravity"), 1, _(""), _("UnityEngine"))));

static auto get_isAlive = reinterpret_cast<bool (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("get_isAlive"), 0, _(""), _(""))));

static auto get_rotation = reinterpret_cast<Vector4(*)(uintptr_t Transform)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_rotation"), 0, _(""), _("UnityEngine"))));

static auto FindShader = reinterpret_cast<uintptr_t(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Shader"), _("Find"), 1, _(""), _("UnityEngine"))));

static auto set_hideFlags = reinterpret_cast<void(*)(uintptr_t material, int value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("set_hideFlags"), 1, _(""), _("UnityEngine"))));

static auto set_name = reinterpret_cast<void(*)(uintptr_t, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("set_name"), 1, _(""), _("UnityEngine"))));

static auto SetInt = reinterpret_cast<void(*)(uintptr_t material, System::string name, int value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetInt"), 1, _(""), _("UnityEngine"))));

static auto get_deltaTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_deltaTime"), 0, _(""), _("UnityEngine"))));

static auto get_smoothdeltaTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_smoothDeltaTime"), 0, _(""), _("UnityEngine"))));

static auto get_fixeddeltaTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_fixedDeltaTime"), 0, _(""), _("UnityEngine"))));

static auto get_fixedTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_fixedTime"), 0, _(""), _("UnityEngine"))));

static auto get_IsNpc = reinterpret_cast<bool(*)(uintptr_t player_model)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerModel"), _("get_IsNpc"), -1, _(""), _(""))));

static auto get_time = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_time"), 0, _(""), _("UnityEngine"))));

static auto _InverseTransformPoint = reinterpret_cast<Vector3(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("InverseTransformPoint"), 1, _(""), _("UnityEngine"))));

static auto _InverseTransformDirection = reinterpret_cast<Vector3(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("InverseTransformDirection"), 1, _(""), _("UnityEngine"))));

static auto get_localToWorldMatrix = reinterpret_cast<VMatrix(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_localToWorldMatrix"), 0, _(""), _("UnityEngine"))));

static auto get_gameObject = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("get_gameObject"), 0, _(""), _("UnityEngine"))));

static auto get_component = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("GetComponent"), 0, _(""), _("UnityEngine"))));

static auto get_components_in_children = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("GetComponentsInChildren"), 0, _(""), _("UnityEngine"))));

static auto set_position = reinterpret_cast<void(*)(Transform*, Vector3 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_position"), 1, _(""), _("UnityEngine"))));

static auto get_up = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_up"), 0, _(""), _("UnityEngine"))));

static auto LookRotation = reinterpret_cast<Vector4(*)(Vector3, Vector3 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Quaternion"), _("LookRotation"), 2, _(""), _("UnityEngine"))));

static auto set_rotation = reinterpret_cast<void(*)(Transform*, Vector4 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_rotation"), 1, _(""), _("UnityEngine"))));

static auto StartAttackCooldown = reinterpret_cast<void(*)(AttackEntity*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AttackEntity"), _("StartAttackCooldown"), 1, _(""), _(""))));

static auto ProcessAttack = reinterpret_cast<void(*)(BaseMelee*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("ProcessAttack"), 1, _(""), _(""))));

static auto GetProjectileVelocityScale = reinterpret_cast<float(*)(BaseProjectile*, bool max)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("GetProjectileVelocityScale"), 1, _("getMax"), _(""), 1)));

//static auto get_Renderers = reinterpret_cast<System::list<uintptr_t>*(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinnedMultiMesh"), _("get_Renderers"), 0, _(""), _(""))));

static auto get_material = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("get_material"), 0, _(""), _("UnityEngine"))));

static auto set_material = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("set_material"), 0, _(""), _("UnityEngine"))));

static auto get_maxspeed = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMaxSpeed"), 0, _(""), _(""))));

static auto SendSignal = reinterpret_cast<void(*)(uintptr_t, Signal, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("SendSignalBroadcast"), 2, _(""), _(""))));

static auto _BoundsPadding = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("BoundsPadding"), 2, _(""), _(""))));

static auto PEyes_get_position = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_position"), 0, _(""), _(""))));

static auto get_center = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_center"), 0, _(""), _(""))));

static auto PEyes_get_rotation = reinterpret_cast<Vector4(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_rotation"), 0, _(""), _(""))));

static auto headforward = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("HeadForward"), 0, _(""), _(""))));

static auto bodyforward = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("BodyForward"), 0, _(""), _(""))));

static auto headright = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("HeadRight"), 0, _(""), _(""))));

static auto bodyright = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("BodyRight"), 0, _(""), _(""))));

static auto _SendClientTick = reinterpret_cast<void(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("SendClientTick"), 0, _(""), _(""))));

static auto _getjumpheight = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetJumpHeight"), 0, _(""), _(""))));

static auto _GetRadius = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetRadius"), 0, _(""), _(""))));

static auto GetHeight = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetHeight"), 0, _(""), _(""))));

static auto terrainheightmap_GetHeight = reinterpret_cast<int(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetHeight"), 0, _(""), _(""))));

static auto updateammodisplay = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("UpdateAmmoDisplay"), 0, _(""), _(""))));

static auto shot_fired = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("ShotFired"), 0, _(""), _(""))));

static auto did_attack_client_side = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("DidAttackClientside"), 0, _(""), _(""))));

static auto getmodifiedaimcone = reinterpret_cast<Vector3(*)(float, Vector3, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AimConeUtil"), _("GetModifiedAimConeDirection"), 0, _(""), _(""))));

static auto canaffordupgrade = reinterpret_cast<bool(*)(uintptr_t, BuildingGrade, BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BuildingBlock"), _("CanAffordUpgrade"), 0, _(""), _(""))));

static auto canchangetograde = reinterpret_cast<bool(*)(uintptr_t, BuildingGrade, BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BuildingBlock"), _("CanChangeToGrade"), 0, _(""), _(""))));

static auto upgradetograde = reinterpret_cast<void(*)(uintptr_t, BuildingGrade, BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BuildingBlock"), _("UpgradeToGrade"), 0, _(""), _(""))));

static auto set_jumped = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_jumped"), 0, _(""), _(""))));
static auto set_sprinting = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_sprinting"), 0, _(""), _(""))));
static auto set_aiming = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_aiming"), 0, _(""), _(""))));
static auto set_ducked = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_ducked"), 0, _(""), _(""))));

static auto do_jump = reinterpret_cast<void(*)(uintptr_t, uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("Jump"), 0, _(""), _(""))));

static auto set_rigidbody_velocity = reinterpret_cast<void(*)(uintptr_t, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Rigidbody"), _("set_velocity"), 0, _(""), _("UnityEngine"))));

static auto get_rigidbody_velocity = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Rigidbody"), _("get_velocity"), 0, _(""), _("UnityEngine"))));

static auto console_msg = reinterpret_cast<void(*)(uintptr_t, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ConsoleMessage"), 0, _(""), _(""))));

static auto teleportto = reinterpret_cast<void(*)(uintptr_t, Vector3, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMovement"), _("TeleportTo"), 0, _(""), _(""))));

static auto GetAimCone = reinterpret_cast<float(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("GetAimCone"), 0, _(""), _(""))));

static auto animcurve_evaluate = reinterpret_cast<float(*)(uintptr_t, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AnimationCurve"), _("Evaluate"), 0, _(""), _("UnityEngine"))));

static auto guidtopath = reinterpret_cast<System::string(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameManifest"), _("GUIDToPath"), 0, _(""), _(""))));

static auto GetBounds = reinterpret_cast<Bounds(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetBounds"), 0, _(""), _(""))));

static auto get_mounted = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMounted"), 0, _(""), _(""))));

static auto get_parent_entity = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseNetworkable"), _("GetParentEntity"), 0, _(""), _(""))));

static auto get_short_prefab_name = reinterpret_cast<System::string(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseNetworkable"), _("get_ShortPrefabName"), 0, _(""), _(""))));

static auto get_game_object_transform = reinterpret_cast<Transform * (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("get_transform"), 0, _(""), _("UnityEngine"))));

static auto get_max_fwd_speed = reinterpret_cast<float(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModularCar"), _("GetMaxForwardSpeed"), 0, _(""), _(""))));

static auto set_SpaceMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_SpaceMaterial"), 0, _(""), _(""))));
static auto set_StarMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_StarMaterial"), 0, _(""), _(""))));
static auto set_SunMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_SunMaterial"), 0, _(""), _(""))));
static auto set_MoonMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_MoonMaterial"), 0, _(""), _(""))));
static auto set_AtmosphereMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_AtmosphereMaterial"), 0, _(""), _(""))));
static auto set_ClearMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_ClearMaterial"), 0, _(""), _(""))));
static auto set_CloudMaterial = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Components"), _("set_CloudMaterial"), 0, _(""), _(""))));
static auto getiteminslot = reinterpret_cast<uintptr_t(*)(PlayerBelt*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerBelt"), _("GetItemInSlot"), 0, _(""), _(""))));
static auto gettrans = reinterpret_cast<uintptr_t(*)(Component*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("get_transform"), 0, _(""), _("UnityEngine"))));
static auto _Text = reinterpret_cast<void (*)(System::string*, Vector3, col, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Text"), 4, _(""), _("UnityEngine"))));
#pragma endregion

class col {
public:
	float r;
	float g;
	float b;
	float a;
	col(float rr, float gg, float bb, float aa) {
		r = rr;
		g = gg;
		b = bb;
		a = aa;
	}
};

float current_time;

void init_bp() {
	//setrayleigh = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Weather"), _("set_atmosphere_rayleigh"), 0, _(""), _(""))));
	//ServerRPC_intstring = reinterpret_cast<void (*)(BaseEntity*, System::string, unsigned int, System::string, uintptr_t)>(mem::game_assembly_base + offsets::BaseEntity$$ServerRPC_uintstring_);
	_Text = reinterpret_cast<void (*)(System::string*, Vector3, col, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Text"), 4, _(""), _("UnityEngine"))));
	viewmodelplay = reinterpret_cast<void(*)(uintptr_t, System::string, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ViewModel"), _("Play"), 2, _(""), _(""))));
	set_ambientintensity = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("RenderSettings"), _("set_ambientIntensity"), 1, _(""), _("UnityEngine"))));
	set_ambientlight = reinterpret_cast<void(*)(uintptr_t, col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TOD_Sky"), _("set_AmbientColor"), 1, _(""), _(""))));
	transsetrot = reinterpret_cast<void(*)(Transform*, Vector4)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_rotation"), 0, _(""), _("UnityEngine"))));
	getmountedvehicle = reinterpret_cast<BaseVehicle * (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMountedVehicle"), 0, _(""), _(""))));
	getmounted = reinterpret_cast<BaseMountable * (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMounted"), 0, _(""), _(""))));
	raycasthit_get_transform = reinterpret_cast<Transform * (*)(RaycastHit*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("RaycastHit"), _("get_transform"), 0, _(""), _("UnityEngine"))));
	bmdoattack = reinterpret_cast<void(*)(BaseMelee*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("DoAttack"), 0, _(""), _(""))));
	dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("DontDestroyOnLoad"), 0, _(""), _("UnityEngine"))));
	create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_CreateGameObject"), 0, _(""), _("UnityEngine"))));
	add_component = reinterpret_cast<Component * (*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("Internal_AddComponentWithType"), 0, _(""), _("UnityEngine"))));

	hasammo = reinterpret_cast<bool(*)(PlayerInventory*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerInventory"), _("HasAmmo"), 0, _(""), _(""))));
	getrenderers = reinterpret_cast<System::list<Renderer*>*(*)(SkinnedMultiMesh*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinnedMultiMesh"), _("get_Renderers"), 0, _(""), _(""))));
	setmaterial = reinterpret_cast<void(*)(Renderer*, Material*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("set_material"), 0, _(""), _("UnityEngine"))));
	getmaterial = reinterpret_cast<Material * (*)(Renderer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("get_material"), 0, _(""), _("UnityEngine"))));
	setfloat = reinterpret_cast<void(*)(Material*, System::string, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetFloat"), 2, _("name"), _("UnityEngine"), 1)));
	setcolor = reinterpret_cast<void(*)(Material*, System::string, col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetColor"), 2, _("name"), _("UnityEngine"), 1)));
	setshader = reinterpret_cast<void(*)(Material*, Shader*)>(il2cpp::methods::resolve_icall(_("UnityEngine.Material::set_shader()")));
	getshader = reinterpret_cast<Shader * (*)(Material*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("get_shader"), 0, _(""), _("UnityEngine"))));
	canhit = reinterpret_cast<bool (*)(BaseMelee*, HitTest*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("CanHit"), 1, _(""), _(""))));
	serverrpc = reinterpret_cast<void (*)(BaseEntity*, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("ServerRPC"), 1, _("funcName"), _(""), 1)));
	getparentvel = reinterpret_cast<Vector3(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("GetParentVelocity"), 0, _(""), _(""))));
	transgetrot = reinterpret_cast<Vector4(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_rotation"), 0, _(""), _("UnityEngine"))));
	gettrans = reinterpret_cast<uintptr_t(*)(Component*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("get_transform"), 0, _(""), _("UnityEngine"))));
	get_game_object_transform = reinterpret_cast<Transform * (*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameObject"), _("get_transform"), 0, _(""), _("UnityEngine"))));
	sendprojupdate = reinterpret_cast<void(*)(BasePlayer*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("SendProjectileUpdate"), 0, _(""), _(""))));
	gamephystrace = reinterpret_cast<bool(*)(Ray r, float f, RaycastHit * r2, float f2, int layer, int querytriggerinteraction)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GamePhysics"), _("Trace"), 0, _(""), _(""))));
	headright = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("HeadRight"), 0, _(""), _(""))));
	headforward = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("HeadForward"), 0, _(""), _(""))));
	get_localscale = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_localScale"), 0, _(""), _("UnityEngine"))));
	set_localscale = reinterpret_cast<void(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_localScale"), 1, _(""), _("UnityEngine"))));
	launchproj = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("LaunchProjectile"), 0, _(""), _(""))));
	capgetheight = reinterpret_cast<float(*)(CapsuleCollider*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("get_height"), 0, _(""), _("UnityEngine"))));
	capsetheight = reinterpret_cast<void(*)(CapsuleCollider*, float f)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("set_height"), 1, _(""), _("UnityEngine"))));
	capgetrad = reinterpret_cast<float(*)(CapsuleCollider*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("get_radius"), 0, _(""), _("UnityEngine"))));
	capsetrad = reinterpret_cast<void(*)(CapsuleCollider*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("CapsuleCollider"), _("set_radius"), 0, _(""), _("UnityEngine"))));
	basemeldothrow = reinterpret_cast<void(*)(BaseMelee*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("DoThrow"), 0, _(""), _(""))));
	baseentgetitem = reinterpret_cast<Item * (*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("GetItem"), 0, _(""), _(""))));
	item_cmd = reinterpret_cast<void(*)(unsigned int, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("LocalPlayer"), _("ItemCommand"), 2, _(""), _(""))));
	get_activemodel = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseViewModel"), _("get_ActiveModel"), 0, _(""), _(""))));
	useaction = reinterpret_cast<void(*)(BasePlayer*, InputState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("UseAction"), 1, _(""), _(""))));
	bmclientinput = reinterpret_cast<void(*)(BaseMovement*, InputState*, ModelState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("ClientInput"), 2, _(""), _(""))));
	timewarnnew = reinterpret_cast<TimeWarning * (*)(System::string, int)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TimeWarning"), _("New"), 2, _(""), _(""))));
	heldentinput = reinterpret_cast<void(*)(BasePlayer*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HeldEntityInput"), 0, _(""), _(""))));
	mapinterfacesetopen = reinterpret_cast<void(*)(bool)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("MapInterface"), _("SetOpen"), 1, _(""), _(""))));
	uiinvclose = reinterpret_cast<void(*)()> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("UIInventory"), _("Close"), -1, _(""), _(""))));
	pbclientinput = reinterpret_cast<void(*)(PlayerBelt*, InputState*)> (*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerBelt"), _("ClientInput"), 1, _(""), _(""))));
	anyactive = reinterpret_cast<bool(*)(int, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("NeedsKeyboard"), _("AnyActive"), 2, _(""), _(""))));
	haslocalcontrols = reinterpret_cast<bool(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HasLocalControls"), 0, _(""), _(""))));
	voicerecclientinput = reinterpret_cast<void(*)(PlayerVoiceRecorder*, InputState*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerVoiceRecorder"), _("ClientInput"), 0, _(""), _(""))));
	waterfactor = reinterpret_cast<float(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("WaterFactor"), 0, _(""), _(""))));
	set_sprinting = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_sprinting"), 0, _(""), _(""))));
	set_aiming = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_aiming"), 0, _(""), _(""))));
	set_ducked = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_ducked"), 0, _(""), _(""))));
	objgetname = reinterpret_cast<System::string * (*)(Object*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("get_name"), 0, _(""), _("UnityEngine"))));
	lpgetent = reinterpret_cast<BasePlayer * (*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("LocalPlayer"), _("get_Entity"), 0, _(""), _(""))));
	closestpoint = reinterpret_cast<Vector3(*)(BaseEntity*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("ClosestPoint"), 1, _(""), _(""))));
	thrownwpn_inheritedvel = reinterpret_cast<Vector3(*)(AttackEntity*, BasePlayer*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ThrownWeapon"), _("GetInheritedVelocity"), 2, _(""), _(""))));
	forceposto = reinterpret_cast<void(*)(BasePlayer*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ForcePositionTo"), 1, _(""), _(""))));
	getignore = reinterpret_cast<bool(*)(TerrainCollision*, Vector3, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainCollision"), _("GetIgnore"), 2, _(""), _(""))));
	sampleheight = reinterpret_cast<float(*)(Terrain*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Terrain"), _("SampleHeight"), 1, _(""), _("UnityEngine"))));
	thmgetheight = reinterpret_cast<float(*)(TerrainHeightMap*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetHeight"), 1, _(""), _(""))));
	transsetpos = reinterpret_cast<void(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_position"), 0, _(""), _("UnityEngine"))));
	getiteminslot = reinterpret_cast<uintptr_t(*)(PlayerBelt*, int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerBelt"), _("GetItemInSlot"), 0, _(""), _(""))));
	transgetpos = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_position"), 0, _(""), _("UnityEngine"))));
	get_visplayerlist = reinterpret_cast<System::Array<BasePlayer*>*(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("get_VisiblePlayerList"), -1, _(""), _(""))));
	damageProperties = il2cpp::value(_("BaseMelee"), _("damageProperties"));
	get_center = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_center"), 0, _(""), _(""))));
	get_max_fwd_speed = reinterpret_cast<float(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModularCar"), _("GetMaxForwardSpeed"), 0, _(""), _(""))));
	get_short_prefab_name = reinterpret_cast<System::string(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseNetworkable"), _("get_ShortPrefabName"), 0, _(""), _(""))));
	get_parent_entity = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseNetworkable"), _("GetParentEntity"), 0, _(""), _(""))));
	get_mounted = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMounted"), 0, _(""), _(""))));
	terrainheightmap_GetHeight = reinterpret_cast<int(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetHeight"), 0, _(""), _(""))));
	GetNormal = reinterpret_cast<Vector3(*)(uintptr_t, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("TerrainHeightMap"), _("GetNormal"), 1, _(""), _(""))));
	Line = reinterpret_cast<void (*)(Vector3, Vector3, col, float, bool, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("DDraw"), _("Line"), 6, _(""), _("UnityEngine"))));
	set_onLadder = reinterpret_cast<void(*)(ModelState*, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_onLadder"), 1, _(""), _(""))));
	HasPlayerFlag = reinterpret_cast<bool(*)(BasePlayer*, PlayerFlags)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("HasPlayerFlag"), 1, _(""), _(""))));
	get_rect = reinterpret_cast<Rect(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Sprite"), _("get_rect"), 0, _(""), _("UnityEngine"))));
	get_iconSprite = reinterpret_cast<uintptr_t(*)(Item * Item)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Item"), _("get_iconSprite"), 0, _(""), _(""))));
	steamItem = il2cpp::value(_("ItemDefinition"), _("steamItem"));
	iconSprite = il2cpp::value(_("ItemDefinition"), _("iconSprite"));
	get_texture = reinterpret_cast<uintptr_t(*)(uintptr_t sprite)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Sprite"), _("get_texture"), 0, _(""), _("UnityEngine"))));
	set_name = reinterpret_cast<void(*)(uintptr_t, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("set_name"), 1, _(""), _("UnityEngine"))));;
	get_gameObject = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("get_gameObject"), 0, _(""), _("UnityEngine"))));
	_InverseTransformDirection = reinterpret_cast<Vector3(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("InverseTransformDirection"), 1, _(""), _("UnityEngine"))));
	LookRotation = reinterpret_cast<Vector4(*)(Vector3, Vector3 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Quaternion"), _("LookRotation"), 2, _(""), _("UnityEngine"))));
	set_rotation = reinterpret_cast<void(*)(Transform*, Vector4 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_rotation"), 1, _(""), _("UnityEngine"))));
	set_position = reinterpret_cast<void(*)(Transform*, Vector3 value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("set_position"), 1, _(""), _("UnityEngine"))));
	SetInt = reinterpret_cast<void(*)(uintptr_t material, System::string name, int value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Material"), _("SetInt"), 1, _(""), _("UnityEngine"))));
	set_hideFlags = reinterpret_cast<void(*)(uintptr_t material, int value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Object"), _("set_hideFlags"), 1, _(""), _("UnityEngine"))));
	FindShader = reinterpret_cast<uintptr_t(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Shader"), _("Find"), 1, _(""), _("UnityEngine"))));
	//get_Renderers = reinterpret_cast<System::list<uintptr_t> *(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinnedMultiMesh"), _("get_Renderers"), 0, _(""), _(""))));
	//get_material = reinterpret_cast<uintptr_t(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("get_material"), 0, _(""), _("UnityEngine"))));
	StringPool_Get = reinterpret_cast<unsigned int(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("StringPool"), _("Get"), 1, _("str"), _(""))));
	set_timeScale = reinterpret_cast<void(*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("set_timeScale"), 1, _(""), _("UnityEngine"))));
	get_timeScale = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_timeScale"), 0, _(""), _("UnityEngine"))));
	GetIndex = reinterpret_cast<int(*)(uintptr_t SkinSetCollection, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("SkinSetCollection"), _("GetIndex"), 1, _(""), _(""))));
	FloorToInt = reinterpret_cast<int (*)(float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Mathf"), _("FloorToInt"), 1, _(""), _("UnityEngine"))));
	get_gravity = reinterpret_cast<Vector3(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Physics"), _("get_gravity"), 1, _(""), _("UnityEngine"))));
	get_isAlive = reinterpret_cast<bool (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Projectile"), _("get_isAlive"), 0, _(""), _(""))));
	get_mousePosition = reinterpret_cast<Vector3(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Input"), _("get_mousePosition"), 0, _(""), _("UnityEngine"))));
	GetSpeed = reinterpret_cast<float (*)(BasePlayer * baseplayer, float running, float ducking, float crawling)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetSpeed"), 3, _(""), _(""))));
	get_frameCount = reinterpret_cast<int(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_frameCount"), 0, _(""), _("UnityEngine"))));
	get_rotation = reinterpret_cast<Vector4(*)(uintptr_t Transform)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_rotation"), 0, _(""), _("UnityEngine"))));
	GetBounds = reinterpret_cast<Bounds(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetBounds"), 0, _(""), _(""))));
	bodyright = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("BodyRight"), 0, _(""), _(""))));
	get_components_in_children = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("GetComponentsInChildren"), 0, _(""), _("UnityEngine"))));
	get_component = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Component"), _("GetComponent"), 0, _(""), _("UnityEngine"))));

	PEyes_get_position = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_position"), 0, _(""), _(""))));

	_GetRadius = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetRadius"), 0, _(""), _(""))));

	_getjumpheight = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetJumpHeight"), 0, _(""), _(""))));

	_BoundsPadding = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("BoundsPadding"), 2, _(""), _(""))));

	_SendClientTick = reinterpret_cast<void(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("SendClientTick"), 0, _(""), _(""))));

	get_maxspeed = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("GetMaxSpeed"), 0, _(""), _(""))));

	get_up = reinterpret_cast<Vector3(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_up"), 0, _(""), _("UnityEngine"))));

	get_fixedTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_fixedTime"), 0, _(""), _("UnityEngine"))));

	SendSignal = reinterpret_cast<void(*)(uintptr_t, Signal, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseEntity"), _("SendSignalBroadcast"), 2, _(""), _(""))));

	updateammodisplay = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("UpdateAmmoDisplay"), 0, _(""), _(""))));

	shot_fired = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("ShotFired"), 0, _(""), _(""))));

	did_attack_client_side = reinterpret_cast<void(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("DidAttackClientside"), 0, _(""), _(""))));

	bodyforward = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("BodyForward"), 0, _(""), _(""))));

	get_localToWorldMatrix = reinterpret_cast<VMatrix(*)(Transform*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("get_localToWorldMatrix"), 0, _(""), _("UnityEngine"))));

	getmodifiedaimcone = reinterpret_cast<Vector3(*)(float, Vector3, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AimConeUtil"), _("GetModifiedAimConeDirection"), 0, _(""), _(""))));

	PEyes_get_rotation = reinterpret_cast<Vector4(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerEyes"), _("get_rotation"), 0, _(""), _(""))));

	set_jumped = reinterpret_cast<void(*)(uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("ModelState"), _("set_jumped"), 0, _(""), _(""))));

	set_rigidbody_velocity = reinterpret_cast<void(*)(uintptr_t, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Rigidbody"), _("set_velocity"), 0, _(""), _("UnityEngine"))));

	get_rigidbody_velocity = reinterpret_cast<Vector3(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Rigidbody"), _("get_velocity"), 0, _(""), _("UnityEngine"))));

	console_msg = reinterpret_cast<void(*)(uintptr_t, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("ConsoleMessage"), 0, _(""), _(""))));

	teleportto = reinterpret_cast<void(*)(uintptr_t, Vector3, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMovement"), _("TeleportTo"), 0, _(""), _(""))));

	set_material = reinterpret_cast<uintptr_t(*)(uintptr_t, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Renderer"), _("set_material"), 0, _(""), _("UnityEngine"))));

	do_jump = reinterpret_cast<void(*)(uintptr_t, uintptr_t, bool)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerWalkMovement"), _("Jump"), 0, _(""), _(""))));

	GetAimCone = reinterpret_cast<float(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("GetAimCone"), 0, _(""), _(""))));

	animcurve_evaluate = reinterpret_cast<float(*)(uintptr_t, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AnimationCurve"), _("Evaluate"), 0, _(""), _("UnityEngine"))));

	guidtopath = reinterpret_cast<System::string(*)(System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GameManifest"), _("GUIDToPath"), 0, _(""), _(""))));

	IsSwimming = reinterpret_cast<bool (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BasePlayer"), _("IsSwimming"), 0, _(""), _(""))));
#pragma region il2

	containerWear = il2cpp::value(_("PlayerInventory"), _("containerWear"));
	canWieldItems = il2cpp::value(_("BaseMountable"), _("canWieldItems"));
	containerBelt = il2cpp::value(_("PlayerInventory"), _("containerBelt"));
	itemList = il2cpp::value(_("ItemContainer"), _("itemList"));
	integrity = il2cpp::value(_("Projectile"), _("integrity"));

	lastHitTime = il2cpp::value(_("Chainsaw"), _("lastHitTime"));
	lastHitMaterial = il2cpp::value(_("Chainsaw"), _("lastHitMaterial"));
	landTime = il2cpp::value(_("PlayerWalkMovement"), _("landTime"));
	jumpTime = il2cpp::value(_("PlayerWalkMovement"), _("jumpTime"));
	groundTime = il2cpp::value(_("PlayerWalkMovement"), _("groundTime"));
	gravityMultiplier = il2cpp::value(_("PlayerWalkMovement"), _("gravityMultiplier"));

	movement = il2cpp::value(_("BasePlayer"), _("movement"));
	_displayName = il2cpp::value(_("BasePlayer"), _("_displayName"));
	player_inventory = il2cpp::value(_("BasePlayer"), _("inventory"));
	clActiveItem = il2cpp::value(_("BasePlayer"), _("clActiveItem"));
	playerModel = il2cpp::value(_("BasePlayer"), _("playerModel"));
	clientTeam = il2cpp::value(_("BasePlayer"), _("clientTeam"));
	playerFlags = il2cpp::value(_("BasePlayer"), _("playerFlags"));
	eyes = il2cpp::value(_("BasePlayer"), _("eyes"));
	modelstate = il2cpp::value(_("BasePlayer"), _("ModelState"));
	input = il2cpp::value(_("BasePlayer"), _("input"));

	newVelocity = il2cpp::value(_("PlayerModel"), _("newVelocity"));
	isLocalPlayer = il2cpp::value(_("PlayerModel"), _("isLocalPlayer"));

	_health = il2cpp::value(_("BaseCombatEntity"), _("_health"));
	_maxHealth = il2cpp::value(_("BaseCombatEntity"), _("_maxHealth"));
	lifestate = il2cpp::value(_("BaseCombatEntity"), _("lifestate"));

	net = il2cpp::value(_("BaseNetworkable"), _("net"));

	heldEntity = il2cpp::value(_("Item"), _("heldEntity"));

	currentVelocity = il2cpp::value(_("Projectile"), _("currentVelocity"));
	currentPosition = il2cpp::value(_("Projectile"), _("currentPosition"));
	thickness_addr = il2cpp::value(_("Projectile"), _("thickness"));
	isAuthoritative = il2cpp::value(_("Projectile"), _("isAuthoritative"));
	mod = il2cpp::value(_("Projectile"), _("mod"));
	hitTest = il2cpp::value(_("Projectile"), _("hitTest"));

	info = il2cpp::value(_("Item"), _("info"));

	category = il2cpp::value(_("ItemDefinition"), _("category"));
	itemid = il2cpp::value(_("ItemDefinition"), _("itemid"));
	displayName = il2cpp::value(_("ItemDefinition"), _("displayName"));

	viewOffset = il2cpp::value(_("PlayerEyes"), _("viewOffset"));

	swimming = il2cpp::value(_("PlayerWalkMovement"), _("swimming"));
	ducking = il2cpp::value(_("BaseMovement"), _("<Ducking>k__BackingField"));
	TargetMovement = il2cpp::value(_("BaseMovement"), _("<TargetMovement>k__BackingField"));
	groundAngleNew = il2cpp::value(_("PlayerWalkMovement"), _("groundAngleNew"));
	flying = il2cpp::value(_("PlayerWalkMovement"), _("flying"));

	get_deltaTime = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_deltaTime"), 0, _(""), _("UnityEngine"))));

	get_IsNpc = reinterpret_cast<bool(*)(uintptr_t player_model)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("PlayerModel"), _("get_IsNpc"), -1, _(""), _(""))));

	get_time = reinterpret_cast<float(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Time"), _("get_time"), 0, _(""), _("UnityEngine"))));

	GetProjectileVelocityScale = reinterpret_cast<float(*)(BaseProjectile*, bool max)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseProjectile"), _("GetProjectileVelocityScale"), 1, _("getMax"), _(""), 1)));

	StartAttackCooldown = reinterpret_cast<void(*)(AttackEntity*, float)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AttackEntity"), _("StartAttackCooldown"), 1, _(""), _(""))));

	_InverseTransformPoint = reinterpret_cast<Vector3(*)(Transform*, Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Transform"), _("InverseTransformPoint"), 1, _(""), _("UnityEngine"))));

	ProcessAttack = reinterpret_cast<void(*)(BaseMelee*, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("BaseMelee"), _("ProcessAttack"), 1, _(""), _(""))));

#pragma endregion
}

#define COMPONENT(space, type) type* { \
	NP(type) \
	static auto ret = ((Component*)this)->GetComponent<type*>(unity::GetType(_(space), _(#type))); \
	return ret; }

class DDraw {
public:
	static void Text(const char* c, Vector3 pos, col clr, float duration) {
		return _Text(System::string::a(c), pos, clr, duration);
	}
};

class Object {
public:
	System::string* get_name() {
		pent
			pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return objgetname(this);
	}
	char* get_class_name() {
		pent
			auto bp = *reinterpret_cast<uintptr_t*>(this);
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return _("");
		return (char*)*reinterpret_cast<uintptr_t*>(bp + 0x10);
	}
};

class Component : public Object {
public:

	Transform* transform() {
		pent
			__try {
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
			return (Transform*)gettrans(this);
		}
		__except (true) {
			pent
				return nullptr;
		}
	}


	rust_str get_object_name() {
		auto obj = *reinterpret_cast<uintptr_t*>(this + 0x8);
		auto nameptr = mem::read<uintptr_t>(obj + 0x60);
		return *reinterpret_cast<rust_str*>(nameptr);
	}

	template<typename T = uintptr_t>
	T* GetComponent(uintptr_t type) {
		pent
			if (!this || !type) return nullptr;
		return (T*)get_component((uintptr_t)this, type);
	}

	System::list<uintptr_t>* GetComponentsInChildren(uintptr_t type) {
		pent
			if (!this || !type) return nullptr;
		return reinterpret_cast<System::list<uintptr_t>*>(get_components_in_children((uintptr_t)this, type));
	}
};

class GameObject : public Object {
public:
	template<typename T>
	T get_class()
	{
		return *reinterpret_cast<T*>((uintptr_t)this + 0x30);
	}

	template<typename T>
	T get_class(uint32_t second_offset)
	{
		pent
			const auto object = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x30);
		if (!object)
			return nullptr;

		return *reinterpret_cast<T*>(object + second_offset);
	}

	char* get_prefab_name() { pent return *reinterpret_cast<char**>((uintptr_t)this + 0x60); }

	uint32_t get_tag() { pent return *reinterpret_cast<uint16_t*>((uintptr_t)this + 0x54); }

	layer get_layer() { pent return *reinterpret_cast<layer*>(this + 0x50); }
};

class Transform : public Component {
public:
	void SetLocalScale(Vector3 v) {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		set_localscale(this, v);
	}

	Vector3 GetLocalScale() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return {};
		return get_localscale(this);
	}

	Vector3 up()
	{
		if (!(uintptr_t)this)
			return {};
		return get_up(this);
	}

	Vector3 get_object_position()
	{
		if (!(uintptr_t)this)
			return {};

		const auto visual_state = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x38);
		if (!visual_state)
			return {};

		return *reinterpret_cast<Vector3*>(visual_state + 0x90);
	}

	Vector3* get_position_ptr()
	{
		if (!(uintptr_t)this)
			return {};

		const auto visual_state = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x38);
		if (!visual_state)
			return {};

		return reinterpret_cast<Vector3*>(visual_state + 0x90);
	}

	Vector3 position() {
		pent
			__try {
			if (!(uintptr_t)this)
				return {};
			return transgetpos(this);
		}
		__except (true) { pent return Vector3::Zero(); }
	}

	void setposition(Vector3 pos) {
		pent
			if (!(uintptr_t)this)
				return;
		return transsetpos(this, pos);
	}

	Vector4 get_rotation() {
		pent
			if (!(uintptr_t)this)
				return {};
		return transgetrot(this);
	}

	void set_rotation(Vector4 v) {
		pent
			if (!(uintptr_t)this)
				return;
		return transsetrot(this, v);
	}

	Vector3 InverseTransformPoint(Vector3 point) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return _InverseTransformPoint((this), point);
	}

	Vector3 InverseTransformDirection(Vector3 point) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return _InverseTransformDirection((this), point);
	}
};

class Behaviour : public Component {

};

class FacepunchBehaviour : public Behaviour {

};

class BaseMonoBehaviour : public FacepunchBehaviour {

};

class MonoBehaviour : public Behaviour {
public:
	Transform* get_Transform()
	{
		// const auto unk = *reinterpret_cast< uintptr_t* >( this + 0x30 );

		return *reinterpret_cast<Transform**>((uintptr_t)this + 0x8);
	}
};

class Bone {
public:
	Vector3 position;
	bool visible;
	Transform* transform;

	Bone() {
		pent
			this->position = Vector3::Zero();
		this->visible = false;
	}
	Bone(Vector3 position, bool visible) {
		pent
			this->position = position;
		this->visible = visible;
		this->transform = nullptr;
	}
	Bone(Vector3 position, bool visible, Transform* traa) {
		pent
			this->position = position;
		this->visible = visible;
		this->transform = traa;
	}
	bool visible_(Vector3 from) {
		pent
			if (this->position.is_empty())
				return false;

		if (!this->transform)
			return false;

		return unity::is_visible(this->position, from, 0);
	}
};

class BoneCache {
public:
	Bone* head;
	Bone* neck;
	Bone* spine4;
	Bone* spine1;
	Bone* l_upperarm;
	Bone* l_forearm;
	Bone* l_hand;
	Bone* r_upperarm;
	Bone* r_forearm;
	Bone* r_hand;
	Bone* pelvis;
	Bone* l_hip;
	Bone* l_knee;
	Bone* l_foot;
	Bone* r_hip;
	Bone* r_knee;
	Bone* r_foot;
	Bone* r_toe;
	Bone* l_toe;
	Bone* penis;

	bounds_t bounds;
	Vector2 dfc;
	Vector2 forward;
	Vector4 eye_rot;

	BoneCache() {
		pent
			head = new Bone();
		neck = new Bone();
		spine4 = new Bone();
		spine1 = new Bone();
		l_upperarm = new Bone();
		l_forearm = new Bone();
		l_hand = new Bone();
		r_upperarm = new Bone();
		r_forearm = new Bone();
		r_hand = new Bone();
		pelvis = new Bone();
		l_hip = new Bone();
		l_knee = new Bone();
		l_foot = new Bone();
		r_hip = new Bone();
		r_knee = new Bone();
		r_foot = new Bone();

		bounds = { 0, 0, 0, 0, 0 };
		dfc = Vector2();
		forward = { };
		eye_rot = { };
	}
};

class BaseNetworkable : public BaseMonoBehaviour {
public:
	FIELD(0x58, net, Networkable*);
};

class TimeWarning {
public:
	static TimeWarning* New(System::string name, int maxmilliseconds = 0) {
		pent
			return timewarnnew(name, maxmilliseconds);
	}
};

class BaseEntity : public BaseNetworkable {
public:
	//FIELD(_("BaseEntity"), _("model"), model, Model*);
	FIELD(O::BaseEntity::_name, name, System::string*);

	void SendSignalBroadcast(Signal signal, wchar_t* str = _(L""))
	{
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return SendSignal((uintptr_t)this, signal, System::string(str));
	}
	Model* model() {
		pent
			return *reinterpret_cast<Model**>((uintptr_t)this + O::BaseEntity::model);
	}
	Item* GetItem() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return baseentgetitem(this);
	}
	float WaterFactor() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0.f;
		return waterfactor(this);
	}
	Vector3 ClosestPoint(Vector3 point) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3::Zero();
		return closestpoint(this, point);
	}
	Vector3 GetWorldVelocity() {
		pent
			__try {
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
			typedef Vector3(*GWV)(BaseEntity*);
			return ((GWV)(mem::game_assembly_base + oGetWorldVelocity))(this);
		}
		__except (true) { pent return Vector3::Zero(); }
	}
	Vector3 GetParentVelocity() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return getparentvel(this);
	}
	void ServerRPC(wchar_t* func) {
		__try {
			pent
				if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
			return serverrpc(this, func);
		} __except (true) { return; }
	}
	void ServerRPCs(const char* func) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		auto s = std::string(func);
		return serverrpc(this, std::wstring(s.begin(), s.end()).c_str());
	}
	bool is_visible(Vector3 source, Vector3 destination, float p1 = 0.03f) {
		//	pent
		return unity::is_visible(source, destination, 0);
	}
};

class BaseCombatEntity : public BaseEntity {
public:
	FIELD(O::BaseCombatEntity::lifestate, lifestate, BaseCombatEntity_LifeState);
	FIELD(_health, health, float);
	FIELD(_maxHealth, maxHealth, float);



	bool is_alive() {
		__try {
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
			return this->lifestate() == BaseCombatEntity_LifeState::Alive;
		}
		__except (true) { return false; }
	}
};

class HeldEntity : public BaseEntity {
public:
	FIELD(O::HeldEntity::viewModel, viewModel, ViewModel*);
};

class BaseHelicopter_Weakspot {

};

class BaseHelicopter : public BaseCombatEntity {
public:
	FIELD(O::BaseHelicopter::weakspots, weakspots, System::list<BaseHelicopter_Weakspot*>*);
};

class ItemModProjectile : public MonoBehaviour {
public:
	FIELD(O::ItemModProjectile::projectileVelocity, projectileVelocity, float);
	FIELD(O::ItemModProjectile::projectileSpread, projectileSpread, float);
	FIELD(O::ItemModProjectile::projectileVelocitySpread, projectileVelocitySpread, float);
	FIELD(O::ItemModProjectile::projectileObject, projectileObject, Projectile*);
};

class BasePlayer;

class RecoilProperties {

};

class DamageProperties {

};

class HitTest {
public:
	enum HitTest_Type {
		Generic = 0,
		ProjectileEffect = 1,
		Projectile = 2,
		MeleeAttack = 3,
		Use = 4
	};

	FIELD(O::HitTest::type, type, HitTest_Type);
	FIELD(O::HitTest::AttackRay, AttackRay, Ray);
	FIELD(O::HitTest::Radius, Radius, float);
	FIELD(O::HitTest::MaxDistance, MaxDistance, float);
	FIELD(O::HitTest::BestHit, BestHit, bool);
	FIELD(O::HitTest::DidHit, DidHit, bool);
	FIELD(O::HitTest::damageProperties, damageProperties, DamageProperties*);
	FIELD(O::HitTest::ignoreEntity, ignoreEntity, BaseEntity*);
	FIELD(O::HitTest::HitEntity, HitEntity, BaseEntity*);
	FIELD(O::HitTest::HitPoint, HitPoint, Vector3);
	FIELD(O::HitTest::HitNormal, HitNormal, Vector3);
	FIELD(O::HitTest::HitDistance, HitDistance, float);
	FIELD(O::HitTest::HitTransform, HitTransform, Transform*);
	FIELD(O::HitTest::HitPart, HitPart, UINT);
	FIELD(O::HitTest::HitMaterial, HitMaterial, System::string);
	FIELD(O::HitTest::gameObject, gameObject, GameObject*);

	void set_hit_Transform(Transform* hit_Transform) {
		pent
			* reinterpret_cast<Transform**>((uintptr_t)this + 0xB0) = hit_Transform;
	}

	void set_did_hit(bool did_hit) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + 0x66) = did_hit;
	}

	void set_attack_ray(Ray ray) {
		pent
			* reinterpret_cast<Ray*>((uintptr_t)this + 0x14) = ray;
	}

	void set_best_hit(bool best_hit) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + 0x65) = best_hit;
	}

	void set_max_distance(float max_dist) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + 0x34) = max_dist;
	}
	void set_hit_distance(float max_dist) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + 0xA8) = max_dist;
	}

	void set_hit_entity(BasePlayer* entity) {
		pent
			* reinterpret_cast<BasePlayer**>((uintptr_t)this + 0x88) = entity;
	}

	BasePlayer* get_hit_entity() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0x1000000000000000) return nullptr;
		pent
			return *reinterpret_cast<BasePlayer**>((uintptr_t)this + 0x88);
	}

	void set_hit_point(Vector3 hit_point) {
		pent
			* reinterpret_cast<Vector3*>((uintptr_t)this + 0x90) = hit_point;
	}

	void set_hit_normal(Vector3 hit_nromal) {
		pent
			* reinterpret_cast<Vector3*>((uintptr_t)this + 0x9C) = hit_nromal;
	}
	void set_damage_properties(uintptr_t damage_properties) {
		pent
			* reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x68) = damage_properties;
	}
};

class GatherPropertyEntry {
public:
	float& gatherDamage() {
		pent
			return *reinterpret_cast<float*>((uintptr_t)this + 0x10);
	}

	float& destroyFraction() {
		pent
			return *reinterpret_cast<float*>((uintptr_t)this + 0x14);
	}

	float& conditionLost() {
		pent
			return *reinterpret_cast<float*>((uintptr_t)this + 0x18);
	}
};

class ItemDefinition : public MonoBehaviour {
public:
	FIELD(O::ItemDefinition::displayName, _displayName, System::string*);
	FIELD(O::ItemDefinition::shortname, shortname, System::string*);
	member(int, itemid, 0x18);
	//FIELD(_("ItemDefinition"), _("itemid"), itemid, int);
};

class GatherProperties {
public:
	GatherPropertyEntry*& tree() {
		pent
			return *reinterpret_cast<GatherPropertyEntry**>((uintptr_t)this + 0x10);
	}

	GatherPropertyEntry*& ore() {
		pent
			return *reinterpret_cast<GatherPropertyEntry**>((uintptr_t)this + 0x18);
	}
};

class AttackEntity : public HeldEntity {
public:
	FIELD(O::AttackEntity::nextAttackTime, nextAttackTime, float);
	FIELD(O::AttackEntity::timeSinceDeploy, timeSinceDeploy, float);
	FIELD(O::AttackEntity::deployDelay, deployDelay, float);
	FIELD(O::AttackEntity::repeatDelay, repeatDelay, float);
};

class BaseMelee : public AttackEntity {
public:
	FIELD(O::BaseMelee::gathering, gathering, GatherProperties*);
	FIELD(O::BaseMelee::damageProperties, damageProperties, DamageProperties*);
	FIELD(O::BaseMelee::maxDistance, maxDistance, float);
	FIELD(O::BaseMelee::attackRadius, attackRadius, float);

	uintptr_t get_damage_properties() {
		pent
			return *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x288);
	}

	bool CanHit(HitTest* ht) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return canhit(this, ht);
	}

	void DoThrow() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return basemeldothrow(this);
	}

	void DoAttack() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		bmdoattack(this);
	}
};

class MapNote {
public:
	member(Vector3, worldPosition, 0x18);
	//FIELD(_("MapNote"), _("worldPosition"), worldPosition, Vector3);
};

class Magazine {
public:
	FIELD(0x18, capacity, int);
	FIELD(0x1C, contents, int);
	FIELD(0x20, ammoType, ItemDefinition*);

	bool CanReload(BasePlayer* owner) {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		typedef bool(*A)(Magazine*, BasePlayer*);
		return ((A)(mem::game_assembly_base + oCanReload))(this, owner);
	}
};

class BaseProjectile : public AttackEntity {
public:  
	FIELD(O::BaseProjectile::reloadTime, reloadTime, float);
	FIELD(O::BaseProjectile::nextReloadTime, nextReloadTime, float);
	FIELD(O::BaseProjectile::recoil, recoil, RecoilProperties*);
	FIELD(O::BaseProjectile::automatic, automatic, bool);
	FIELD(O::BaseProjectile::aimSway, aimSway, float);
	FIELD(O::BaseProjectile::aimSwaySpeed, aimSwaySpeed, float);
	FIELD(O::BaseProjectile::primaryMagazine, primaryMagazine, Magazine*);
	FIELD(O::BaseProjectile::aimCone, aimCone, float);
	FIELD(O::BaseProjectile::hipAimCone, hipAimCone, float);
	FIELD(O::BaseProjectile::aimConePenaltyMax, aimConePenaltyMax, float);
	FIELD(O::BaseProjectile::aimconePenaltyPerShot, aimconePenaltyPerShot, float);
	FIELD(O::BaseProjectile::stancePenaltyScale, stancePenaltyScale, float);
	FIELD(O::BaseProjectile::projectileVelocityScale, projectileVelocityScale, float);

	void set_projectile_thickness(float thickness)
	{
		*reinterpret_cast<float*>((uintptr_t)this + thickness_addr) = thickness;
	}

	void LaunchProjectile() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return launchproj((uintptr_t)this);
	}

	void set_last_hit_time(float time) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + lastHitTime) = time;
	}

	void set_last_hit_material(System::string material) {
		pent
			* reinterpret_cast<System::string*>((uintptr_t)this + lastHitMaterial) = material;
	}

	HitTest* get_hit_test() {
		pent
			return *reinterpret_cast<HitTest**>((uintptr_t)this + hitTest);
	}

	void set_integrity(float to_set) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + integrity) = to_set;
	}

	ItemModProjectile* get_item_mod_projectile() {
		pent
			return *reinterpret_cast<ItemModProjectile**>((uintptr_t)this + mod);
	}

	void remove_ammo() {
		pent
			const auto mag = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x2C0);
		if (!mag || mag < 0xFFFF) return;
		auto ammo = *reinterpret_cast<int*>(mag + 0x1C);
		*reinterpret_cast<int*>(mag + 0x1C) = (ammo - 1);

		updateammodisplay((uintptr_t)this);
		shot_fired((uintptr_t)this);
		did_attack_client_side((uintptr_t)this);
		return;
	}
	
	int ammo_left() {
		pent
			__try {
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
			const auto mag = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x2C0);
			if (!mag || mag < 0xFFFF) return 0;
			const auto ammo = *reinterpret_cast<int*>(mag + 0x1C);
			return ammo;
		} __except (true) { return 0; }
	}

	uintptr_t get_damage_properties() {
		pent
			return *reinterpret_cast<uintptr_t*>((uintptr_t)this + damageProperties);
	}

	weapon_stats_t get_stats(int32_t weapon_id) {
		pent
			const auto primary_magazine = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x2C0);
		if (!primary_magazine)
			return weapon_stats_t{ xf(1000) };

		float velocity = xf(1000);
		float gravity_modifier = xf(1);
		float drag = xf(.001f);
		float distance = xf(0);

		auto velocity_scale = xf(1);
		bool scale_velocity = false;

		const auto ammo_definition = *reinterpret_cast<uintptr_t*>((uintptr_t)primary_magazine + 0x20);
		//auto ammo_definition = primary_magazine->ammoType();//*reinterpret_cast<uintptr_t*>((uintptr_t)primary_magazine + 0x20);
		if (ammo_definition) {
			pent
				// itemid
				//const auto ammo_id = ammo_definition->itemid;//*reinterpret_cast<int32_t*>((uintptr_t)ammo_definition + 0x18);
				const auto ammo_id = *reinterpret_cast<int32_t*>((uintptr_t)ammo_definition + 0x18);
			if (ammo_id)
			{
				switch (ammo_id) {
					pent
				case rocket_basic:
					drag = xf(0.1f);
					gravity_modifier = xf(10.f);
					break;
				case rocket_hv:
					drag = xf(0.f);
					gravity_modifier = xf(0.f);
					break;
					break;
				case rocket_incen:
					drag = xf(0.1f);
					gravity_modifier = xf(10.f);
					break;
					break;
				case shotgun:
					velocity = xf(225);
					drag = xf(1);
					distance = xf(3);
					break;
				case shotgun_slug:
					velocity = xf(225);
					drag = xf(1);
					distance = xf(10);
					break;
				case shotgun_fire:
					velocity = 100;
					drag = 1;
					distance = 3;
					break;
				case shotgun_handmade:
					velocity = 100;
					drag = 1;
					distance = 0;
					break;
				case rifle_556:
					velocity = 375;
					drag = .6;
					distance = 15;
					break;
				case rifle_556_hv:
					velocity = 450;
					drag = .6;
					distance = 15;
					break;
				case rifle_556_fire:
					velocity = 225;
					drag = .6;
					distance = 15;
					break;
				case rifle_556_explosive:
					velocity = 225;
					gravity_modifier = 1.25;
					drag = .6;
					distance = 15;
					break;
				case pistol:
					velocity = 300;
					drag = .7;
					distance = 15;
					break;
				case pistol_hv:
					velocity = 400;
					drag = .7;
					distance = 15;
					break;
				case pistol_fire:
					velocity = 225;
					drag = .7;
					distance = 15;
					break;
				case arrow_wooden:
					velocity = 50;
					gravity_modifier = .75;
					drag = .005;
					break;
				case arrow_hv:
					velocity = 80;
					gravity_modifier = .5;
					drag = .005;
					break;
				case arrow_fire:
					velocity = 40;
					gravity_modifier = 1;
					drag = .01;
					break;
				case arrow_bone:
					velocity = 45;
					gravity_modifier = .75;
					drag = .01;
					break;
				case nailgun_nails:
					velocity = 50;
					gravity_modifier = .75;
					drag = .005;
					break;
				}
			}

			scale_velocity = true;
			velocity_scale =
				GetProjectileVelocityScale(this, false);
		}

		switch (weapon_id) {
		case spear_wooden:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case spear_stone:
			velocity = 30;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case hatchet:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case stonehatchet:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case pickaxe:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case stonepickaxe:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case salvageaxe:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case cleaver:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case hammer:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case icepick:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case sword:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case boneknife:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case butcherknife:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case combatknife:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case rock:
			velocity = 15;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		case snowball:
			velocity = 25;
			scale_velocity = false;
			gravity_modifier = 2;
			drag = .1f;
			distance = .25f;
			break;
		}

		if (scale_velocity && (velocity_scale != 0))
			velocity *= velocity_scale;

		return { velocity, gravity_modifier, drag, distance };
	}

	Vector3 get_current_position() {
		pent
			return *reinterpret_cast<Vector3*>((uintptr_t)this + currentPosition);
	}

	void set_current_position(Vector3 cp) {
		pent
			* reinterpret_cast<Vector3*>((uintptr_t)this + currentPosition) = cp;
	}

	void set_travel_time(float f) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + traveledTime) = f;
	}

	void set_current_velocity(Vector3 position)
	{
		*reinterpret_cast<Vector3*>((uintptr_t)this + currentVelocity) = position;
	}

	uint32_t get_size() { pent return *reinterpret_cast<uint32_t*>((uintptr_t)this + 0x18); }

	float oyx = 0.f;
	float oyy = 0.f;
	float opx = 0.f;
	float opy = 0.f;

	void set_recoil() {
		pent
			auto recoil_properties = *reinterpret_cast<uintptr_t*>((uintptr_t)this + il2cpp::value(_("BaseProjectile"), _("recoil")));//this->recoil();//

		float recoilx = vars->combat.recoilx;
		float recoily = vars->combat.recoily;

		if (!recoil_properties) return;
		//after update June 5th 2022
		auto new_recoil_properties = *reinterpret_cast<uintptr_t*>((uintptr_t)recoil_properties + 0x78);

		if (new_recoil_properties)
		{
			*reinterpret_cast<float*>(new_recoil_properties + 0x18) = recoily; //yaw min
			*reinterpret_cast<float*>(new_recoil_properties + 0x1C) = recoily; //yaw max
			*reinterpret_cast<float*>(new_recoil_properties + 0x20) = -recoilx; //pitch min
			*reinterpret_cast<float*>(new_recoil_properties + 0x24) = -recoilx; //pitch max
			//*reinterpret_cast<float*>(new_recoil_properties + 0x2C) = lastrecoil * recoil; //timeToTakeMax
			//*reinterpret_cast<float*>(new_recoil_properties + 0x30) = recoil; //ADSScale
		}
		else
		{
			*reinterpret_cast<float*>(recoil_properties + 0x18) = recoily; //yaw min
			*reinterpret_cast<float*>(recoil_properties + 0x1C) = recoily; //yaw max
			*reinterpret_cast<float*>(recoil_properties + 0x20) = -recoilx; //pitch min
			*reinterpret_cast<float*>(recoil_properties + 0x24) = -recoilx; //pitch max
			//*reinterpret_cast<float*>(recoil_properties + 0x2C) = lastrecoil * recoil; //timeToTakeMax
			//*reinterpret_cast<float*>(recoil_properties + 0x30) = recoil; //ADSScale
		}
	}

	void set_no_spread(float scale = 0.f) {
		pent
			auto recoil_properties = *reinterpret_cast<uintptr_t*>((uintptr_t)this + il2cpp::value(_("BaseProjectile"), _("recoil")));//this->recoil();//

		float recoilx = vars->combat.recoilx;
		float recoily = vars->combat.recoily;

		if (!recoil_properties) return;
		//after update June 5th 2022
		auto new_recoil_properties = *reinterpret_cast<uintptr_t*>((uintptr_t)recoil_properties + 0x78);

		this->aimCone() = scale;
		this->hipAimCone() = scale;
		this->aimConePenaltyMax() = scale;
		this->aimconePenaltyPerShot() = scale;
		this->stancePenaltyScale() = scale;

		if (new_recoil_properties)
			*reinterpret_cast<float*>(new_recoil_properties + 0x60) = scale; //aimconeCurveScale
		else *reinterpret_cast<float*>(recoil_properties + 0x60) = scale; //aimconeCurveScale
	}

	void set_success_fraction() {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + successFraction) = 1.f;
	}

	int ammo_cap() {

		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		const auto mag = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x2C0);
		if (!mag || mag < 0xFFFF || mag > 0x1000000000000000) return 0;
		const auto ammo = *reinterpret_cast<int*>(mag + 0x18);
		return ammo;
	}
	//////EOKA/////
	void set_did_spark_this_frame(bool state) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + didSparkThisFrame) = state;
	}
};

class PlayerNameID {
public:
	wchar_t* get_username() {
		pent
			auto username = (str)(*reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x18));

		return username->str;
	}

	unsigned long get_user_id()
	{
		return *reinterpret_cast<unsigned long*>((uintptr_t)this + 0x20);
	}
};

class RaycastHit {
public:
	Vector3 point() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3::Zero();
		return *reinterpret_cast<Vector3*>(this);
	}
	Vector3 normal() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3::Zero();
		return *reinterpret_cast<Vector3*>(this + 0xC);
	}
	Transform* transform() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0x1000000000000000) return nullptr;
		return raycasthit_get_transform(this);
	}
};

class Item : public Component {
public:
	FIELD(O::Item::heldEntity, heldEntity, HeldEntity*);
	FIELD(O::Item::info, info, ItemDefinition*);
	FIELD(O::Item::name, name, System::string*);
	FIELD(O::Item::uid, uid, unsigned int);

	uintptr_t get_icon_sprite() {
		pent
			const auto item_definition = this->info();
		if (!item_definition)
			return 0;

		return *reinterpret_cast<uintptr_t*>(item_definition + iconSprite);
	}

	uintptr_t get_steam_icon_sprite() {
		pent
			const auto item_definition = this->info();
		if (!item_definition)
			return 0;

		auto SteamInventoryItem = *reinterpret_cast<uintptr_t*>((uintptr_t)item_definition + steamItem);
		if (!SteamInventoryItem)
			return 0;

		return *reinterpret_cast<uintptr_t*>(SteamInventoryItem + 0x20);
	}

	int GetAmount()
	{
		return *reinterpret_cast<int*>(this + O::Item::amount);
	}

	wchar_t* get_weapon_name() {
		pent
			//const auto item_definition = this->info();
			auto item_definition = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x20);
		if (!item_definition)
			return {};

		auto display_name = *reinterpret_cast<uintptr_t*>((uintptr_t)item_definition + displayName);
		if (!display_name)
			return {};

		auto weapon_name = (str)(*reinterpret_cast<uintptr_t*>(display_name + 0x18));

		return weapon_name->str;
	}

	bool is_weapon() {
		pent
			const auto item_definition = this->info();
		if (!item_definition)
			return false;

		return mem::read<uint32_t>((uintptr_t)item_definition + category) == 0;
	}

	int32_t get_item_definition_id()
	{
		const auto item_definition = this->info();
		if (!item_definition)
			return 0;

		return *reinterpret_cast<int32_t*>((uintptr_t)item_definition + itemid);
	}

	template<typename T = BaseEntity>
	T* GetHeldEntity() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		//return ((T*)this->heldEntity());
		return *reinterpret_cast<T**>(this + O::Item::heldEntity);
	}
};

class ModelState {
public:
	float get_water_level() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		return *reinterpret_cast<float*>((uintptr_t)this + 0x14);
	}

	void set_water_level(float water_level) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
			* reinterpret_cast<float*>((uintptr_t)this + 0x14) = water_level;
	}

	void setjumped(bool j) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return set_jumped((uintptr_t)this, j);
	}

	void setsprinting(bool j) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return set_sprinting((uintptr_t)this, j);
	}

	void setaiming(bool j) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return set_aiming((uintptr_t)this, j);
	}

	void setducked(bool j) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return set_ducked((uintptr_t)this, j);
	}

	void remove_flag(ModelState_Flag flag) {
		pent
			int flags = *reinterpret_cast<int*>((uintptr_t)this + O::BasePlayer::playerFlags);
		flags &= ~(int)flag;

		*reinterpret_cast<int*>((uintptr_t)this + O::BasePlayer::playerFlags) = flags;
	}

	void set_flag(ModelState_Flag flag) {
		pent
			int flags = *reinterpret_cast<int*>((uintptr_t)this + O::BasePlayer::playerFlags);

		*reinterpret_cast<int*>((uintptr_t)this + O::BasePlayer::playerFlags) = flags |= (int)flag;
	}

	bool has_flag(ModelState_Flag f)
	{
		int& flags = *reinterpret_cast<int*>((uintptr_t)this + O::BasePlayer::playerFlags);

		return (flags & (int)f) == (int)f;
	}
};

class BaseMovement : public MonoBehaviour {
public:
	void ClientInput(InputState* i, ModelState* m) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return bmclientinput(this, i, m);
	}
};

class Collider : public Component {

};

class CapsuleCollider : public Collider {
public:
	float GetRadius() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		return capgetrad(this);
	}
	void SetRadius(float f) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return capsetrad(this, f);
	}
	float GetHeight() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		return capgetheight(this);
	}
	void SetHeight(float f) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return capsetheight(this, f);
	}
};

class PlayerWalkMovement : public BaseMovement {
public:
	FIELD(O::PlayerWalkMovement::capsuleHeight, capsuleHeight, float);
	FIELD(O::PlayerWalkMovement::capsuleCenter, capsuleCenter, float);
	FIELD(O::PlayerWalkMovement::capsuleHeightDucked, capsuleHeightDucked, float);
	FIELD(O::PlayerWalkMovement::capsuleCenterDucked, capsuleCenterDucked, float);
	FIELD(O::PlayerWalkMovement::capsule, capsule, CapsuleCollider*);

	void set_swimming(bool flag) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + swimming) = flag;
	}
	bool get_swimming() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + swimming);
	}
	float get_ducking() {
		pent
			return *reinterpret_cast<float*>((uintptr_t)this + ducking);
	}
	void set_TargetMovement(Vector3 v) {
		pent
			* reinterpret_cast<Vector3*>((uintptr_t)this + TargetMovement) = v;
	}
	Vector3 get_TargetMovement() {
		pent
			return *reinterpret_cast<Vector3*>((uintptr_t)this + TargetMovement);
	}
	bool get_admin_cheat() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x18);
	}

	uintptr_t get_body() {
		pent
			return *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0xA0);
	}

	Vector3 get_body_velocity() {
		pent
			return get_rigidbody_velocity(get_body());
	}

	void set_body_velocity(Vector3 v) {
		pent
			set_rigidbody_velocity(get_body(), v);
	}

	void set_admin_cheat(bool admin_cheat) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + 0x18) = admin_cheat;
	}

	bool get_flying() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + flying);
	}

	bool get_grounded() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x140); //private bool grounded
	}

	bool get_climbing() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x141); //private bool climbing
	}

	bool get_sliding() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x142); //private bool sliding
	}

	void force_jump(ModelState* state, bool indirection = false) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return do_jump((uintptr_t)this, (uintptr_t)state, indirection);
	}

	void set_flying(bool fly) {
		pent
			* reinterpret_cast<bool*>((uintptr_t)this + flying) = fly;
	}

	void set_ground_angles_new(float angle) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + groundAngleNew) = angle;
	}

	void set_land_time(float time) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + landTime) = time;
	}

	void set_jump_time(float time) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + jumpTime) = time;
	}

	void set_ground_time(float time) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + groundTime) = time;
	}

	void set_gravity_multiplier(float multiplier) {
		pent
			* reinterpret_cast<float*>((uintptr_t)this + gravityMultiplier) = multiplier;
	}

	bool set_grounded(bool g) {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x140) = g; //private bool grounded
	}

	bool set_climbing(bool g) {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x141) = g; //private bool climbing
	}

	bool set_sliding(bool g) {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + 0x142) = g; //private bool sliding
	}

	void teleport_to(Vector3 p, BasePlayer* ply) {
		pent
			return teleportto((uintptr_t)this, p, (uintptr_t)ply);
	}
};

auto cliententities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);

class BaseNetwork {
public:
};

class Client : public BaseNetwork {
public:
	FIELD(0x40, ConnectedAddress, System::string*);
	FIELD(0x48, ConnectedPort, int);
	FIELD(0x50, ServerName, System::string*);
};

class Server {
public:
	FIELD(0x28, ip, System::string*);
	FIELD(0x30, port, int);
};

class Networkable {
public:
	FIELD(0x48, sv, Server*);
	FIELD(0x50, cl, Client*);

	unsigned int get_id() {

		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return -1;
		return *reinterpret_cast<unsigned int*>((uintptr_t)this + 0x10);
	}

	template<typename T = uintptr_t>
	T* GetComponent(uintptr_t type) {
		pent
			if (!this || !type) return nullptr;
		return (T*)get_component((uintptr_t)this, type);
	}

	System::list<uintptr_t>* GetComponentsInChildren(uintptr_t type) {
		pent
			if (!this || !type) return nullptr;
		return reinterpret_cast<System::list<uintptr_t>*>(get_components_in_children((uintptr_t)this, type));
	}
};


namespace Convar {
	class Admin {
	public:
		static void Set_admintime(float f) {
			pent
				auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oConvarAdmin);
			auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
			mem::write<float>(fieldz + 0x0, f);
		}
	};
}

float get_2d_dist(const Vector2& Src, const Vector3& Dst) {
	pent
		return Vector3::my_sqrt(powFFFFFFFFFFFFFFFFFFFFFF(Src.x - Dst.x) + powFFFFFFFFFFFFFFFFFFFFFF(Src.y - Dst.y));
}

class PlayerEyes : public Component {
public:
	void set_view_offset(Vector3 offset) {
		pent
			* reinterpret_cast<Vector3*>((uintptr_t)this + viewOffset) = offset;
	}

	Vector3 get_view_offset() {
		pent
			return *reinterpret_cast<Vector3*>((uintptr_t)this + viewOffset);
	}

	Vector3 position() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return PEyes_get_position((uintptr_t)this);
	}

	Vector4 rotation() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector4(0, 0, 0, 0);
		return PEyes_get_rotation((uintptr_t)this);
	}

	Vector3 head_forward() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return headforward((uintptr_t)this);
	}

	Vector3 head_right() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return headright((uintptr_t)this);
	}

	Vector3 body_forward() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return bodyforward((uintptr_t)this);
	}

	Vector3 body_right() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return bodyright((uintptr_t)this);
	}

	Vector3 GetCenter() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return Vector3(0, 0, 0);
		return get_center((uintptr_t)this);
	}

	Vector3 EyeOffset() {
		auto kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oPlayerEyes_TypeInfo);
		return *reinterpret_cast<Vector3*>(kl + 0xB8); //eye offset is at + 0x0 from class
	}
};

class InputMessage {
public:
	FIELD(0x24, mouseDelta, Vector3);
};

class InputState {
public:
	FIELD(0x10, current, InputMessage*)

	void set_aim_angles(Vector3 aim_angle) {
		pent
			auto current = mem::read<uintptr_t>((uintptr_t)this + 0x10);
		if (!current) return;
		*reinterpret_cast<Vector3*>(current + 0x18) = aim_angle;
	}

	Vector3 get_aim_angles() {
		auto current = mem::read<uintptr_t>((uintptr_t)this + 0x10);
		if (!current) return Vector3::Zero();
		return *reinterpret_cast<Vector3*>(current + 0x18);
	}
};

class BaseMountable : public BaseCombatEntity {
public:
	bool& canwielditem() {
		pent
			return *reinterpret_cast<bool*>((uintptr_t)this + canWieldItems);
	}
};

class Shader : public Object {
public:

};

class Material : public Object {
public:
	Shader* GetShader() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return getshader(this);
	}
	void SetShader(Shader* s) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return setshader(this, s);
	}
	void SetColor(System::string s, col c) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return setcolor(this, s, c);
	}
	void SetFloat(System::string s, float n) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return setfloat(this, s, n);
	}
};

class Renderer : public Component {
public:
	Material* GetMaterial() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return getmaterial(this);
	}

	void SetMaterial(Material* m) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return setmaterial(this, m);
	}
};

class SkinnedMultiMesh : public MonoBehaviour {
public:

	System::list<Renderer*>* get_Renderers() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return getrenderers(this);
	}
};

class PlayerInput : public Component {
public:
	FIELD(O::PlayerInput::state, state, InputState*);
	FIELD(O::PlayerInput::bodyAngles, bodyAngles, Vector3);
	FIELD(O::PlayerInput::recoilAngles, recoilAngles, Vector3);
};

class PlayerModel : public Component {
public:
	FIELD(O::PlayerModel::newVelocity, newVelocity, Vector3);
	FIELD(O::PlayerModel::_multiMesh, _multiMesh, SkinnedMultiMesh*);

	bool isnpc() {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return get_IsNpc((uintptr_t)this);
	}
	bool isLocalPlayer() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF) return false;
		//if ((uintptr_t)this > 0xFFFFFFFFFFFF0000)
		return *reinterpret_cast<bool*>((uintptr_t)this + 0x299);
	}
};

class ItemSlot {

};

class ItemCrafter {

};

class ItemContainer {
public:
	FIELD(O::ItemContainer::capacity, capacity, int);
	FIELD(O::ItemContainer::itemList, itemList, System::list<Item*>*);
	FIELD(O::ItemContainer::availableSlots, availableSlots, System::list<ItemSlot*>*);
};

class PlayerLoot {

};

class PlayerInventory {
public:
	FIELD(O::PlayerInventory::containerMain, containerMain, ItemContainer*);
	FIELD(O::PlayerInventory::containerBelt, containerBelt, ItemContainer*);
	FIELD(O::PlayerInventory::containerWear, containerWear, ItemContainer*);
	FIELD(O::PlayerInventory::crafting, crafting, ItemCrafter*);
	FIELD(O::PlayerInventory::loot, loot, PlayerLoot*);


	bool HasAmmo(int ammotype) {
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return hasammo(this, ammotype);
	}
};

class weapon {
public:
	BaseProjectile* get_base_projetile() {
		pent
			return *reinterpret_cast<BaseProjectile**>((uintptr_t)this + heldEntity);
	}
};

class PlayerBelt {
public:
	void SetSelectedSlot(int slot) {
		pent
			uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oPlayerBelt_TypeInfo);//il2cpp::type_object(_(""), _("PlayerBelt"));
		*reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(kl + 0xB8)) = slot;
	}
	int GetSelectedSlot() {
		pent
			uintptr_t kl = *reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oPlayerBelt_TypeInfo);//il2cpp::type_object(_(""), _("PlayerBelt"));
		return *reinterpret_cast<int*>(*reinterpret_cast<uintptr_t*>(kl + 0xB8));
	}
	Item* GetItemInSlot(int slot) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return (Item*)getiteminslot(this, slot);
	}
	void ClientInput(InputState* state) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return pbclientinput(this, state);
	}
};

class PlayerVoiceRecorder : public Component {
public:
	void ClientInput(InputState* state) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		voicerecclientinput(this, state);
	}
};

class LocalPlayer {
public:
	static BasePlayer* Entity() {
		pent
			return lpgetent();
	}

	static void ItemCommand(unsigned int ui, System::string cmd) {
		pent
			item_cmd(ui, cmd);
	}
};

class BaseVehicle : public BaseMountable {
public:

};

class BaseHelicopterVehicle : public BaseVehicle {
public:
	FIELD(0x49C, currentThrottle, float);
};

class MiniCopter : public BaseHelicopterVehicle {
public:
	FIELD(0x5D0, rotorSpeed, float);
	FIELD(0x5D4, cachedPitch, float);
	FIELD(0x5D8, cachedYaw, float);
	FIELD(0x5DC, cachedRoll, float);
};

class BasePlayer : public BaseCombatEntity {
public:
	FIELD(O::BasePlayer::playerModel, playerModel, PlayerModel*);
	FIELD(O::BasePlayer::input, input, PlayerInput*);

	FIELD(O::BasePlayer::movement, movement, PlayerWalkMovement*);

	FIELD(O::BasePlayer::_lookingAtEntity, _lookingAtEntity, BaseEntity*);
	FIELD(O::BasePlayer::clActiveItem, clActiveItem, UINT);
	FIELD(O::BasePlayer::modelState, modelState, ModelState*);
	FIELD(O::BasePlayer::eyes, eyes, PlayerEyes*);

	FIELD(O::BasePlayer::lastSentTickTime, lastSentTickTime, float);

	FIELD(O::BasePlayer::_displayName, _displayName, str);
	FIELD(O::BasePlayer::userID, userID, ULONG);
	FIELD(O::BasePlayer::clientTickInterval, clientTickInterval, float);
	FIELD(O::BasePlayer::mounted, mounted, BaseMountable*);
	FIELD(O::BasePlayer::inventory, inventory, PlayerInventory*);
	FIELD(O::BasePlayer::ClientCurrentMapNote, ClientCurrentMapNote, MapNote*);
	FIELD(O::BasePlayer::Belt, Belt, PlayerBelt*);
	FIELD(O::BasePlayer::voiceRecorder, voiceRecorder, PlayerVoiceRecorder*);
	FIELD(O::BasePlayer::Frozen, Frozen, bool);

	void SendProjectileUpdate(uintptr_t p) {
		if (!this) return;
		sendprojupdate(this, p);
	}

	void UseAction(InputState* s) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return useaction(this, s);
	}

	BaseMountable* GetMounted() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return getmounted(this);
	}

	BaseVehicle* GetMountedVehicle() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		return getmountedvehicle(this);
	}

	void HeldEntityInput() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return heldentinput(this);
	}

	bool HasLocalControls() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return haslocalcontrols(this);
	}

	void ForcePositionTo(Vector3 worldPos) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		forceposto(this, worldPos);
	}

	bool isFriend() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		if (map_contains_key(vars->gui_player_map, this->userID()))
			return vars->gui_player_map[this->userID()]->is_friend;
		return false;
	}

	bool isCached() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return (map_contains_key(cachedBones, this->userID()));
	}

	bool visible() {
		pent
			if (!this->isCached()) return false;
		if (cachedBones[this->userID()]->head->visible ||
			cachedBones[this->userID()]->neck->visible ||
			cachedBones[this->userID()]->spine4->visible ||
			cachedBones[this->userID()]->pelvis->visible ||
			cachedBones[this->userID()]->r_foot->visible ||
			cachedBones[this->userID()]->l_foot->visible ||
			cachedBones[this->userID()]->r_knee->visible ||
			cachedBones[this->userID()]->l_knee->visible) {
			pent
				return true;
		}
		return false;
	}

	BoneCache* bones() {
		pent
			return (this->isCached() ? cachedBones[this->userID()] : new BoneCache());
	}

	Vector3 GetVisibleBone(Vector3 from) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return {};
		if (from.is_empty()) return {};
		for (auto bone : { 48, 3, 4, 15, 14, 26, 57 }) {
			pent
				Vector3 TargetPosition;
			TargetPosition = this->get_bone_Transform(bone)->position();
			if (this->is_visible(from, TargetPosition)) {
				pent
					//settings::HitScanBone = bone;
					return TargetPosition;
			}
		}
		//empty
		return Vector3::Zero();
	}

	void GroundAngleNew() {
		pent
			* reinterpret_cast<float*>(this + 0xb0) = -1.0f;
	}

	float GetJumpHeight() {
		pent
			return _getjumpheight(this);
	}

	float GetRadius() {
		pent
			return _GetRadius(this);
	}

	float BoundsPadding() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0;
		return _BoundsPadding(this);
	}

	float max_velocity() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0.f;

		float s = get_maxspeed(this);
		auto m = this->mounted() || vars->misc.interactive_debug;
		if (m)
			return s * 4;
		return s;
	}

	void SendClientTick() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		return _SendClientTick(this);
	}

	protobuf::PlayerTick* lastSentTick() {
		pent
			return *reinterpret_cast<protobuf::PlayerTick**>((uintptr_t)this + 0x660);
	}

	wchar_t* get_player_name() {
		pent
			auto player_name = (str)(*reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x6F0)); //zzzz
		return player_name->str;
	}

	std::string GetName() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return "";
		auto w = std::wstring(this->get_player_name());
		return std::string(w.begin(), w.end());
	}

	bool is_npc() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return this->playerModel()->isnpc();
	}

	void fov() {
		pent
			bool zooming = false;


		if (vars->visual.zoom && unity::GetKey(vars->keybinds.zoom)) {
			pent
				zooming = true;
		}
		else {
			zooming = false;
		}

		if (zooming) {
			pent//0x32182E0
//auto convar = *reinterpret_cast<uintptr_t*>((uintptr_t)mem::game_assembly_base + 52689952); //"ConVar_Graphics_c*" alkad rust
auto convar = *reinterpret_cast<uintptr_t*>((uintptr_t)mem::game_assembly_base + oConvarGraphics); //"	" real rust
			auto unknown = *reinterpret_cast<uintptr_t*>((uintptr_t)convar + 0xb8);
			*reinterpret_cast<float*>(unknown + 0x18) = vars->visual.zoomfov;
		}

		if (!zooming) {
			auto convar = *reinterpret_cast<uintptr_t*>((uintptr_t)mem::game_assembly_base + oConvarGraphics); //"ConVar_Graphics_c*" real rust
				//auto convar = *reinterpret_cast<uintptr_t*>((uintptr_t)mem::game_assembly_base + 52527840); //"ConVar_Graphics_c*" alkad rust
			auto unknown = *reinterpret_cast<uintptr_t*>((uintptr_t)convar + 0xb8);
			*reinterpret_cast<float*>(unknown + 0x18) = vars->visual.playerfov;
		}
	}

	void set_player_flag(PlayerFlags flag) {
		pent
			int PlayerFlag = *reinterpret_cast<int*>((uintptr_t)this + playerFlags);

		*reinterpret_cast<int*>((uintptr_t)this + playerFlags) = PlayerFlag |= (int)flag;
	}

	bool is_teammate(BasePlayer* local_player) {
		pent
			auto team = mem::read<uintptr_t>((uintptr_t)local_player + clientTeam);

		auto member = mem::read<uintptr_t>(team + 0x30);

		auto size = mem::read<int>(member + 0x18);

		auto list = mem::read<uintptr_t>(member + 0x10);

		auto steam_id = this->userID();

		for (int i = 0; i < size; i++) {
			pent
				auto ent = mem::read<uintptr_t>(list + 0x20 + i * 0x8);

			auto id = mem::read<unsigned long>(ent + 0x20);

			if (id == steam_id)
				return true;
		}
		return false;
	}

	//auto lastSentTickTime() { pent
	//	return *reinterpret_cast<float*>((uintptr_t)this + _lastSentTickTime);
	//}
	Bone* closest_bone(BasePlayer* lp, Vector3 point, bool vischeck = true) {
		pent
			if (!this->isCached()) return nullptr;
		float bdist = 1000.f;
		Bone* bestbone = nullptr;
		if (cachedBones[this->userID()]->head->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->head->visible : true))
		{
			bestbone = cachedBones[this->userID()]->head;
			bdist = cachedBones[this->userID()]->head->position.distance(point);
		}
		if (cachedBones[this->userID()]->neck->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->neck->visible : true))
		{
			bestbone = cachedBones[this->userID()]->neck;
			bdist = cachedBones[this->userID()]->neck->position.distance(point);
		}
		if (cachedBones[this->userID()]->spine4->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->spine4->visible : true))
		{
			bestbone = cachedBones[this->userID()]->spine4;
			bdist = cachedBones[this->userID()]->spine4->position.distance(point);
		}
		if (cachedBones[this->userID()]->pelvis->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->pelvis->visible : true))
		{
			bestbone = cachedBones[this->userID()]->pelvis;
			bdist = cachedBones[this->userID()]->pelvis->position.distance(point);
		}
		if (cachedBones[this->userID()]->r_foot->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->r_foot->visible : true))
		{
			bestbone = cachedBones[this->userID()]->r_foot;
			bdist = cachedBones[this->userID()]->r_foot->position.distance(point);
		}
		if (cachedBones[this->userID()]->l_foot->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->l_foot->visible : true))
		{
			bestbone = cachedBones[this->userID()]->l_foot;
			bdist = cachedBones[this->userID()]->l_foot->position.distance(point);
		}
		if (cachedBones[this->userID()]->r_knee->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->r_knee->visible : true))
		{
			bestbone = cachedBones[this->userID()]->r_knee;
			bdist = cachedBones[this->userID()]->r_knee->position.distance(point);
		}
		if (cachedBones[this->userID()]->l_knee->position.distance(point) < bdist
			&& (vischeck ? cachedBones[this->userID()]->l_knee->visible : true))
		{
			bestbone = cachedBones[this->userID()]->l_knee;
			bdist = cachedBones[this->userID()]->l_knee->position.distance(point);
		}
		return bestbone;
	}

	System::list<Item*>* get_belt_items()
	{
		__try {
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0x1000000000000000) return nullptr;
			const auto inventory = *reinterpret_cast<uintptr_t*>((uintptr_t)this + player_inventory);
			if (!inventory)
				return {};

			const auto container_belt = *reinterpret_cast<uintptr_t*>(inventory + containerBelt);
			if (!container_belt)
				return {};

			// get_once(uint32_t, itemList, 0, get_offset("ItemContainer", "itemList"));
			const auto list_wrapper = *reinterpret_cast<uintptr_t*>(container_belt + itemList);
			if (!list_wrapper)
				return {};

			const auto item_list =
				*reinterpret_cast<System::list<Item*>**>(list_wrapper + 0x10);
			if (!item_list)
				return {};

			return item_list;
		}
		__except (true) { pent return nullptr; }
	}

	System::list<Item*>* get_wearable_items()
	{
		const auto     inventory = *reinterpret_cast<uintptr_t*>((uintptr_t)this + player_inventory);
		if (!inventory)
			return {};

		const auto container_belt = *reinterpret_cast<uintptr_t*>(inventory + containerWear);
		if (!container_belt)
			return {};

		// get_once(uint32_t, itemList, 0, get_offset("ItemContainer", "itemList"));
		const auto list_wrapper = *reinterpret_cast<uintptr_t*>(container_belt + itemList);
		if (!list_wrapper)
			return {};

		const auto item_list =
			*reinterpret_cast<System::list<Item*>**>(list_wrapper + 0x10);
		if (!item_list)
			return {};

		return item_list;
	}

	template<typename T = Networkable*>
	T find_closest(Networkable* target_entity, float max_distance, const char* class_name = "", const char* object_name = "") {
		pent
			auto client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
		if (!client_entities)
			return { nullptr };

		rust::list* entity_list = (rust::list*)client_entities;

		auto list_value = entity_list->get_value<uintptr_t>();
		if (!list_value)
			return { nullptr };

		auto size = entity_list->get_size();
		if (!size)
			return { nullptr };

		auto buffer = entity_list->get_buffer<uintptr_t>();
		if (!buffer)
			return { nullptr };

		auto closest_entity_distance = 9999;
		T best_ent = nullptr;

		for (int i = 0; i <= size; i++) {
			pent
				auto current_object = *reinterpret_cast<uintptr_t*>(buffer + 0x20 + (i * 0x8));
			if (!current_object)
				continue;

			auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
			if (!base_object)
				continue;

			auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
			if (!object)
				continue;

			auto ent = *reinterpret_cast<uintptr_t*>(base_object + 0x28);
			auto ent_class = *reinterpret_cast<uintptr_t*>(ent);
			auto entity_class_name = (char*)*reinterpret_cast<uintptr_t*>(ent_class + 0x10);

			auto name = *(int*)(entity_class_name);

			auto target_position = ((BaseEntity*)target_entity)->transform()->position();
			auto ent_position = ((BaseEntity*)ent)->transform()->position();
			auto best_position = Vector3(0, 0, 0);
			if (best_ent)
				best_position = ((BaseEntity*)best_ent)->transform()->position();

			if (strlen(class_name) > 0)
			{
				if (!LI_FIND(strcmp)(entity_class_name, class_name)
					&& ent_position.distance(target_position) < max_distance
					&& (!best_ent || ent_position.distance(target_position) < best_position.distance(target_position)))
				{
					if (strlen(object_name) > 0)
					{
						uintptr_t object_name_ptr = mem::read<uintptr_t>(object + 0x60);
						if (!object_name_ptr)
							continue;
						auto obj_name = *reinterpret_cast<rust_str*>(object_name_ptr);
						auto n = obj_name.zpad;
						if (std::string(n).find(object_name) != std::string::npos
							&& ent_position.distance(target_position) < max_distance
							&& (!best_ent || ent_position.distance(target_position) < best_position.distance(target_position)))
						{
							best_ent = reinterpret_cast<T>(ent);
						}
					}
					else
						best_ent = reinterpret_cast<T>(ent);
				}
			}
			else if (strlen(object_name) > 0)
			{
				uintptr_t object_name_ptr = mem::read<uintptr_t>(object + 0x60);
				if (!object_name_ptr)
					continue;
				auto obj_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				auto n = obj_name.zpad;
				if (std::string(n).find(object_name) != std::string::npos
					&& ent_position.distance(target_position) < max_distance
					&& (!best_ent || ent_position.distance(target_position) < best_position.distance(target_position)))
				{
					best_ent = reinterpret_cast<T>(ent);
				}
			}
		}
		return best_ent;
	}

	Transform* get_bone_Transform(int bone_id) {
		pent
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0x1000000000000000) return nullptr;
		uintptr_t entity_model = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x130); //public Model model; // 
		if (!entity_model || (uintptr_t)entity_model < 0xFFFFFFFF || (uintptr_t)entity_model > 0x1000000000000000) return nullptr;
		if (!entity_model) return nullptr;
		uintptr_t bone_dict = *reinterpret_cast<uintptr_t*>(entity_model + 0x48);
		if (!bone_dict) return nullptr;
		Transform* BoneValue = *reinterpret_cast<Transform**>(bone_dict + 0x20 + bone_id * 0x8);

		return BoneValue;
	}

	Vector3 GetBonePos(int id) {
		pent
			auto t = get_bone_Transform(id)->position();
		if (!t.is_empty()) return t;
		return Vector3::Zero();
	}

	std::pair<aim_target, bool> resolve_closest_entity(float max_distance, bool get_code = true) {
		pent
			aim_target closest_entity;
		auto client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
		if (!client_entities)
			return { closest_entity, false };

		rust::list* entity_list = (rust::list*)client_entities;

		auto list_value = entity_list->get_value<uintptr_t>();
		if (!list_value)
			return { closest_entity, false };

		auto size = entity_list->get_size();
		if (!size)
			return { closest_entity, false };

		auto buffer = entity_list->get_buffer<uintptr_t>();
		if (!buffer)
			return { closest_entity, false };

		auto closest_entity_distance = 9999;
		bool is_code_lock = false;/*iscodelock/is_tree*/

		aim_target result = aim_target();

		for (int i = 0; i <= size; i++) {
			pent
				auto current_object = *reinterpret_cast<uintptr_t*>(buffer + 0x20 + (i * 0x8));
			if (!current_object)
				continue;

			auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
			if (!base_object)
				continue;

			auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
			if (!object)
				continue;

			auto ent = *reinterpret_cast<uintptr_t*>(base_object + 0x28);
			auto ent_class = *reinterpret_cast<uintptr_t*>(ent);
			auto entity_class_name = (char*)*reinterpret_cast<uintptr_t*>(ent_class + 0x10);

			auto name = *(int*)(entity_class_name);

			if (get_code) {
				pent

					if (!(name == 'edoC') && !(name == 'LyeK'))
						continue;

				if (name == 'edoC')
					is_code_lock = true;

				auto flag = *reinterpret_cast<int*>(ent + 0x128);

				if (flag == 16)//Locked
					continue;
			}
			else {

				uintptr_t object_name_ptr = mem::read<uintptr_t>(object + 0x60);
				if (!object_name_ptr)
					continue;
				auto obj_name = *reinterpret_cast<rust_str*>(object_name_ptr);
				auto n = obj_name.zpad;

				if (name != 'eerT' &&
					name != 'HerO' &&
					name != 'RerO' &&
					strcmp(entity_class_name, _("Door")) &&
					std::string(n).find(_("barrel")) == std::string::npos &&
					strcmp(entity_class_name, _("TreeMarker")))
					continue;

				//if (name == 'HerO')
				//	is_code_lock = false;
				//else
				//	is_code_lock = true;

				//if (name == 'eerT') { pent
				//	if (*(int*)(entity_class_name + 4) != 'itnE'
				//		|| *(int*)(entity_class_name + 4) != 'kraM')
				//		continue;
				//}
			}

			auto game_object = *reinterpret_cast<uintptr_t*>(object + 0x30);

			auto transform = *reinterpret_cast<uintptr_t*>(game_object + 0x8);

			auto visual_state = *reinterpret_cast<uintptr_t*>(transform + 0x38);

			auto world_position = *reinterpret_cast<Vector3*>(visual_state + 0x90);
			//auto world_position = ((Transform*)transform)->position();

			auto bone_pos = this->get_bone_Transform(48)->position();

			auto distance = bone_pos.get_3d_dist(world_position);

			if (distance < 2.f
				&& !strcmp(entity_class_name, _("TreeMarker"))) {
				aim_target new_target;
				new_target.pos = world_position;//this->ClosestPoint(world_position);
				new_target.ent = (BaseCombatEntity*)ent;
				new_target.distance = distance;
				new_target.visible = /*unity::is_visible(bone_pos, world_position)*/true;
				new_target.found = true;
				return { new_target, true };
			}
			else if (distance < 2.f
				&& !strcmp(entity_class_name, _("TreeEntity"))) {
				aim_target new_target;
				world_position.y = vars->local_player->eyes()->position().y;
				new_target.pos = world_position;//this->ClosestPoint(world_position);
				new_target.ent = (BaseCombatEntity*)ent;
				new_target.distance = distance;
				new_target.visible = /*unity::is_visible(bone_pos, world_position)*/true;
				new_target.found = true;
				return { new_target, true };
			}
			else if (distance < 2.f
				&& !strcmp(entity_class_name, _("OreHotSpot"))) {
				aim_target new_target;
				new_target.pos = world_position;//this->ClosestPoint(world_position);
				new_target.ent = (BaseCombatEntity*)ent;
				new_target.distance = distance;
				new_target.visible = /*unity::is_visible(bone_pos, world_position)*/true;
				new_target.found = true;
				return { new_target, false };
			}
		}

		if (result.found)
			return { result, true };

		return { closest_entity , is_code_lock };
	}

	aim_target resolve_closest_player(float max_distance) {
		pent
			aim_target closest_entity;
		auto client_entities = il2cpp::value(_("BaseNetworkable"), _("clientEntities"), false);
		if (!client_entities)
			return closest_entity;

		rust::list* entity_list = (rust::list*)client_entities;

		auto list_value = entity_list->get_value<uintptr_t>();
		if (!list_value)
			return closest_entity;

		auto size = entity_list->get_size();
		if (!size)
			return closest_entity;

		auto buffer = entity_list->get_buffer<uintptr_t>();
		if (!buffer)
			return closest_entity;

		auto closest_entity_distance = 9999;

		for (int i = 0; i <= size; i++) {
			pent
				auto current_object = *reinterpret_cast<uintptr_t*>(buffer + 0x20 + (i * 0x8));
			if (!current_object)
				continue;

			auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
			if (!base_object)
				continue;

			auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
			if (!object)
				continue;

			auto ent = *reinterpret_cast<uintptr_t*>(base_object + 0x28);
			auto ent_class = *reinterpret_cast<uintptr_t*>(ent);
			auto entity_class_name = (char*)*reinterpret_cast<uintptr_t*>(ent_class + 0x10);

			auto name = *(int*)(entity_class_name);

			if (strcmp(entity_class_name, _("BasePlayer")))
				continue;
			if (((BasePlayer*)ent)->is_local_player())
				continue;

			auto game_object = *reinterpret_cast<uintptr_t*>(object + 0x30);

			auto Transform = *reinterpret_cast<uintptr_t*>(game_object + 0x8);

			auto visual_state = *reinterpret_cast<uintptr_t*>(Transform + 0x38);

			auto world_position = *reinterpret_cast<Vector3*>(visual_state + 0x90);

			auto bone_pos = this->get_bone_Transform(48)->position();

			auto distance = bone_pos.get_3d_dist(world_position);
			if (distance < closest_entity_distance && distance < max_distance) {
				pent
					auto object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);
				if (!object_class)
					continue;

				auto entity = *reinterpret_cast<uintptr_t*>(object_class + 0x18);
				if (!entity)
					continue;

				auto baseentity = *reinterpret_cast<uintptr_t*>(entity + 0x28);
				if (!baseentity)
					continue;

				auto player = reinterpret_cast<BasePlayer*>(baseentity);

				aim_target new_target;

				auto local = this->ClosestPoint(world_position);
				if (local.get_3d_dist(world_position) >= max_distance)
					continue;

				new_target.pos = this->ClosestPoint(world_position);
				new_target.ent = (BaseCombatEntity*)ent;
				new_target.distance = distance;
				new_target.visible = /*unity::is_visible(bone_pos, world_position)*/true;
				new_target.found = true;

				closest_entity_distance = distance;
				closest_entity = new_target;
			}
		}

		return { closest_entity };
	}

	bool is_sleeping()
	{
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		auto Flags = *reinterpret_cast<int*>((uintptr_t)this + playerFlags);
		return Flags & 16;
	}

	Item* GetActiveItem()
	{
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return nullptr;
		//unsigned int ActUID = this->clActiveItem();
		unsigned int ActUID = mem::read<unsigned int>((uintptr_t)this + 0x5D0); //private uint clActiveItem; //0x5D0
		if (!ActUID)
			return 0;
		Item* ActWeapon;

		uint64_t Inventory = mem::read<uint64_t>((uintptr_t)this + player_inventory);
		uint64_t Belt = mem::read<uint64_t>(Inventory + 0x28);
		uint64_t ItemList = mem::read<uint64_t>(Belt + 0x38);

		auto items = mem::read<uint64_t>(ItemList + 0x10);
		if (!items)
			return 0;

		for (int i = 0; i < 6; i++) //For each slot	
		{
			Item* WeaponInfo = mem::read<Item*>(items + 0x20 + (i * 0x8));

			unsigned int WeaponUID = WeaponInfo->uid();
			if (!WeaponUID)
				return 0;
			if (ActUID == WeaponUID)
			{
				ActWeapon = WeaponInfo;
				return ActWeapon;
			}
		}
		return 0;
	}

	Item* get_active_weapon()
	{
		unsigned int ActUID = mem::read<unsigned int>((uintptr_t)this + 0x5D0);
		if (!ActUID)
			return 0;
		Item* ActWeapon;

		uint64_t Inventory = mem::read<uint64_t>((uintptr_t)this + player_inventory);
		uint64_t Belt = mem::read<uint64_t>(Inventory + 0x28);
		uint64_t ItemList = mem::read<uint64_t>(Belt + 0x38);

		auto items = mem::read<uint64_t>(ItemList + 0x10);
		if (!items)
			return 0;

		for (int i = 0; i < 6; i++) //For each slot	
		{
			Item* WeaponInfo = mem::read<Item*>(items + 0x20 + (i * 0x8));
			unsigned int WeaponUID = WeaponInfo->uid();
			if (!WeaponUID)
				return 0;
			if (ActUID == WeaponUID)
			{
				ActWeapon = WeaponInfo;
				return ActWeapon;
			}
		}
		return 0;
	}

	bool is_local_player() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF)
				return false;

		return LocalPlayer::Entity()->userID() == this->userID();

		auto player_model = this->playerModel();
		if (!player_model)
			return false;
		return player_model->isLocalPlayer();
	}

	GameObject* get_game_object()
	{
		if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0x1000000000000000) return nullptr;
		if (!(uintptr_t)this)
			return nullptr;

		const auto unk0 = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x10);
		if (!unk0)
			return nullptr;

		const auto unk1 = *reinterpret_cast<GameObject**>(unk0 + 0x30);
		if (!unk1)
			return nullptr;

		return unk1;
	}

	void console_echo(const wchar_t* str) {
		pent
			//string::format(("%s %d"), _("B:"), (int)vars->visual.VisBcolor))
			//auto s = string::wformat(_(L"trap [%d]: %s"), (int)get_fixedTime(), str);
			if (vars->misc.logs)
				console_msg((uintptr_t)this, str);
		//else {
		//	freopen_s(reinterpret_cast<FILE**>(stdin), _("CONIN$"), _("r"), stdin);
		//	freopen_s(reinterpret_cast<FILE**>(stdout), _("CONOUT$"), _("w"), stdout);
		//	wcscat(const_cast<wchar_t*>(str), _(L"\n"));
		//	wprintf(str);
		//}
	}
};

class Model : public MonoBehaviour {
public:
	//FIELD(_("Model"), _("boneTransforms"), boneTransforms, System::Array<Transform*>*);
	FIELD(O::Model::boneNames, boneNames, System::Array<System::string*>*);

	System::Array<Transform*>* boneTransforms() {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF) return nullptr;
		return *reinterpret_cast<System::Array<Transform*>**>(this + O::Model::boneTransforms);
	}
	Bone* resolve(const wchar_t* bone_name, BasePlayer* lp) {
		pent
			__try {
			//auto lp = LocalPlayer::ent();
			if (!this || (uintptr_t)this > 0xF000000000000000 || (uintptr_t)this < 0xFFFFFFFF || !lp) return nullptr;

			auto trans = *reinterpret_cast<System::Array<Transform*>**>((uintptr_t)this + 0x48);
			auto names = *reinterpret_cast<System::Array<System::string*>**>((uintptr_t)this + 0x50);

			//auto names = this->boneNames();
			//auto trans = this->boneTransforms();
			if (!names || !trans) return nullptr;

			for (size_t i = 0; i < names->size(); i++)
			{
				//array may mishandle so could crash here
				auto name = names->get(i);
				auto tr = trans->get(i);
				if (!name || !tr) continue;
				auto name_w = name->str;
				if (!(wcscmp(name_w, bone_name))) {
					pent
						Vector3 ref = lp->transform()->position() + lp->transform()->up() * (lp->eyes()->EyeOffset().y + lp->eyes()->get_view_offset().y); ref.y += 1.6f;
					return new Bone(tr->position(), unity::is_visible(tr->position(), ref, 0), tr);
				}
			}
		}
		__except (true) { return nullptr; }
	}
};

class Planner : public HeldEntity {
public:
	Vector3 rotationoffset() { pent return mem::read<Vector3>((uintptr_t)this + planner_rotationoffset); }
	void rotationoffset(Vector3 o) { pent mem::write((uintptr_t)this + planner_rotationoffset, o); }
	uintptr_t currentconstruction() { pent return mem::read<uintptr_t>((uintptr_t)this + planner_currentconstruction); }
	void currentconstruction(uintptr_t o) { pent mem::write((uintptr_t)this + planner_rotationoffset, o); }
	uintptr_t guide() { pent return mem::read<uintptr_t>((uintptr_t)this + planner_guide); }
};

class DecayEntity : public Component {
public:
};

class StabilityEntity : public DecayEntity {
public:
};

class BowWeapon : public BaseProjectile {
public:
	FIELD(O::BowWeapon::arrowBack, arrowBack, float);
	FIELD(O::BowWeapon::wasAiming, wasAiming, bool);
	FIELD(O::BowWeapon::attackReady, attackReady, bool);
};

class BuildingBlock : public StabilityEntity {
public:
	BuildingGrade grade() {
		pent
			//public BuildingGrade.Enum grade; // 0x274
			return *reinterpret_cast<BuildingGrade*>((uintptr_t)this + 0x274);
	}

	bool CanAffordUpgrade(BuildingGrade g, BasePlayer* p) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return canaffordupgrade((uintptr_t)this, g, p);
	}

	bool CanChangeToGrade(BuildingGrade g, BasePlayer* p) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;

		//typedef bool (*AAA)(uintptr_t, int, BasePlayer*);//real rust 7203152
		//return ((AAA)(mem::game_assembly_base + 0x6D3430))((uintptr_t)this, (int)g, p);
		return canchangetograde((uintptr_t)this, g, p);
	}

	void Upgrade(BuildingGrade g, BasePlayer* p) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return;
		//typedef void (*AAA)(uintptr_t, int, BasePlayer*);//real rust 7203152
		//return ((AAA)(mem::game_assembly_base + 0x6D6A50))((uintptr_t)this, (int)g, p);
		return upgradetograde((uintptr_t)this, g, p);
	}
};

class BaseNavigator : public BaseMonoBehaviour {
public:
	//FIELD(_("BaseNavigator"), _("Destination"), Destination, Vector3);
};

class ThrownWeapon : public AttackEntity {
public:
	FIELD(0x288, maxThrowVelocity, float);
	FIELD(0x28C, tumbleVelocity, float);
	FIELD(0x290, overrideAngle, Vector3);
	Vector3 GetInheritedVelocity(BasePlayer* ply, Vector3 dir) {
		pent
			if (!this || !ply) return Vector3::Zero();
		return thrownwpn_inheritedvel(this, ply, dir);
	}
};

class GrenadeWeapon : public AttackEntity {
public:
};

class MolotovCocktail : public AttackEntity {
public:
};

class Effect {

};


class ScriptableObject : public Object {

};

class SkinnableGroup {
public:
	member(System::string*, Name, 0x10);
};

class Skinnable : public ScriptableObject {
public:
	FIELD(0x18, Name, System::string*);
	FIELD(0x20, ItemName, System::string*);
	FIELD(0x30, EntityPrefabName, System::string*);
	FIELD(0x40, ViewmodelPrefabName, System::string*);
	FIELD(0x28, EntityPrefab, GameObject*);
	FIELD(0x38, ViewmodelPrefab, GameObject*);
	member(System::Array<Material*>*, _sourceMaterials, 0x68);
};


class BaseViewModel : public MonoBehaviour {
public:

};

class ViewModel : public BaseViewModel {
public:

};

namespace ConVar {
	class Graphics {
	public:
		static float& _fov() {
			pent
				static auto clazz = il2cpp::init_class(_("Graphics"), _("ConVar"));//CLASS("Assembly-CSharp::ConVar::Graphics");
			return *reinterpret_cast<float*>(std::uint64_t(clazz + 0x00B8) + 0x18);
		}
	};
}

class CheaterCache {
public:
	struct Look {
		float f_Duration;
		float f_Start;
	};
	struct Idle {
		float f_Duration;
		float f_Start;
	};

	BasePlayer* self;
	bool b_PotentialCheater;
	bool b_Cheater;
	bool b_Idle;
	float f_FlyhackDistanceX;
	float f_FlyhackDistanceY;
	Vector3 currentAngles;
	std::vector<uintptr_t> v_firedProjectiles;
	std::vector<Look> v_Throughwalls;
	std::vector<Idle> v_Idles;
	std::vector<Vector3> v_Angles;
	Idle currentIdle;

	CheaterCache(BasePlayer* s) : self(s) {
		pent
			b_PotentialCheater = 0;
		b_Cheater = 0;
		b_Idle = 1;
		f_FlyhackDistanceX = 0;
		f_FlyhackDistanceY = 0;
		v_firedProjectiles = {};
		v_Throughwalls = {};
		v_Idles = {};
		currentIdle = { 0, 0 };
		currentAngles = Vector3::Zero();
	}

	bool CheckIdle(float deltaTime) {
		pent
			if (b_Idle) {
				pent
					if (!self->GetWorldVelocity().is_empty()) {
						pent
							//has started moving
							b_Idle = false;
						if (currentIdle.f_Duration > .1f
							&& currentIdle.f_Duration < 1.f) {
							pent
								v_Idles.push_back(currentIdle);
							currentIdle = { 0, get_fixedTime() };
						}
					}

				currentIdle.f_Duration += deltaTime;
			}
			else { //Check
				if (self->GetWorldVelocity().is_empty()) {
					pent
						//has turned to idle
						b_Idle = true;

				}
			}
	}

	void RecordFrame(float deltaTime) {
		pent
			if (CheckIdle(deltaTime)) {
				pent

			}
	}
};

struct OBB {
public:
	Vector4 rotation;
	Vector3 position;
	Vector3 extents;
	Vector3 forward;
	Vector3 right;
	Vector3 up;
	float reject;
	OBB(Vector3 position, Vector4 rotation, Bounds bounds) {
		pent
			this->rotation = rotation;
		auto r = Vector3(rotation.x, rotation.y, rotation.z);
		this->position = (position + r).multiply(bounds.center);
		this->extents = bounds.extents;
		auto temp = rotation * Vector4(0, 0, 1, 0);
		this->forward = Vector3(temp.x, temp.y, temp.z);
		temp = rotation * Vector4(1, 0, 0, 0);
		this->right = Vector3(temp.x, temp.y, temp.z);
		temp = rotation * Vector4(0, 1, 0, 0);
		this->up = Vector3(temp.x, temp.y, temp.z);
		this->reject = this->extents.Length();
	}

	Vector3 ClosestPoint(Vector3 target)
	{
		bool flag = false;
		bool flag2 = false;
		bool flag3 = false;
		Vector3 vector = this->position;
		Vector3 lhs = target - this->position;
		float num = lhs.dot(this->right);
		if (num > this->extents.x)
		{
			vector += this->right * this->extents.x;
		}
		else if (num < -this->extents.x)
		{
			vector -= this->right * this->extents.x;
		}
		else
		{
			flag = true;
			vector += this->right * num;
		}
		float num2 = lhs.dot(this->up);
		if (num2 > this->extents.y)
		{
			vector += this->up * this->extents.y;
		}
		else if (num2 < -this->extents.y)
		{
			vector -= this->up * this->extents.y;
		}
		else
		{
			flag2 = true;
			vector += this->up * num2;
		}
		float num3 = lhs.dot(this->forward);
		if (num3 > this->extents.z)
		{
			vector += this->forward * this->extents.z;
		}
		else if (num3 < -this->extents.z)
		{
			vector -= this->forward * this->extents.z;
		}
		else
		{
			flag3 = true;
			vector += this->forward * num3;
		}
		if (flag && flag2 && flag3)
		{
			return target;
		}
		return vector;
	}

	float Distance(OBB other) {
		pent
			OBB obb = *this;
		OBB obb2 = other;
		Vector3 vector = obb.position;
		Vector3 vector2 = obb2.position;
		vector = obb.ClosestPoint(vector2);
		vector2 = obb2.ClosestPoint(vector);
		vector = obb.ClosestPoint(vector2);
		vector2 = obb2.ClosestPoint(vector);
		return vector.distance(vector2);
	}
	float Distance(Vector3 position) { pent return position.distance(this->ClosestPoint(position)); }
};

float clamp01(float f, float min, float max) {
	pent
		return f < min ? min : f > max ? max : f;
}

class _Line {
public:
	Vector3 start;
	Vector3 end;
	_Line(Vector3 s, Vector3 e) {
		pent
			start = s; end = e;
	}
	_Line() { pent }
	Vector3 ClosestPoint(Vector3 pos)
	{
		Vector3 a = end - start;
		float magnitude = a.Length();
		if (magnitude == 0.f) return start;
		Vector3 vector = a / magnitude;
		Vector3 lhs = pos - start;

		return start + vector * clamp01(lhs.dot(vector), 0.f, magnitude);
	}
};

class TickInterpolator {
private:
	struct Segment {
		Vector3 point;
		float length;
		Segment(Vector3 a, Vector3 b) {
			pent
				this->point = b;
			this->length = a.distance(b);
		}
	};
public:
	std::vector<Segment> points = std::vector<Segment>();
	int index;
	float len;
	Vector3 currentPoint;
	Vector3 startPoint;
	Vector3 endPoint;

	void Reset() {
		pent
			this->index = 0;
		this->currentPoint = this->startPoint;
	}
	void Reset(Vector3 point) {
		pent
			this->points.clear();
		this->index = 0;
		this->len = 0.f;
		this->endPoint = point;
		this->startPoint = point;
		this->currentPoint = point;
	}
	void AddPoint(Vector3 point) {
		pent
			Segment segment = Segment(this->endPoint, point);
		this->points.push_back(segment);
		this->len += segment.length;
		this->endPoint = segment.point;
	}
	bool MoveNext(float distance) {
		pent
			float num = 0.f;
		while (num < distance && this->index < this->points.size()) {
			pent
				Segment segment = this->points[this->index];
			this->currentPoint = segment.point;
			num += segment.length;
			this->index++;
		}
		return num > 0.f;
	}
	bool HasNext() {
		pent
			return this->index < this->points.size();
	}
};

class TickHistory
{
public:
	std::vector<Vector3> points = {};
	int Count()
	{
		return points.size();
	}

	float Distance(BasePlayer* ent, Vector3 point)
	{
		Vector3 position = ent->transform()->position();
		if (Count() == 0)
		{
			return position.distance(point);
		}
		Vector4 rotation = ent->transform()->get_rotation();
		Bounds bounds = GetBounds((uintptr_t)ent);

		auto trans = ent->transform();
		bool flag = trans ? !(!trans) : false;
		VMatrix _mv; _mv.matrix_identity();
		VMatrix matrix4x = flag ? _mv : get_localToWorldMatrix(trans);

		VMatrix tickHistoryMatrix = matrix4x;
		float num = FLT_MAX;
		for (int i = 0; i < Count(); i++)
		{
			Vector3 point2 = tickHistoryMatrix.MultiplyPoint3x4(points[i]);
			Vector3 point3 = (i == Count() - 1) ? position : tickHistoryMatrix.MultiplyPoint3x4(points[i + 1]);
			_Line line(point2, point3);
			Vector3 position2 = line.ClosestPoint(point);
			OBB obb(position2, rotation, bounds);
			num = min(num, obb.Distance(point));
		}
		return num;
	}

	void AddPoint(Vector3 point, int limit = -1)
	{
		while (limit > 0 && Count() >= limit)
		{
			pop_front(this->points);
			//this->points.pop_back();
			//this->points.insert(this->points.begin(), 1, point);
		}
		this->points.push_back(point);
	}

	void TransformEntries(VMatrix matrix)
	{
		for (int i = 0; i < Count(); i++)
		{
			Vector3 vector = this->points[i];
			vector = matrix.MultiplyPoint3x4(vector);
			this->points[i] = vector;
		}
	}
};

class HitTest2 {
public:
	uintptr_t game_object() {

		return *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x70);
	}

	GameObject* get_gameobject()
	{
		auto gameObject = game_object();
		if (!gameObject)
			return nullptr;

		return *reinterpret_cast<GameObject**>(gameObject + 0x10);
	}

	void set_hit_transform(Transform* hit_transform) {

		*reinterpret_cast<Transform**>((uintptr_t)this + 0xB0) = hit_transform;
	}

	void set_hit_material(uintptr_t material) {

		*reinterpret_cast<uintptr_t*>((uintptr_t)this + 0xC0) = material;
	}

	void set_hit_type(HitTestType hit_type) {

		*reinterpret_cast<int*>((uintptr_t)this + 0x10) = (int)hit_type;
	}

	void set_ignore_entity(BasePlayer* entity_to_ignore) {

		*reinterpret_cast<BasePlayer**>((uintptr_t)this + 0x80) = entity_to_ignore;
	}

	void set_radius(float radius) {

		*reinterpret_cast<float*>((uintptr_t)this + 0x2C) = radius;
	}

	void set_did_hit(bool did_hit) {

		*reinterpret_cast<bool*>((uintptr_t)this + 0x66) = did_hit;
	}

	void set_attack_ray(Ray ray) {

		*reinterpret_cast<Ray*>((uintptr_t)this + 0x14) = ray;
	}

	void set_best_hit(bool best_hit) {

		*reinterpret_cast<bool*>((uintptr_t)this + 0x65) = best_hit;
	}

	void set_max_distance(float max_dist) {

		*reinterpret_cast<float*>((uintptr_t)this + 0x34) = max_dist;
	}

	void set_hit_entity(BasePlayer* entity) {

		*reinterpret_cast<BasePlayer**>((uintptr_t)this + 0x88) = entity;
	}

	BasePlayer*& get_hit_entity() {

		return *reinterpret_cast<BasePlayer**>((uintptr_t)this + 0x88);
	}

	void set_hit_point(Vector3 hit_point) {

		*reinterpret_cast<Vector3*>((uintptr_t)this + 0x90) = hit_point;
	}

	void set_hit_normal(Vector3 hit_nromal) {

		*reinterpret_cast<Vector3*>((uintptr_t)this + 0x9C) = hit_nromal;
	}
	void set_damage_properties(uintptr_t damage_properties) {

		*reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x68) = damage_properties;
	}
};


class NeedsKeyboard {
public:
	static bool AnyActive(int k = 0, int b = 0) {
		pent
			return anyactive(k, b);
	}
};

class MapInterface {
public:
	static void SetOpen(bool b) {
		pent
			return mapinterfacesetopen(b);
	}
};

class UIInventory {
public:
	static void Close() {
		pent
			return uiinvclose();
	}
};

class ConButton {
public:
	member(bool, IsDown, 0x10);
};

class Buttons {
public:
	static ConButton* Chat() {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		return mem::read<ConButton*>(fieldz + 0x50 + 0x10);
	}
	static bool SetForward(bool b) {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		auto p = mem::read<uintptr_t>(fieldz + 0x8);
		typedef void(*A)(uintptr_t, bool);
		((A)(mem::game_assembly_base + 0x5BC4A0))(p, b);
		return 1;
	}
	static bool SetRight(bool b) {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		auto p = mem::read<uintptr_t>(fieldz + 0x20);
		typedef void(*A)(uintptr_t, bool);
		((A)(mem::game_assembly_base + 0x5BC4A0))(p, b);
		return 1;
	}
	static bool SetLeft(bool b) {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		auto p = mem::read<uintptr_t>(fieldz + 0x18);
		typedef void(*A)(uintptr_t, bool);
		((A)(mem::game_assembly_base + 0x5BC4A0))(p, b);
		return 1;
	}
	static bool SetBackwards(bool b) {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		auto p = mem::read<uintptr_t>(fieldz + 0x10);
		typedef void(*A)(uintptr_t, bool);
		((A)(mem::game_assembly_base + 0x5BC4A0))(p, b);
		return 1;
	}
	static ConButton* Gestures() {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		return mem::read<ConButton*>(fieldz + 0x188 + 0x10);
	}
	static ConButton* Map() {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		return mem::read<ConButton*>(fieldz + 0xF0 + 0x10);
	}
	static ConButton* Hoverloot() {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		return mem::read<ConButton*>(fieldz + 0x78 + 0x10);
	}
	static ConButton* Inventory() {
		pent
			auto kl = mem::read<uintptr_t>(mem::game_assembly_base + oButtons_TypeInfo);
		auto fieldz = *reinterpret_cast<uintptr_t*>(kl + 0xB8);
		return mem::read<ConButton*>(fieldz + 0x48 + 0x10);
	}
};

class GamePhysics {
public:
	static bool Trace(Ray r, float f, RaycastHit* r2, float f2, int layer, int querytriggerinteraction) {
		return gamephystrace(r, f, r2, f2, layer, querytriggerinteraction);
	}
};

class Terrain {
public:
	float SampleHeight(Vector3 worldPos) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0.f;
		return sampleheight(this, worldPos);
	}
};

class TerrainHeightMap {
public:
	float GetHeight(Vector3 worldPos) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return 0.f;
		return thmgetheight(this, worldPos);
	}
};

class TerrainCollision {
public:
	bool GetIgnore(Vector3 worldPos, float radius = 0.01f) {
		pent
			if (!this || (uintptr_t)this < 0xFFFFFFFF || (uintptr_t)this > 0xF000000000000000) return false;
		return getignore(this, worldPos, radius);
	}
};

class TerrainMeta {

};

class Hammer : public BaseMelee {

};

class SphereEntity : public BaseEntity {
public:
	member(float, currentRadius, 0x168);
	member(float, lerpRadius, 0x16C);
	member(float, lerpSpeed, 0x170);
};

class Skin {
public:
	member(System::string*, manifestName, 0x10);
	member(System::string*, manifestContent, 0x18);
	member(System::string*, iconName, 0x30);
	member(Skinnable*, skinnable, 0x48);
	member(System::Array<Material*>*, materials, 0x50);
};

class RenderSettings {
public:
	static void set_ambientIntensity(float f) {
		set_ambientintensity(f);
	}
	static void set_ambientLight(uintptr_t todsky, col c) {
		set_ambientlight(todsky, c);
	}
};

void attack_melee(aim_target target, BaseProjectile* melee, BasePlayer* lp, bool is_player = false) {
	pent
		if (!target.visible)
			return;

	if (target.teammate)
		return;

	Vector3 local_position = unity::get_camera_pos();

	if (melee->nextAttackTime() >= get_time())
		return;

	if (melee->timeSinceDeploy() < melee->deployDelay())
		return;

	auto hit_test_class = il2cpp::init_class(_("HitTest"));
	if (!hit_test_class)
		return;

	if (!is_player) {
		pent
			HitTest2* hit_test = (HitTest2*)il2cpp::methods::object_new(hit_test_class);

		Ray ray = Ray(lp->eyes()->position(), (target.pos - lp->eyes()->position()).Normalized());

		Transform* trans = target.ent->transform();


		auto v = trans->InverseTransformPoint(lp->ClosestPoint(target.pos));//_InverseTransformPoint(trans, target.pos);

		if (!trans)
			return;

		hit_test->set_max_distance(1000);
		hit_test->set_hit_transform(trans);
		hit_test->set_attack_ray(ray);
		hit_test->set_did_hit(true);
		hit_test->set_hit_entity((BasePlayer*)target.ent);
		hit_test->set_hit_point(v);
		//hit_test->set_hit_normal(Vector3(0, 0, 0));
		hit_test->set_hit_normal(trans->InverseTransformDirection(target.pos));
		hit_test->set_damage_properties(melee->get_damage_properties());

		//hit_test->MaxDistance() = 1000.f;
		//hit_test->HitTransform() = trans;
		//hit_test->AttackRay() = ray;
		//hit_test->DidHit() = true;
		//hit_test->HitEntity() = target.ent;
		//hit_test->HitPoint() = _InverseTransformPoint(trans, target.pos);
		//hit_test->HitNormal() = Vector3(0, 0, 0);
		//hit_test->damageProperties() = (DamageProperties*)*reinterpret_cast<uintptr_t*>((uintptr_t)melee + damageProperties); //basemelee + damageproperties

		if (!strcmp(melee->get_class_name(), _("Jackhammer"))) {
			StartAttackCooldown(melee, 0.05f);
		}
		else StartAttackCooldown(melee, melee->repeatDelay());

		ProcessAttack((BaseMelee*)melee, (uintptr_t)hit_test);
	}
	else {
		HitTest* hit_test = (HitTest*)il2cpp::methods::object_new(hit_test_class);

		Ray ray = Ray(local_position, (target.pos - local_position).Normalized());

		auto trans = is_player ? target.ent->model()->boneTransforms()->get(48) : target.ent->transform();

		if (!trans)
			return;

		hit_test->set_max_distance(1000);
		hit_test->set_hit_Transform(trans);
		hit_test->set_attack_ray(ray);
		hit_test->set_did_hit(true);
		hit_test->set_hit_entity((BasePlayer*)target.ent);
		hit_test->set_hit_point(trans->InverseTransformPoint(target.pos));
		hit_test->set_hit_normal(Vector3(0, 0, 0));
		hit_test->set_damage_properties(melee->get_damage_properties());

		StartAttackCooldown((BaseMelee*)melee, melee->repeatDelay());

		ProcessAttack((BaseMelee*)melee, (uintptr_t)hit_test);
	}
	return;
}

Vector3 WorldToScreen(Vector3 position)
{
	auto matrix = unity::get_view_matrix();

	if (!matrix.m) {
		pent
			return { 0,0,0 };
	}
	Vector3 out;
	const auto temp = matrix.transpose();

	auto translation_vector = Vector3{ temp[3][0], temp[3][1], temp[3][2] };
	auto up = Vector3{ temp[1][0], temp[1][1], temp[1][2] };
	auto right = Vector3{ temp[0][0], temp[0][1], temp[0][2] };

	float w = translation_vector.dot(position) + temp[3][3];

	if (w < 0.098f) {
		pent
			return Vector3(0, 0, 0);
	}

	float x = up.dot(position) + temp._24;
	float y = right.dot(position) + temp._14;

	out.x = (static_cast<float>(unity::get_width()) / 2) * (1 + y / w);
	out.y = (static_cast<float>(unity::get_height()) / 2) * (1 - x / w);
	out.z = w;

	return out;
}

float GetFuseLength(ThrownWeapon* w) {
	pent
		if (std::string(w->get_object_name().zpad).find(_("F1")) != std::string::npos) {
			pent
				return 3.0f;
		}
}

Vector3 GrenadeTracer(float speed,
	float angle,
	float drag,
	float gravityMod,
	float maxtime = 8.f,
	bool draw = false, Vector3 addto = Vector3::Zero(), BasePlayer* pl = nullptr)
{
	float pitchRad = DEG2RAD(angle);

	Vector3 dir = pl->input()->state()->get_aim_angles();

	Vector3 position = Vector3();
	Vector3 lastposition = position;
	Vector3 velCheck = dir * speed;

	const float stepSize = 0.03125f;

	for (float travelTime = 0.f; travelTime < maxtime; travelTime += stepSize)
	{
		position += velCheck * stepSize;
		if (!unity::is_visible(position, lastposition, 0) && draw)
			return lastposition;
		if (draw)
			Line(addto + position, addto + lastposition, { 1, 1, 1, 1 }, 0.02f, false, false);
		velCheck.y -= 9.81f * gravityMod * stepSize;
		velCheck -= velCheck * drag * stepSize;
		lastposition = position;
	}

	return position;
}

class GrenadePath {
public:
	BasePlayer* ply;
	ThrownWeapon* weapon;
	std::vector<Vector3> positions;
	Vector3 endposition;

	GrenadePath() : ply(nullptr), weapon(nullptr), positions({}), endposition({}) { pent }

	GrenadePath(BasePlayer* p, ThrownWeapon* t) : ply(p), weapon(t) {
		pent
			auto eyepos = p->eyes()->position();
		auto dir = p->eyes()->body_forward().normalize();
		auto d = 1.f;
		auto velocity = t->GetInheritedVelocity(p, dir) + dir * t->maxThrowVelocity() * d + p->GetWorldVelocity() * .5f;
		Vector3 pos = eyepos;
		Vector3 lp = pos;
		Vector3 vel = velocity;
		for (float travelTime = 0.f; travelTime < 8.f; travelTime += 0.03125f)
		{
			pos += vel * 0.03125f;
			if (!unity::is_visible(pos, lp, 0))
				break;
			vel.y -= 9.81f * 1.f * 0.03125f;
			vel -= vel * 0.1f * 0.03125f;
			lp = pos;
			positions.push_back(pos);
		}
		endposition = pos;
		return;
	}
};

std::map<int32_t, GrenadePath*> grenade_map = {};

namespace cache {
	std::array<int, 20> boneids = {
		1, 2, 3, 5, 6, 14, 15, 17, 18, 21, 23, 24, 25, 26, 27, 48, 55, 56, 57, 76
	};

	bounds_t get_bounds(BasePlayer* player, float expand = 0) {
		pent
			bounds_t ret = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX };
		for (auto id : boneids) {
			pent
				auto arr = player->model()->boneTransforms();
			if (arr)
			{
				auto Transform = player->model()->boneTransforms()->get(id);
				if (Transform) {
					pent
						auto pos = Transform->position();
					if (id == 48)
						pos.y += .2f;
					auto v3 = WorldToScreen(pos);
					Vector2 screen = { v3.x, v3.y };
					if (!screen.empty()) {
						pent
							if (screen.x < ret.left)
								ret.left = screen.x;
						if (screen.x > ret.right)
							ret.right = screen.x;
						if (screen.y < ret.top)
							ret.top = screen.y;
						if (screen.y > ret.bottom)
							ret.bottom = screen.y;
					}
				}
			}
		}

		if (ret.left == FLT_MAX
			|| ret.right == FLT_MIN
			|| ret.top == FLT_MAX
			|| ret.bottom == FLT_MIN)
			return { 0, 0, 0, 0, 0 };
		ret.left -= expand;
		ret.right += expand;
		ret.top -= expand;
		ret.bottom += expand;

		ret.center = ret.left + ((ret.right - ret.left) / 2);

		return ret;
	}
	void CacheBones(BasePlayer* player, BasePlayer* lp) {
		pent
			if (!player || !lp || !player->is_alive()) return;
		auto model = player->model();
		auto pid = player->userID();
		if (model) {
			pent
				auto bones = new BoneCache();

			bones->head = model->resolve(_(L"head"), lp);
			if (!bones->head)
				return;
			bones->neck = model->resolve(_(L"neck"), lp);
			bones->spine4 = model->resolve(_(L"spine4"), lp);
			bones->spine1 = model->resolve(_(L"spine1"), lp);
			bones->l_upperarm = model->resolve(_(L"l_upperarm"), lp);
			bones->l_forearm = model->resolve(_(L"l_forearm"), lp);
			bones->l_hand = model->resolve(_(L"l_hand"), lp);
			bones->r_upperarm = model->resolve(_(L"r_upperarm"), lp);
			bones->r_forearm = model->resolve(_(L"r_forearm"), lp);
			bones->r_hand = model->resolve(_(L"r_hand"), lp);
			bones->pelvis = model->resolve(_(L"pelvis"), lp);
			bones->l_hip = model->resolve(_(L"l_hip"), lp);
			bones->l_knee = model->resolve(_(L"l_knee"), lp);
			bones->l_foot = model->resolve(_(L"l_foot"), lp);
			bones->r_hip = model->resolve(_(L"r_hip"), lp);
			bones->r_knee = model->resolve(_(L"r_knee"), lp);
			bones->r_foot = model->resolve(_(L"r_foot"), lp);
			bones->r_toe = model->resolve(_(L"r_toe"), lp);
			bones->l_toe = model->resolve(_(L"l_toe"), lp);
			bones->penis = model->resolve(_(L"penis"), lp);

			if (pid != lp->userID()) {
				pent
					bounds_t bo = get_bounds(player, 4.f);
				if (bo.bottom != FLT_MIN)
					bones->bounds = bo;

				auto v3 = WorldToScreen(bones->head->position);
				Vector2 footpos = { v3.x, v3.y };
				if (!footpos.empty())
					bones->dfc = footpos;

				v3 = WorldToScreen(bones->head->position + (player->eyes()->body_forward() * 2));
				Vector2 fwd = { v3.x, v3.y };
				if (!fwd.empty())
					bones->forward = fwd;

				//dont need nanohacks 'target' as it doesn't make sense and is only local? lol
				//lp->console_echo(string::wformat(_(L"[matrix]: BoneCache - Finished caching for %d"), lp->userID()));
				bones->eye_rot = player->eyes()->rotation();
			}

			if (!map_contains_key(cachedBones, pid))
				cachedBones.insert(std::make_pair(pid, bones));
			else
				cachedBones[pid] = bones;
		}
	}
}

Vector3 GetEndPointForTrajectory(float speed, float angle, float drag, float gravityMod, bool draw = false, Vector3 addto = Vector3::Zero())
{
	float pitchRad = DEG2RAD(angle);

	Vector3 dir = {
		(float)(sinf(90.f) * cosf(pitchRad)),
		(float)sinf(pitchRad),
		(float)(cosf(90.f) * cosf(pitchRad))
	};

	Vector3 position = Vector3::Zero();
	Vector3 lastposition = position;
	Vector3 velCheck = dir * speed;

	const float stepSize = 0.03125f;

	for (float travelTime = 0.f; travelTime < 8.f; travelTime += stepSize)
	{
		position += velCheck * stepSize;
		//if (!unity::is_visible(position, lastposition, 0) && draw)
		//	return lastposition;
		//if (draw)
		//	Line(addto + position, addto + lastposition, { 1, 1, 1, 1 }, 10.f, false, false);
		velCheck.y -= 9.81f * gravityMod * stepSize;
		velCheck -= velCheck * drag * stepSize;
		lastposition = position;
	}

	return position;
}

Vector2 CalcAngle(const Vector3& src, const Vector3& dst) {
	Vector3 d = src - dst;
	return Vector2(RAD2DEG(Vector3::my_asin(d.y / d.length())), RAD2DEG(-Vector3::my_atan2(d.x, -d.z)));
};

void GenerateBuilletDropPredictionData(float drag, float gravityMod)
{
	int currentIndex = 0;
	Vector3 l1;
	Vector3 l2;
	for (float angle = 35.f; angle <= 85.f; angle += 1.f)
	{
		BulletDropPredictionData& predData = bulletDropData[currentIndex++];

		Vector3 a1 = GetEndPointForTrajectory(30.f, angle, drag, gravityMod, true, vars->local_player->GetBonePos(48));
		Vector3 a2 = GetEndPointForTrajectory(50.f, angle, drag, gravityMod, true, vars->local_player->GetBonePos(48));

		predData.distCoeff = a2.length_2d() / 50.f;
		predData.startY = a1.y;
		predData.yCoeff = (a2.y - a1.y) / 20.f;
	}
}