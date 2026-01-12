/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * DirectShow Video Playback
 */

extern "C" {
#include "stratohal/platform.h"
#include "dsvideo.h"
#include <assert.h>
};

#include <dshow.h>

#define PATH_SIZE	(1024)

// DirectShow Interface
static IGraphBuilder *pBuilder;
static IMediaControl *pControl;
static IVideoWindow *pWindow;
static IMediaEventEx *pEvent;

static BOOL DisplayRenderFileErrorMessage(HRESULT hr);

//
// Play a video.
//
BOOL DShowPlayVideo(HWND hWnd, const char *pszFileName, int nOfsX, int nOfsY, int nWidth, int nHeight)
{
	HRESULT hRes;

	// Get an instance of IGraphBuilder.
	hRes = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
							IID_IGraphBuilder, (void**)&pBuilder);
	if(hRes != S_OK || !pBuilder)
	{
		log_error("CoCreateInstance() failed.");
		return FALSE;
	}

	// Generate a filter graph.
	WCHAR wFileName[PATH_SIZE];
	mbstowcs(wFileName, pszFileName, PATH_SIZE);
	HRESULT hr = pBuilder->RenderFile(wFileName, NULL);
	if(!DisplayRenderFileErrorMessage(hr))
		return FALSE;

	// Specify an owner window.
	pBuilder->QueryInterface(IID_IVideoWindow, (void **)&pWindow);
	if(pBuilder == NULL)
	{
		log_error("IGraphBuilder::QueryInterface() failed.");
		return FALSE;
	}
	pWindow->put_Owner((OAHWND)hWnd);
	pWindow->put_MessageDrain((OAHWND)hWnd);
	pWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	pWindow->SetWindowPosition(nOfsX, nOfsY, nWidth, nHeight);

	// Set hWnd to get events.
	pBuilder->QueryInterface(IID_IMediaEventEx, (void**)&pEvent);
	if(pEvent == NULL)
	{
		log_error("IGraphBuilder::QueryInterface() failed.");
		return FALSE;
	}
	pEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0);

	// Play a file.
	pBuilder->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if(pControl == NULL)
	{
		log_error("IGraphBuilder::QueryInterface() failed.");
		return FALSE;
	}
	hRes = pControl->Run();
	if (FAILED(hRes))
	{
		log_error("IMediaControl::Run() failed.");
		return FALSE;
	}

	return TRUE;
}

static BOOL DisplayRenderFileErrorMessage(HRESULT hr)
{
	switch(hr)
	{
	case S_OK:
	case VFW_S_DUPLICATE_NAME:
		break;
	case VFW_S_AUDIO_NOT_RENDERED:
	case VFW_S_PARTIAL_RENDER:
	case VFW_S_VIDEO_NOT_RENDERED:
		log_error("Unsupported codec.");
		return FALSE;
	case E_ABORT:
	case E_FAIL:
	case E_INVALIDARG:
	case E_OUTOFMEMORY:
	case E_POINTER:
	case VFW_E_CANNOT_CONNECT:
	case VFW_E_CANNOT_LOAD_SOURCE_FILTER:
	case VFW_E_CANNOT_RENDER:
		log_error("Video runtime error.");
		return FALSE;
	case VFW_E_INVALID_FILE_FORMAT:
		log_error("Invalid video file format.");
		return FALSE;
	case VFW_E_NOT_FOUND:
		log_error("File not found.");
		return FALSE;
	case VFW_E_UNKNOWN_FILE_TYPE:
		log_error("Unknown video file type.");
		return FALSE;
	case VFW_E_UNSUPPORTED_STREAM:
		log_error("Unsupported video stream.");
		return FALSE;
	default:
		log_error("Unknown video error.");
		return FALSE;
	}

	return TRUE;
}

//
// Stop a video playback.
//
VOID DShowStopVideo(void)
{
	if(pControl != NULL)
	{
		pControl->Stop();

		if(pBuilder)
		{
			pBuilder->Release();
			pBuilder = NULL;
		}
		if(pWindow)
		{
			pWindow->put_Visible(OAFALSE);
			pWindow->put_Owner((OAHWND)NULL);
			pWindow->Release();
			pWindow = NULL;
		}
		if(pControl)
		{
			pControl->Release();
			pControl = NULL;
		}
		if(pEvent)
		{
			pEvent->Release();
			pEvent = NULL;
		}
	}
}

//
// Process events (Called from WndProc)
//
BOOL DShowProcessEvent(void)
{
	assert(pEvent != NULL);

	// Get a playback complete event. (Don't process other events.)
	LONG code;
	LONG_PTR p, q;
	BOOL bContinue = TRUE;
	while(SUCCEEDED(pEvent->GetEvent(&code, &p, &q, 0)))
	{
		switch(code)
		{
		case EC_COMPLETE:
			// Playback complete event.
			bContinue = FALSE;
			pEvent->SetNotifyWindow((OAHWND)NULL, 0, 0);
			break;
		}

		pEvent->FreeEventParams(code, p, q);
	}

	// Release resources if a playback is completed.
	if(!bContinue)
	{
		if(pBuilder)
		{
			pBuilder->Release();
			pBuilder = NULL;
		}
		if(pWindow)
		{
			pWindow->put_Visible(OAFALSE);
			pWindow->put_Owner((OAHWND)NULL);
			pWindow->Release();
			pWindow = NULL;
		}
		if(pControl)
		{
			pControl->Release();
			pControl = NULL;
		}
		if(pEvent)
		{
			pEvent->Release();
			pEvent = NULL;
		}
	}

	// TRUE for continue, FALSE for exit.
	return bContinue;
}
