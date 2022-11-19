#pragma once

#include "Blitter.h"

DEFINE_RLE_BLITTER(RLEBlitTransRemapDestZRead)
{
public:
	inline explicit RLEBlitTransRemapDestZRead(T* remap, T* palette) noexcept
	{
		RemapData = remap;
		PaletteData = palette;
	}

	virtual ~RLEBlitTransRemapDestZRead() override final = default;

	virtual void Blit_Copy(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust)
	{
		auto dest = reinterpret_cast<T*>(dst);

		Process_Pre_Lines<true, false>(dest, src, len, line, zbuf, abuf);

		auto handler = [this](T& dest, byte srcv, int zbase, WORD zbufv, byte zadjustv)
		{
			int zval = zbase - zadjustv;
			if (zval < zbufv)
				dest = PaletteData[RemapData[srcv]];
		};

		Process_Pixel_Datas<true, false, true>(dest, src, len, zbase, zbuf, abuf, alvl, alpha_idx, zadjust, handler);
	}

	virtual void Blit_Copy_Tinted(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust, int tint)
	{
		Blit_Copy(dst, src, len, line, zbase, zbuf, abuf, alvl, alpha_idx, zadjust);
	}

private:
	T* RemapData;
	T* PaletteData;
};