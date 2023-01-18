#pragma once
#include "../rust/unity.hpp"
#include "../rust/features/player_esp.hpp"
#include "../settings.hpp"
#include "../utils/string_format.h"

#include "../esp.hpp"	

#define rgba(r,g,b,a) gui::Color(r / 255.f, g / 255.f, b / 255.f, a)

//static float r = 1.00f, g = 0.00f, b = 1.00f;

static float lastchamsupdate = 0.f;
static float lastvisupdate = 0.f;

namespace gui {
	class Color {
	public:
		float r;
		float g;
		float b;
		float a;
		Color(float rr, float gg, float bb, float aa) {
			r = rr;
			g = gg;
			b = bb;
			a = aa;
		}
	};

	namespace methods {
		//static auto get_whiteTexture = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Texture2D"), _("get_whiteTexture"), 0, _(""), _("UnityEngine"))));
		//
		//static auto get_skin = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("get_skin"), 0, _(""), _("UnityEngine"))));
		//
		//static auto set_fontSize = reinterpret_cast<void(*)(uintptr_t label, int size)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIStyle"), _("set_fontSize"), -1, _(""), _("UnityEngine"))));
		//
		//static auto set_color = reinterpret_cast<void(*)(gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("set_color"), -1, _(""), _("UnityEngine"))));
		//
		//static auto Label = reinterpret_cast<void(*)(Rect position, uintptr_t content, uintptr_t style)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 3, _("content"), _("UnityEngine"), 2)));
		//
		//static auto _Label = reinterpret_cast<void(*)(Rect, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 2, _("text"), _("UnityEngine"), 2)));
		//static auto ptr_Label = reinterpret_cast<void(*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 2, _("text"), _("UnityEngine"), 2)));
		//
		//static auto set_alignment = reinterpret_cast<void(*)(uintptr_t label, uintptr_t value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIStyle"), _("set_alignment"), -1, _(""), _("UnityEngine"))));
		//
		//static auto get_current = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Event"), _("get_current"), 0, _(""), _("UnityEngine"))));
		//
		//static auto gui_content_temp = reinterpret_cast<uintptr_t(*)(System::string*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIContent"), _("Temp"), 1, _("t"), _("UnityEngine"), 1)));
		//
		//static auto gui_content_temp_ptr = reinterpret_cast<uintptr_t(*)(uintptr_t*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIContent"), _("Temp"), 1, _("t"), _("UnityEngine"), 1)));
		//
		//static auto set_skin = reinterpret_cast<void(*)(uintptr_t skin)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("set_skin"), 0, _(""), _("UnityEngine"))));
		//
		//static auto get_type = reinterpret_cast<EventType(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Event"), _("get_type"), 0, _(""), _("UnityEngine"))));
		//
		//static auto DrawTexture = reinterpret_cast<void (*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("DrawTexture"), 2, _("image"), _("UnityEngine"), 2)));
		//
		////Rect position, Texture image, ScaleMode scaleMode, bool alphaBlend, float imageAspect, Color color, float borderWidth, float borderRadius
		//static auto Box = reinterpret_cast<void (*)(Rect, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Box"), 2, _("text"), _("UnityEngine"), 2)));
		//
		//static auto DrawLine = reinterpret_cast<void (*)(Vector3, Vector3, gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Debug"), _("DrawLine"), 3, _("color"), _("UnityEngine"), 3)));
		//
		//static auto set_gizmos_color = reinterpret_cast<void(*)(gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Gizmos"), _("set_color"), -1, _(""), _("UnityEngine"))));
		//
		//static auto HorizontalSlider = reinterpret_cast<float(*)(Rect position, float value, float leftValue, float rightValue)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("HorizontalSlider"), 4, _(""), _("UnityEngine"))));
		//
		//
		////gl stuff?
		//
		//static auto Vertex = reinterpret_cast<void(*)(Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Vertex"), 0, _(""), _("UnityEngine"))));
		//
		//static auto Begin = reinterpret_cast<void(*)(int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Begin"), 0, _(""), _("UnityEngine"))));
		//
		//static auto End = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("End"), 0, _(""), _("UnityEngine"))));
		//
		//static auto Color = reinterpret_cast<void(*)(col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Color"), 0, _(""), _("UnityEngine"))));
		//
		//static auto PushMatrix = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("PushMatrix"), 0, _(""), _("UnityEngine"))));
		//
		//static auto PopMatrix = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("PopMatrix"), 0, _(""), _("UnityEngine"))));
		
		static auto dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oDontDestroyOnLoad));
		
		static auto create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oInternal_CreateGameObject));
		
		static auto add_component = reinterpret_cast<void(*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oInternal_AddComponentWithType));

		//static auto name = reinterpret_cast<void(*)( )>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_(""), _(""), 0, _(""), _(""))));
	}

	uintptr_t white_texture;

	void init_gui() {
		//methods::ptr_Label = reinterpret_cast<void(*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 2, _("text"), _("UnityEngine"), 2)));
		//
		//methods::gui_content_temp_ptr = reinterpret_cast<uintptr_t(*)(uintptr_t*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIContent"), _("Temp"), 1, _("t"), _("UnityEngine"), 1)));
		//
		//methods::HorizontalSlider = reinterpret_cast<float(*)(Rect position, float value, float leftValue, float rightValue)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("HorizontalSlider"), 4, _(""), _("UnityEngine"))));
		//
		//methods::get_whiteTexture = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Texture2D"), _("get_whiteTexture"), 0, _(""), _("UnityEngine"))));
		//
		//methods::get_skin = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("get_skin"), 0, _(""), _("UnityEngine"))));
		//
		//methods::set_fontSize = reinterpret_cast<void(*)(uintptr_t label, int size)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIStyle"), _("set_fontSize"), -1, _(""), _("UnityEngine"))));
		//
		//methods::set_color = reinterpret_cast<void(*)(gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("set_color"), -1, _(""), _("UnityEngine"))));
		//
		//methods::Label = reinterpret_cast<void(*)(Rect position, uintptr_t content, uintptr_t style)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 3, _("content"), _("UnityEngine"), 2)));
		//
		//methods::_Label = reinterpret_cast<void(*)(Rect, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Label"), 2, _("text"), _("UnityEngine"), 2)));
		//
		//methods::set_alignment = reinterpret_cast<void(*)(uintptr_t label, uintptr_t value)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIStyle"), _("set_alignment"), -1, _(""), _("UnityEngine"))));
		//
		//methods::get_current = reinterpret_cast<uintptr_t(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Event"), _("get_current"), 0, _(""), _("UnityEngine"))));
		//
		//methods::gui_content_temp = reinterpret_cast<uintptr_t(*)(System::string*)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUIContent"), _("Temp"), 1, _("t"), _("UnityEngine"), 1)));
		//
		//methods::set_skin = reinterpret_cast<void(*)(uintptr_t skin)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("set_skin"), 0, _(""), _("UnityEngine"))));
		//
		//methods::get_type = reinterpret_cast<EventType(*)(uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Event"), _("get_type"), 0, _(""), _("UnityEngine"))));
		//
		////unity::LoadAllAssets = reinterpret_cast<uintptr_t(*)(uintptr_t bundle, uintptr_t type)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("AssetBundle"), _("LoadAllAssets"), 1, _("type"), _("UnityEngine"), 1)));
		//
		//methods::DrawTexture = reinterpret_cast<void (*)(Rect, uintptr_t)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("DrawTexture"), 2, _("image"), _("UnityEngine"), 2)));
		//
		//methods::Box = reinterpret_cast<void (*)(Rect, System::string)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GUI"), _("Box"), 2, _("text"), _("UnityEngine"), 2)));
		//
		//methods::DrawLine = reinterpret_cast<void (*)(Vector3, Vector3, gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Debug"), _("DrawLine"), 3, _("color"), _("UnityEngine"), 3)));
		//
		//methods::set_gizmos_color = reinterpret_cast<void(*)(gui::Color)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("Gizmos"), _("set_color"), -1, _(""), _("UnityEngine"))));
		//
		//methods::Vertex = reinterpret_cast<void(*)(Vector3)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Vertex"), 0, _(""), _("UnityEngine"))));
		//
		//methods::Begin = reinterpret_cast<void(*)(int)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Begin"), 0, _(""), _("UnityEngine"))));
		//
		//methods::End = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("End"), 0, _(""), _("UnityEngine"))));
		//
		//methods::Color = reinterpret_cast<void(*)(col)>(*reinterpret_cast<uintptr_t*>(il2cpp::method(_("GL"), _("Color"), 0, _(""), _("UnityEngine"))));
		methods::dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oDontDestroyOnLoad));
		methods::create = reinterpret_cast<void(*)(uintptr_t self, System::string shader)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oInternal_CreateGameObject));
		methods::add_component = reinterpret_cast<void(*)(uintptr_t self, uintptr_t componentType)>(*reinterpret_cast<uintptr_t*>(mem::game_assembly_base + oInternal_AddComponentWithType));

		//set font????????????

		//white_texture = methods::get_whiteTexture();
	}

	uintptr_t label, skin;
	bool open;
	bool InsertPressed;
	float opacity = 0.f;
	float tick_time_when_called;

	const wchar_t* get_keystr(int key) {
		switch (key)
		{
		case 0:
			return _(L"none");
		case 8:
			return _(L"backspace");
		case 127:
			return _(L"delete");
		case 9:
			return _(L"tab");
		case 12:
			return _(L"clear");
		case 13:
			return _(L"return");
		case 19:
			return _(L"pause");
		case 27:
			return _(L"none");
		case 32:
			return _(L"space");
		case 256:
			return _(L"0");
		case 257:
			return _(L"1");
		case 258:
			return _(L"2");
		case 259:
			return _(L"3");
		case 260:
			return _(L"4");
		case 261:
			return _(L"5");
		case 262:
			return _(L"6");
		case 263:
			return _(L"7");
		case 264:
			return _(L"8");
		case 265:
			return _(L"9");
		case 266:
			return _(L".");
		case 267:
			return _(L"/");
		case 268:
			return _(L"*");
		case 269:
			return _(L"+");
		case 270:
			return _(L"-");
		case 271:
			return _(L"enter");
		case 272:
			return _(L"equals");
		case 273:
			return _(L"up");
		case 274:
			return _(L"down");
		case 275:
			return _(L"right");
		case 276:
			return _(L"left");
		case 277:
			return _(L"ins");
		case 278:
			return _(L"home");
		case 279:
			return _(L"end");
		case 280:
			return _(L"pageup");
		case 281:
			return _(L"pagedown");
		case 282:
			return _(L"f1");
		case 283:
			return _(L"f2");
		case 284:
			return _(L"f3");
		case 285:
			return _(L"f4");
		case 286:
			return _(L"f5");
		case 287:
			return _(L"f6");
		case 288:
			return _(L"f7");
		case 289:
			return _(L"f8");
		case 290:
			return _(L"f9");
		case 291:
			return _(L"f10");
		case 292:
			return _(L"f11");
		case 293:
			return _(L"f12");
		case 294:
			return _(L"f13");
		case 295:
			return _(L"f14");
		case 296:
			return _(L"f15");
		case 48:
			return _(L"0");
		case 49:
			return _(L"1");
		case 50:
			return _(L"2");
		case 51:
			return _(L"3");
		case 52:
			return _(L"4");
		case 53:
			return _(L"5");
		case 54:
			return _(L"6");
		case 55:
			return _(L"7");
		case 56:
			return _(L"8");
		case 57:
			return _(L"9");
		case 33:
			return _(L"!");
		case 34:
			return _(L"\"");
		case 35:
			return _(L"#");
		case 36:
			return _(L"$");
		case 37:
			return _(L"%");
		case 38:
			return _(L"&");
		case 39:
			return _(L"'");
		case 40:
			return _(L"(");
		case 41:
			return _(L")");
		case 42:
			return _(L"*");
		case 43:
			return _(L"+");
		case 44:
			return _(L",");
		case 45:
			return _(L"-");
		case 46:
			return _(L".");
		case 47:
			return _(L"/");
		case 58:
			return _(L":");
		case 59:
			return _(L";");
		case 60:
			return _(L"<");
		case 61:
			return _(L"=");
		case 62:
			return _(L">");
		case 63:
			return _(L"?");
		case 64:
			return _(L"@");
		case 91:
			return _(L"[");
		case 92:
			return _(L"\\");
		case 93:
			return _(L"]");
		case 94:
			return _(L"^");
		case 95:
			return _(L"_");
		case 96:
			return _(L"`");
		case 97:
			return _(L"a");
		case 98:
			return _(L"b");
		case 99:
			return _(L"c");
		case 100:
			return _(L"d");
		case 101:
			return _(L"e");
		case 102:
			return _(L"f");
		case 103:
			return _(L"g");
		case 104:
			return _(L"h");
		case 105:
			return _(L"i");
		case 106:
			return _(L"j");
		case 107:
			return _(L"k");
		case 108:
			return _(L"l");
		case 109:
			return _(L"m");
		case 110:
			return _(L"n");
		case 111:
			return _(L"o");
		case 112:
			return _(L"p");
		case 113:
			return _(L"q");
		case 114:
			return _(L"r");
		case 115:
			return _(L"s");
		case 116:
			return _(L"t");
		case 117:
			return _(L"u");
		case 118:
			return _(L"v");
		case 119:
			return _(L"w");
		case 120:
			return _(L"x");
		case 121:
			return _(L"y");
		case 122:
			return _(L"z");
		case 123:
			return _(L"{");
		case 124:
			return _(L"|");
		case 125:
			return _(L"}");
		case 126:
			return _(L"~");
		case 300:
			return _(L"numlk");
		case 301:
			return _(L"caps");
		case 302:
			return _(L"scrl");
		case 303:
			return _(L"rshift");
		case 304:
			return _(L"lshift");
		case 305:
			return _(L"rctrl");
		case 306:
			return _(L"lctrl");
		case 307:
			return _(L"ralt");
		case 308:
			return _(L"lalt");
		case 310:
			return _(L"lcmd");
		case 311:
			return _(L"lwin");
		case 309:
			return _(L"rcmd");
		case 312:
			return _(L"rwin");
		case 313:
			return _(L"altgr");
		case 315:
			return _(L"help");
		case 316:
			return _(L"pr");
		case 317:
			return _(L"sysreq");
		case 318:
			return _(L"break");
		case 319:
			return _(L"menu");
		case 323:
			return _(L"m0");
		case 324:
			return _(L"m1");
		case 325:
			return _(L"m2");
		case 326:
			return _(L"m3");
		case 327:
			return _(L"m4");
		case 328:
			return _(L"m5");
		case 329:
			return _(L"m");
		}
		return _(L"undefined");
	}

	void outline_box(Vector2 pos, Vector2 size, Color clr)
	{
		//methods::set_color(clr);
		//
		//methods::DrawTexture(Rect(pos.x, pos.y, 1, size.y), white_texture);
		//methods::DrawTexture(Rect(pos.x + size.x, pos.y, 1, size.y), white_texture);
		//methods::DrawTexture(Rect(pos.x, pos.y, size.x, 1), white_texture);
		//methods::DrawTexture(Rect(pos.x, pos.y + size.y, size.x, 1), white_texture);
	}

	void horizontal_line(Vector2 pos, float size, Color clr)
	{
		//methods::set_color(clr);
		//
		//methods::DrawTexture(Rect(pos.x, pos.y, size, 1), white_texture);
	}


	void line(Vector2 start, Vector2 end, Color clr, float thickness = 1.f, bool outline = false)
	{
		//col c = col(clr.r, clr.g, clr.b, clr.a);
		//gui::methods::PushMatrix();
		//gui::methods::Begin(1);
		//gui::methods::Color(c);
		//
		//if (outline)
		//{
		//	gui::methods::Color({ 0,0,0,255 });
		//	gui::methods::Vertex(Vector3(start.x - thickness, start.y, 0));
		//	gui::methods::Vertex(Vector3(end.x + thickness, end.y, 0));
		//}
		//gui::methods::Color(c);
		//gui::methods::Vertex(Vector3(start.x, start.y, 0));
		//gui::methods::Vertex(Vector3(end.x, end.y, 0));
		//
		//gui::methods::End();
		//gui::methods::PopMatrix();
		//return;
	}

	void circle(Vector2 o, float r, col clr, bool filled = false) {
		//gui::methods::Begin(1.5);
		//gui::methods::Color(clr);
		//for (float num = 0.f; num < 6.2831855f; num += 0.05f)
		//{
		//	gui::methods::Vertex(Vector3(cos(num) * r + o.x, sin(num) * r + o.y, 0));
		//	gui::methods::Vertex(Vector3(cos(num + 0.05f) * r + o.x, sin(num + 0.05f) * r + o.y, 0));
		//}
		//gui::methods::End();
	}

	void vertical_line(Vector2 pos, float size, Color clr)
	{
		//methods::set_color(clr);
		//
		//methods::DrawTexture(Rect(pos.x, pos.y, 1, size), white_texture);
	}

	void fill_box(Rect pos, Color clr)
	{
		//methods::set_color(clr);
		//methods::DrawTexture(pos, white_texture);
	}


	void init() {
		//if (label && skin)
		//	return;
		//
		//skin = methods::get_skin();
		//label = mem::read<uintptr_t>(skin + 0x38);
		//
		//if (std::filesystem::exists(_("RustClient_Data\\assets1.shared")))
		//	unity::bundle = unity::LoadFromFile(_(L"RustClient_Data\\assets1.shared "));
		//if (std::filesystem::exists(_("RustClient_Data\\assets2.shared")))
		//	unity::bundle2 = unity::LoadFromFile(_(L"RustClient_Data\\assets2.shared"));
		//if (std::filesystem::exists(_("RustClient_Data\\assets3.shared")))
		//	unity::galaxy_bundle = unity::LoadFromFile(_(L"RustClient_Data\\assets3.shared"));
		////unity::galaxy_bundle = unity::LoadFromFile(_(L"galaxy.chams"));
		////unity::bundle_font = unity::LoadFromFile(_(L"font.assets"));
		//
		//const auto set_font = [&](System::string font_name, int size) {
		//	static auto font = unity::LoadAsset(unity::bundle_font, font_name, il2cpp::type_object("UnityEngine", "Font"));
		//	*reinterpret_cast<std::uintptr_t*>(skin + 0x18) = font; // lol.
		//	methods::set_fontSize(label, size);
		//};
		//
		////static auto font = methods::LoadAsset(unity::bundle_font, System::string(_(L"minecraftchmc.ttf")), il2cpp::type_object(_("UnityEngine"), _("Font")));
		////*reinterpret_cast<uintptr_t*>(skin + 0x18) = font;
		//
		////methods::set_fontSize(label, 24);
		////set_font(_(L"tahoma.ttf"), 14);
		//
		//methods::set_alignment(label, 0);
		//methods::set_color(Color(1, 1, 1, 1));
	}

	void Label(Rect pos, System::string text, Color clr, bool centered = false, float size = 12) {
		//methods::set_color(clr);
		//
		//uintptr_t content = methods::gui_content_temp(&text);
		//
		//methods::set_fontSize(label, size);          //prepare style	
		//
		//if (centered)
		//	methods::set_alignment(label, 0x4);
		//else
		//	methods::set_alignment(label, 0x0);
		//
		//methods::Label(pos, content, label);
	}

	void Label(Rect pos, uintptr_t text, Color clr, bool centered = false, float size = 12) {
		//methods::set_color(clr);
		//
		//methods::set_fontSize(label, size);
		//
		//methods::ptr_Label(pos, text);
	}

	void Label(Rect pos, System::string text, float font_size = 0) {

		//if (!font_size)
		//	methods::_Label(pos, text);
		//else {
		//	uintptr_t content = methods::gui_content_temp(&text);
		//
		//	methods::set_fontSize(label, font_size);          //prepare style	
		//
		//	methods::Label(pos, content, label);
		//}
	}

	int active_tab = 0;
	Vector2 tab_size;

	Color perc_col(float Num, float alpha = 1.0f)
	{
		float red, green, blue;
		if (Num < 0.5)
		{
			red = 255.f;
			green = 0.f;
			blue = 255.f;
		}
		else
		{
			red = Num * 2.f * 255.f;
			green = 0.f;
			blue = (2.f - 2.f * Num) * 255.f;
		}

		red -= 100;
		green -= 100;
		blue -= 100;
		return Color(red / 255, green / 255, blue / 255, alpha);
	}

	int last_active_id = 0;

	void tab(EventType event, Vector2 pos, Vector2 mouse, const wchar_t* tab_name, int id) {
		if (event == EventType::MouseDown) {
			if (Rect(pos.x + 2, pos.y - 4 + id * tab_size.y + 2.0f, tab_size.x, tab_size.y).Contains(mouse)) {
				last_active_id = active_tab;
				active_tab = id;
				opacity = 0.0f;
			}
		}
		else if (event == EventType::Repaint) {
			if (active_tab == id) {
				float o = (opacity / 255.f);
				fill_box(Rect(pos.x, pos.y + id * tab_size.y, tab_size.x + 2, tab_size.y), rgba(21.f, 27.f, 37.f, o));
			}
			else if (active_tab == last_active_id) {
				float o = ((255.f - opacity) / 255.f);
				fill_box(Rect(pos.x, pos.y + id * tab_size.y, tab_size.x + 2, tab_size.y), rgba(21.f, 27.f, 37.f, o));
			}

			Label(Rect{ pos.x + 2, pos.y + 1 + id * tab_size.y + 1, tab_size.x, tab_size.y }, tab_name, rgba(0, 0, 0, 255.f), true, 12);
			Label(Rect{ pos.x + 2, pos.y + id * tab_size.y, tab_size.x, tab_size.y }, tab_name, rgba(159.f, 163.f, 169.f, 255.f), true, 12);
		}
	}

	const wchar_t* keybind_str;
	int* kbref = 0;
	int* ref_clicked = 0;
	bool getting_keybind = false;
	bool combo_clicked = false;
	bool true_placehold = true;
	bool false_placehold = false;
	std::wstring* pinputtext;
	bool _ti = false;
	bool inputting_text = false;

	bool button(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, wchar_t* name) {
		pos.x += 5;
		current_pos.y += 5;
		Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 40, 14);
		fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));
		if (poz.Contains(mouse))
		{
			Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 40, 14);
			fill_box(poz, rgba(39, 47, 59, 255.f));
		}
		if (event == EventType::MouseDown) {
			if (poz.Contains(mouse)) {
				Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 40, 14);
				fill_box(poz, rgba(249.f, 130.f, 109.f, 255.f));
				return true;
			}
		}

		gui::Label({ poz.x , poz.y, poz.wid, poz.hei }, System::string(name), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), true, 10);
		current_pos.y += 15;
		return false;
	}

	void textbox(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, wchar_t* name, std::wstring* str) {

		pos.x += 5;

		Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));
		if (poz.Contains(mouse))
		{
			Rect t = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + 18, 150, 2);
			fill_box(t, rgba(159.f, 163.f, 169.f, 255.f));
		}
		if (event == EventType::MouseDown) {
			if (poz.Contains(mouse))
				inputting_text = !inputting_text;
			else inputting_text = false;
		}

		
		gui::Label({ poz.x + 4, poz.y, poz.wid, poz.hei }, System::string(name), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 10);

		if(inputting_text)
		{
			Rect t = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + 18, 150, 2);
			fill_box(t, rgba(249.f, 130.f, 109.f, 255.f));
			pinputtext = str;
			gui::Label({ poz.x + 44, poz.y, poz.wid, poz.hei }, System::string(str->c_str()), rgba(249.f, 130.f, 109.f, (opacity / 255.f)), false, 10);
		}
		else
			gui::Label({ poz.x + 44, poz.y, poz.wid, poz.hei }, System::string(str->c_str()), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 10);
		//draw label of current_text
		current_pos.y += 20;
	}

	void combobox(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, const wchar_t* name, std::array<wchar_t*, 8> combo_str, std::array<bool*, 8> combo) {
		pos.x += 10;
		Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));

		if (poz.Contains(mouse))
		{
			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + 18, 150, 2);
			fill_box(poz, rgba(181, 140, 132, (opacity / 255.f)));
			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		}

		gui::Label({ poz.x + 5, poz.y, poz.wid, poz.hei }, System::string(name), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 13);

		if (combo_clicked)
		{
			gui::line({ poz.x + 130, poz.y + 13 }, { poz.x + 135, poz.y + 8 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
			gui::line({ poz.x + 140, poz.y + 13 }, { poz.x + 135, poz.y + 8 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));

			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + 18, 150, 2);
			fill_box(poz, rgba(249.f, 130.f, 109.f, (opacity / 255.f)));
			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		}
		else {
			gui::line({ poz.x + 130, poz.y + 8 }, { poz.x + 135, poz.y + 13 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
			gui::line({ poz.x + 140, poz.y + 8 }, { poz.x + 135, poz.y + 13 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
		}

		if (event == EventType::MouseDown) 
		{
			if (poz.Contains(mouse))
				combo_clicked = !combo_clicked;
			else
			{
				bool clicked_inside = false;
				if (combo_clicked)
				{
					for (size_t i = 1; i < 8; i++)
					{
						poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + (i * 20), 150, 15);
						if (poz.Contains(mouse))
						{
							if (&combo[i])
							{
								*combo[i] = !*combo[i];
								clicked_inside = true;
							}
						}
					}
				}
				if(clicked_inside == false)
					combo_clicked = false;
			}
		}
		if (combo_clicked)
		{
			for (size_t i = 1; i < 8; i++)
			{
				poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + (i * 20), 150, 20);
				fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));

				auto name = combo_str[i];

				gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), true, 10);

				bool s = *combo[i];
				if (s)
					gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(249.f, 130.f, 109.f, (opacity / 255.f)), true, 10);

				if (poz.Contains(mouse))
					gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(181, 140, 132, (opacity / 255.f)), true, 10);
			}
		}
		current_pos.y += 30;
	}

	int* list_open = 0;
	bool list_clicked = false;
	void listbox(EventType event,
		Vector2 pos,
		Vector2& current_pos,
		Vector2 mouse,
		const wchar_t* button_name,
		std::array<wchar_t*, 13> str_list,
		int* selected = 0) 
	{
		pos.x += 10;
		Rect poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));

		auto sz = sizeof(str_list) / sizeof(str_list[0]);

		gui::Label({ poz.x + 5, poz.y, poz.wid, poz.hei }, System::string(string::wformat(_(L"%s: %s"), button_name, str_list[*selected])), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 13);

		if (list_clicked && list_open == selected)
		{
			gui::line({ poz.x + 130, poz.y + 13 }, { poz.x + 135, poz.y + 8 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
			gui::line({ poz.x + 140, poz.y + 13 }, { poz.x + 135, poz.y + 8 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));

			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + 18, 150, 2);
			fill_box(poz, rgba(249.f, 130.f, 109.f, (opacity / 255.f)));
			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
		}
		else
		{
			gui::line({ poz.x + 130, poz.y + 8 }, { poz.x + 135, poz.y + 13 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
			gui::line({ poz.x + 140, poz.y + 8 }, { poz.x + 135, poz.y + 13 }, rgba(159.f, 163.f, 169.f, (opacity / 255.f)));
		}


		if (event == EventType::MouseDown)
		{
			poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y, 150, 20);
			if (poz.Contains(mouse)) {
				list_clicked = !list_clicked;
				if (list_open != selected)
					list_open = selected;
			}
			if (list_clicked && list_open == selected)
			{
				for (size_t i = 1; i < sz; i++)
				{
					if (str_list[i][0] == '\x00') continue;
					poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + (i * 20), 150, 15);
					if (poz.Contains(mouse))
						if (str_list[i])
							*selected = i;
				}
			}
		}

		if (list_clicked && list_open == selected) {
			for (size_t i = 1; i < sz; i++)
			{
				if (str_list[i][0] == '\x00') continue;
				poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + (i * 20), 150, 20);

				fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));

				auto name = str_list[i];

				gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), true, 10);

				if (*selected > 8)
					*selected = 0;
				bool s = LI_FIND(wcscmp)(str_list[*selected], str_list[i]) == 0;
				if (s)
					gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(249.f, 130.f, 109.f, (opacity / 255.f)), true, 10);

				if (poz.Contains(mouse))
				{
					poz = Rect(pos.x + tab_size.x + 2.0f + 2, pos.y + current_pos.y + (i * 20) + 2, 146, 16);
					fill_box(poz, rgba(22, 27, 36, 255.f));
					poz = Rect(pos.x + tab_size.x + 2.0f, pos.y + current_pos.y + (i * 20), 150, 20);
					gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(name), rgba(181, 140, 132, (opacity / 255.f)), true, 10);
				}
			}
		}

		current_pos.y += 30;
	}

	void checkbox(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, const wchar_t* button_name, bool* checked_ref, int id, bool keybind = false, int* keybind_ref = 0, Color col = Color(255, 255, 255, 220)) {

		pos.x += 5;
		const float button_size = 20;
		if (event == EventType::MouseDown && !combo_clicked && !list_clicked) {
			if (Rect(pos.x + tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4 + 3 + 2.0f, 100, button_size + 3).Contains(mouse)) {
				*checked_ref = !*checked_ref;
			}
		}

		if (keybind)
		{
			Rect poz = Rect(pos.x + tab_size.x + 100 + 2.0f, pos.y + current_pos.y, 31, 15);
			fill_box(poz, rgba(14.f, 18.f, 24.f, 255.f));
			if (event == EventType::MouseDown && !combo_clicked && !list_clicked) {
				if (poz.Contains(mouse)) {
					getting_keybind = !getting_keybind;
					if (getting_keybind)
					{
						ref_clicked = keybind_ref;
						kbref = keybind_ref;
					}
				}
			}
			if (getting_keybind && keybind_ref == ref_clicked)
			{
				fill_box(Rect(poz.x, poz.y + 14.f, poz.wid, 2.f), rgba(249.f, 130.f, 109.f, (opacity / 255.f)));
				gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(get_keystr(*keybind_ref)), rgba(249.f, 130.f, 109.f, (opacity / 255.f)), true, 10);
			}

			gui::Label({ poz.x + 1, poz.y, poz.wid, poz.hei }, System::string(get_keystr(*keybind_ref)), rgba(159.f, 163.f, 169.f, (opacity / 255.f)), true, 10);
		}

		float o1 = (opacity / 255.f);
		//outline_box(Vector2(pos.x + tab_size.x + 3 + 2.0f, pos.y + 7 + current_pos.y - 4), Vector2(10, 10), rgba(14.f, 18.f, 24.f, o1));
		//outline_box(Vector2(pos.x + 2 + tab_size.x + 3 + 2.0f, pos.y + 10 + current_pos.y - 5), Vector2(7, 7), rgba(249.f, 130.f, 109.f, o1));
		fill_box(Rect{ pos.x + 3 + tab_size.x + 3 + 2.0f, pos.y + 10 + current_pos.y - 4, 5, 5 }, rgba(14.f, 18.f, 24.f, (opacity / 255.f)));

		if (event == EventType::Repaint) {
			gui::Label(Rect{ pos.x + 15 + tab_size.x + 3 + 2.0f + 1, pos.y + current_pos.y - 4 + 1, 200, button_size + 3 }, button_name, rgba(0, 0, 0, (opacity / 255.f)), false, 14);
			if (*checked_ref) {
				//fill_box(Rect{ pos.x + tab_size.x + 3 + 2.0f, pos.y + 7 + current_pos.y - 4, 10, 10 }, rgba(14.f, 18.f, 24.f, (opacity / 255.f)));
				fill_box(Rect{ pos.x + 3 + tab_size.x + 3 + 2.0f, pos.y + 10 + current_pos.y - 4, 5, 5 }, rgba(249.f, 130.f, 109.f, (opacity / 255.f)));
				gui::Label(Rect{ pos.x + 15 + tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, rgba(249.f, 130.f, 109.f, (opacity / 255.f)), false, 14);
			}
			else
				gui::Label(Rect{ pos.x + 15 + tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 14);


			//Rect poz = Rect(pos.x + tab_size.x + 100 + 2.0f, pos.y + current_pos.y - 4 + 3 + 2.0f, 23, button_size + 3);
			//fill_box(poz, rgba(14.f, 18.f, 24.f, (opacity / 255.f)));
			//gui::Label(poz, button_name, rgba(159.f, 163.f, 169.f, (opacity / 255.f)), false, 14);

		}
		current_pos.y += button_size;
	}

	void OnKeyUp(KeyCode key) {
		if (key == KeyCode::Insert) {
			InsertPressed = false;
		}
	}

	void OnKeyDown(KeyCode key) {
		if (!InsertPressed && key == KeyCode::Insert) {
			InsertPressed = true;
			open = !open;
			if (!open) getting_keybind = false;
		}

		if (getting_keybind && (int)(key) != 0)
		{
			*kbref = (int)(key);
			getting_keybind = false;
			//keybind_map[keybind_str] = key;
		}
		if (inputting_text)
		{
			auto og = *pinputtext;

			auto keystr = get_keystr((int)key);
			if (((int)key >= 97 && (int)key <= 122) || (int)key == 8)
			{
				std::wstring new_text = og;

				if (!wcscmp(keystr, _(L"backspace"))
					|| (int)key == 8) //backspace
					new_text = new_text.substr(0, new_text.size() - 1);
				else if (pinputtext->size() < 16)
				{
					if (!wcscmp(keystr, _(L"space"))) //spacebar
						new_text.append(_(L" "));
					else if (wcscmp(keystr, _(L"none"))) //none
						new_text.append(keystr);
				}
				*pinputtext = new_text;
			}
		}
	}

	void buttonvis(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, const wchar_t* button_name, bool* out, int id, Color col = Color(vars->visual.VisRcolor, vars->visual.VisGcolor, vars->visual.VisBcolor, 1)) {
		const float button_size = 20;
		if (event == EventType::MouseDown) {
			if (Rect(pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 100, button_size + 3).Contains(mouse)) {
				*out = !*out;
			}
		}


		if (event == EventType::Repaint) {
			gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f + 1, pos.y + current_pos.y - 4 + 1, 200, button_size + 3 }, button_name, gui::Color(0, 0, 0, 120), false, 10);
			if (*out) {
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.VisRcolor, vars->visual.VisGcolor, vars->visual.VisBcolor, 1), false, 10);
			}
			else
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.VisRcolor, vars->visual.VisGcolor, vars->visual.VisBcolor, 1), false, 10);
		}
		current_pos.y += button_size;
	}

	void buttoninv(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, const wchar_t* button_name, bool* out, int id, Color col = Color(vars->visual.InvRcolor, vars->visual.InvGcolor, vars->visual.InvBcolor, 1)) {
		const float button_size = 20;
		if (event == EventType::MouseDown) {
			if (Rect(pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 100, button_size + 3).Contains(mouse)) {
				*out = !*out;
			}
		}


		if (event == EventType::Repaint) {
			gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f + 1, pos.y + current_pos.y - 4 + 1, 200, button_size + 3 }, button_name, gui::Color(0, 0, 0, 120), false, 10);
			if (*out) {
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.InvRcolor, vars->visual.InvGcolor, vars->visual.InvBcolor, 1), false, 10);
			}
			else
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.InvRcolor, vars->visual.InvGcolor, vars->visual.InvBcolor, 1), false, 10);
		}
		current_pos.y += button_size;
	}

	void buttonteam(EventType event, Vector2 pos, Vector2& current_pos, Vector2 mouse, const wchar_t* button_name, bool* out, int id, Color col = Color(vars->visual.TeamRcolor, vars->visual.TeamGcolor, vars->visual.TeamBcolor, 1)) {
		const float button_size = 20;
		if (event == EventType::MouseDown) {
			if (Rect(pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 100, button_size + 3).Contains(mouse)) {
				*out = !*out;
			}
		}


		if (event == EventType::Repaint) {
			gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f + 1, pos.y + current_pos.y - 4 + 1, 200, button_size + 3 }, button_name, gui::Color(0, 0, 0, 120), false, 10);
			if (*out) {
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.TeamRcolor, vars->visual.TeamGcolor, vars->visual.TeamBcolor, 1), false, 10);
			}
			else
				gui::Label(Rect{ pos.x + id * tab_size.x + 3 + 2.0f, pos.y + current_pos.y - 4, 200, button_size + 3 }, button_name, Color(vars->visual.TeamRcolor, vars->visual.TeamGcolor, vars->visual.TeamBcolor, 1), false, 10);
		}
		current_pos.y += button_size;
	}

	void Slider(EventType event, Vector2 pos, Vector2 mouse, uintptr_t text, Vector2& current_pos, float& out, float max, int id, float font_size = 0, const char* fmt = _("%.1f"))
	{
		pos.x += 100;
		pos.y += 9;
		const float button_size = 8;

		int wid = 150;

		int pzx = pos.x + id + 3 + 2.0f + 1;

		Rect pz = Rect(pzx, pos.y + current_pos.y - 4 + 1, wid, button_size);

		//if mouse within box highlight thumb
		//if mouse down within box, set thumb to position

		if (event == EventType::MouseDown) {
			if (pz.Contains(mouse)) {
				out = (((pos.x + 7) - mouse.x) * (max / wid)) * -1;
			}
		}

		float val = -((wid - 1) * (out / max));
		if (event == EventType::Repaint) {
			fill_box(pz, rgba(14.f, 18.f, 24.f, (opacity / 255.f)));
			if (val < 0) val = val * -1;
			Rect pzf = Rect(pz.x + 2, pz.y + 2, val, 4);
			fill_box(pzf, rgba(249.f, 130.f, 109.f, (opacity / 255.f)));
		}


		//out = gui::methods::HorizontalSlider(pz, out, value.x, value.y);

		//slider label
		gui::Label(Rect{ pos.x + id + 3 + 2.0f + 6, pos.y + current_pos.y - 3 - 10, 200, 16 }, text, gui::Color(0, 0, 0, (opacity / 255.f) > 120 ? 120 : opacity), false, 10);
		gui::Label(Rect{ pos.x + id + 3 + 2.0f + 5, pos.y + current_pos.y - 4 - 10, 200, 16 }, text, Color(255, 255, 255, (opacity / 255.f)), false, 10);

		auto nstr = il2cpp::methods::new_string(string::format(fmt, out));
		//slider value
		gui::Label(Rect{ pos.x + id + val, pos.y + current_pos.y - 3, 200, 16 }, nstr, gui::Color(0, 0, 0, (opacity / 255.f) > 120 ? 120 : opacity), false, 8);
		gui::Label(Rect{ pos.x + id + val, pos.y + current_pos.y - 4, 200, 16 }, nstr, Color(255, 255, 255, (opacity / 255.f)), false, 8);

		current_pos.y += button_size + 8;
	}

	void Progbar(Vector2 start, Vector2 end, float a, float b)
	{
		if (a < 0) return;
		fill_box(Rect(start.x, start.y, end.x, end.y + 1), rgba(21.f, 27.f, 37.f, 255.f));

		if ((a / b) > 1)
			a = b;

		fill_box(Rect(start.x + 1, start.y + 1, (end.x * (a / b)) - 1, end.y - 1), 
			gui::Color(vars->accent_color[0], vars->accent_color[1], vars->accent_color[2], 1.f));
	}

	float out;

	Vector2 window_position = { 650, 200 };
	Vector2 lmp = { 650, 200 };

	void InGameOnGui(uintptr_t rcx)
	{
		return;
	}

	int lastslot = -1;
	bool b_init = false;
	void OnGUI(uintptr_t rcx)
	{
		tab_size = Vector2(102, 35);
		if (!b_init) {
			init();
			b_init = true;
		}

		static int cases = 0;
		switch (cases) {
		case 0: { r -= 0.0015f; if (r <= 0) cases += 1; break; }
		case 1: { g += 0.0015f; b -= 0.0015f; if (g >= 1) cases += 1; break; }
		case 2: { r += 0.0015f; if (r >= 1) cases += 1; break; }
		case 3: { b += 0.0015f; g -= 0.0015f; if (b >= 1) cases = 0; break; }
		default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
		}
		const float ScreenWidth = vars->ScreenX;
		const float ScreenHeight = vars->ScreenY;
		const Vector2 screen_center = Vector2(ScreenWidth / 2, ScreenHeight / 2);

		if (vars->local_player)
		{
			if (vars->visual.flyhack_indicator) {
				if (settings::vert_flyhack >= 3.f) {
					gui::Progbar({ screen_center.x - 300, screen_center.y - 500 },
						{ 600, 5 },
						settings::vert_flyhack,
						settings::vert_flyhack);
				}
				else if (settings::vert_flyhack >= .1f) {
					gui::Progbar({ screen_center.x - 300, screen_center.y - 500 },
						{ 600, 5 },
						settings::vert_flyhack,
						3.f);
				}

				if (settings::hor_flyhack >= 6.5f) {
					gui::Progbar({ screen_center.x - 300, screen_center.y - 470 },
						{ 600, 5 },
						settings::hor_flyhack,
						settings::hor_flyhack);
				}
				else if (settings::hor_flyhack >= .1f) {
					gui::Progbar({ screen_center.x - 300, screen_center.y - 470 },
						{ 600, 5 },
						settings::hor_flyhack,
						6.5f);
				}
			}

			vars->matrix = unity::get_view_matrix();

			//int fff = 0;

			if (lastchamsupdate + 60.f < get_fixedTime()
				|| vars->local_player->Belt()->GetSelectedSlot() != lastslot) {
				lastslot = vars->local_player->Belt()->GetSelectedSlot();
				for (auto p : player_map)
					if (vars->visual.playeresp
						&& p.second)
					{
						esp::do_chams(p.second);
						//vars->local_player->console_echo(string::wformat(_(L"[matrix]: OnGUI - Caching bones for: %d"), p->userID()));
					}
				lastchamsupdate = get_fixedTime();
			}
			if (lastvisupdate + 0.2f < get_fixedTime()) {
				for (auto p : player_map)
					if (vars->visual.playeresp
						&& p.second
						&& vars->combat.vischeck) {
						if (!p.second->get_belt_items())
							player_map.erase(p.first);
						cache::CacheBones(p.second, vars->local_player);
					}
				lastvisupdate = get_fixedTime();
			}


			if (vars->misc.auto_upgrade)
			{
				auto block = (BuildingBlock*)vars->closest_building_block;

				if (block) {
					BuildingGrade upgrade_tier = (BuildingGrade)(vars->misc.upgrade_tier + 1);

					auto tranny = block->transform();
					if (tranny)
					{
						auto pos = tranny->position();
						auto distance = vars->local_player->eyes()->position().distance(pos);

						auto local_player = vars->local_player;

						if (distance < 4.2f) {
							if (get_fixedTime() > vars->time_last_upgrade + 0.35f)
							{
								auto grade = block->grade();
								if ((int)upgrade_tier == 1) {

									if (block->CanAffordUpgrade(BuildingGrade::Wood, local_player)
										&& block->CanChangeToGrade(BuildingGrade::Wood, local_player)
										&& grade != BuildingGrade::Wood)
									{
										block->Upgrade(BuildingGrade::Wood, local_player);
										vars->time_last_upgrade = get_fixedTime();
									}
								}
								else if ((int)upgrade_tier == 2) {
									if (block->CanAffordUpgrade(BuildingGrade::Stone, local_player)
										&& block->CanChangeToGrade(BuildingGrade::Stone, local_player)
										&& grade != BuildingGrade::Stone)
									{
										block->Upgrade(BuildingGrade::Stone, local_player);
										vars->time_last_upgrade = get_fixedTime();
									}
								}
								else if ((int)upgrade_tier == 3) {
									if (block->CanAffordUpgrade(BuildingGrade::Metal, local_player)
										&& block->CanChangeToGrade(BuildingGrade::Metal, local_player)
										&& grade != BuildingGrade::Metal)
									{
										block->Upgrade(BuildingGrade::Metal, local_player);
										vars->time_last_upgrade = get_fixedTime();
									}
								}
								else if ((int)upgrade_tier == 4) {
									if (block->CanAffordUpgrade(BuildingGrade::TopTier, local_player)
										&& block->CanChangeToGrade(BuildingGrade::TopTier, local_player)
										&& grade != BuildingGrade::TopTier)
									{
										block->Upgrade(BuildingGrade::TopTier, local_player);
										vars->time_last_upgrade = get_fixedTime();
									}
								}
							}
						}
					}
				}
			}
			auto baseplayer = vars->local_player;

			if (vars->misc.autorefill) {
				auto inv = baseplayer->inventory();
				if (inv)
				{
					auto main = inv->containerMain();
					auto belt = inv->containerBelt();
					if (main && belt)
					{
						auto mainitems = main->itemList();
						auto beltitems = belt->itemList();
						if (mainitems && beltitems)
						{
							auto size = beltitems->get_size();

							beltitems->for_each([&](Item* item, int32_t idx) {
								{
									vars->local_player->console_echo(item->get_weapon_name());
								} });
							mainitems->for_each([&](Item* item, int32_t idx) {
								{
									vars->local_player->console_echo(item->get_weapon_name());
								} });
						}
					}
				}
			}
		}
		else {
			vars->player_name_list.clear();
			player_map.clear();
			vars->gui_player_map.clear();
		}
		return;
	}
}

float unity::get_fov(Vector3 Pos) {
	vars->matrix = unity::get_view_matrix();

	Vector3 ScreenPos = WorldToScreen(Pos);
	if (ScreenPos.is_empty())
		return 1000;
	return get_2d_dist(Vector2(unity::get_width() / 2, unity::get_height() / 2), ScreenPos);
}

void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
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

gui::Color HSV(float h, float s, float v, float a = 1.0f) { float r, g, b; ColorConvertHSVtoRGB(h, s, v, r, g, b); return gui::Color(r, g, b, a); }

Vector2 CosTanSinLineH(float flAngle, float range, int x, int y, int LineLength) {
	float mainAngle = flAngle;
	mainAngle += 45.f;

	float flYaw = (mainAngle) * (M_PI / 180.0);

	float viewcosyawzzzzzzz = cos(flYaw);
	float viewsinyawzzzzzzz = sin(flYaw);

	float x2 = range * (-viewcosyawzzzzzzz) + range * viewsinyawzzzzzzz;
	float y2 = range * (-viewcosyawzzzzzzz) - range * viewsinyawzzzzzzz;

	int posonscreenX = x + int(x2 / range * (LineLength));
	int posonscreenY = y + int(y2 / range * (LineLength));

	return Vector2(posonscreenX, posonscreenY);
}

namespace esp
{
	void draw_heli(float x, float y, float w, float h) {
		gui::outline_box({ x - w / 2, y - h / 2 }, { w, h }, { 255, 255, 255, 255 });
		gui::fill_box({ x - w / 2, y - h / 2, w ,h }, { 0.2,0.2,0.2,0.3 });
		gui::outline_box({ x - w / 2 - 1, y - h / 2 - 1 }, { w + 2, h + 2 }, { 0, 0, 0, 1 });
	}

	void draw_tool_cupboard(Vector2 w2s_position, uintptr_t new_str_text, Vector4 color, System::list<PlayerNameID*>* authorizedPlayers_list)
	{
		auto label = [&](float y, wchar_t* text)
		{
			gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3 - y, 150, 20 }, text, gui::Color(0, 0, 0, 120), false, 12);
			gui::Label(Rect{ w2s_position.x, w2s_position.y + 2 - y, 150, 20 }, text, gui::Color(color.x, color.y, color.z, color.w), false, 12);
		};

		gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3, 150, 20 }, new_str_text, gui::Color(0, 0, 0, 120), false, 12);
		gui::Label(Rect{ w2s_position.x, w2s_position.y + 2, 150, 20 }, new_str_text, gui::Color(color.x, color.y, color.z, color.w), false, 12);
		float info_y = 12;

		authorizedPlayers_list->for_each([&](PlayerNameID* authorizedPlayer, int32_t idx) {
			if (authorizedPlayer)
			{
				auto user = authorizedPlayer->get_username();
				label(info_y, user);

				LOG("Username: %ls", user);
				info_y += 30;
			}
			});
	}

	void draw_hackable_crate(Vector2 w2s_position, uintptr_t crate, Vector4 color)
	{
		auto label = [&](float y, wchar_t* text)
		{
			gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3 + y, 150, 20 }, text, gui::Color(0, 0, 0, 120), false, 14);
			gui::Label(Rect{ w2s_position.x, w2s_position.y + 2 + y, 150, 20 }, text, gui::Color(color.x, color.y, color.z, color.w), false, 14);
		};

		auto timer_text = *reinterpret_cast<uintptr_t*>(crate + 0x440);
		if (!timer_text)
			return;

		auto text = (str)(*reinterpret_cast<uintptr_t*>(timer_text + 0xD0));
		if (!text)
			return;

		auto m_Text = text->str;
		if (!m_Text)
			return;

		gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3, 150, 20 }, _(L"Hackable Crate"), gui::Color(0, 0, 0, 120), false, 14);
		gui::Label(Rect{ w2s_position.x, w2s_position.y + 2, 150, 20 }, _(L"Hackable Crate"), gui::Color(color.x, color.y, color.z, color.w), false, 14);
		label(12, m_Text);
	}

	void draw_item(Vector2 w2s_position, uintptr_t label, Vector4 color, wchar_t* name) { // vector4 because color gui im mretard dont talk about it
		color /= 255;
		if (label) {
			gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3, 150, 20 }, label, gui::Color(0, 0, 0, 120), false, 12);
			gui::Label(Rect{ w2s_position.x, w2s_position.y + 2, 150, 20 }, label, gui::Color(color.x, color.y, color.z, color.w), false, 12);
		}
		else {
			gui::Label(Rect{ w2s_position.x + 1, w2s_position.y + 3, 150, 20 }, name, gui::Color(0, 0, 0, 120), false, 12);
			gui::Label(Rect{ w2s_position.x, w2s_position.y + 2, 150, 20 }, name, gui::Color(color.x, color.y, color.z, color.w), false, 12);
		}
	}

	void draw_target_hotbar(aim_target target) {
		auto screen = WorldToScreen(target.pos);

		if (target.ent && !target.is_heli && screen.z != 0 && !target.teammate) {
			auto draw_item = [&](bool draw_wearable, float x, float width) {
				System::list<Item*>* belt = ((BasePlayer*)target.ent)->get_belt_items();
				if (draw_wearable)
					belt = ((BasePlayer*)target.ent)->get_wearable_items();

				if (belt) {
					float info_y = 0;
					Item* result = nullptr;


					int current_item = 0;
					//hotbar esp
					belt->for_each([&](Item* item, int32_t idx) {
						{
							static int cases = 0;
							//static float r = 1.00f, g = 0.00f, b = 1.00f;
							switch (cases) {
							case 0: { r -= 0.0008f; if (r <= 0) cases += 1; break; }
							case 1: { g += 0.0008f; b -= 0.0008f; if (g >= 1) cases += 1; break; }
							case 2: { r += 0.0008f; if (r >= 1) cases += 1; break; }
							case 3: { b += 0.0008f; g -= 0.0008f; if (b >= 1) cases = 0; break; }
							default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
							}

							if (!item)
								return;
							const auto item_name = item->get_weapon_name();
						}
						});
					info_y = 0;
				}
			};

			float width = 260;
			draw_item(false, (unity::get_width() - width) / 2, width);
		}
	}

	void draw_text(Vector2 worldpos, wchar_t* str, Vector4 col, bool outline) {
		if (str) {
			if(outline)
				gui::Label(Rect{ worldpos.x, worldpos.y, 150, 20 }, str, gui::Color(0, 0, 0, 120), false, 10.5);
			gui::Label(Rect{ worldpos.x, worldpos.y, 150, 20 }, str, gui::Color(col.x, col.y, col.z, col.w), false, 10);
		}
	}

	void draw_name(Vector3 position, wchar_t* name)
	{
		if (name)
		{
			gui::Label(Rect{ position.x + 2.0f + 1, position.y - 4 + 1, 150, 20 }, name, gui::Color(0, 0, 0, 120), false, 12);
			gui::Label(Rect{ position.x + 2.0f, position.y - 4, 150, 20 }, name, gui::Color(255, 255, 255, 220), false, 12);
		}
	}

	void draw_middle(aim_target target) {
		//check if player
		if (target.ent && !target.teammate && !target.sleeping && target.ent != vars->local_player) {
			//check if enabled
			if (vars->visual.midname) {
				//draw player name dropshadow
				gui::Label(Rect{ 861, 442 , 200, 260 }, ((BasePlayer*)target.ent)->_displayName()->str, gui::Color(0, 0, 0, 1), true, 12);
				// draw player name
				gui::Label(Rect{ 862, 441, 200, 260 }, ((BasePlayer*)target.ent)->_displayName()->str, gui::Color(1, 1, 1, 1), true, 12);
			}
			if (vars->visual.midhealth) {
				const auto cur_health = target.ent->health();
				//draw player health dropshadow
				gui::Label(Rect{ 861 , 457, 200, 260 }, il2cpp::methods::new_string(string::format(("[%.f]"), cur_health)), gui::Color(0, 0, 0, 1), true, 10);
				//draw player health
				gui::Label(Rect{ 862 , 456, 200, 260 }, il2cpp::methods::new_string(string::format(("[%.f]"), cur_health)), gui::Color(1, 1, 1, 1), true, 10);
			}
		}
	}

	void draw_target_fov(col color, Vector2 o, float r) {
		gui::circle(o, r, color, true);
	}

	void lollll(Vector2 center) {
		gui::Color col = gui::Color(r, g, b, 1);
		auto pixelsize = 5; auto trianglecount = 5;
		Vector2 triangleList[5][3] = {
			{ Vector2{ 0, 0 }, Vector2{ 0, 0 }, Vector2{ 0, 0 } },
			{ Vector2{ 0, 0 }, Vector2{ 0, 0 }, Vector2{ 0, 0 } },
			{ Vector2{ 0, 0 }, Vector2{ 0, 0 }, Vector2{ 0, 0 } },
			{ Vector2{ 0, 0 }, Vector2{ 0, 0 }, Vector2{ 0, 0 } },
			{ Vector2{ 0, 0 }, Vector2{ 0, 0 }, Vector2{ 0, 0 } }
		};
		for (size_t n = 0; n < trianglecount; n++)
		{
			std::vector<Vector2> v = {};
			for (size_t i = 0; i < 3; i++)
			{
				triangleList[n][i].x = rand() % (pixelsize/2) + 1;
				triangleList[n][i].y = rand() % (pixelsize/2) + 1;

				if (triangleList[n][i].x > triangleList[n][i].y)
				{
					float tmp = triangleList[n][i].x;
					triangleList[n][i].x = triangleList[n][i].y;
					triangleList[n][i].y = tmp;
				}
			}
		}
		
		
		auto morphTriangleIdx = new int[pixelsize];
		auto morphVertexIdx =	new int[pixelsize];
		auto morphDeltaX =		new int[pixelsize];
		auto morphDeltaY =		new int[pixelsize];
		auto setMorphParams = [&]() {
			for (size_t i = 0; i < 3; i++)
			{
				morphTriangleIdx[i] = rand() % pixelsize + 0;
				morphVertexIdx[i] = rand() % 3 + 0;
				morphDeltaX[i] = 0;
				morphDeltaY[i] = 0;
				if ((rand() % 1 + 0) == 0)
					morphDeltaX[i] = 1;
				else
					morphDeltaX[i] = -1;
			}
		};
		
		if((rand() % 10 + 1) == 1)
			setMorphParams();

		for (size_t i = 0; i < 5; i++)
		{
			float x = triangleList[morphTriangleIdx[i]][morphVertexIdx[i]].x = morphDeltaX[i];
			float y = triangleList[morphTriangleIdx[i]][morphVertexIdx[i]].y = morphDeltaY[i];
			
			if (x > pixelsize / 2 - 1)
			{
				x = pixelsize / 2 - 1;
				morphDeltaX[i] = -1;
			}
			else if (x < 0)
			{
				x = 0;
				morphDeltaX[i] = 1;
			}
			
			if(y > pixelsize / 2 - 1)
			{
				y = pixelsize / 2 - 1;
				morphDeltaY[i] = -1;
			}
			else if (y < 0)
			{
				y = 0;
				morphDeltaY[i] = 1;
			}

			if (x > y) {
				float tmp = x;
				x = y;
				y = tmp;
			}
			
			triangleList[morphTriangleIdx[i]][morphVertexIdx[i]].x = x;
			triangleList[morphTriangleIdx[i]][morphVertexIdx[i]].y = y;
		}
		
		std::vector<Vector2> v = { };
		std::vector<Vector2> w = { };
		
		auto reflect = [&](std::vector<Vector2>& v, std::vector<Vector2>& w, int n, int offset) {
			for (int i = 0; i < v.size(); i++)
			{
				if (n == 0)
				{
					w[i].x = v[i].x + offset;
					w[i].y = v[i].y + offset;
				}
				else if (n == 1)
				{
					w[i].x = -v[i].x + offset;
					w[i].y = v[i].y + offset;
				}
				else if (n == 2)
				{
					w[i].x = v[i].x + offset;
					w[i].y = -v[i].y + offset;
				}
				else if (n == 3)
				{
					w[i].x = -v[i].x + offset;
					w[i].y = -v[i].y + offset;
				}
				else if (n == 4)
				{
					w[i].x = v[i].y + offset;
					w[i].y = v[i].x + offset;
				}
				else if (n == 5)
				{
					w[i].x = -v[i].y + offset;
					w[i].y = v[i].x + offset;
				}
				else if (n == 6)
				{
					w[i].x = v[i].y + offset;
					w[i].y = -v[i].x + offset;
				}
				else if (n == 7)
				{
					w[i].x = -v[i].y + offset;
					w[i].y = -v[i].x + offset;
				}
			}
		};

		for (size_t n = 0; n < trianglecount; n++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				v[k].x = triangleList[n][k].x;
				v[k].y = triangleList[n][k].y;
			}
			
			for (size_t k = 0; k < 3; k++)
			{
				reflect(v, w, k, pixelsize / 2);

				for (size_t z = 0; z < trianglecount; z++)
				{
					for (size_t ii = 0; ii < 3; ii++)
					{
						w[ii].x += center.x;
						w[ii].y += center.y;
					}

					gui::line(w[0], w[1], col);
					gui::line(w[1], w[2], col);
					gui::line(w[0], w[2], col);
				}
			}
		}
	}

	void offscreen_indicator(Vector3 position) {
		Vector3 local = vars->local_player->eyes()->position();
		lollll(Vector2(1920 / 2, 1080 / 2));
		float num = atan2(local.x - position.x, local.z - position.z) * 57.29578f - 180.f - EulerAngles(vars->local_player->eyes()->rotation()).y;

		if (!(num < -420 || num > -300)) return;

		Vector2 tp0 = CosTanSinLineH(num, 5.f,		 1920 / 2, 1080 / 2, 150.f);
		Vector2 tp1 = CosTanSinLineH(num + 2.f, 5.f, 1920 / 2, 1080 / 2, 140.f);
		Vector2 tp2 = CosTanSinLineH(num - 2.f, 5.f, 1920 / 2, 1080 / 2, 140.f);

		Vector2 p = { tp0.x, tp0.y }, p1 = { tp1.x, tp1.y }, p2 = { tp2.x, tp2.y };
		

		gui::line(tp0, tp1, rgba(249.f, 130.f, 109.f, 255.f));
		gui::line(tp0, tp2, rgba(249.f, 130.f, 109.f, 255.f));
		gui::line(tp1, tp2, rgba(249.f, 130.f, 109.f, 255.f));
	}

	int lastcham = 0;
	int lastchamh = 0;
	int lastslot = 69;

	void do_chams(BasePlayer* ent)
	{
		if (!vars->local_player) return;
		if (!ent->is_alive() || (ent->is_sleeping() && !vars->visual.sleeper_esp)) return;

		if (unity::bundle && unity::bundle2)
		{
			Shader* shader = 0;
			Material* mat = 0;

			//auto lastchamh = vars->handchams_player_map[ent->userID()];
			//auto lastcham = vars->chams_player_map[ent->userID()];

			//if (lastchamh != vars->visual.hand_chams)
			//if ((lastslot != vars->local_player->Belt()->GetSelectedSlot() || vars->visual.rainbow_chams)
			//	&& ent->is_local_player())
			if (ent->is_local_player())
			{
				switch (vars->visual.hand_chams)
				{
				case 1:
					shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"SeethroughShader"), unity::GetType(_("UnityEngine"), _("Shader")));
					break;
				case 2:
					shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"Chams"), unity::GetType(_("UnityEngine"), _("Shader")));
					break;
				case 3:
					mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/force field.mat"), unity::GetType(_("UnityEngine"), _("Material")));
					break;
				case 4:
					mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/distortionrim.mat"), unity::GetType(_("UnityEngine"), _("Material")));
					break;
				case 5:
					mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/2dmat.mat"), unity::GetType(_("UnityEngine"), _("Material")));
					break;
				case 6:
					mat = (Material*)unity::LoadAsset(unity::galaxy_bundle, string::wformat(vars->visual.galaxymathand < 10 ? _(L"GalaxyMaterial_0%d") : _(L"GalaxyMaterial_%d"), vars->visual.galaxymathand), unity::GetType(_("UnityEngine"), _("Material")));
					break;
				}
				//vars->handchams_player_map[ent->userID()] = vars->visual.hand_chams;
				//lastslot = vars->local_player->Belt()->GetSelectedSlot();

				if ((!shader && !mat) && (vars->visual.hand_chams < 1 && vars->visual.shader < 1)) return;

				if (vars->visual.hand_chams >= 1) { //crashes on islocalplayer
					auto model = get_activemodel();
					auto renderers = ((Networkable*)model)->GetComponentsInChildren(unity::GetType(_("UnityEngine"), _("Renderer")));
					if (renderers)
					{
						auto sz = *reinterpret_cast<int*>(renderers + 0x18);

						auto setrenderer = [&](Renderer* renderer) {
							if (!renderer) return;
							Material* material = renderer->GetMaterial();
							if (!material) return;

							if (vars->visual.hand_chams == 2)
							{
								auto s = (Shader*)FindShader(System::string(_(L"Standard")));
								material->SetShader(s);
								material->SetColor(_(L"_Color"), col(r, g, b, 0.5));
								SetInt((uintptr_t)material, _(L"_ZTest"), 4);
								return;
							}

							//if (sz == 2) i == 1; //skips front of weapon

							if (mat) {
								if (material != mat) {
									renderer->SetMaterial(mat);
									SetInt((uintptr_t)material, _(L"_ZTest"), 8);
									if (vars->visual.hand_chams == 5)
										mat->SetFloat(_(L"_OutlineSize"), 20.f);
								}
							}
							else if (shader != material->GetShader()) {
								if (vars->visual.shader >= 5 && unity::galaxy_material && (uintptr_t)renderer->GetMaterial() != unity::galaxy_material)
									renderer->SetMaterial((Material*)unity::galaxy_material);
								else material->SetShader(shader);
							}
						};

						for (int i = 0; i < sz; i++) {
							auto renderer = *reinterpret_cast<Renderer**>(renderers + 0x20 + i * 0x8);
							setrenderer(renderer);
						}
					}
				}
			}

			switch (vars->visual.shader)
			{
			case 1:
				shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"Chams"), unity::GetType(_("UnityEngine"), _("Shader")));
				break;
			case 2:
				shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"SeethroughShader"), unity::GetType(_("UnityEngine"), _("Shader")));
				break;
			case 3:
				shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"WireframeTransparent"), unity::GetType(_("UnityEngine"), _("Shader")));
				break;
			case 4:
				shader = (Shader*)unity::LoadAsset(unity::bundle, _(L"chamslit"), unity::GetType(_("UnityEngine"), _("Shader")));
				break;
			case 5:
				mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/force field.mat"), unity::GetType(_("UnityEngine"), _("Material")));
				break;
			case 6:
				mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/distortionrim.mat"), unity::GetType(_("UnityEngine"), _("Material")));
				break;
			case 7:
				mat = (Material*)unity::LoadAsset(unity::bundle2, _(L"assets/2dmat.mat"), unity::GetType(_("UnityEngine"), _("Material")));
				break;
			case 8:
				mat = (Material*)unity::LoadAsset(unity::galaxy_bundle, string::wformat(vars->visual.galaxymat < 10 ? _(L"GalaxyMaterial_0%d") : _(L"GalaxyMaterial_%d"), vars->visual.galaxymat), unity::GetType(_("UnityEngine"), _("Material")));
				break;
			}
			//vars->chams_player_map[ent->userID()] = vars->visual.shader;

			if (vars->visual.shader >= 1 && (shader || mat)) {
				uintptr_t chams_shader = 0;

				//static int cases = 0;
				//switch (cases) {
				//case 0: { r -= 0.004f; if (r <= 0) cases += 1; break; }
				//case 1: { g += 0.004f; b -= 0.004f; if (g >= 1) cases += 1; break; }
				//case 2: { r += 0.004f; if (r >= 1) cases += 1; break; }
				//case 3: { b += 0.004f; g -= 0.004f; if (b >= 1) cases = 0; break; }
				//default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
				//}
				//unity::chams_shader = unity::LoadAsset(unity::bundle, _(L"Chams"), unity::GetType(_("UnityEngine"), _("Shader")));

				//this breaks?
				//auto _multiMesh = ent->playerModel()->_multiMesh();//mem::read<uintptr_t>(player->playerModel() + 0x2D0); //SkinnedMultiMesh _multiMesh;
				//auto model = ent->playerModel();
				auto model = *reinterpret_cast<PlayerModel**>((uintptr_t)ent + 0x4D0);
				if (!model || (uintptr_t)model < 0xFFFF|| (uintptr_t)model > 0xF000000000000000) return;
				auto _multiMesh = *reinterpret_cast<SkinnedMultiMesh**>((uintptr_t)model + 0x2D0);
				if (!_multiMesh) return;
				auto render = _multiMesh->get_Renderers(); //get_Renderers(_multiMesh);
				if (!render) return;

				for (int i = 0; i < render->get_size(); i++) {
					auto renderer = render->get_value(i);
					if (!renderer) continue;
					auto material = renderer->GetMaterial();
					if (!material) continue;
					auto viscolor = col(vars->colors.players.chams.visible[0], vars->colors.players.chams.visible[1], vars->colors.players.chams.visible[2], 1);
					auto inviscolor = col(vars->colors.players.chams.invisible[0], vars->colors.players.chams.invisible[1], vars->colors.players.chams.invisible[2], 1);

					if (vars->visual.rainbow_chams)
					{
						viscolor = col(r, g, b, 1);
						inviscolor = col(1.f - r, 1.f - g, 1.f - b, 1);
					}

					if (shader || mat)//(vars->visual.shader >= 5 && unity::galaxy_material))
					{
						if (mat) {
							auto cm = renderer->GetMaterial();
							auto shader1 = cm->GetShader();
							auto shader2 = mat->GetShader();
							if (shader1 != shader2)
							{
								if (material != mat) {
									renderer->SetMaterial(mat);
									SetInt((uintptr_t)material, _(L"_ZTest"), 8);
									if (vars->visual.shader == 6)
									{
										mat->SetColor(_(L"_RimColor"), viscolor);
									}
									else if (vars->visual.shader == 7)
									{
										mat->SetColor(_(L"_Color"), viscolor);
										mat->SetColor(_(L"_OutlineColor"), inviscolor);
										mat->SetFloat(_(L"_OutlineSize"), 20.f);
									}
								}
							}
						}
						else if (shader != material->GetShader()) {
							if (vars->visual.shader >= 5 && unity::galaxy_material && (uintptr_t)renderer->GetMaterial() != unity::galaxy_material)
								renderer->SetMaterial((Material*)unity::galaxy_material);
							else material->SetShader(shader);
						}
						else
						{
							switch (vars->visual.shader)
							{
							case 1:
								material->SetColor(_(L"_ColorVisible"), viscolor);
								material->SetColor(_(L"_ColorBehind"), inviscolor);
								break;
							case 3:
								material->SetColor(_(L"_WireColor"), viscolor);
								break;
							case 4:
								material->SetColor(_(L"_ColorVisible"), viscolor);
								material->SetColor(_(L"_ColorBehind"), inviscolor);
								break;
							}
						}
					}
				}
			}

			//clothes chams


			return;
		}
	}
}