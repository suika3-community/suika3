/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * VM API
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2026 The Suika3 Community
 * Copyright (c) 2025-2026 The Playfield Engine Project
 * Copyright (c) 2025-2026 The NoctVM Project
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 2016-2024 The Suika2 Development Team
 * Copyright (c) 1996-2024 Keiichi Tabata
 *
 * This software is derived from the codebase of Playfield Engine, NoctLang,
 * Suika2, Suika Studio, Wind Game Lib, and 98/AT Game Lib.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <suika3/suika3.h>
#include <playfield/playfield.h>
#include <noct/noct.h>
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* API function entry. */
struct api_func {
	const char *name;
	bool (*func)(void *);
	int param_count;
	const char **params;
};

/* Skeleton */
static bool Suika_start(void *p);
static bool Suika_update(void *p);
static bool Suika_render(void *p);

/* Fundamental */
static bool Suika_loadPlugin(void *p);
static bool Suika_print(void *p);

/* Config */
static bool Suika_setConfig(void *p);
static bool Suika_getConfigCount(void *p);
static bool Suika_getConfigKey(void *p);
static bool Suika_isGlobalConfig(void *p);
static bool Suika_getConfigType(void *p);
static bool Suika_getStringConfig(void *p);
static bool Suika_getBoolConfig(void *p);
static bool Suika_getIntConfig(void *p);
static bool Suika_getFloatConfig(void *p);
static bool Suika_getConfigAsString(void *p);
static bool Suika_compareLocale(void *p);

/* Input */
static bool Suika_getMousePosX(void *p);
static bool Suika_getMousePosY(void *p);
static bool Suika_isMouseLeftPressed(void *p);
static bool Suika_isMouseRightPressed(void *p);
static bool Suika_isMouseLeftClicked(void *p);
static bool Suika_isMouseRightClicked(void *p);
static bool Suika_isMouseDragging(void *p);
static bool Suika_isReturnKeyPressed(void *p);
static bool Suika_isSpaceKeyPressed(void *p);
static bool Suika_isEscapeKeyPressed(void *p);
static bool Suika_isUpKeyPressed(void *p);
static bool Suika_isDownKeyPressed(void *p);
static bool Suika_isLeftKeyPressed(void *p);
static bool Suika_isRightKeyPressed(void *p);
static bool Suika_isPageUpKeyPressed(void *p);
static bool Suika_isPageDownKeyPressed(void *p);
static bool Suika_isControlKeyPressed(void *p);
static bool Suika_isSKeyPressed(void *p);
static bool Suika_isLKeyPressed(void *p);
static bool Suika_isHKeyPressed(void *p);
static bool Suika_isTouchCanceled(void *p);
static bool Suika_isSwiped(void *p);
static bool Suika_clearInputState(void *p);

/* Game */
static bool Suika_startCommandRepetition(void *p);
static bool Suika_stopCommandRepetition(void *p);
static bool Suika_isInCommandRepetition(void *p);
static bool Suika_setMessageActive(void *p);
static bool Suika_clearMessageActive(void *p);
static bool Suika_isMessageActive(void *p);
static bool Suika_startAutoMode(void *p);
static bool Suika_stopAutoMode(void *p);
static bool Suika_isAutoMode(void *p);
static bool Suika_startSkipMode(void *p);
static bool Suika_stopSkipMode(void *p);
static bool Suika_isSkipMode(void *p);
static bool Suika_setSaveLoad(void *p);
static bool Suika_isSaveLoadEnabled(void *p);
static bool Suika_setNonInterruptible(void *p);
static bool Suika_isNonInterruptible(void *p);
static bool Suika_setPenPosition(void *p);
static bool Suika_getPenPositionX(void *p);
static bool Suika_getPenPositionY(void *p);
static bool Suika_pushForCall(void *p);
static bool Suika_popForReturn(void *p);
static bool Suika_readCallStack(void *p);
static bool Suika_writeCallStack(void *p);
static bool Suika_setCallArgument(void *p);
static bool Suika_getCallArgument(void *p);
static bool Suika_isPageMode(void *p);
static bool Suika_appendBufferedMessage(void *p);
static bool Suika_getBufferedMessage(void *p);
static bool Suika_clearBufferedMessage(void *p);
static bool Suika_resetPageLine(void *p);
static bool Suika_incPageLine(void *p);
static bool Suika_isPageTop(void *p);
static bool Suika_registerBgvoice(void *p);
static bool Suika_getBgvoice(void *p);
static bool Suika_setBgvoicePlaying(void *p);
static bool Suika_isBgvoicePlaying(void *p);
static bool Suika_setChapterName(void *p);
static bool Suika_getChapterName(void *p);
static bool Suika_setLastMessage(void *p);
static bool Suika_setPrevLastMessage(void *p);
static bool Suika_getLastMessage(void *p);
static bool Suika_getPrevLastMessage(void *p);
static bool Suika_setTextSpeed(void *p);
static bool Suika_getTextSpeed(void *p);
static bool Suika_setAutoSpeed(void *p);
static bool Suika_getAutoSpeed(void *p);
static bool Suika_markLastEnglishTagIndex(void *p);
static bool Suika_getLastEnglishTagIndex(void *p);
static bool Suika_clearLastEnglishTagIndex(void *p);
static bool Suika_getLastTagName(void *p);

/* Image */
static bool Suika_createImageFromFile(void *p);
static bool Suika_createImage(void *p);
static bool Suika_getImageWidth(void *p);
static bool Suika_getImageHeight(void *p);
static bool Suika_loadGlyphImage(void *p);
static bool Suika_destroyImage(void *p);
static bool Suika_notifyImageUpdate(void *p);
static bool Suika_drawImageCopy(void *p);
static bool Suika_drawImageAlpha(void *p);
static bool Suika_drawImageAdd(void *p);
static bool Suika_drawImageSub(void *p);
static bool Suika_drawImageDim(void *p);
static bool Suika_drawImageGlyph(void *p);
static bool Suika_drawImageEmoji(void *p);
static bool Suika_drawImageScale(void *p);
static bool Suika_makePixel(void *p);
static bool Suika_fillImageRect(void *p);
static bool Suika_getImagePixels(void *p);

/* Stage */
static bool Suika_reloadStageImages(void *p);
static bool Suika_reloadStagePositions(void *p);
static bool Suika_getLayerX(void *p);
static bool Suika_getLayerY(void *p);
static bool Suika_setLayerPosition(void *p);
static bool Suika_getLayerScaleX(void *p);
static bool Suika_getLayerScaleY(void *p);
static bool Suika_setLayerScale(void *p);
static bool Suika_getLayerCenterX(void *p);
static bool Suika_getLayerCenterY(void *p);
static bool Suika_setLayerCenter(void *p);
static bool Suika_getLayerRotate(void *p);
static bool Suika_setLayerRotate(void *p);
static bool Suika_getLayerDim(void *p);
static bool Suika_setLayerDim(void *p);
static bool Suika_getLayerWidth(void *p);
static bool Suika_getLayerHeight(void *p);
static bool Suika_getLayerAlpha(void *p);
static bool Suika_setLayerAlpha(void *p);
static bool Suika_setLayerBlend(void *p);
static bool Suika_getLayerFileName(void *p);
static bool Suika_setLayerFileName(void *p);
static bool Suika_getLayerImage(void *p);
static bool Suika_setLayerImage(void *p);
static bool Suika_setLayerFrame(void *p);
static bool Suika_getLayerText(void *p);
static bool Suika_setLayerText(void *p);
static bool Suika_getSysbtnIdleImage(void *p);
static bool Suika_getSysbtnHoverImage(void *p);
static bool Suika_clearStageBasic(void *p);
static bool Suika_clearStage(void *p);
static bool Suika_chposToLayer(void *p);
static bool Suika_chposToEyeLayer(void *p);
static bool Suika_chposToLipLayer(void *p);
static bool Suika_layerToChpos(void *p);
static bool Suika_renderStage(void *p);
static bool Suika_drawStageToThumb(void *p);
static bool Suika_getThumbImage(void *p);
static bool Suika_getFadeMethod(void *p);
static bool Suika_getAccelMethod(void *p);
static bool Suika_startFade(void *p);
static bool Suika_setShakeOffset(void *p);
static bool Suika_isFadeRunning(void *p);
static bool Suika_finishFade(void *p);
static bool Suika_setChNameMapping(void *p);
static bool Suika_getChTalking(void *p);
static bool Suika_setChTalking(void *p);
static bool Suika_getTalkingChpos(void *p);
static bool Suika_updateChDimByTalkingCh(void *p);
static bool Suika_forceChDim(void *p);
static bool Suika_getChDim(void *p);
static bool Suika_fillNamebox(void *p);
static bool Suika_getNameboxRect(void *p);
static bool Suika_showNamebox(void *p);
static bool Suika_fillMsgbox(void *p);
static bool Suika_showMsgbox(void *p);
static bool Suika_getMsgboxRect(void *p);
static bool Suika_setClickPosition(void *p);
static bool Suika_showClick(void *p);
static bool Suika_setClickIndex(void *p);
static bool Suika_getClickRect(void *p);
static bool Suika_fillChooseboxIdleImage(void *p);
static bool Suika_fillChooseboxHoverImage(void *p);
static bool Suika_showChoosebox(void *p);
static bool Suika_getChooseboxRect(void *p);
static bool Suika_showAutomodeBanner(void *p);
static bool Suika_showSkipmodeBanner(void *p);
static bool Suika_renderImage(void *p);
static bool Suika_renderImage3d(void *p);

/* Mixer */
static bool Suika_setMixerInputFile(void *p);
static bool Suika_setMixerVolume(void *p);
static bool Suika_getMixerVolume(void *p);
static bool Suika_setMasterVolume(void *p);
static bool Suika_getMasterVolume(void *p);
static bool Suika_setMixerGlobalVolume(void *p);
static bool Suika_getMixerGlobalVolume(void *p);
static bool Suika_setCharacterVolume(void *p);
static bool Suika_getCharacterVolume(void *p);
static bool Suika_isMixerSoundFinished(void *p);
static bool Suika_getTrackFileName(void *p);
static bool Suika_applyCharacterVolume(void *p);

/* System */
static bool Suika_showSysbtn(void *p);
static bool Suika_isSysbtnVisible(void *p);
static bool Suika_updateSysbtnState(void *p);
static bool Suika_isSysbtnPointed(void *p);
static bool Suika_isSysbtnClicked(void *p);

/* Text */
static bool Suika_utf8ToUtf32(void *p);
static bool Suika_countUtf8Chars(void *p);
static bool Suika_getStringWidth(void *p);
static bool Suika_getStringHeight(void *p);
static bool Suika_drawGlyph(void *p);
static bool Suika_constructDrawMsgContext(void *p);
static bool Suika_countCharsCommon(void *p);
static bool Suika_drawMsgCommon(void *p);
static bool Suika_getPenPositionCommon(void *p);
static bool Suika_setIgnoreInlineWait(void *p);
static bool Suika_isQuotedSerif(void *p);
static bool Suika_isEscapeSequenceChar(void *p);

/* Tag */
static bool Suika_getTagCount(void *p);
static bool Suika_moveToTagFile(void *p);
static bool Suika_moveToTagIndex(void *p);
static bool Suika_moveToNextTag(void *p);
static bool Suika_moveToLabelTag(void *p);
static bool Suika_moveToMacroTag(void *p);
static bool Suika_moveToElseTag(void *p);
static bool Suika_moveToEndifTag(void *p);
static bool Suika_moveToEndmacroTag(void *p);
static bool Suika_getTagFile(void *p);
static bool Suika_getTagIndex(void *p);
static bool Suika_getTagLine(void *p);
static bool Suika_getTagName(void *p);
static bool Suika_getTagPropertyCount(void *p);
static bool Suika_getTagPropertyName(void *p);
static bool Suika_getTagPropertyValue(void *p);
static bool Suika_checkTagArg(void *p);
static bool Suika_getTagArgBool(void *p);
static bool Suika_getTagArgInt(void *p);
static bool Suika_getTagArgFloat(void *p);
static bool Suika_getTagArgString(void *p);
static bool Suika_evaluateTag(void *p);
static bool Suika_pushTagStackIf(void *p);
static bool Suika_popTagStackIf(void *p);
static bool Suika_pushTagStackWhile(void *p);
static bool Suika_popTagStackWhile(void *p);
static bool Suika_pushTagStackFor(void *p);
static bool Suika_popTagStackFor(void *p);

/* Anime */
static bool Suika_loadAnimeFromFile(void *p);
static bool Suika_clearLayerAnimeSequence(void *p);
static bool Suika_clearAllAnimeSequence(void *p);
static bool Suika_newAnimeSequence(void *p);
static bool Suika_addAnimeSequencePropertyF(void *p);
static bool Suika_addAnimeSequencePropertyI(void *p);
static bool Suika_startLayerAnime(void *p);
static bool Suika_isAnimeRunning(void *p);
static bool Suika_isAnimeRunningWithLayerMask(void *p);
static bool Suika_isAnimeFinishedForLayer(void *p);
static bool Suika_updateAnimeFrame(void *p);
static bool Suika_unregisterAnime(void *p);
static bool Suika_getRegAnimeName(void *p);
static bool Suika_getRegAnimeFileName(void *p);
static bool Suika_loadEyeImageIfExists(void *p);
static bool Suika_reloadEyeAnime(void *p);
static bool Suika_loadLipImageIfExists(void *p);
static bool Suika_runLipAnime(void *p);
static bool Suika_stopLipAnime(void *p);

/* Variable */
static bool Suika_setVariableInt(void *p);
static bool Suika_setVariableFloat(void *p);
static bool Suika_setVariableString(void *p);
static bool Suika_unsetVariable(void *p);
static bool Suika_makeVariableGlobal(void *p);
static bool Suika_getVariableInt(void *p);
static bool Suika_getVariableFloat(void *p);
static bool Suika_getVariableString(void *p);
static bool Suika_getVariableCount(void *p);
static bool Suika_getVariableName(void *p);
static bool Suika_checkVariableExists(void *p);
static bool Suika_isGlobalVariable(void *p);
static bool Suika_unsetLocalVariables(void *p);

/* Save */
static bool Suika_executeSaveGlobal(void *p);
static bool Suika_executeLoadGlobal(void *p);
static bool Suika_executeSaveLocal(void *p);
static bool Suika_executeLoadLocal(void *p);
static bool Suika_checkSaveExists(void *p);
static bool Suika_deleteLocalSave(void *p);
static bool Suika_deleteGlobalSave(void *p);
static bool Suika_checkRightAfterLoad(void *p);
static bool Suika_getSaveTimestamp(void *p);
static bool Suika_getLatestSaveIndex(void *p);
static bool Suika_getSaveChapterName(void *p);
static bool Suika_getSaveLastMessage(void *p);
static bool Suika_getSaveThumbnail(void *p);

/* History  */
static bool Suika_clearHistory(void *p);
static bool Suika_addHistory(void *p);
static bool Suika_getHistoryCount(void *p);
static bool Suika_getHistoryName(void *p);
static bool Suika_getHistoryMessage(void *p);
static bool Suika_getHistoryVoice(void *p);

/* Seen */
static bool Suika_loadSeen(void *p);
static bool Suika_saveSeen(void *p);
static bool Suika_getSeenFlags(void *p);
static bool Suika_setSeenFlags(void *p);

/* GUI */
static bool Suika_checkRightAfterSysGUI(void *p);
static bool Suika_loadGUIFile(void *p);
static bool Suika_startGUI(void *p);
static bool Suika_stopGUI(void *p);
static bool Suika_isGUIRunning(void *p);
static bool Suika_isGUIFinished(void *p);
static bool Suika_getGUIResultLabel(void *p);
static bool Suika_isGUIResultTitle(void *p);
static bool Suika_checkIfSavedInGUI(void *p);
static bool Suika_checkIfLoadedInGUI(void *p);

/* HAL */
static bool Suika_getMillisec(void *p);
static bool Suika_checkFileExists(void *p);
static bool Suika_read_file_content(void *p);
static bool Suika_writeSaveData(void *p);
static bool Suika_readSaveData(void *p);
static bool Suika_playVideo(void *p);
static bool Suika_stopVideo(void *p);
static bool Suika_isVideoPlaying(void *p);
static bool Suika_speakText(void *p);

/* API function table. */
static const char *single_param[] = {"p"};
static struct api_func api_func[] = {
	/* Skeleton API */
	{"start", Suika_start, 0, NULL},
	{"update", Suika_update, 0, NULL},
	{"render", Suika_render, 0, NULL},

	/* Fundamental API */
	{"print", Suika_print, 1, single_param},
	{"loadPlugin", Suika_loadPlugin, 1, single_param},

	/* Config */
	{"setConfig",			Suika_setConfig,		1, single_param},
	{"getConfigCount",		Suika_getConfigCount,		1, single_param},
	{"getConfigKey",		Suika_getConfigKey,		1, single_param},
	{"isGlobalConfig",		Suika_isGlobalConfig,		1, single_param},
	{"getConfigType",		Suika_getConfigType,		1, single_param},
	{"getBoolConfig",		Suika_getBoolConfig,		1, single_param},
	{"getIntConfig",		Suika_getIntConfig,		1, single_param},
	{"getFloatConfig",		Suika_getFloatConfig,		1, single_param},
	{"getStringConfig",		Suika_getStringConfig,		1, single_param},
	{"getConfigAsString",		Suika_getConfigAsString,	1, single_param},
	{"compareLocale",		Suika_compareLocale,		1, single_param},

	/* Input */
	{"getMousePosX",		Suika_getMousePosX,		0, NULL},
	{"getMousePosY",		Suika_getMousePosY,		0, NULL},
	{"isMouseLeftPressed",		Suika_isMouseLeftPressed,	0, NULL},
	{"isMouseRightPressed",		Suika_isMouseRightPressed,	0, NULL},
	{"isMouseLeftClicked",		Suika_isMouseLeftClicked,	0, NULL},
	{"isMouseRightClicked",		Suika_isMouseRightClicked,	0, NULL},
	{"isMouseDragging",		Suika_isMouseDragging,		0, NULL},
	{"isReturnKeyPressed",		Suika_isReturnKeyPressed,	0, NULL},
	{"isSpaceKeyPressed",		Suika_isSpaceKeyPressed,	0, NULL},
	{"isEscapeKeyPressed",		Suika_isEscapeKeyPressed,	0, NULL},
	{"isUpKeyPressed",		Suika_isUpKeyPressed,		0, NULL},
	{"isDownKeyPressed",		Suika_isDownKeyPressed,		0, NULL},
	{"isLeftKeyPressed",		Suika_isLeftKeyPressed,		0, NULL},
	{"isRightKeyPressed",		Suika_isRightKeyPressed,	0, NULL},
	{"isPageUpKeyPressed",		Suika_isPageUpKeyPressed,	0, NULL},
	{"isPageDownKeyPressed",	Suika_isPageDownKeyPressed,	0, NULL},
	{"isControlKeyPressed",		Suika_isControlKeyPressed,	0, NULL},
	{"isSKeyPressed",		Suika_isSKeyPressed,		0, NULL},
	{"isLKeyPressed",		Suika_isLKeyPressed,		0, NULL},
	{"isHKeyPressed",		Suika_isHKeyPressed,		0, NULL},
	{"isTouchCanceled",		Suika_isTouchCanceled,		0, NULL},
	{"isSwiped",			Suika_isSwiped,			0, NULL},
	{"clearInputState",		Suika_clearInputState,		0, NULL},

	/* Game */
	{"startCommandRepetition",	Suika_startCommandRepetition,	0, NULL},
	{"stopCommandRepetition",	Suika_stopCommandRepetition,	0, NULL},
	{"isInCommandRepetition",	Suika_isInCommandRepetition,	0, NULL},
	{"setMessageActive",		Suika_setMessageActive,		0, NULL},
	{"clearMessageActive",		Suika_clearMessageActive,	0, NULL},
	{"isMessageActive",		Suika_isMessageActive,		0, NULL},
	{"startAutoMode",		Suika_startAutoMode,		0, NULL},
	{"stopAutoMode",		Suika_stopAutoMode,		0, NULL},
	{"isAutoMode",			Suika_isAutoMode,		0, NULL},
	{"startSkipMode",		Suika_startSkipMode,		0, NULL},
	{"stopSkipMode",		Suika_stopSkipMode,		0, NULL},
	{"isSkipMode",			Suika_isSkipMode,		0, NULL},
	{"setSaveLoad",			Suika_setSaveLoad,		1, single_param},
	{"isSaveLoadEnabled",		Suika_isSaveLoadEnabled,	0, NULL},
	{"setNonInterruptible",		Suika_setNonInterruptible,	1, single_param},
	{"isNonInterruptible",		Suika_isNonInterruptible,	0, NULL},
	{"setPenPosition",		Suika_setPenPosition,		1, single_param},
	{"getPenPositionX",		Suika_getPenPositionX,		0, NULL},
	{"getPenPositionY",		Suika_getPenPositionY,		0, NULL},
	{"pushForCall",			Suika_pushForCall,		1, single_param},
	{"popForReturn",		Suika_popForReturn,		0, NULL},
	//{"readCallStack",		Suika_readCallStack,		1, single_param},
	//{"writeCallStack",		Suika_writeCallStack,		1, single_param},
	{"setCallArgument",		Suika_setCallArgument,		1, single_param},
	{"getCallArgument",		Suika_getCallArgument,		1, single_param},

	/* Image */
	{"createImageFromFile",		Suika_createImageFromFile,	1, single_param},
	{"createImage",			Suika_createImage,		1, single_param},
	{"getImageWidth",		Suika_getImageWidth,		1, single_param},
	{"getImageHeight",		Suika_getImageHeight,		1, single_param},
	{"destroyImage",		Suika_destroyImage,		1, single_param},
	{"drawImageCopy",		Suika_drawImageCopy,		1, single_param},
	{"drawImageAlpha",		Suika_drawImageAlpha,		1, single_param},
	{"drawImageAdd",		Suika_drawImageAdd,		1, single_param},
	{"drawImageSub",		Suika_drawImageSub,		1, single_param},
	{"makePixel",			Suika_makePixel,		1, single_param},
	{"fillImageRect",		Suika_fillImageRect,		1, single_param},
	{"getImagePixels",		Suika_getImagePixels,		1, single_param},

	/* Stage */
	{"reloadStageImages",		Suika_reloadStageImages,	0, NULL},
	{"reloadStagePositions",	Suika_reloadStagePositions,	0, NULL},
	{"getLayerX",			Suika_getLayerX,		1, single_param},
	{"getLayerY",			Suika_getLayerY,		1, single_param},
	{"setLayerPosition",		Suika_setLayerPosition,		1, single_param},
	{"getLayerScaleX",		Suika_getLayerScaleX,		1, single_param},
	{"getLayerScaleY",		Suika_getLayerScaleY,		1, single_param},
	{"setLayerScale",		Suika_setLayerScale,		1, single_param},
	{"getLayerRotate",		Suika_getLayerRotate,		1, single_param},
	{"setLayerRotate",		Suika_setLayerRotate,		1, single_param},
	{"getLayerDim",			Suika_getLayerDim,		1, single_param},
	{"setLayerDim",			Suika_setLayerDim,		1, single_param},
	{"getLayerAlpha",		Suika_getLayerAlpha,		1, single_param},
	{"setLayerAlpha",		Suika_setLayerAlpha,		1, single_param},
	{"setLayerBlend",		Suika_setLayerBlend,		1, single_param},
	{"setLayerFileName",		Suika_setLayerFileName,		1, single_param},
	{"setLayerFrame",		Suika_setLayerFrame,		1, single_param},
	{"getLayerText",		Suika_getLayerText,		1, single_param},
	{"setLayerText",		Suika_setLayerText,		1, single_param},
	{"renderStage",			Suika_renderStage,		0, NULL},
	{"startFade",			Suika_startFade,		1, single_param},
	{"isFadeRunning",		Suika_isFadeRunning,		0, NULL},
	{"finishFade",			Suika_finishFade,		0, NULL},
	{"showMsgbox",			Suika_showMsgbox,		1, single_param},
	{"showNamebox",			Suika_showNamebox,		1, single_param},
	{"showChoosebox",		Suika_showChoosebox,		1, single_param},

	/* Mixer */
	{"setMixerInputFile",		Suika_setMixerInputFile,	1, single_param},
	{"setMixerVolume",		Suika_setMixerVolume,		1, single_param},
	{"getMixerVolume",		Suika_getMixerVolume,		1, single_param},
	{"setMasterVolume",		Suika_setMasterVolume,		1, single_param},
	{"getMasterVolume",		Suika_getMasterVolume,		0, NULL},
	{"setMixerGlobalVolume",	Suika_setMixerGlobalVolume,	1, single_param},
	{"getMixerGlobalVolume",	Suika_getMixerGlobalVolume,	1, single_param},
	{"setCharacterVolume",		Suika_setCharacterVolume,	1, single_param},
	{"getCharacterVolume",		Suika_getCharacterVolume,	1, single_param},
	{"isMixerSoundFinished",	Suika_isMixerSoundFinished,	1, single_param},
	{"getTrackFileName",		Suika_getTrackFileName,		1, single_param},
	{"applyCharacterVolume",	Suika_applyCharacterVolume,	1, single_param},

	/* Text */
	{"getStringWidth",		Suika_getStringWidth,		1, single_param},
	{"getStringHeight",		Suika_getStringHeight,		1, single_param},
	{"drawGlyph",			Suika_drawGlyph,		1, single_param},
	{"constructDrawMsgContext",	Suika_constructDrawMsgContext,	1, single_param}, 
	{"drawMsgCommon",		Suika_drawMsgCommon,		1, single_param},
	{"getPenPositionCommon",	Suika_getPenPositionCommon,	1, single_param},
	{"isEscapeSequenceChar",	Suika_isEscapeSequenceChar,	1, single_param},

	/* Tag */
	{"getTagCount",			Suika_getTagCount,		0, NULL},
	{"moveToTagFile",		Suika_moveToTagFile,		1, single_param},
	{"moveToTagIndex",		Suika_moveToTagIndex,		1, single_param},
	{"moveToNextTag",		Suika_moveToNextTag,		0, NULL},
	{"moveToLabelTag",		Suika_moveToLabelTag,		1, single_param},
	{"moveToMacroTag",		Suika_moveToMacroTag,		1, single_param},
	{"getTagFile",			Suika_getTagFile,		0, NULL},
	{"getTagName",			Suika_getTagName,		0, NULL},
	{"getTagPropertyCount",		Suika_getTagPropertyCount,	0, NULL},
	{"getTagPropertyName",		Suika_getTagPropertyName,	1, single_param},
	{"getTagPropertyValue",		Suika_getTagPropertyValue,	1, single_param},
	{"getTagArgBool",		Suika_getTagArgBool,		1, single_param},
	{"getTagArgInt",		Suika_getTagArgInt,		1, single_param},
	{"getTagArgFloat",		Suika_getTagArgFloat,		1, single_param},
	{"getTagArgString",		Suika_getTagArgString,		1, single_param},
	{"evaluateTag",			Suika_evaluateTag,		0, NULL},
	{"pushTagStackIf",		Suika_pushTagStackIf,		0, NULL},
	{"popTagStackIf",		Suika_popTagStackIf,		0, NULL},
	{"pushTagStackWhile",		Suika_pushTagStackWhile,	0, NULL},
	{"pushTagStackFor",		Suika_pushTagStackFor,		0, NULL},

	/* Anime */
	{"loadAnimeFromFile",		Suika_loadAnimeFromFile,		1, single_param},
	{"clearLayerAnimeSequence",	Suika_clearLayerAnimeSequence,		1, single_param},
	{"clearAllAnimeSequence",	Suika_clearAllAnimeSequence,		0, NULL},
	{"newAnimeSequence",		Suika_newAnimeSequence,			1, single_param},
	{"addAnimeSequencePropertyF",	Suika_addAnimeSequencePropertyF,	1, single_param},
	{"addAnimeSequencePropertyI",	Suika_addAnimeSequencePropertyI,	1, single_param},
	{"startLayerAnime",		Suika_startLayerAnime,			1, single_param},
	{"isAnimeRunning",		Suika_isAnimeRunning,			0, NULL},
	{"isAnimeFinishedForLayer",	Suika_isAnimeFinishedForLayer,		1, single_param},
	{"updateAnimeFrame",		Suika_updateAnimeFrame,			0, NULL},
	{"loadEyeImageIfExists",	Suika_loadEyeImageIfExists,		1, single_param},
	{"reloadEyeAnime",		Suika_reloadEyeAnime,			1, single_param},
	{"runLipAnime",			Suika_runLipAnime,			1, single_param},
	{"stopLipAnime",		Suika_stopLipAnime,			1, single_param},

	/* Variable */
	{"setVariableInt",		Suika_setVariableInt,		1, single_param},
	{"setVariableFloat",		Suika_setVariableFloat,		1, single_param},
	{"setVariableString",		Suika_setVariableString,	1, single_param},
	{"getVariableInt",		Suika_getVariableInt,		1, single_param},
	{"getVariableFloat",		Suika_getVariableFloat,		1, single_param},
	{"getVariableString",		Suika_getVariableString,	1, single_param},
	{"getVariableCount",		Suika_getVariableCount,		0, NULL},
	{"getVariableName",		Suika_getVariableName,		1, single_param},
	{"checkVariableExists",		Suika_checkVariableExists,	1, single_param},
	{"makeVariableGlobal",		Suika_makeVariableGlobal,	1, single_param},
	{"unsetLocalVariables",		Suika_unsetLocalVariables,	0, NULL},

	/* Save */
	{"executeSaveGlobal",		Suika_executeSaveGlobal,	0, NULL},
	{"executeLoadGlobal",		Suika_executeLoadGlobal,	0, NULL},
	{"executeSaveLocal",		Suika_executeSaveLocal,		1, single_param},
	{"executeLoadLocal",		Suika_executeLoadLocal,		1, single_param},
	{"checkSaveExists",		Suika_checkSaveExists,		1, single_param},
	{"deleteLocalSave",		Suika_deleteLocalSave,		1, single_param},
	{"deleteGlobalSave",		Suika_deleteGlobalSave,		0, NULL},
	{"checkRightAfterLoad",		Suika_checkRightAfterLoad,	0, NULL},
	{"getSaveTimestamp",		Suika_getSaveTimestamp,		1, single_param},
	{"getLatestSaveIndex",		Suika_getLatestSaveIndex,	0, NULL},
	{"getSaveChapterName",		Suika_getSaveChapterName,	1, single_param},
	{"getSaveLastMessage",		Suika_getSaveLastMessage,	1, single_param},
	{"getSaveThumbnail",		Suika_getSaveThumbnail,		1, single_param},

	/* History */
	{"clearHistory",		Suika_clearHistory,		0, NULL},
	{"addHistory",			Suika_addHistory,		1, single_param},
	{"getHistoryCount",		Suika_getHistoryCount,		0, NULL},
	{"getHistoryName",		Suika_getHistoryName,		1, single_param},
	{"getHistoryMessage",		Suika_getHistoryMessage,	1, single_param},
	{"getHistoryVoice",		Suika_getHistoryVoice,		1, single_param},
	{"loadSeen",			Suika_loadSeen,			0, NULL},
	{"saveSeen",			Suika_saveSeen,			0, NULL},
	{"getSeenFlags",		Suika_getSeenFlags,		0, NULL},
	{"setSeenFlags",		Suika_setSeenFlags,		1, single_param},

	/* GUI */
	{"checkRightAfterSysGUI",	Suika_checkRightAfterSysGUI,	0, NULL},
	{"loadGUIiFile",		Suika_loadGUIFile,		1, single_param},
	{"startGUI",			Suika_startGUI,			0, NULL},
	{"stopGUI",			Suika_stopGUI,			0, NULL},
	{"isGUIRunning",		Suika_isGUIRunning,		0, NULL},
	{"isGUIFinished",		Suika_isGUIFinished,		0, NULL},
	{"getGUIResultLabel",		Suika_getGUIResultLabel,	0, NULL},
	{"isGUIResultTitle",		Suika_isGUIResultTitle,		0, NULL},
	{"checkIfSavedInGUI",		Suika_checkIfSavedInGUI,	0, NULL},
	{"checkIfLoadedInGUI",		Suika_checkIfLoadedInGUI,	0, NULL},

	/* HAL */
	{"getMillisec",			Suika_getMillisec,		0, NULL},
	{"checkFileExists",		Suika_checkFileExists,		1, single_param},
	{"readFileContent",		Suika_read_file_content,	1, single_param},
	{"writeSaveData",		Suika_writeSaveData,		1, single_param},
	{"readSaveData",		Suika_readSaveData,		1, single_param},
	{"playVideo",			Suika_playVideo,		1, single_param},
	{"stopVideo",			Suika_stopVideo,		0, NULL},
	{"isVideoPlaying",		Suika_isVideoPlaying,		0, NULL},
	{"speakText",			Suika_speakText,		1, single_param},
};

/* Time origin. */
static uint64_t time_origin;

/* Forward declaration. */
static bool serialize_printer(NoctEnv *env, char *buf, size_t size, NoctValue *value, bool is_inside_obj);

/*
 * Install the API.
 */
bool
s3i_install_default_api(void)
{
	int i;

	/* Register functions. */
	for (i = 0; i < sizeof(api_func) / sizeof(struct api_func); i++) {
		if (!s3_install_api(api_func[i].name,
				    api_func[i].func,
				    api_func[i].param_count,
				    api_func[i].params))
			return false;
	}

	/* Get the time origin. */
	s3_reset_lap_timer(&time_origin);

	return true;
}

/*
 * Skeleton
 */

static bool
Suika_start(
	void *p)
{
	if (!s3i_on_game_start())
		return false;

	return true;
}

static bool
Suika_update(
	void *p)
{
	if (!s3i_on_game_update())
		return false;

	return true;
}

static bool
Suika_render(
	void *p)
{
	if (!s3i_on_game_render())
		return false;

	return true;
}

/*
 * Fundamental
 */

static bool
Suika_print(
	void *p)
{
	NoctEnv *env;
	char buf[8192];
	NoctValue value;

	env = pf_get_vm_env();
	if (!noct_get_arg(env, 0, &value))
		return false;

	memset(buf, 0, sizeof(buf));
	serialize_printer(env, buf, sizeof(buf), &value, false);

	s3_log_info("%s", buf);

	return true;
}

static bool
serialize_printer(NoctEnv *env, char *buf, size_t size, NoctValue *value, bool is_inside_obj)
{
	int type;
	int ival;
	float fval;
	const char *sval;
	uint32_t items;
	int i;
	char digits[1024];

	if (!noct_get_value_type(env, value, &type))
		return false;

	switch (type) {
	case NOCT_VALUE_INT:
		if (!noct_get_int(env, value, &ival))
			return false;
		snprintf(digits, sizeof(digits), "%d", ival);
		strncat(buf, digits, size);
		break;
	case NOCT_VALUE_FLOAT:
		if (!noct_get_float(env, value, &fval))
			return false;
		snprintf(digits, sizeof(digits), "%f", fval);
		strncat(buf, digits, size);
		break;
	case NOCT_VALUE_STRING:
		if (!noct_get_string(env, value, &sval))
			return false;
		if (is_inside_obj)
			strncat(buf, "\"", size);
		strncat(buf, sval, size);
		if (is_inside_obj)
			strncat(buf, "\"", size);
		break;
	case NOCT_VALUE_ARRAY:
		if (!noct_get_array_size(env, value, &items))
			return false;
		strncat(buf, "[", size);
		for (i = 0; i < items; i++) {
			NoctValue elem;
			if (!noct_get_array_elem(env, value, i, &elem))
				return false;
			if (!serialize_printer(env, buf, size, &elem, true))
				return false;
			if (i != items - 1)
				strncat(buf, ", ", size);
		}
		strncat(buf, "]", size);
		break;
	case NOCT_VALUE_DICT:
		if (!noct_get_dict_size(env, value, &items))
			return false;
		strncat(buf, "{", size);
		for (i = 0; i < items; i++) {
			NoctValue k, v;
			if (!noct_get_dict_key_by_index(env, value, i, &k))
				return false;
			if (!noct_get_string(env, &k, &sval))
				return false;
			strncat(buf, sval, size);
			strncat(buf, ": ", size);
			if (!noct_get_dict_value_by_index(env, value, i, &v))
				return false;
			serialize_printer(env, buf, size, &v, true);
			if (i != items - 1)
				strncat(buf, ", ", size);
		}
		strncat(buf, "}", size);
		break;
	case NOCT_VALUE_FUNC:
		strncat(buf, "<func>", size);
		break;
	default:
		assert(0);
		break;
	}

	return true;
}

static bool
Suika_loadPlugin(
	void *p)
{
	NoctEnv *env;
	NoctValue name_val;
	const char *name;
	char path[256];
	char func_name[256];
	char *data;
	size_t len;

	/* Get the argument. */
	env = pf_get_vm_env();
	if (!noct_get_arg_check_string(env, 0, &name_val, &name))
		return false;

	/* Load a source file content. */
	snprintf(path, sizeof(path), "plugins/%s/%s.pf", name, name);
	if (!s3_read_file_content(path, &data, &len))
		return false;

	/* Check for the bytecode header. */
	if (strncmp(data, NOCT_BYTECODE_HEADER, strlen(NOCT_BYTECODE_HEADER)) != 0) {
		/* It's a source file. */
		if (!noct_register_source(env, path, data)) {
			free(data);
			return false;
		}
	} else {
		/* It's a bytecode file. */
		if (!noct_register_bytecode(env, (void *)data, len)) {
			free(data);
			return false;
		}
	}
	free(data);

	/* Call the initializer. */
	snprintf(func_name, sizeof(func_name), "plugin_init_%s", name);
	if (!pf_call_vm_function(func_name))
		return false;
		
	return true;
}

/*
 * Config
 */

static bool
Suika_setConfig(
	void *p)
{
	char *key;
	char *value;
	bool ret;

	key = NULL;
	value = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;
		if (!pf_get_call_arg_string("value", &value))
			break;

		/* Set the config value. */
		if (!s3_set_config(key, value))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);
	if (value != NULL)
		free(value);

        return ret;
}

static bool
Suika_getConfigCount(
	void *p)
{
	bool ret;
	int count;

	ret = false;
	do {
		/* Get the config count. */
		count = s3_get_config_count();

		/* Set the return value. */
		if (!pf_set_return_int(count))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_getConfigKey(
	void *p)
{
	int index;
	const char *key;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("index", &index))
			break;

		/* Get the config key. */
		key = s3_get_config_key(index);
		assert(key != NULL);
		if (key == NULL)
			break;

		/* Set the return value. */
		if (!pf_set_return_string(key))
			break;

		ret = true;
	} while (0);

        return ret;


        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isGlobalConfig(
	void *p)
{
	char *key;
	bool is_global;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Check if the config is global. */
		is_global = s3_is_global_config(key);
			break;

		/* Set the return value. */
		if (!pf_set_return_int(is_global ? 1 : 0))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getConfigType(
	void *p)
{
	char *key;
	char type;
	const char *type_s;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config type. */
		type = s3_get_config_type(key);
		switch (type) {
		case 's':
			type_s = "s";
			break;
		case 'b':
			type_s = "b";
			break;
		case 'i':
			type_s = "i";
			break;
		case 'f':
			type_s = "f";
			break;
		default:
			assert(NEVER_COME_HERE);
			break;
		}

		/* Set the return value. */
		if (!pf_set_return_string(type_s))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getStringConfig(
	void *p)
{
	char *key;
	const char *val;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config value. */
		val = s3_get_string_config(key);

		/* Set the return value. */
		if (!pf_set_return_string(val))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getBoolConfig(
	void *p)
{
	char *key;
	bool val;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config value. */
		val = s3_get_bool_config(key);

		/* Set the return value. */
		if (!pf_set_return_int(val ? 1 : 0))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getIntConfig(
	void *p)
{
	char *key;
	int val;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config value. */
		val = s3_get_int_config(key);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getFloatConfig(
	void *p)
{
	char *key;
	float val;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config value. */
		val = s3_get_float_config(key);

		/* Set the return value. */
		if (!pf_set_return_float(val))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_getConfigAsString(
	void *p)
{
	char *key;
	const char *val;
	bool ret;

	key = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("key", &key))
			break;

		/* Get the config value. */
		val = s3_get_config_as_string(key);

		/* Set the return value. */
		if (!pf_set_return_string(val))
			break;

		ret = true;
	} while (0);

	if (key != NULL)
		free(key);

        return ret;
}

static bool
Suika_compareLocale(
	void *p)
{
	char *locale;
	bool val;
	bool ret;

	locale = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("locale", &locale))
			break;

		/* Get the config value. */
		val = s3_compare_locale(locale);

		/* Set the return value. */
		if (!pf_set_return_int(val ? 1 : 0))
			break;

		ret = true;
	} while (0);

	if (locale != NULL)
		free(locale);

        return ret;
}

/*
 * Input
 */

static bool
Suika_getMousePosX(void *p)
{
	int val;

	/* Get the value. */
	val = s3_get_mouse_pos_x();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;
	
        return true;
}

static bool
Suika_getMousePosY(void *p)
{
	int val;

	/* Get the value. */
	val = s3_get_mouse_pos_y();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;
	
        return true;
}

static bool
Suika_isMouseLeftPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_mouse_left_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isMouseRightPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_mouse_right_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isMouseLeftClicked(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_mouse_left_clicked();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isMouseRightClicked(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_mouse_right_clicked();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isMouseDragging(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_mouse_dragging();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isReturnKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_return_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isSpaceKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_space_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isEscapeKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_escape_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isUpKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_up_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isDownKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_down_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isLeftKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_left_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isRightKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_right_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isPageUpKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_pageup_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isPageDownKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_pagedown_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isControlKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_control_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isSKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_s_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isLKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_l_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isHKeyPressed(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_h_key_pressed();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isTouchCanceled(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_touch_canceled();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_isSwiped(void *p)
{
	bool val;

	/* Get the value. */
	val = s3_is_swiped();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 0 : 1))
		return false;
	
        return true;
}

static bool
Suika_clearInputState(void *p)
{
	/* Clear the input state. */
	s3_clear_input_state();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

/*
 * Game
 */

static bool
Suika_startCommandRepetition(void *p)
{
	s3_start_command_repetition();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_stopCommandRepetition(void *p)
{
	s3_stop_command_repetition();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_isInCommandRepetition(void *p)
{
	bool val;

	val = s3_is_in_command_repetition();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_setMessageActive(void *p)
{
	s3_set_message_active();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_clearMessageActive(void *p)
{
	s3_clear_message_active();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_isMessageActive(void *p)
{
	bool val;

	val = s3_is_message_active();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_startAutoMode(void *p)
{
	s3_start_auto_mode();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_stopAutoMode(void *p)
{
	s3_stop_auto_mode();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_isAutoMode(void *p)
{
	bool val;

	val = s3_is_auto_mode();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_startSkipMode(void *p)
{
	s3_start_skip_mode();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_stopSkipMode(void *p)
{
	s3_stop_skip_mode();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;
	
        return true;
}

static bool
Suika_isSkipMode(void *p)
{
	bool val;

	val = s3_is_skip_mode();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_setSaveLoad(void *p)
{
	int enable;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("enable", &enable))
			break;

		s3_set_save_load(enable ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_isSaveLoadEnabled(void *p)
{
	bool val;

	val = s3_is_save_load_enabled();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_setNonInterruptible(void *p)
{
	int enable;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("enable", &enable))
			break;

		s3_set_non_interruptible(enable ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_isNonInterruptible(void *p)
{
	bool val;

	val = s3_is_non_interruptible();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;
	
        return true;
}

static bool
Suika_setPenPosition(void *p)
{
	int x;
	int y;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("x", &x))
			break;
		if (!pf_get_call_arg_int("y", &y))
			break;

		s3_set_pen_position(x, y);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_getPenPositionX(void *p)
{
	int val;

	val = s3_get_pen_position_x();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_getPenPositionY(void *p)
{
	int val;

	val = s3_get_pen_position_y();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_pushForCall(void *p)
{
	char *file;
	int index;
	bool ret;

	file = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_string("file", &file))
			break;
		if (!pf_get_call_arg_int("index", &index))
			break;

		if (!s3_push_for_call(file, index))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (file != NULL)
		free(file);

        return ret;
}

static bool
Suika_popForReturn(void *p)
{
	NoctEnv *env;
	int sp;
	char *file;
	int index;
	NoctValue dic, tmp;
	bool ret;

	env = p;

	file = NULL;
	ret = false;
	do {
		if (!s3_pop_for_return(&file, &index))
			return false;

		/* Set the return value. */
		if (!noct_make_empty_dict(env, &dic))
			break;
		if (!noct_set_dict_elem_make_string(env, &dic, "file", &tmp, file))
			break;
		if (!noct_set_dict_elem_make_int(env, &dic, "index", &tmp, index))
			break;
		if (!noct_set_return(env, &dic))
			break;

		ret = true;
	} while (0);

	if (file != NULL)
		free(file);

	return ret;
}

static bool
Suika_readCallStack(void *p)
{
	NoctEnv *env;
	int sp;
	char *file;
	int index;
	NoctValue dic, tmp;
	bool ret;

	env = p;

	file = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("sp", &sp))
			break;

		if (!s3_read_call_stack(sp, &file, &index))
			break;

		/* Set the return value. */
		if (!noct_make_empty_dict(env, &dic))
			break;
		if (!noct_set_dict_elem_make_string(env, &dic, "file", &tmp, file))
			break;
		if (!noct_set_dict_elem_make_int(env, &dic, "index", &tmp, index))
			break;
		if (!noct_set_return(env, &dic))
			break;

		ret = true;
	} while (0);

	if (file != NULL)
		free(file);

        return ret;
}

static bool
Suika_writeCallStack(void *p)
{
	int sp;
	char *file;
	int index;
	bool ret;

	file = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("sp", &sp))
			break;
		if (!pf_get_call_arg_string("file", &file))
			break;
		if (!pf_get_call_arg_int("index", &index))
			break;

		if (!s3_write_call_stack(sp, file, index))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (file != NULL)
		free(file);

        return ret;
}

static bool
Suika_setCallArgument(void *p)
{
	int index;
	char *value;
	bool ret;

	value = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("index", &index))
			break;
		if (!pf_get_call_arg_string("value", &value))
			break;

		if (!s3_set_call_argument(index, value))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (value != NULL)
		free(value);

        return ret;
}

static bool
Suika_getCallArgument(void *p)
{
	int index;
	const char *value;
	bool ret;

	value = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("index", &index))
			break;

		value = s3_get_call_argument(index);
		assert(value != NULL);
		if (value == NULL)
			break;

		/* Set the return value. */
		if (!pf_set_return_string(value))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_isPageMode(void *p)
{
	bool val;

	val = s3_is_page_mode();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;

	return true;
}

static bool
Suika_appendBufferedMessage(void *p)
{
	char *message;
	bool ret;

	message = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_string("message", &message))
			break;

		if (!s3_append_buffered_message(message))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (message != NULL)
		free(message);

        return ret;
}

static bool
Suika_getBufferedMessage(void *p)
{
	const char *val;

	val = s3_get_buffered_message();
	if (val == NULL)
		val = "";

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

static bool
Suika_clearBufferedMessage(void *p)
{
	s3_clear_buffered_message();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_resetPageLine(void *p)
{
	s3_reset_page_line();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_incPageLine(void *p)
{
	s3_inc_page_line();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_isPageTop(void *p)
{
	bool val;

	val = s3_is_page_top();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;

	return true;
}

static bool
Suika_registerBGVoice(void *p)
{
	char *file;
	bool ret;

	file = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_string("file", &file))
			break;

		if (!s3_register_bgvoice(file))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (file != NULL)
		free(file);

        return ret;
}

static bool
Suika_getBGVoice(void *p)
{
	const char *val;

	val = s3_get_bgvoice();

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

static bool
Suika_setBGVoicePlaying(void *p)
{
	int is_playing;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("isPlaying", &is_playing))
			break;

		s3_set_bgvoice_playing(is_playing ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

        return ret;
}

static bool
Suika_isBgvoicePlaying(void *p)
{
	bool val;

	val = s3_is_bgvoice_playing();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;

	return true;
}

static bool
Suika_setChapterName(void *p)
{
	char *name;
	bool ret;

	name = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("name", &name))
			break;

		if (!s3_set_chapter_name(name))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (name != NULL)
		free(name);

        return ret;
}

static bool
Suika_getChapterName(void *p)
{
	const char *val;

	val = s3_get_chapter_name();

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

static bool
Suika_setLastMessage(void *p)
{
	char *message;
	int is_append;
	bool ret;

	message = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("message", &message))
			break;
		if (!pf_get_call_arg_int("isAppend", &is_append))
			break;

		if (!s3_set_last_message(message, is_append ? true : false))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (message != NULL)
		free(message);

        return ret;
}

static bool
Suika_setPrevLastMessage(void *p)
{
	char *message;
	int is_append;
	bool ret;

	message = NULL;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_string("message", &message))
			break;
		if (!pf_get_call_arg_int("isAppend", &is_append))
			break;

		if (!s3_set_prev_last_message(message, is_append ? true : false))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (message != NULL)
		free(message);

		return ret;
}

static bool
Suika_getLastMessage(void *p)
{
	const char *val;

	val = s3_get_last_message();

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

static bool
Suika_getPrevLastMessage(void *p)
{
	const char *val;

	val = s3_get_prev_last_message();

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

static bool
Suika_setTextSpeed(void *p)
{
	float speed;
	bool ret;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_float("speed", &speed))
			break;

		s3_set_text_speed(speed);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getTextSpeed(void *p)
{
	float val;

	val = s3_get_text_speed();

	/* Set the return value. */
	if (!pf_set_return_float(val))
		return false;

	return true;
}

static bool
Suika_setAutoSpeed(void *p)
{
	float speed;
	bool ret;
	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_float("speed", &speed))
			break;

		s3_set_auto_speed(speed);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getAutoSpeed(void *p)
{
	float val;

	val = s3_get_auto_speed();

	/* Set the return value. */
	if (!pf_set_return_float(val))
		return false;

	return true;
}

static bool
Suika_markLastEnglishTagIndex(void *p)
{
	s3_mark_last_english_tag_index();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_getLastEnglishTagIndex(void *p)
{
	int val;

	val = s3_get_last_english_tag_index();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_clearLastEnglishTagIndex(void *p)
{
	s3_clear_last_english_tag_index();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_getLastTagName(void *p)
{
	const char *val;

	val = s3_get_last_tag_name();

	/* Set the return value. */
	if (!pf_set_return_string(val))
		return false;

	return true;
}

/*
 * Image
 */

static bool
Suika_createImageFromFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_createImage(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getImageWidth(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getImageHeight(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_loadGlyphImage(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_destroyImage(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_notifyImageUpdate(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageCopy(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageAlpha(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageAdd(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageSub(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageDim(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageGlyph(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageEmoji(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawImageScale(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_makePixel(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_fillImageRect(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getImagePixels(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Stage
 */

static bool
Suika_reloadStageImages(void *p)
{
	s3_reload_stage_images();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_reloadStagePositions(void *p)
{
	s3_reload_stage_positions();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_getLayerX(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_x(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerY(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_y(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerPosition(void *p)
{
	int layer;
	int x;
	int y;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("x", &x))
			break;
		if (!pf_get_call_arg_int("y", &y))
			break;

		s3_set_layer_position(layer, x, y);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerScaleX(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_scale_x(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerScaleY(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_scale_y(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerScale(void *p)
{
	int layer;
	int scale_x;
	int scale_y;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("scaleX", &scale_x))
			break;
		if (!pf_get_call_arg_int("scaleY", &scale_y))
			break;

		s3_set_layer_scale(layer, scale_x, scale_y);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerCenterX(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_center_x(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerCenterY(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_center_y(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerCenter(void *p)
{
	int layer;
	int center_x;
	int center_y;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("centerX", &center_x))
			break;
		if (!pf_get_call_arg_int("centerY", &center_y))
			break;

		s3_set_layer_center(layer, center_x, center_y);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerRotate(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_rotate(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerRotate(void *p)
{
	int layer;
	int rotate;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("rotate", &rotate))
			break;

		s3_set_layer_rotate(layer, rotate);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerDim(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_dim(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerDim(void *p)
{
	int layer;
	int dim;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("dim", &dim))
			break;

		s3_set_layer_dim(layer, dim);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerWidth(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_width(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerHeight(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_height(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerAlpha(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_alpha(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerAlpha(void *p)
{
	int layer;
	int alpha;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("alpha", &alpha))
			break;

		s3_set_layer_alpha(layer, alpha);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setLayerBlend(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getLayerFileName(void *p)
{
	int layer;
	const char *val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
	
		val = s3_get_layer_file_name(layer);
		if (val == NULL)
			val = "";

		/* Set the return value. */
		if (!pf_set_return_string(val))
			break;

		ret = true;
	} while (0);

	return ret;	
}

static bool
Suika_setLayerFileName(void *p)
{
	int layer;
	char *file;
	bool ret;

	file_name = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_string("file", &file))
			break;

		if (!s3_set_layer_file_name(layer, file))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (file_name != NULL)
		free(file_name);

	return ret;	
}

static bool
Suika_getLayerImage(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
	
		val = s3_image_to_int(s3_get_layer_image(layer));

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;	
}

static bool
Suika_setLayerImage(void *p)
{
	int layer;
	int image;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("image", &image))
			break;

		if (!s3_set_layer_image(layer, s3_int_to_image(image)))
			break;

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;	
}

static bool
Suika_setLayerFrame(void *p)
{
	int layer;
	int frame;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_int("frame", &frame))
			break;

		s3_set_layer_frame(layer, frame);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getLayerText(void *p)
{
	int layer;
	const char *val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_get_layer_text(layer);
		if (val == NULL)
			val = "";

		/* Set the return value. */
		if (!pf_set_return_string(val))
			break;

		ret = true;
	} while (0);

	return ret;	
}

static bool
Suika_setLayerText(void *p)
{
	int layer;
	char *text;
	bool ret;

	text = NULL;
	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;
		if (!pf_get_call_arg_string("text", &text))
			break;

		s3_set_layer_text(layer, text);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	if (text != NULL)
		free(text);

	return ret;
}

static bool
Suika_getSysbtnIdleImage(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_image_to_int(s3_get_sysbtn_idle_image(layer));

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getSysbtnHoverImage(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_image_to_int(s3_get_sysbtn_hover_image(layer));

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_clearStageBasic(void *p)
{
	s3_clear_stage_basic();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_clearStage(void *p)
{
	s3_clear_stage();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_chposToLayer(void *p)
{
	int chpos;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;

		val = s3_chpos_to_layer(chpos);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_chposToEyeLayer(void *p)
{
		int chpos;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;

		val = s3_chpos_to_eye_layer(chpos);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_chposToLipLayer(void *p)
{
	int chpos;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;

		val = s3_chpos_to_lip_layer(chpos);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_layerToChpos(void *p)
{
	int layer;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		val = s3_layer_to_chpos(layer);

		/* Set the return value. */
		if (!pf_set_return_int(val))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_renderStage(void *p)
{
	int layer;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("layer", &layer))
			break;

		s3_render_stage(layer);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}	

static bool
Suika_drawStageToThumb(void *p)
{
	s3_draw_stage_to_thumb();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_getThumbImage(void *p)
{
	int val;

	val = s3_image_to_int(s3_get_thumb_image());

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_getFadeMethod(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getAccelMethod(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_startFade(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setShakeOffset(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isFadeRunning(void *p)
{
	int val;

	val = s3_is_fade_running();

	/* Set the return value. */
	if (!pf_set_return_int(val ? 1 : 0))
		return false;

	return true;
}

static bool
Suika_finishFade(void *p)
{
	s3_finish_fade();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_setChNameMapping(void *p)
{
	int chpos;
	int chNameIndex;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;
		if (!pf_get_call_arg_int("chNameIndex", &chNameIndex))
			break;

		s3_set_ch_name_mapping(chpos, chNameIndex);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getChTalking(void *p)
{
	int val;

	val = s3_get_ch_talking();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_setChTalking(void *p)
{
	int chpos;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;

		s3_set_ch_talking(chpos);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getTalkingChpos(void *p)
{
	int val;

	val = s3_get_talking_chpos();

	/* Set the return value. */
	if (!pf_set_return_int(val))
		return false;

	return true;
}

static bool
Suika_updateChDimByTalkingCh(void *p)
{
	s3_update_ch_dim_by_talking_ch();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_forceChDim(void *p)
{
	int chpos;
	int dim;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;
		if (!pf_get_call_arg_int("dim", &dim))
			break;

		s3_force_ch_dim(chpos, dim ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getChDim(void *p)
{
	int chpos;
	int val;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("chpos", &chpos))
			break;

		val = s3_get_ch_dim(chpos);

		/* Set the return value. */
		if (!pf_set_return_int(val ? 1 : 0))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_fillNamebox(void *p)
{
	s3_fill_namebox();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_getNameboxRect(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_showNamebox(void *p)
{
        s3_show_namebox();

        /* Set the return value. */
        if (!pf_set_return_int(1))
                return false;

        return true;
}

static bool
Suika_fillMsgbox(void *p)
{
        s3_fill_msgbox();

        /* Set the return value. */
        if (!pf_set_return_int(1))
                return false;

        return true;
}

static bool
Suika_showMsgbox(void *p)
{
        s3_show_msgbox();

        /* Set the return value. */
        if (!pf_set_return_int(1))
                return false;

        return true;
}

static bool
Suika_getMsgboxRect(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setClickPosition(void *p)
{
	int x;
	int y;
	bool ret;

	ret = false;
	do {
		/* Get the arguments. */
		if (!pf_get_call_arg_int("x", &x))
			break;
		if (!pf_get_call_arg_int("y", &y))
			break;

		s3_set_click_position(x, y);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_showClick(void *p)
{
	int show;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("show", &show))
			break;

		s3_show_click(show ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
				return false;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_setClickIndex(void *p)
{
	int index;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("index", &index))
			break;

		s3_set_click_index(index);

		/* Set the return value. */
		if (!pf_set_return_int(1))
				return false;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getClickRect(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_fillChooseboxIdleImage(void *p)
{
	s3_fill_choosebox_idle_image();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_fillChooseboxHoverImage(void *p)
{
	s3_fill_choosebox_hover_image();

	/* Set the return value. */
	if (!pf_set_return_int(1))
		return false;

	return true;
}

static bool
Suika_showChoosebox(void *p)
{
	int show;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("show", &show))
			break;

		s3_show_choosebox(show ? true : false);

		/* Set the return value. */
		if (!pf_set_return_int(1))
				return false;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_getChooseboxRect(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_showAutomodeBanner(void *p)
{
	int show;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("show", &show))
			break;

		s3_show_automode_banner(show ? true : false);
	
		/* Set the return value. */
		if (!pf_set_return_int(1))
			return false;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_showSkipmodeBanner(void *p)
{
	int show;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("show", &show))
			break;

		s3_show_skipmode_banner(show ? true : false);
	
		/* Set the return value. */
		if (!pf_set_return_int(1))
			return false;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_renderImage(void *p)
{
	int dstLeft;
	int dstTop;
	int image;
	int srcLeft;
	int srcTop;
	int srcWidth;
	int srcHeight;
	int alpha;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("dstLeft", &dstLeft))
			break;
		if (!pf_get_call_arg_int("dstTop", &dstTop))
			break;
		if (!pf_get_call_arg_int("image", &image))
			break;
		if (!pf_get_call_arg_int("srcLeft", &srcLeft))
			break;
		if (!pf_get_call_arg_int("srcTop", &srcTop))
			break;
		if (!pf_get_call_arg_int("srcWidth", &srcWidth))
			break;
		if (!pf_get_call_arg_int("srcHeight", &srcHeight))
			break;
		if (!pf_get_call_arg_int("alpha", &alpha))
			break;

		s3_render_image(
			dstLeft,
			dstTop,
			s3_int_to_image(image),
			srcLeft,
			srcTop,
			srcWidth,
			srcHeight,
			alpha);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

static bool
Suika_renderImage3d(void *p)
{
	int x1, y1, x2, y2, x3, y3, x4, y4;
	int srcLeft, srcTop, srcWidth, srcHeight;
	int image;
	int alpha;
	bool ret;

	ret = false;
	do {
		/* Get the argument. */
		if (!pf_get_call_arg_int("x1", &x1))
			break;
		if (!pf_get_call_arg_int("y1", &y1))
			break;
		if (!pf_get_call_arg_int("x2", &x2))
			break;
		if (!pf_get_call_arg_int("y2", &y2))
			break;
		if (!pf_get_call_arg_int("x3", &x3))
			break;
		if (!pf_get_call_arg_int("y3", &y3))
			break;
		if (!pf_get_call_arg_int("x4", &x4))
			break;
		if (!pf_get_call_arg_int("y4", &y4))
			break;
		if (!pf_get_call_arg_int("image", &image))
			break;
		if (!pf_get_call_arg_int("srcLeft", &srcLeft))
			break;
		if (!pf_get_call_arg_int("srcTop", &srcTop))
			break;
		if (!pf_get_call_arg_int("srcWidth", &srcWidth))
			break;
		if (!pf_get_call_arg_int("srcHeight", &srcHeight))
			break;
		if (!pf_get_call_arg_int("alpha", &alpha))
			break;

		s3_render_image_3d(
			x1, y1, x2, y2, x3, y3, x4, y4,
			s3_int_to_image(image),
			srcLeft,
			srcTop,
			srcWidth,
			srcHeight,
			alpha);

		/* Set the return value. */
		if (!pf_set_return_int(1))
			break;

		ret = true;
	} while (0);

	return ret;
}

/*
 * Sound
 */

/*
 * System Button Subsystem
 */

static bool
Suika_showSysbtn(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isSysbtnVisible(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_updateSysbtnState(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isSysbtnPointed(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isSysbtnClicked(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Text Subsystem
 */

static bool
Suika_utf8ToUtf32(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_countUtf8Chars(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getGlyphWidth(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getGlyphHeight(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getStringWidth(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getStringHeight(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawGlyph(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_constructDrawMsgContext(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_countCharsCommon(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_drawMsgCommon(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getPenPositionCommon(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setIgnoreInlineWait(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isQuotedSerif(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isEscapeSequenceChar(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Mixer Subsystem
 */

static bool
Suika_setMixerInputFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setMixerVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getMixerVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setMasterVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getMasterVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setMixerGlobalVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getMixerGlobalVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setCharacterVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getCharacterVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isMixerSoundFinished(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTrackFileName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_applyCharacterVolume(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Tag
 */

static bool
Suika_getTagCount(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToTagFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToTagIndex(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToNextTag(
	void *p)
{
	if (!s3_move_to_next_tag())
		return false;
	return true;
}

static bool
Suika_moveToLabelTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToMacroTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToElseTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToEndifTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_moveToEndmacroTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagIndex(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagLine(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagPropertyCount(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagPropertyName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagPropertyValue(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkTagArg(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagArgBool(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagArgInt(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagArgFloat(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getTagArgString(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_evaluateTag(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_pushTagStackIf(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_popTagStackIf(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_pushTagStackWhile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_popTagStackWhile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_pushTagStackFor(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_popTagStackFor(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Anime
 */

static bool
Suika_loadAnimeFromFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_clearLayerAnimeSequence(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_clearAllAnimeSequence(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_newAnimeSequence(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_addAnimeSequencePropertyF(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_addAnimeSequencePropertyI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_startLayerAnime(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isAnimeRunning(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isAnimeRunningWithLayerMask(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isAnimeFinishedForLayer(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_updateAnimeFrame(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_unregisterAnime(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getRegAnimeName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getRegAnimeFileName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_loadEyeImageIfExists(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_reloadEyeAnime(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_loadLipImageIfExists(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_runLipAnime(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_stopLipAnime(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Variable
 */

static bool
Suika_setVariableInt(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setVariableFloat(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setVariableString(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_unsetVariable(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_makeVariableGlobal(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getVariableInt(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getVariableFloat(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getVariableString(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getVariableCount(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getVariableName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkVariableExists(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isGlobalVariable(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_unsetLocalVariables(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Save
 */

static bool
Suika_executeSaveGlobal(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_executeLoadGlobal(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_executeSaveLocal(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_executeLoadLocal(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkSaveExists(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_deleteLocalSave(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_deleteGlobalSave(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkRightAfterLoad(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getSaveTimestamp(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getLatestSaveIndex(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getSaveChapterName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getSaveLastMessage(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getSaveThumbnail(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * History
 */

static bool
Suika_clearHistory(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_addHistory(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getHistoryCount(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getHistoryName(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getHistoryMessage(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getHistoryVoice(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * Seen
 */

static bool
Suika_loadSeen(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_saveSeen(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getSeenFlags(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_setSeenFlags(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * GUI
 */

static bool
Suika_checkRightAfterSysGUI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_loadGUIFile(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_startGUI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_stopGUI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isGUIRunning(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isGUIFinished(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_getGUIResultLabel(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isGUIResultTitle(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkIfSavedInGUI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_checkIfLoadedInGUI(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

/*
 * HAL
 */

static bool
Suika_getMillisec(void *p)
{
	NoctEnv *env;
	NoctValue val;
	uint64_t lap;

	env = pf_get_vm_env();
	lap = pf_get_lap_timer_millisec(&time_origin);
	if (!noct_set_return_make_int(env, &val, (uint32_t)lap))
		return false;

	return false;
}

static bool
Suika_checkFileExists(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_read_file_content(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_writeSaveData(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_readSaveData(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_playVideo(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_stopVideo(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_isVideoPlaying(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}

static bool
Suika_speakText(void *p)
{
        s3_log_error(S3_TR("This API is not implemented yet."));
        return false;
}
