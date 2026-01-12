/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Abstraction for Windows Graphic Subsystems
 */

#include "d3drender.h"

enum GraphicsMode {
	MODE_INIT,
	MODE_D3D12,
	MODE_D3D11,
	MODE_D3D9,
	MODE_GDI,
};

static int nGraphicsMode;

BOOL D3DInitialize(HWND hWnd, int nWidth, int nHeight, BOOL bForceD3D9)
{
	BOOL bAvoidD3D12, bAvoidD3D11;

	bAvoidD3D12 = bAvoidD3D11 = FALSE;
	if (bForceD3D9)
		bAvoidD3D12 = bAvoidD3D11 = TRUE;

#if 0
	/* Avoid using Direct3D 12 on Wine for now. */
	if (LOBYTE(LOWORD(GetVersion())) < 10)
		bAvoidD3D12 = TRUE;
#endif

	/* Try Direct3D 12. */
	if (!bAvoidD3D12)
	{
		if (D3D12Initialize(hWnd, nWidth, nHeight))
		{
			nGraphicsMode = MODE_D3D12;
			return TRUE;
		}
	}

	/* Try Direct3D 11. */
	if (!bAvoidD3D11)
	{
		if (D3D11Initialize(hWnd, nWidth, nHeight))
		{
			nGraphicsMode = MODE_D3D11;
			return TRUE;
		}
	}

	/* Try Direct3D 9. */
	if (D3D9Initialize(hWnd, nWidth, nHeight))
	{
		nGraphicsMode = MODE_D3D9;
		return TRUE;
	}

#if 0
	/* Fallback to GDI. */
	if (GDIInitialize(hWnd, nWidth, nHeight))
	{
		nGraphicsMode = MODE_GDI;
		return TRUE;
	}
#endif

	log_error("Failed to initialize graphics.");

	return FALSE;
}

VOID D3DCleanup(void)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12Cleanup();
		break;
	case MODE_D3D11:
		D3D11Cleanup();
		break;
	case MODE_D3D9:
		D3D11Cleanup();
		break;
	case MODE_GDI:
		GDICleanup();
		break;
	}
}

BOOL D3DResizeWindow(int nScreenWidth, int nScreenHeight, int nOffsetX, int nOffsetY, int nViewportWidth, int nViewportHeight, float scale)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12ResizeWindow(nScreenWidth, nScreenHeight, nOffsetX, nOffsetY, nViewportWidth, nViewportHeight, scale);
		break;
	case MODE_D3D11:
		D3D11ResizeWindow(nScreenWidth, nScreenHeight, nOffsetX, nOffsetY, nViewportWidth, nViewportHeight, scale);
		break;
	case MODE_D3D9:
		D3D9ResizeWindow(nScreenWidth, nScreenHeight, nOffsetX, nOffsetY, nViewportWidth, nViewportHeight, scale);
		break;
	case MODE_GDI:
		break;
	}

	return TRUE;
}

VOID D3DStartFrame(void)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12StartFrame();
		break;
	case MODE_D3D11:
		D3D11StartFrame();
		break;
	case MODE_D3D9:
		D3D9StartFrame();
		break;
	case MODE_GDI:
		GDIStartFrame();
		break;
	}
}

VOID D3DEndFrame(void)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12EndFrame();
		break;
	case MODE_D3D11:
		D3D11EndFrame();
		break;
	case MODE_D3D9:
		D3D9EndFrame();
		break;
	case MODE_GDI:
		GDIEndFrame();
		break;
	}
}

BOOL D3DIsSoftRendering(void)
{
	if (nGraphicsMode == MODE_GDI)
		return TRUE;

	return FALSE;
}

void notify_image_update(struct image *img)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12NotifyImageUpdate(img);
		break;
	case MODE_D3D11:
		D3D11NotifyImageUpdate(img);
		break;
	case MODE_D3D9:
		D3D9NotifyImageUpdate(img);
		break;
	case MODE_GDI:
		GDINotifyImageUpdate(img);
		break;
	}
}

void notify_image_free(struct image *img)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12NotifyImageFree(img);
		break;
	case MODE_D3D11:
		D3D11NotifyImageFree(img);
		break;
	case MODE_D3D9:
		D3D9NotifyImageFree(img);
		break;
	case MODE_GDI:
		GDINotifyImageFree(img);
		break;
	}
}

void
render_image_normal(
	int dst_left,				/* The X coordinate of the screen */
	int dst_top,				/* The Y coordinate of the screen */
	int dst_width,				/* The width of the destination rectangle */
	int dst_height,				/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] an image to be rendered */
	int src_left,				/* The X coordinate of a source image */
	int src_top,				/* The Y coordinate of a source image */
	int src_width,				/* The width of the source rectangle */
	int src_height,				/* The height of the source rectangle */
	int alpha)					/* The alpha value (0 to 255) */
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageNormal(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImageNormal(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImageNormal(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImageNormal(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_add(
	int dst_left,				/* The X coordinate of the screen */
	int dst_top,				/* The Y coordinate of the screen */
	int dst_width,				/* The width of the destination rectangle */
	int dst_height,				/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] an image to be rendered */
	int src_left,				/* The X coordinate of a source image */
	int src_top,				/* The Y coordinate of a source image */
	int src_width,				/* The width of the source rectangle */
	int src_height,				/* The height of the source rectangle */
	int alpha)					/* The alpha value (0 to 255) */
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageAdd(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImageAdd(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImageAdd(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImageAdd(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_sub(
	int dst_left,				/* The X coordinate of the screen */
	int dst_top,				/* The Y coordinate of the screen */
	int dst_width,				/* The width of the destination rectangle */
	int dst_height,				/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] an image to be rendered */
	int src_left,				/* The X coordinate of a source image */
	int src_top,				/* The Y coordinate of a source image */
	int src_width,				/* The width of the source rectangle */
	int src_height,				/* The height of the source rectangle */
	int alpha)					/* The alpha value (0 to 255) */
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageSub(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImageSub(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImageSub(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImageSub(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_dim(
	int dst_left,				/* The X coordinate of the screen */
	int dst_top,				/* The Y coordinate of the screen */
	int dst_width,				/* The width of the destination rectangle */
	int dst_height,				/* The height of the destination rectangle */
	struct image *src_image,	/* [IN] an image to be rendered */
	int src_left,				/* The X coordinate of a source image */
	int src_top,				/* The Y coordinate of a source image */
	int src_width,				/* The width of the source rectangle */
	int src_height,				/* The height of the source rectangle */
	int alpha)					/* The alpha value (0 to 255) */
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageDim(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImageDim(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImageDim(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImageDim(dst_left, dst_top, dst_width, dst_height, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void render_image_rule(struct image *src_image, struct image *rule_image, int threshold)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageRule(src_image, rule_image, threshold);
		break;
	case MODE_D3D11:
		D3D11RenderImageRule(src_image, rule_image, threshold);
		break;
	case MODE_D3D9:
		D3D9RenderImageRule(src_image, rule_image, threshold);
		break;
	case MODE_GDI:
		GDIRenderImageRule(src_image, rule_image, threshold);
		break;
	}
}

void render_image_melt(struct image *src_image, struct image *rule_image, int progress)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImageMelt(src_image, rule_image, progress);
		break;
	case MODE_D3D11:
		D3D11RenderImageMelt(src_image, rule_image, progress);
		break;
	case MODE_D3D9:
		D3D9RenderImageMelt(src_image, rule_image, progress);
		break;
	case MODE_GDI:
		GDIRenderImageMelt(src_image, rule_image, progress);
		break;
	}
}

void
render_image_3d_normal(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImage3DNormal(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImage3DNormal(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImage3DNormal(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImage3DNormal(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_3d_add(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImage3DAdd(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImage3DAdd(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImage3DAdd(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImage3DAdd(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_3d_sub(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	/* TODO */
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImage3DSub(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImage3DSub(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImage3DSub(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImage3DSub(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}

void
render_image_3d_dim(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	/* TODO */
	switch (nGraphicsMode)
	{
	case MODE_D3D12:
		D3D12RenderImage3DDim(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D11:
		D3D11RenderImage3DDim(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_D3D9:
		D3D9RenderImage3DDim(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	case MODE_GDI:
		GDIRenderImage3DDim(x1, y1, x2, y2, x3, y3, x4, y4, src_image, src_left, src_top, src_width, src_height, alpha);
		break;
	}
}
