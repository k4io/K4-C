#pragma once
#include <d2d1.h>
//#include <wincodec.h>
#include <dwrite_1.h>
#include <intrin.h>
#include "utils/vector.hpp"
#include <D3D11.h>
#include <d3d9.h>
#include "imgui/imgui.h"

#include <wingdi.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
//#pragma comment(lib, "windowscodecs.lib")

D3DCOLOR FLOAT4TOD3DCOLOR(float Col[])
{
	ImU32 col32_no_alpha = im::ColorConvertFloat4ToU32(ImVec4(Col[0], Col[1], Col[2], Col[3]));
	float a = (col32_no_alpha >> 24) & 255;
	float r = (col32_no_alpha >> 16) & 255;
	float g = (col32_no_alpha >> 8) & 255;
	float b = col32_no_alpha & 255;
	return D3DCOLOR_ARGB((int)a, (int)b, (int)g, (int)r);
}

class RenderClass
{
private:
	//render context
	ID2D1Factory* Interface;
	ID2D1RenderTarget* Canvas; 
	IDWriteFactory1* TextEngine;
	IDWriteTextFormat* TextFormat;
	IDWriteFontCollection* fontCollection;
	ID2D1SolidColorBrush* SolidColor;
	ID2D1LinearGradientBrush* LinearGradientBrush;
	ID2D1RadialGradientBrush* RadialGradientBrush;

	//fast get wstring length
	__forceinline UINT wcslen(const wchar_t* Str)
	{
		const wchar_t* TempStr = Str;
		for (; *TempStr; ++TempStr);
		return (UINT)(TempStr - Str);
	}

	ID2D1Bitmap* custom_box_bitmap = NULL;

	ID2D1Bitmap* boxBitmap = nullptr;

	//HRESULT LoadBitmapFromFile(const wchar_t* filename, ID2D1Bitmap** pBitmap, bool custombox = false)
	//{
	//	HRESULT hr = S_FALSE;
	//	IWICImagingFactory* wic_factory = NULL;
	//	IWICBitmapDecoder* decoder = NULL;
	//	IWICBitmapFrameDecode* frame = NULL;
	//	IWICFormatConverter* converter = NULL;
	//
	//	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&wic_factory));
	//	if FAILED(hr) goto clenaup;
	//
	//	hr = wic_factory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	//	if FAILED(hr) goto clenaup;
	//
	//	hr = decoder->GetFrame(0, &frame);
	//	if FAILED(hr) goto clenaup;
	//
	//	hr = wic_factory->CreateFormatConverter(&converter);
	//	if FAILED(hr) goto clenaup;
	//
	//	hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	//	if FAILED(hr) goto clenaup;
	//
	//	if (custombox)
	//		hr = Canvas->CreateBitmapFromWicBitmap(converter, 0, &boxBitmap);
	//	else
	//		hr = Canvas->CreateBitmapFromWicBitmap(converter, 0, pBitmap);
	//	if FAILED(hr) goto clenaup;
	//
	//clenaup:
	//	decoder->Release();
	//	converter->Release();
	//	frame->Release();
	//	wic_factory->Release();
	//	return hr;
	//}

	__declspec(noinline) bool InitRender(IDXGISwapChain* SwapChain)
	{
		//prep d2d render & text engine
		static bool initialized; if (!initialized)
		{
			initialized = true; D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };
			auto dwrite = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(TextEngine), (IUnknown**)&TextEngine);
			auto d2d1 = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&Interface);
			
			auto fontname = _(L"mc.ttf");
			if (std::filesystem::exists(fontname))
			{
				if (SUCCEEDED(TextEngine->GetSystemFontCollection(&fontCollection, false)))
				{
					if (SUCCEEDED(TextEngine->CreateTextFormat(_(L"MinecraftCHMC"),
						fontCollection,
						DWRITE_FONT_WEIGHT_NORMAL,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						14.f,
						_(L""),
						&TextFormat)))
					{
						if (!Interface || !TextEngine || !TextFormat) return false;
					}
				}
			}
			//TextEngine->CreateTextFormat(_(L"Consolas"), NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 14.f, L"", &TextFormat);
			if (!Interface || !TextEngine || !TextFormat) return false;
			RemoveFontResource(fontname);
		}
		//get window flags var
		ID3D11Device* d3d_device;
		if (SwapChain->GetDevice(IID_PPV_ARGS(&d3d_device))) return false;
		WORD flagsOffset = *(WORD*)((*(DWORD64**)d3d_device)[38] + 2); //x64
		int& flags = *(INT*)((DWORD64)d3d_device + flagsOffset);
		d3d_device->Release();
		//get d3d backbuffer
		IDXGISurface* d3d_bbuf;
		if (SwapChain->GetBuffer(0, IID_PPV_ARGS(&d3d_bbuf)))
			return false;
		//create canvas & cleanup
		D2D1_RENDER_TARGET_PROPERTIES d2d_prop = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
		flags |= 0x20; HRESULT canvas_state = Interface->CreateDxgiSurfaceRenderTarget(d3d_bbuf, d2d_prop, &Canvas); flags &= ~0x20; d3d_bbuf->Release(); if (canvas_state) return false;
		if (!SolidColor) Canvas->CreateSolidColorBrush({}, &SolidColor); return true;
	}

public:
	//canvas mgr
	__forceinline bool NewFrame(IDXGISwapChain* SwapChain)
	{
		//need prep d2d canvas
		if (!Canvas && !InitRender(SwapChain))
			return false;
		//draw start
		Canvas->BeginDraw();
		return true;
	}

	__forceinline Vector2 CanvasSize()
	{
		D2D1_SIZE_F Size = Canvas->GetSize();
		return Vector2{ Size.width, Size.height };
	}

	__forceinline void ResetCanvas()
	{
		if (Canvas) {
			Canvas->Release();
			Canvas = nullptr;
		}
	}

	__forceinline void EndFrame()
	{
		HRESULT state = Canvas->EndDraw();
		if (state == D2DERR_RECREATE_TARGET)
			ResetCanvas();
	}

	////custom box
	//__forceinline HRESULT SetCustomBox(std::string image_path) {
	//	std::wstring ws(image_path.begin(), image_path.end());
	//	return LoadBitmapFromFile(ws.c_str(), &boxBitmap, true);
	//}
	//
	//__forceinline void DrawCustomBox(const Vector2& start, const Vector2& sz) {
	//	if (!boxBitmap) return;
	//	D2D1_SIZE_F size = boxBitmap->GetSize();
	//	Canvas->DrawBitmap(boxBitmap, 
	//		D2D1::RectF(start.x, start.y, start.x + sz.x, start.y + sz.y), 
	//		1.0f, 
	//		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, 
	//		D2D1::RectF(0, 0, size.width, size.height));
	//}

	//line
	__forceinline void Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawLine({ Start.x, Start.y }, { End.x, End.y }, SolidColor, Thick);
	}
	//circle
	__forceinline void Circle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor, Thick);
	}

	__forceinline void F_C(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor, Thick);
	}

	__forceinline void FillCircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillEllipse({ { Start.x, Start.y }, Rad, Rad }, SolidColor);
	}

	__forceinline void FillRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr) {
		SolidColor->SetColor(Clr); Canvas->FillRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, SolidColor);
	}

	//rectangle
	__forceinline void Rectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, SolidColor, Thick);
	}

	__forceinline void RoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad, float Thick = 1.5f)
	{
		SolidColor->SetColor(Clr); Canvas->DrawRoundedRectangle({ {Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, Rad, Rad }, SolidColor, Thick);
	}

	__forceinline void FillRoundedRectangle(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& Clr, float Rad)
	{
		SolidColor->SetColor(Clr); Canvas->FillRoundedRectangle({ {Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y}, Rad, Rad }, SolidColor);
	}

	__forceinline void FillRectangle_GradientLinear(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& ClrTop, const D2D1::ColorF& ClrBottom)
	{
		ID2D1GradientStopCollection* pStops;
		D2D1_GRADIENT_STOP stops[2];
		stops[0].color = ClrTop;
		stops[0].position = 0.f;
		stops[1].color = ClrBottom;
		stops[1].position = 1.f;
		auto hr = Canvas->CreateGradientStopCollection(
			stops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pStops
		);
		if (SUCCEEDED(hr))
		{
			hr = Canvas->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(
					D2D1::Point2F(Start.x, Start.y),
					D2D1::Point2F(Start.x + Sz.x, Start.y + Sz.y)),
				pStops,
				&LinearGradientBrush
			);
		}
		Canvas->FillRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, LinearGradientBrush);
	}
	__forceinline void FillRoundedRectangle_GradientLinear(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& ClrTop, const D2D1::ColorF& ClrBottom, float Rad)
	{
		ID2D1GradientStopCollection* pStops;
		D2D1_GRADIENT_STOP stops[2];
		stops[0].color = ClrTop;
		stops[0].position = 0.f;
		stops[1].color = ClrBottom;
		stops[1].position = 1.f;
		auto hr = Canvas->CreateGradientStopCollection(
			stops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pStops
		);
		if (SUCCEEDED(hr))
		{
			hr = Canvas->CreateLinearGradientBrush(
				D2D1::LinearGradientBrushProperties(
					D2D1::Point2F(Start.x, Start.y),
					D2D1::Point2F(Start.x + Sz.x, Start.y + Sz.y)),
				pStops,
				&LinearGradientBrush
			);
		}
		Canvas->FillRoundedRectangle({ { Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, Rad, Rad }, LinearGradientBrush);
	}

	__forceinline void FillRectangle_GradientRadial(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& ClrMid, const D2D1::ColorF& ClrOut, float Rad)
	{
		ID2D1GradientStopCollection* pStops;
		D2D1_GRADIENT_STOP stops[2];
		stops[0].color = ClrMid;
		stops[0].position = 0.f;
		stops[1].color = ClrOut;
		stops[1].position = 1.f;
		auto hr = Canvas->CreateGradientStopCollection(
			stops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pStops
		);
		if (SUCCEEDED(hr))
		{
			hr = Canvas->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(
					D2D1::Point2F((Start.x + (Sz.x / 2), (Start.y + (Sz.y / 2)))),
					D2D1::Point2F(0, 0),
					Rad, Rad),
				pStops,
				&RadialGradientBrush
			);
		}
		Canvas->FillRectangle({ Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, RadialGradientBrush);
	}
	__forceinline void FillRoundedRectangle_GradientRadial(const Vector2& Start, const Vector2& Sz, const D2D1::ColorF& ClrMid, const D2D1::ColorF& ClrOut, float Rad)
	{
		ID2D1GradientStopCollection* pStops;
		D2D1_GRADIENT_STOP stops[2];
		stops[0].color = ClrMid;
		stops[0].position = 0.f;
		stops[1].color = ClrOut;
		stops[1].position = 1.f;
		auto hr = Canvas->CreateGradientStopCollection(
			stops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pStops
		);
		if (SUCCEEDED(hr))
		{
			hr = Canvas->CreateRadialGradientBrush(
				D2D1::RadialGradientBrushProperties(
					D2D1::Point2F((Start.x + (Sz.x / 2), (Start.y + (Sz.y / 2)))),
					D2D1::Point2F(0, 0),
					Rad, Rad),
				pStops,
				&RadialGradientBrush
			);
		}
		Canvas->FillRoundedRectangle({ { Start.x, Start.y, Start.x + Sz.x, Start.y + Sz.y }, Rad, Rad }, RadialGradientBrush);
	}

	//text
	__forceinline Vector2 StringCenter(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White), bool outline = true, float fontsize = 12.f)
	{
		if (vars->visual.text_background_box && outline)
		{
			SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			IDWriteTextLayout* TextLayout;
			TextEngine->CreateTextLayout(Str, wcslen(Str), TextFormat, 200.f, 100.f, &TextLayout);
			DWRITE_TEXT_METRICS TextInfo;
			TextLayout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width, TextInfo.height };
			TextLayout->SetFontSize(fontsize, { 20, (uint32_t)std::wstring(Str).size() });
			Vector2 st = { Start.x - (TextSize.x / 2) - 3, Start.y - (TextSize.y / 2) - 1 };
			TextSize.x += 5; TextSize.y += 2;
			switch (vars->visual.text_background_box) {
			case 0:
				break;
			case 1: //Gradient full
				FillRectangle({ st.x, st.y }, TextSize, { 0.07f, 0.07f, 0.07f, 0.65f });
				//Rectangle({ st.x, st.y }, TextSize, { 0.21, 0.21, 0.21, 1 }, 0.5f);
				break;
			case 2: //Gradient rounded
				FillRoundedRectangle({ st.x, st.y }, TextSize, { 0.07f, 0.07f, 0.07f, 0.65f }, 4);
				//RoundedRectangle({ st.x, st.y }, TextSize, { 0.21, 0.21, 0.21, 1 }, 4, 0.5f);
				break;
			}
		}
		if (outline) {
			StringCenter({ Start.x - 1, Start.y }, Str, { 0, 0, 0, 1 }, false);
			StringCenter({ Start.x + 1, Start.y }, Str, { 0, 0, 0, 1 }, false);
			StringCenter({ Start.x, Start.y - 1 }, Str, { 0, 0, 0, 1 }, false);
			StringCenter({ Start.x, Start.y + 1 }, Str, { 0, 0, 0, 1 }, false);
		}

		SolidColor->SetColor(Clr); IDWriteTextLayout* TextLayout; TextEngine->CreateTextLayout(Str, this->wcslen(Str), TextFormat, 200.f, 100.f, &TextLayout);
		DWRITE_TEXT_METRICS TextInfo; TextLayout->GetMetrics(&TextInfo); Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
		Canvas->DrawTextLayout({ Start.x - TextSize.x, Start.y - TextSize.y }, TextLayout, SolidColor); TextLayout->Release();
		return TextSize; //return half text size
	}

	__forceinline D2D1::ColorF perc_col(float Num, float alpha = 1.0f)
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
		return D2D1::ColorF(red / 255.f, green / 255.f, blue / 255.f, alpha);
	}

	__forceinline void ProgressBar(Vector2 start, Vector2 size, float a, float b)
	{
		if (a <= 0) return;
		FillRectangle({ start.x, start.y }, { size.x, size.y }, D2D1::ColorF(21.f / 255.f, 27.f / 255.f, 37.f / 255.f, 255.f));
		//fill_box(rust::classes::Rect(start.x, start.y, end.x, end.y + 1), rgba(21.f, 27.f, 37.f, 255.f));

		if ((a / b) > 1)
			a = b;

		//FillRectangle({ start.x + 1, start.y + 1 }, { (size.x * (a/b)) - 2, size.y - 2}, perc_col(a / b));
		FillRectangle({ start.x + 1, start.y + 1 }, { (size.x * (a/b)) - 2, size.y - 2}, FLOAT4TOD3DCOLOR(vars->accent_color));
	}

	Vector2 RenderString(const Vector2& pos, const wchar_t* text, bool center = false, const D2D1::ColorF& color = D2D1::ColorF(D2D1::ColorF::White), bool outline = false, bool Big = false)
	{
		if (center)
		{
			SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			IDWriteTextLayout* TextLayout;
			TextEngine->CreateTextLayout(text, wcslen(text), TextFormat, 200.f, 100.f, &TextLayout);
			DWRITE_TEXT_METRICS TextInfo;
			TextLayout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width / 2.f, TextInfo.height / 2.f };
			if (outline)
			{
				const auto x = pos.x - TextSize.x;
				const auto y = pos.y - TextSize.y;
				Canvas->DrawTextLayout(D2D1::Point2F(x - 1, y), TextLayout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x + 1, y), TextLayout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x, y - 1), TextLayout, SolidColor);
				Canvas->DrawTextLayout(D2D1::Point2F(x, y + 1), TextLayout, SolidColor);
			}

			SolidColor->SetColor(color);

			Canvas->DrawTextLayout({ pos.x - TextSize.x, pos.y - TextSize.y }, TextLayout, SolidColor);
			TextLayout->Release();
			return TextSize;
		}

		IDWriteTextLayout* TextLayout;
		TextEngine->CreateTextLayout(text, wcslen(text), TextFormat, 200.f, 100.f, &TextLayout);
		SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));

		if (outline)
		{
			const auto x = pos.x;
			const auto y = pos.y;

			Canvas->DrawTextLayout(D2D1::Point2F(x - 1, y), TextLayout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x + 1, y), TextLayout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x, y - 1), TextLayout, SolidColor);
			Canvas->DrawTextLayout(D2D1::Point2F(x, y + 1), TextLayout, SolidColor);
		}

		SolidColor->SetColor(color);
		Canvas->DrawTextW(text, wcslen(text), TextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, SolidColor);

		TextLayout->Release();

		return { 0,0 };
	}

	__forceinline void String(const Vector2& Start, const wchar_t* Str, const D2D1::ColorF& Clr = D2D1::ColorF(D2D1::ColorF::White))
	{
		if (vars->visual.text_background_box)
		{
			SolidColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			IDWriteTextLayout* TextLayout;
			TextEngine->CreateTextLayout(Str, wcslen(Str), TextFormat, 200.f, 100.f, &TextLayout);
			DWRITE_TEXT_METRICS TextInfo;
			TextLayout->GetMetrics(&TextInfo);
			Vector2 TextSize = { TextInfo.width + 2, TextInfo.height + 3 };
			switch (vars->visual.text_background_box) {
			case 0:
				break;
			case 1: //Gradient full
				FillRectangle_GradientLinear({ Start.x - 1, Start.y - 1 }, TextSize,
					{ 0.14f, 0.14f, 0.14f, 0.65f },
					{ 159.f / 255.f, 40.f / 255.f, 29.f / 255.f, 0.65f });
				Rectangle({ Start.x - 1, Start.y - 1 }, TextSize, { 0.21, 0.21, 0.21, 1 }, 2);
				break;
			case 2: //Gradient rounded
				FillRoundedRectangle_GradientLinear({ Start.x - 1, Start.y - 1 }, TextSize,
					{ 0.14f, 0.14f, 0.14f, 0.65f },
					{ 159.f / 255.f, 40.f / 255.f, 29.f / 255.f, 0.65f }, 2);
				RoundedRectangle({ Start.x - 1, Start.y - 1 }, TextSize, { 0.21, 0.21, 0.21, 1 }, 3, 2);
				break;
			}
		}
		SolidColor->SetColor(Clr); 
		Canvas->DrawTextW(Str, this->wcslen(Str), TextFormat, { Start.x, Start.y, FLT_MAX, FLT_MAX }, SolidColor);
	}
};

RenderClass render;