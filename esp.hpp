#pragma once
#include "RenderClass.h"

void iterate_entities() {

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
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->colors.ui.crosshair), 2.3f);
}

int indicators_on_screen = 0;

void IndicatorDesync() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 20 }, { 120, 4 }, vars->desyncTime, 1.0f);
}
void IndicatorReload() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	if(vars->time_since_last_shot < vars->reload)
		render.ProgressBar({ x - 60, y + 30 }, { 120, 4 }, vars->time_since_last_shot, (vars->reload - 0.2f));
}
void IndicatorFlyhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;

}
void IndicatorSpeedhack() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.ProgressBar({ x - 60, y + 40 }, { 120, 4 }, vars->speedhack, 1.0f);
}

void DrawSnapline() {
	Vector2 start = vars->visual.snapline == 2 ? Vector2(vars->ScreenX / 2, 0) :
		vars->visual.snapline == 3 ? Vector2(vars->ScreenX / 2, vars->ScreenY / 2) :
		vars->visual.snapline == 4 ? Vector2(vars->ScreenX / 2, vars->ScreenY) :
		Vector2(vars->ScreenX / 2, 1080);
	
}

void DrawFov() {
	float x = vars->ScreenX / 2, y = vars->ScreenY / 2;
	render.Circle(Vector2(x, y), FLOAT4TOD3DCOLOR(vars->colors.ui.fov), vars->combat.aimbotfov, 0.7f);
}

void Watermark() {
	render.FillRectangle(Vector2(7, 7), Vector2(60, 16), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
	render.FillRectangle(Vector2(6, 6), Vector2(62, 18), D2D1::ColorF(0.11f, 0.11f, 0.11f));
	render.FillRectangle(Vector2(7, 21), Vector2(60, 2), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
	render.String({ 14,7 }, _(L"rust"), D2D1::ColorF(249.f / 255.f, 130.f / 255.f, 109.f / 255.f));
}

void new_frame() {
	float Y = GetSystemMetrics(SM_CYSCREEN);
	float X = GetSystemMetrics(SM_CXSCREEN);
	Vector2 vScreen = render.CanvasSize();

	vars->ScreenX = vScreen.x;
	vars->ScreenY = vScreen.y;

	//Draw crosshairs
	if (vars->visual.crosshair1)
		Crosshair1();
	if (vars->visual.crosshair2)
		Crosshair2();
	if (vars->visual.crosshair3)
		Crosshair3();

	//Draw indicators
	if (vars->visual.desync_indicator)
		IndicatorDesync();
	if (vars->combat.always_reload)
		IndicatorReload();
	if (vars->visual.flyhack_indicator)
		IndicatorFlyhack();
	if (vars->visual.speedhack_indicator)
		IndicatorSpeedhack();

	//Draw FOV
	if (vars->visual.show_fov)
		DrawFov();

	//Draw watermark
	Watermark();

	iterate_entities();
}