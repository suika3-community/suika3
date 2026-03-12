Suika3 APIリファレンス
====================

## インデックス

* 基本 (Fundamental)
    * [Suika.print()](#suikaprint)
    * [Suika.loadPlugin()](#suikaloadplugin)
* 設定 (Config)
    * [Suika.setConfig()](#suikasetconfig)
    * [Suika.getConfigCount()](#suikagetconfigcount)
    * [Suika.getConfigKey()](#suikagetconfigkey)
    * [Suika.isGlobalConfig()](#suikaisglobalconfig)
    * [Suika.getConfigType()](#suikagetconfigtype)
    * [Suika.getStringConfig()](#suikagetstringconfig)
    * [Suika.getBoolConfig()](#suikagetboolconfig)
    * [Suika.getIntConfig()](#suikagetintconfig)
    * [Suika.getFloatConfig()](#suikagetfloatconfig)
    * [Suika.getConfigAsString()](#suikagetconfigasstring)
    * [Suika.compareLocale()](#suikacomparelocale)
* 入力 (Input)
    * [Suika.getMousePosX()](#suikagetmouseposx)
    * [Suika.getMousePosY()](#suikagetmouseposy)
    * [Suika.isMouseLeftPressed()](#suikaismouseleftpressed)
    * [Suika.isMouseRightPressed()](#suikaismouserightpressed)
    * [Suika.isMouseLeftClicked()](#suikaismouseleftclicked)
    * [Suika.isMouseRightClicked()](#suikaismouserightclicked)
    * [Suika.isMouseDragging()](#suikaismousedragging)
    * [Suika.isReturnKeyPressed()](#suikaisreturnkeypressed)
    * [Suika.isSpaceKeyPressed()](#suikaisspacekeypressed)
    * [Suika.isEscapeKeyPressed()](#suikaisescapekeypressed)
    * [Suika.isUpKeyPressed()](#suikaisupkeypressed)
    * [Suika.isDownKeyPressed()](#suikaisdownkeypressed)
    * [Suika.isLeftKeyPressed()](#suikaisleftkeypressed)
    * [Suika.isRightKeyPressed()](#suikaisrightkeypressed)
    * [Suika.isPageUpKeyPressed()](#suikaispageupkeypressed)
    * [Suika.isPageDownKeyPressed()](#suikaispagedownkeypressed)
    * [Suika.isControlKeyPressed()](#suikaiscontrolkeypressed)
    * [Suika.isSKeyPressed()](#suikaisskeypressed)
    * [Suika.isLKeyPressed()](#suikaislkeypressed)
    * [Suika.isHKeyPressed()](#suikaishkeypressed)
    * [Suika.isTouchCanceled()](#suikaistouchcanceled)
    * [Suika.isSwiped()](#suikaisswiped)
    * [Suika.clearInputState()](#suikaclearinputstate)
* ゲーム (Game)
    * [Suika.startCommandRepetition()](#suikastartcommandrepetition)
    * [Suika.stopCommandRepetition()](#suikastopcommandrepetition)
    * [Suika.isInCommandRepetition()](#suikaisincommandrepetition)
    * [Suika.setMessageActive()](#suikasetmessageactive)
    * [Suika.clearMessageActive()](#suikaclearmessageactive)
    * [Suika.isMessageActive()](#suikaismessageactive)
    * [Suika.startAutoMode()](#suikastartautomode)
    * [Suika.stopAutoMode()](#suikastopautomode)
    * [Suika.isAutoMode()](#suikaisautomode)
    * [Suika.startSkipMode()](#suikastartskipmode)
    * [Suika.stopSkipMode()](#suikastopskipmode)
    * [Suika.isSkipMode()](#suikaisskipmode)
    * [Suika.setSaveLoad()](#suikasetsaveload)
    * [Suika.isSaveLoadEnabled()](#suikaissaveloadenabled)
    * [Suika.setNonInterruptible()](#suikasetnoninterruptible)
    * [Suika.isNonInterruptible()](#suikaisnoninterruptible)
    * [Suika.setPenPosition()](#suikasetpenposition)
    * [Suika.getPenPositionX()](#suikagetpenpositionx)
    * [Suika.getPenPositionY()](#suikagetpenpositiony)
    * [Suika.pushForCall()](#suikapushforcall)
    * [Suika.popForReturn()](#suikapopforreturn)
    * [Suika.readCallStack()](#suikareadcallstack)
    * [Suika.writeCallStack()](#suikawritecallstack)
    * [Suika.setCallArgument()](#suikasetcallargument)
    * [Suika.getCallArgument()](#suikagetcallargument)
    * [Suika.isPageMode()](#suikaispagemode)
    * [Suika.appendBufferedMessage()](#suikaappendbufferedmessage)
    * [Suika.getBufferedMessage()](#suikagetbufferedmessage)
    * [Suika.clearBufferedMessage()](#suikaclearbufferedmessage)
    * [Suika.resetPageLine()](#suikaresetpageline)
    * [Suika.incPageLine()](#suikaincpageline)
    * [Suika.isPageTop()](#suikaispagetop)
    * [Suika.registerBGVoice()](#registerbgvoice)
    * [Suika.getBVoice()](#suikagetbgvoice)
    * [Suika.setBGVoicePlaying()](#suikasetbgvoiceplaying]
    * [Suika.isBGVoicePlaying()](#suikaisbgvoiceplaying)
    * [Suika.setChapterName()](#suikasetchaptername)
    * [Suika.getChapterName()](#suikagetchaptername)
    * [Suika.setLastMessage()](#suikasetlastmessage)
    * [Suika.setPrevLastMessage()](#suikasetprevlastmessage)
    * [Suika.getLastMessage()](#suikagetlastmessage)
    * [Suika.getPrevLastMessage()](#suikagetprevlastmessage)
    * [Suika.setTextSpeed()](#suikasettextspeed)
    * [Suika.getTextSpeed()](#suikagettextspeed)
    * [Suika.setAutoSpeed()](#suikasetautospeed)
    * [Suika.getAutoSpeed()](#suikagetautospeed)
    * [Suika.markLastEnglishTagIndex()](#suikamarklastenglishtagindex)
    * [Suika.getLastEnglishTagIndex()](#suikagetlastenglishtagindex)
    * [Suika.clearLastEnglishTagIndex()](#suikaclearlastenglishtagindex)
    * [Suika.getLastTagName()](#suikagetlasttagname)

* 画像 (Image)
    * [Suika.createImageFromFile()](#suikacreateimagefromfile)
    * [Suika.createImage()](#suikacreateimage)
    * [Suika.getImageWidth()](#suikagetimagewidth)
    * [Suika.getImageHeight()](#suikagetimageheight)
    * [Suika.destroyImage()](#suikadestroyimage)
    * [Suika.drawImageCopy()](#suikadrawimagecopy)
    * [Suika.drawImageAlpha()](#suikadrawimagealpha)
    * [Suika.drawImageAdd()](#suikadrawimageadd)
    * [Suika.drawImageSub()](#suikadrawimagesub)
    * [Suika.makeColor()](#suikamakecolor)
    * [Suika.fillImageRect()](#suikafillimagerect)
* ステージ (Stage)
    * [Suika.reloadStageImages()](#suikareloadstageimages)
    * [Suika.reloadStagePositions()](#suikareloadstagepositions)
    * [Suika.getLayerX()](#suikagetlayerx)
    * [Suika.getLayerY()](#suikagetlayery)
    * [Suika.setLayerPosition()](#suikasetlayerposition)
    * [Suika.getLayerScaleX()](#suikagetlayerscalex)
    * [Suika.getLayerScaleY()](#suikagetlayerscaley)
    * [Suika.setLayerScale()](#suikasetlayerscale)
    * [Suika.getLayerRotate()](#suikagetlayerrotate)
    * [Suika.setLayerRotate()](#suikasetlayerrotate)
    * [Suika.getLayerDim()](#suikagetlayerdim)
    * [Suika.setLayerDim()](#suikasetlayerdim)
    * [Suika.getLayerAlpha()](#suikagetlayeralpha)
    * [Suika.setLayerAlpha()](#suikasetlayeralpha)
    * [Suika.setLayerBlend()](#suikasetlayerblend)
    * [Suika.setLayerFileName()](#suikasetlayerfilename)
    * [Suika.setLayerFrame()](#suikasetlayerframe)
    * [Suika.getLayerText()](#suikagetlayertext)
    * [Suika.setLayerText()](#suikasetlayertext)
    * [Suika.getSysBtnIdleImage()](#suikagetsysbtnidleimage)
    * [Suika.getSysBtnHoverImage()](#suikagetsysbtnhoverimage)
    * [Suika.clearStageBasic()](#suikaclearstagebasic)
    * [Suika.clearStage()](#suikaclearstage)
    * [Suika.chposToLayer()](#suikachpostolayer)
    * [Suika.chposToEyeLayer()](#suikachpostoeyelayer)
    * [Suika.chposToLipLayer()](#suikachpostoliplayer)
    * [Suika.layerToChpos()](#suikalayertochpos)
    * [Suika.renderStage()](#suikarenderstage)
    * [Suika.drawStageToThumb()](#suikadrawstagetothumb)
    * [Suika.getThumbImage()](#suikagetthumbimage())
    * [Suika.getFadeMethod()](#suikagetfademethod)
    * [Suika.getAccelMethod()](#suikagetaccelmethod)
    * [Suika.startFade()](#suikastartfade)
    * [Suika.getShakeOffset()](#suikagetshakeoffset)
    * [Suika.setShakeOffset()](#suikasetshakeoffset)
    * [Suika.isFadeRunning()](#suikaisfaderunning)
    * [Suika.finishFade()](#suikafinishfade)
    * [Suika.setChNameMapping()](#suikasetchnamemapping)
    * [Suika.getTalkingChpos()](#suikagettalkingchpos)
    * [Suika.setChTalking()](#suikasetchtalking)
    * [Suika.getTalkingChpos()](#suikagettalkingchpos)
    * [Suika.updateChDimByTalkingCh()](#suikaupdatechdimbytalkingch)
    * [Suika.forceChDim()](#suikaforcechdim)
    * [Suika.getChDim()](#suikagetchdim)
    * [Suika.updateChDimByTalkingCh()](#suikaupdatechdimbytalkingch)
    * [Suika.fillMessageBox()](#suikafillmessagebox)
    * [Suika.showMessageBox()](#suikashowmessagebox)
    * [Suika.getMessageBoxRect()](#suikagetmessageboxrect]
    * [Suika.fillNameBox()](#suikafillnamebox)
    * [Suika.showNameBox()](#suikashownamebox)
    * [Suika.getNameBoxRect()](#suikagetnameboxrect)
    * [Suika.showChoosebox()](#suikashowchoosebox)
    * [Suika.showAutoModeBanner()](#suikashowautomodebanner)
    * [Suika.showSkipModeBanner()](#suikashowskipmodebanner)
    * [Suika.renderImage()](#suikarenderimage)
    * [Suika.renderImage3d()](#suikarenderimage3d)
    * [Suika.setClickPosition()](#suikasetclickposition)
    * [Suika.showClick()](#suikashowclick)
    * [Suika.setClickIndex()](#suikasetclickindex]
    * [Suika.getClickRect()](#suikagetclickrect)
    * [Suika.fillChooseBoxIdleImage()](#suikafillchooseboxidleimage)
    * [Suika.fillChooseBoxHoverImage()](#suikafillchooseboxhoverimage)
    * [Suika.getChooseBoxRect()](#suikagetchooseboxrect)
    * [Suika.startKirakira()](#suikastartkirakira)
    * [Suika.renderKirakira()](#suikarenderkirakira)
* ミキサー (Mixer)
    * [Suika.setMixerInputFile()](#suikasetmixerinputfile)
    * [Suika.setMixerVolume()](#suikasetmixervolume)
    * [Suika.getMixerVolume()](#suikagetmixervolume)
    * [Suika.setMasterVolume()](#suikasetmastervolume)
    * [Suika.getMasterVolume()](#suikagetmastervolume)
    * [Suika.setMixerGlobalVolume()](#suikasetmixerglobalvolume)
    * [Suika.getMixerGlobalVolume()](#suikagetmixerglobalvolume)
    * [Suika.setCharacterVolume()](#suikasetcharactervolume)
    * [Suika.getCharacterVolume()](#suikagetcharactervolume)
    * [Suika.isMixerSoundFinished()](#suikaismixersoundfinished)
    * [Suika.getTrackFileName()](#suikagettrackfilename)
    * [Suika.applyCharacterVolume()](#suikaapplycharactervolume)
* テキスト (Text)
    * [Suika.getStringWidth()](#suikagetstringwidth)
    * [Suika.getStringHeight()](#suikagetstringheight)
    * [Suika.drawGlyph()](#suikadrawglyph)
    * [Suika.createDrawMsg()](#suikacreatedrawmsg)
    * [Suika.destroyDrawMsg()](#suikadestroydrawmsg)
    * [Suika.countDrawMsgChars()](#suikacountdrawmsgchars)
    * [Suika.drawMessage()](#suikadrawmsgcommon)
    * [Suika.getDrawMsgPenPosition()](#suikagetpenpositioncommon)
    * [Suika.isEscapeSequenceChar()](#suikaisescapesequencechar)
* タグ (Tag)
    * [Suika.getTagCount()](#suikagettagcount)
    * [Suika.moveToTagFile()](#suikamovetotagfile)
    * [Suika.moveToTagIndex()](#suikamovetotagindex)
    * [Suika.moveToNextTag()](#suikamovetonexttag)
    * [Suika.moveToLabelTag()](#suikamovetolabeltag)
    * [Suika.moveToMacroTag()](#suikamovetomacrotag)
    * [Suika.moveToElseTag()](#suikamovetoelsetag)
    * [Suika.moveToEndIfTag()](#suikamovetoendiftag)
    * [Suika.moveToEndMacroTag()](#suikamovetoendmacrotag)
    * [Suika.getTagFileName()](#suikagettagfilename)
    * [Suika.getTagName()](#suikagettagname)
    * [Suika.getTagPropertyCount()](#suikagettagpropertycount)
    * [Suika.getTagPropertyName()](#suikagettagpropertyname)
    * [Suika.getTagPropertyValue()](#suikagettagpropertyvalue)
    * [Suika.getTagArgBool()](#suikagettagargbool)
    * [Suika.getTagArgInt()](#suikagettagargint)
    * [Suika.getTagArgFloat()](#suikagettagargfloat)
    * [Suika.getTagArgString()](#suikagettagargstring)
    * [Suika.evaluateTag()](#suikaevaluatetag)
    * [Suika.pushTagStackIf()](#suikapushtagstackif)
    * [Suika.popTagStackIf()](#suikapoptagstackif)
    * [Suika.pushTagStackWhile()](#suikapushtagstackwhile)
    * [Suika.pushTagStackFor()](#suikapushtagstackfor)
* アニメーション (Anime)
    * [Suika.loadAnimeFromFile()](#suikaloadanimefromfile)
    * [Suika.newAnimeSequence()](#suikanewanimesequence)
    * [Suika.addAnimeSequencePropertyF()](#suikaaddanimesequencepropertyf)
    * [Suika.addAnimeSequencePropertyI()](#suikaaddanimesequencepropertyi)
    * [Suika.startLayerAnime()](#suikastartlayeranime)
    * [Suika.isAnimeRunning()](#suikaisanimerunning)
    * [Suika.isAnimeFinishedForLayer()](#suikaisanimefinishedforlayer)
    * [Suika.updateAnimeFrame()](#suikaupdateanimeframe)
    * [Suika.loadEyeImageIfExists()](#suikaloadeyeimageifexists)
    * [Suika.reloadEyeAnime()](#suikareloadeyeanime)
    * [Suika.runLipAnime()](#suikarunlipanime)
    * [Suika.stopLipAnime()](#suikastoplipanime)
    * [Suika.clearLayerAnimeSequence()](#suikaclearlayeranimesequence)
    * [Suika.clearAllAnimeSequence()](#suikaclearallanimesequnce)
* 変数 (Variable)
    * [Suika.setVariableInt()](#suikasetvariableint)
    * [Suika.setVariableFloat()](#suikasetvariablefloat)
    * [Suika.setVariableString()](#suikasetvariablestring)
    * [Suika.getVariableInt()](#suikagetvariableint)
    * [Suika.getVariableFloat()](#suikagetvariablefloat)
    * [Suika.getVariableString()](#suikagetvariablestring)
    * [Suika.unsetVariable()](#suikaunsetvariable)
    * [Suika.unsetLocalVariables()](#suikaunsetlocalvariables)
    * [Suika.makeVariableGlobal()](#suikamakevariableglobal)
    * [Suika.isGlobalVariable()](#suikaisglobalvariable)
    * [Suika.getVariableCount()](#suikagetvariablecount)
    * [Suika.getVariableName()](#suikagetvariablename)
    * [Suika.checkVariableExists()](#suikacheckvariableexists)
    * [Suika.expandStringWithVariable()](#suikaexpandstringwithvariable)
* セーブ (Save)
    * [Suika.executeSaveGlobal()](#suikaexecutesaveglobal)
    * [Suika.executeLoadGlobal()](#suikaexecuteloadglobal)
    * [Suika.executeSaveLocal()](#suikaexecutesavelocal)
    * [Suika.executeLoadLocal()](#suikaexecuteloadlocal)
    * [Suika.checkSaveExists()](#suikachecksaveexists)
    * [Suika.deleteLocalSave()](#suikadeletelocalsave)
    * [Suika.deleteGlobalSave()](#suikadeleteglobalsave)
    * [Suika.checkRightAfterLoad()](#suikacheckrightafterload)
    * [Suika.getSaveTimestamp()](#suikagetsavetimestamp)
    * [Suika.getLatestSaveIndex()](#suikagetlatestsaveindex)
    * [Suika.getSaveChapterName()](#suikagetsavechaptername)
    * [Suika.getSaveLastMessage()](#suikagetsavelastmessage)
    * [Suika.getSaveThumbnail()](#suikagetsavethumbnail)
* 履歴 (History)
    * [Suika.clearHistory()](#suikaclearhistory)
    * [Suika.addHistory()](#suikaaddhistory)
    * [Suika.getHistoryCount()](#suikagethistorycount)
    * [Suika.getHistoryName()](#suikagethistoryname)
    * [Suika.getHistoryMessage()](#suikagethistorymessage)
    * [Suika.getHistoryVoice()](#suikagethistoryvoice)
* 既読 (Seen)
    * [Suika.loadSeen()](#suikaloadseen)
    * [Suika.saveSeen()](#suikasaveseen)
    * [Suika.getSeenFlags()](#suikagetseenflags)
    * [Suika.setSeenFlags()](#suikasetseenflags)
* GUI
    * [Suika.loadGUIFile()](#suikaloadguifile)
    * [Suika.startGUI()](#suikastartgui)
    * [Suika.stopGUI()](#suikastopgui)
    * [Suika.isGUIRunning()](#suikaisguirunning)
    * [Suika.isGUIFinished()](#suikaisguifinished)
    * [Suika.getGUIResultLabel()](#suikagetguiresultlabel)
    * [Suika.isGUIResultTitle()](#suikaisguiresulttitle)
    * [Suika.checkIfSavedInGUI()](#suikacheckifsavedingui)
    * [Suika.checkIfLoadedInGui()](#suikacheckifloadedingui)
    * [Suika.checkRightAfterSysGUI()](#suikacheckrightaftersysgui)
* HAL
    * [Suika.getMillisec()](#suikagetmillisec)
    * [Suika.checkFileExists()](#suikacheckfileexists)
    * [Suika.readFileContent()](#suikareadfilecontent)
    * [Suika.writeSaveData()](#suikawritesavedata)
    * [Suika.readSaveData()](#suikareadsavedata)
    * [Suika.installAPI()](#suikainstallapi)
    * [Suika.installTag()](#suikainstalltag)
    * [Suika.getVmInt()](#suikagetvmint)
    * [Suika.setVmInt()](#suikasetvmint)
    * [Suika.callVmFunction()](#suikacallvmfunction)
    * [Suika.playVideo()](#suikaplayvideo)
    * [Suika.stopVideo()](#suikastopvideo)
    * [Suika.isVideoPlaying()](#suikaisvideoplaying)
    * [Suika.isFullScreenSupported()](#suikaisfullscreensupported)
    * [Suika.enterFullScreenMode()](#suikaenterfullscreenmode)
    * [Suika.logInfo()](#suikaloginfo)
    * [Suika.logWarn()](#suikalogwarn)
    * [Suika.logError()](#suikalogerror)
    * [Suika.speakText()](#suikaspeaktext)

---

## Suika.print()

デバッグメッセージを出力します。

### 引数 (直接)

| 引数 | 型   | 説明     |
|-----------|--------|-----------------|
| msg       | Any    | 出力する値。 |

### 戻り値

なし。

---

## Suika.loadPlugin()

プラグインをロードします。

### 引数 (直接)

| 引数 | 型   | 説明     |
|-----------|--------|-----------------|
| name      | String | プラグイン名。    |

### 戻り値

なし。

---

## Suika.setConfig()

設定（Config）をセットします。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | 設定のキー。                         |
| value     | String | 設定の値。                       |

### 戻り値

なし。

---

## Suika.getConfigCount()

設定キーの個数を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

設定キーの個数を表す整数。

---

## Suika.getConfigKey()

インデックスに対応する設定キーを取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型    | 説明                                |
|-----------|---------|--------------------------------------------|
| index     | Integer | 設定のインデックス。                         |

### 戻り値

指定されたインデックスの設定キーを表す文字列。

---

## Suika.isConfigGlobal()

設定キーがグローバルセーブデータに保存されるかどうかをチェックします。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

設定がグローバルであるかどうかを表す真偽値。

---

## Suika.getConfigType()

設定値の型を取得します。("s", "b", "i", "f")

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

以下のいずれかの文字列。

| 値      | 意味                  |
|------------|--------------------------|
| "s"        | 文字列型。        |
| "b"        | 真偽値型。       |
| "i"        | 整数型。       |
| "f"        | 浮動小数点型。         |

---

## Suika.getStringConfig()

文字列型の設定値を取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

設定の文字列値。

---

## Suika.getBoolConfig()

真偽値型の設定値を取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

設定の真偽値。

---

## Suika.getIntConfig()

整数型の設定値を取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

設定の整数値。

---

## Suika.getFloatConfig()

浮動小数点型の設定値を取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

設定の浮動小数点値。

---

## Suika.getConfigAsString()

設定値を文字列として取得します。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| key       | String | キー名。                                  |

### 戻り値

文字列化された設定値。

---

## Suika.compareLocale()

指定されたロケールが現在のロケールと同じかどうかをチェックします。

### 引数 (ディクショナリ形式)

| 引数 | 型   | 説明                                |
|-----------|--------|--------------------------------------------|
| locale    | String | ロケール名。                               |


### 戻り値

指定されたロケールが現在のものと一致するかどうかを表す真偽値。

---

## Suika.getMousePosX()

マウスのX座標を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

現在のマウスのX座標を表す整数。

---

## Suika.getMousePosY()

マウスのY座標を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

現在のマウスのY座標を表す整数。

---

## Suika.isMouseLeftPressed()

マウスの左ボタンが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

左ボタンが現在押し下げられているかを表す真偽値。

---

## Suika.isMouseRightPressed()

マウスの右ボタンが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

右ボタンが現在押し下げられているかを表す真偽値。

---

## Suika.isMouseLeftClicked()

マウスの左ボタンが押されてから離されたかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

現在のフレームで左クリックが発生したかを表す真偽値。

---

## Suika.isMouseRightClicked()

マウスの右ボタンが押されてから離されたかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

現在のフレームで右クリックが発生したかを表す真偽値。

---

## Suika.isMouseDragging()

マウスがドラッグされているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

ボタンが押された状態でマウスが移動しているかを表す真偽値。

---

## Suika.isReturnKeyPressed()

Returnキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isSpaceKeyPressed()

Spaceキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isEscapeKeyPressed()

Escapeキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isUpKeyPressed()

上キーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isDownKeyPressed()

下キーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isLeftKeyPressed()

左キーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isRightKeyPressed()

右キーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isPageUpKeyPressed()

PageUpキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isPageDownKeyPressed()

PageDownキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isControlKeyPressed()

Controlキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isSKeyPressed()

Sキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isLKeyPressed()

Lキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isHKeyPressed()

Hキーが押されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isTouchCanceled()

タッチがキャンセルされたかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isSwiped()

スワイプされたかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.clearInputState()

現在のフレームでそれ以上の入力処理を行わないように、入力状態をクリアします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。


---

## Suika.startCommandRepetition()

複数フレームにわたるコマンド実行を開始します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.stopCommandRepetition()

複数フレームにわたるコマンド実行を停止します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isInCommandRepetition()

複数フレームにわたるコマンド実行中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.setMessageActive()

メッセージ表示状態をアクティブに設定します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.clearMessageActive()

メッセージ表示状態をリセットします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isMessageActive()

メッセージがアクティブに設定されているかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.startAutoMode()

オートモードを開始します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.stopAutoMode()

オートモードを停止します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isAutoMode()

オートモード中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.startSkipMode()

スキップモードを開始します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.stopSkipMode()

スキップモードを停止します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isSkipMode()

スキップモード中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.setSaveLoad()

セーブ・ロードの有効/無効を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                              |
|-----------|---------|-----------------------------------|
| enable    | 真偽値  | セーブとロードを有効にするか。    |

### 戻り値

なし。

---

## Suika.isSaveLoadEnabled()

セーブ・ロードの有効設定を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.setNonInterruptible()

割り込み不可モードを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                              |
|-----------|---------|-----------------------------------|
| enable    | 真偽値  | 割り込み不可モードにするか。      |

### 戻り値

なし。

---

## Suika.isNonInterruptible()

割り込み不可モードの設定を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.setPenPosition()

テキスト描画用のペン位置を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| x         | 整数    | X座標。               |
| y         | 整数    | Y座標。               |

### 戻り値

なし。

---

## Suika.getPenPositionX()

ペンのX座標を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

整数。

---

## Suika.getPenPositionY()

ペンのY座標を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

整数。

---

## Suika.pushForCall()

コールスタックに呼び出し元をプッシュします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| file      | 文字列  | スクリプトファイル名。|
| index     | 整数    | コマンドインデックス。|

### 戻り値

成功か失敗かを表す真偽値。

---

## Suika.popForReturn()

コールスタックから呼び出し元をポップします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むディクショナリを返します。

* obj.file: ファイル名
* obj.index: タグインデックス

---

## Suika.readCallStack()

指定したインデックスのコールスタック要素を読み取ります。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| sp        | 整数    | スタック要素のインデックス。 |

### 戻り値

以下の内容を含むディクショナリを返します。

* obj.file: ファイル名
* obj.index: タグインデックス

---

## Suika.writeCallStack()

指定したインデックスのコールスタック要素を書き込みます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| sp        | 整数    | スタック要素のインデックス。 |
| file      | 文字列  | スクリプトファイル名。|
| index     | 整数    | タグインデックス。    |

### 戻り値

なし。

---

## Suika.setCallArgument()

Set a calling argument for GUI or anime.

### Parameters (Dictionary)

| Parameter | Type    | Description           |
|-----------|---------|-----------------------|
| index     | Integer | Argument index.       |
| value     | String  | Argument value.       |

### Return

Boolean value.

---

## Suika.getCallArgument()

呼び出し引数を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| index     | 整数    | 引数のインデックス。  |

### 戻り値

文字列。

---

## Suika.isPageMode()

スクリプトのページモードが有効かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値を返します。

---

## Suika.appendBufferedMessage()

ページモードのバッファ文字列に文字列を追加します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| message   | 文字列  | メッセージ。  |

### 戻り値

なし。

---

## Suika.getBufferedMessage()

ページモードのバッファ文字列を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

文字列を返します。

---

## Suika.clearBufferedMessage()

ページモードのバッファ文字列をクリアします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.resetPageLine()

ページ内のメッセージ行数をリセットします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.incPageLine()

ページ内の行数をインクリメントします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isPageTop()

ページ内の最初の行であるかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.registerBGVoice()

BGVoiceを登録します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明              |
|-----------|---------|-------------------|
| file      | 文字列  | BGVoiceファイル。 |

### 戻り値

なし。

---

## Suika.getBVoice()

BGVoiceを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

ファイル名の文字列を返します。

---

## Suika.setBGVoicePlaying()

BGVoiceの再生状態を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明      |
|-----------|---------|-----------|
| isPlaying | 真偽値  | 状態。    |

### 戻り値

なし。

---

## Suika.isBGVoicePlaying()

BGVoiceが再生中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値を返します。

---

## Suika.setChapterName()

チャプター名を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| name      | 文字列  | チャプター名。|

### 戻り値

なし。

---

## Suika.getChapterName()

チャプター名を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

文字列を返します。

---

## Suika.setLastMessage()

最後のメッセージを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| message   | 文字列  | メッセージ。          |
| isAppend  | 真偽値  | 追加か置き換えか。    |

### 戻り値

なし。

---

## Suika.setPrevLastMessage()

一つ前の最後のメッセージを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| message   | 文字列  | メッセージ。  |

### 戻り値

なし。

---

## Suika.getLastMessage()

最後のメッセージを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

文字列を返します。

---

## Suika.getPrevLastMessage()

一つ前の最後のメッセージを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

文字列を返します。

---

## Suika.setTextSpeed()

テキスト表示速度を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| speed     | 浮動小数| テキスト速度。|

### 戻り値

なし。

---

## Suika.getTextSpeed()

テキスト表示速度を取得します。

### 引数 (ディクショナリ形式)

なし。

### Return

Returns a float.

---

## Suika.setAutoSpeed()

## Suika.setAutoSpeed()

オートモードの速度を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| speed     | 浮動小数| オート速度。  |

### 戻り値

なし。

---

## Suika.getAutoSpeed()

オートモードの速度を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

浮動小数を返します。

---

## Suika.markLastEnglishTagIndex()

英語の最終インデックスをマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.getLastEnglishTagIndex()

英語の最終インデックスを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

整数を返します。

---

## Suika.clearLastEnglishTagIndex()

英語の最終インデックスをクリアします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.getLastTagName()

最後のタグ名を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

文字列を返します。

---

## Suika.createImageFromFile()

ファイルから画像を読み込みます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                          |
|-----------|---------|-------------------------------|
| file      | 文字列  | 画像ファイルへのパス。        |

### 戻り値

画像オブジェクト。失敗した場合は null。

---

## Suika.createImage()

新しい空の画像を作成します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                          |
|-----------|---------|-------------------------------|
| width     | 整数    | 画像の幅。                    |
| height    | 整数    | 画像の高さ。                  |

### 戻り値

画像オブジェクト。

---

## Suika.getImageWidth()

画像の幅を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                          |
|-----------|---------|-------------------------------|
| img       | オブジェクト | 画像オブジェクト。             |

### 戻り値

幅を表す整数。

---

## Suika.getImageHeight()

画像の高さを取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                          |
|-----------|---------|-------------------------------|
| image     | オブジェクト | 画像オブジェクト。             |

### 戻り値

高さを表す整数。

---

## Suika.destroyImage()

画像を破棄し、メモリを解放します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                          |
|-----------|---------|-------------------------------|
| image     | オブジェクト | 破棄する画像オブジェクト。     |

### 戻り値

なし。

---

## Suika.drawImageCopy()

画像を別の画像にコピーします（ブレンドなし）。

### 引数 (ディクショナリ形式)

| 引数       | 型      | 説明                              |
|------------|---------|-----------------------------------|
| dstImage   | オブジェクト | コピー先画像。                    |
| dstLeft    | 整数    | コピー先のX座標。                 |
| dstTop     | 整数    | コピー先のY座標。                 |
| srcImage   | オブジェクト | コピー元画像。                    |
| dstWidth   | 整数    | 描画する幅。                      |
| dstHeight  | 整数    | 描画する高さ。                    |
| srcLeft    | 整数    | コピー元のX座標。                 |
| srcTop     | 整数    | コピー元のY座標。                 |

### 戻り値

なし。

No return.

---

## Suika.drawImageAlpha()

アルファブレンドを適用して画像を描画します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                              |
|-----------|---------|-----------------------------------|
| dstImage  | オブジェクト | 描画先画像。                      |
| dstLeft   | 整数    | 描画先のX座標。                   |
| dstTop    | 整数    | 描画先のY座標。                   |
| dstWidth  | 整数    | 描画する幅。                      |
| dstHeight | 整数    | 描画する高さ。                    |
| srcImage  | オブジェクト | 描画元画像。                      |
| srcLeft   | 整数    | 描画元のX座標。                   |
| srcTop    | 整数    | 描画元のY座標。                   |
| alpha     | 整数    | アルファ値 (`0`-`255`)。          |

### 戻り値

なし。

---

## Suika.drawImageAdd()

加算ブレンドで画像を描画します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                              |
|-----------|---------|-----------------------------------|
| dstImage  | オブジェクト | 描画先画像。                      |
| dstLeft   | 整数    | 描画先のX座標。                   |
| dstTop    | 整数    | 描画先のY座標。                   |
| dstWidth  | 整数    | 描画する幅。                      |
| dstHeight | 整数    | 描画する高さ。                    |
| srcImage  | オブジェクト | 描画元画像。                      |
| srcLeft   | 整数    | 描画元のX座標。                   |
| srcTop    | 整数    | 描画元のY座標。                   |
| alpha     | 整数    | アルファ値 (`0`-`255`)。          |

### 戻り値

なし。

---

## Suika.drawImageSub()

減算ブレンドで画像を描画します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                              |
|-----------|---------|-----------------------------------|
| dstImage  | オブジェクト | 描画先画像。                      |
| dstLeft   | 整数    | 描画先のX座標。                   |
| dstTop    | 整数    | 描画先のY座標。                   |
| dstWidth  | 整数    | 描画する幅。                      |
| dstHeight | 整数    | 描画する高さ。                    |
| srcImage  | オブジェクト | 描画元画像。                      |
| srcLeft   | 整数    | 描画元のX座標。                   |
| srcTop    | 整数    | 描画元のY座標。                   |
| alpha     | 整数    | アルファ値 (`0`-`255`)。          |

### 戻り値

なし。

---

## Suika.makeColor()

RGBA値からピクセル値を作成します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明              |
|-----------|---------|-------------------|
| r         | 整数    | 赤 (0-255)。      |
| g         | 整数    | 緑 (0-255)。      |
| b         | 整数    | 青 (0-255)。      |
| a         | 整数    | アルファ (0-255)。|

### 戻り値

ピクセル値。

---

## Suika.fillImageRect()

画像上の矩形領域を指定した色で塗りつぶします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                               |
|-----------|---------|------------------------------------|
| image     | オブジェクト | 対象画像。                         |
| left      | 整数    | X座標。                            |
| top       | 整数    | Y座標。                            |
| width     | 整数    | 幅。                               |
| height    | 整数    | 高さ。                             |
| color     | 整数    | Suika.makeColor() で作成された色。 |

### 戻り値

なし。

---

## Suika.reloadStageImages()

設定に基づいてステージ画像を再読み込みします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

成功か失敗かを表す真偽値。

---

## Suika.reloadStagePositions()

設定に基づいてステージ位置を再読み込みします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.getLayerX()

指定したレイヤーの現在のX座標を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

座標を表す整数。

---

## Suika.getLayerY()

指定したレイヤーの現在のY座標を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

座標を表す整数。

---

## Suika.setLayerPosition()

指定したレイヤーの位置を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| x         | 整数    | X座標。                   |
| y         | 整数    | Y座標。                   |

### 戻り値

なし。

---

## Suika.getLayerScaleX()

指定したレイヤーのX方向の拡大率を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

スケールを表す浮動小数。

---

## Suika.getLayerScaleY()

指定したレイヤーのY方向の拡大率を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

スケールを表す浮動小数。

---

## Suika.setLayerScale()

指定したレイヤーの拡大率を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| scale_x   | 浮動小数 | 水平方向のスケール。      |
| scale_y   | 浮動小数 | 垂直方向のスケール。      |

### 戻り値

なし。

---

## Suika.getLayerRotate()

指定したレイヤーの回転角を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

浮動小数を返します。

---

## Suika.setLayerRotate()

指定したレイヤーの回転角を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| rot       | 浮動小数 | 回転角（ラジアン）。      |

### 戻り値

なし。

---

## Suika.getLayerDim()

指定したレイヤーの暗転状態を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

真偽値を返します。

---

## Suika.setLayerDim()

指定したレイヤーの暗転状態を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| dim       | 真偽値  | レイヤーを暗くするか。    |

### 戻り値

なし。

---

## Suika.getLayerAlpha()

指定したレイヤーの不透明度を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |

### 戻り値

整数を返します。

---

## Suika.setLayerAlpha()

指定したレイヤーの不透明度を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| alpha     | 整数    | アルファ値 (0-255)。      |

### 戻り値

なし。

---

## Suika.setLayerBlend()

レイヤーのブレンドモードを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                  |
|-----------|---------|---------------------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。      |
| blend     | 整数    | ブレンドモード (Alpha, Add, Sub)。    |

### 戻り値

なし。

---

## Suika.setLayerFile()

レイヤーに表示するファイルを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| file_name | 文字列  | 画像ファイルへのパス。    |

### 戻り値

成功か失敗かを表す真偽値。

---

## Suika.setLayerFrame()

目パチや口パク用のフレームインデックスを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| layer     | 整数    | ステージレイヤーのインデックス。 |
| frame     | 整数    | フレームインデックス。     |

### 戻り値

なし。

---

## Suika.getLayerText()

テキストレイヤーに表示されている文字列を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| index     | 整数    | テキストレイヤーのインデックス。 |

### 戻り値

文字列を返します。

---

## Suika.setLayerText()

テキストレイヤーに表示する文字列を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| index     | 整数    | テキストレイヤーのインデックス。 |
| text      | 文字列  | 設定するテキスト。         |

### 戻り値

なし。

---

## Suika.getSysBtnIdleImage()

システムボタンの待機状態(idle)の画像を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

画像オブジェクトを返します。

---

## Suika.getSysBtnHoverImage()

システムボタンのホバー状態(hover)の画像を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

画像オブジェクトを返します。

---

## Suika.clearStageBasic()

基本レイヤーをクリアします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

画像オブジェクトを返します。

---

## Suika.clearStage()

ステージをクリアして初期状態に戻します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

画像オブジェクトを返します。

---

## Suika.chposToLayer()

キャラ位置をステージレイヤーのインデックスに変換します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| chpos     | 整数    | キャラ位置。  |

### 戻り値

整数を返します。

---

## Suika.chposToEyeLayer()

キャラ位置をステージレイヤーのインデックス（目のレイヤー）に変換します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| chpos     | 整数    | キャラ位置。  |

### 戻り値

整数を返します。

---

## Suika.chposToLipLayer()

キャラ位置をステージレイヤーのインデックス（口のレイヤー）に変換します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| chpos     | 整数    | キャラ位置。  |

### 戻り値

整数を返します。

---

## Suika.layerToChpos()

ステージレイヤーのインデックスをキャラ位置に変換します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| layer     | 整数    | レイヤーインデックス。|

### 戻り値

整数を返します。

---

## Suika.renderStage()

すべてのレイヤーを含むステージをレンダリングします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.startFade()

トランジション（フェード）エフェクトを開始します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                          |
|-----------|---------|-----------------------------------------------|
| desc      | 配列    | フェード記述子。                              |
| method    | 文字列  | フェード手法。                                |
| time      | 浮動小数| 所要時間（秒）。                               |
| ruleImage | オブジェクト | ルール画像オブジェクト（オプション）。   |

### 戻り値

真偽値。

---

## Suika.getShakeOffset()

シェイクコマンドのオフセット量を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むオブジェクトを返します。
* x
* y

---

## Suika.setShakeOffset()

シェイクコマンドのオフセット量を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| x         | 整数    | Xオフセット。 |
| y         | 整数    | Yオフセット。 |

### 戻り値

なし。

---

## Suika.isFadeRunning()

フェードが実行中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.finishFade()

フェードエフェクトを直ちに終了させます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.setChNameMapping()

キャラ位置に対するキャラ名インデックスを指定します。

### 引数 (ディクショナリ形式)

| 引数        | 型      | 説明                  |
|-------------|---------|-----------------------|
| chpos       | 整数    | キャラ位置。          |
| chNameIndex | 整数    | キャラ名インデックス。|

### 戻り値

なし。

---

## Suika.getTalkingChpos()

現在話しているキャラクターの位置を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

整数を返します。

---

## Suika.setChTalking()

話しているキャラクターを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| chpos     | 整数    | キャラクター位置。    |

### 戻り値

なし。

---

## Suika.updateChDimByTalkingCh()

誰が話しているかに基づいて、キャラクターの暗転状態を自動的に更新します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.forceChDim()

キャラクターの暗転状態を手動で更新します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| chpos     | 整数    | キャラクター位置。    |
| dim       | 真偽値  | 暗転させるかどうか。  |

### 戻り値

なし。

---

## Suika.getChDim()

暗転状態を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| chpos     | 整数    | キャラクター位置。    |

### 戻り値

真偽値を返します。

---

## Suika.fillNameBox()

名前ボックスを名前ボックス用画像で塗りつぶします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.getNameBoxRect()

名前ボックスの位置とサイズを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むオブジェクトを返します。

* x
* y
* w
* h

---

## Suika.showNameBox()

名前ボックスの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| show      | 真偽値  | 表示するか非表示にするか。 |

### 戻り値

なし。

---

## Suika.fillMessageBox()

メッセージボックスをメッセージボックス用画像で塗りつぶします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.showMessageBox()

メッセージボックスの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| show      | 真偽値  | ボックスを表示するかどうか。 |

### 戻り値

なし。

---

## Suika.getMessageBoxRect()

メッセージボックスの領域を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むオブジェクトを返します。
* `x`
* `y`
* `w`
* `h`

---

## Suika.setClickPosition()

クリック待ちアニメーションの位置を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明          |
|-----------|---------|---------------|
| x         | 整数    | X座標。       |
| y         | 整数    | Y座標。       |

### 戻り値

なし。

---

## Suika.showClick()

クリック待ちアニメーションの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| show      | 真偽値  | 表示するか非表示にするか。 |

### 戻り値

なし。

---

## Suika.setClickIndex()

クリック待ちアニメーションのフレームインデックスを設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                  |
|-----------|---------|-----------------------|
| index     | 整数    | フレームインデックス。 |

### 戻り値

なし。

---

## Suika.getClickRect()

クリック待ちアニメーションの領域を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むオブジェクトを返します。
* `x`
* `y`
* `w`
* `h`

---

## Suika.fillChooseBoxIdleImage()

選択肢ボックスの待機状態(idle)レイヤーを選択肢ボックス用画像で塗りつぶします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| index     | 整数    | 選択肢ボックスのインデックス。 |

### 戻り値

なし。

---

## Suika.fillChooseBoxHoverImage()

選択肢ボックスのホバー状態(hover)レイヤーを選択肢ボックス用画像で塗りつぶします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| index     | 整数    | 選択肢ボックスのインデックス。 |

### 戻り値

なし。

---

## Suika.showChoosebox()

選択肢ボックスの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数       | 型      | 説明                                  |
|------------|---------|---------------------------------------|
| index      | 整数    | 選択肢ボックスのインデックス (`0`-`7`) |
| showIdle   | 真偽値  | 待機状態を表示するか。                |
| showHover  | 真偽値  | ホバー状態を表示するか。              |

### 戻り値

なし。

---

## Suika.getChooseBoxRect()

選択肢ボックスの領域を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

以下の内容を含むオブジェクトを返します。
* `x`
* `y`
* `w`
* `h`

---

## Suika.showAutoModeBanner()

オートモードバナーの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数       | 型      | 説明                  |
|------------|---------|-----------------------|
| show       | 真偽値  | 表示するか非表示にするか。 |

### 戻り値

なし。

---

## Suika.showSkipModeBanner()

スキップモードバナーの表示・非表示を切り替えます。

### 引数 (ディクショナリ形式)

| 引数       | 型      | 説明                  |
|------------|---------|-----------------------|
| show       | 真偽値  | 表示するか非表示にするか。 |

### 戻り値

なし。

---

## Suika.renderImage()

画面に対して画像を直接レンダリングします。

通常のレンダリングにはステージレイヤーの使用を検討してください。
このAPIはエフェクトに便利です。

### 引数 (ディクショナリ形式)

| 引数      | 省略可能     | 型      | 説明                                      |
|-----------|--------------|---------|-------------------------------------------|
| dstLeft   | いいえ       | 整数    | 描画先の左上X座標。                       |
| dstTop    | いいえ       | 整数    | 描画先の左上Y座標。                       |
| image     | いいえ       | オブジェクト | 画像。                                   |
| srcLeft   | いいえ       | 整数    | 描画元の左上X座標。                       |
| srcTop    | いいえ       | 整数    | 描画元の左上Y座標。                       |
| srcWidth  | いいえ       | 整数    | 描画元の幅。                             |
| srcHeight | いいえ       | 整数    | 描画元の高さ。                           |
| alpha     | いいえ       | 整数    | アルファ値 (`0`-`255`)。                 |

### 戻り値

なし。

---

## Suika.renderImage3d()

3D変換を適用して画面に対して画像を直接レンダリングします。

通常のレンダリングにはステージレイヤーの使用を検討してください。
このAPIはエフェクトに便利です。

### 引数 (ディクショナリ形式)

| 引数      | 省略可能     | 型      | 説明                                      |
|-----------|--------------|---------|-------------------------------------------|
| x1        | いいえ       | 整数    | 描画先頂点1のX座標。                     |
| y1        | いいえ       | 整数    | 描画先頂点1のY座標。                     |
| x2        | いいえ       | 整数    | 描画先頂点2のX座標。                     |
| y2        | いいえ       | 整数    | 描画先頂点2のY座標。                     |
| x3        | いいえ       | 整数    | 描画先頂点3のX座標。                     |
| y3        | いいえ       | 整数    | 描画先頂点3のY座標。                     |
| x4        | いいえ       | 整数    | 描画先頂点4のX座標。                     |
| y4        | いいえ       | 整数    | 描画先頂点4のY座標。                     |
| tex       | いいえ       | オブジェクト | 画像。                                   |
| srcLeft   | いいえ       | 整数    | 描画元の左上X座標。                       |
| srcTop    | いいえ       | 整数    | 描画元の左上Y座標。                       |
| srcWidth  | いいえ       | 整数    | 描画元の幅。                             |
| srcHeight | いいえ       | 整数    | 描画元の高さ。                           |
| alpha     | いいえ       | 整数    | アルファ値 (`0`-`255`)。                 |

### 戻り値

なし。

---

## Suika.startKirakira()

キラキラエフェクトを開始します。

キラキラエフェクトは、マウスカーソルがクリックされた画面位置に表示されるアニメーションです。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.renderKirakira()

キラキラエフェクトをレンダリングします。

---

## Suika.setMixerInputFile()

特定のミキサートラックでサウンドファイルを再生します。

### 引数 (ディクショナリ形式)

| 引数      | 省略可能          | 型      | 説明                                      |
|-----------|-------------------|---------|-------------------------------------------|
| track     | いいえ            | 文字列  | ミキサートラック名。                       |
| file      | いいえ            | 文字列  | サウンドファイルへのパス。                 |
| isLooped  | はい(`false`)    | 真偽値  | ループ再生するかどうか。                   |

### トラック名

| 名前   | 説明                     |
|--------|--------------------------|
| bgm    | BGMトラック。            |
| se     | 効果音(SE)トラック。     |
| voice  | キャラクターボイストラック。|
| sys    | システムサウンドトラック。|

### 戻り値

ファイルが正常に開かれたかを表す真偽値。

---

## Suika.setMixerVolume()

特定のミキサートラックの音量を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 文字列  | ミキサートラック名。                       |
| vol       | 浮動小数 | 音量レベル (0.0 から 1.0)。                |
| span      | 浮動小数 | フェード時間（秒）。                       |

### トラック名

| 名前   | 説明                     |
|--------|--------------------------|
| bgm    | BGMトラック。            |
| se     | 効果音(SE)トラック。     |
| voice  | キャラクターボイストラック。|
| sys    | システムサウンドトラック。|

### 戻り値

なし。

---

## Suika.getMixerVolume()

特定のミキサートラックの音量を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 文字列  | ミキサートラック名。                       |

### トラック名

| 名前   | 説明                     |
|--------|--------------------------|
| bgm    | BGMトラック。            |
| se     | 効果音(SE)トラック。     |
| voice  | キャラクターボイストラック。|
| sys    | システムサウンドトラック。|

### 戻り値

浮動小数を返します。

---

## Suika.setMasterVolume()

すべてのトラックに影響するマスター音量を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| volume    | 浮動小数 | マスター音量レベル (0.0 から 1.0)。        |

### 戻り値

なし。

---

## Suika.getMasterVolume()

すべてのトラックに影響するマスター音量を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

浮動小数を返します。

---

## Suika.setMixerGlobalVolume()

トラックのグローバル音量を設定します（主に設定画面で使用されます）。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 文字列  | ミキサートラック名。                       |
| vol       | 浮動小数 | グローバル音量レベル。                     |

### トラック名

| 名前   | 説明                     |
|--------|--------------------------|
| bgm    | BGMトラック。            |
| se     | 効果音(SE)トラック。     |
| voice  | キャラクターボイストラック。|
| sys    | システムサウンドトラック。|

### 戻り値

なし。

---

## Suika.getMixerGlobalVolume()

トラックのグローバル音量を取得します（主に設定画面で使用されます）。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 文字列  | ミキサートラック名。                       |

### トラック名

| 名前   | 説明                     |
|--------|--------------------------|
| bgm    | BGMトラック。            |
| se     | 効果音(SE)トラック。     |
| voice  | キャラクターボイストラック。|
| sys    | システムサウンドトラック。|

### 戻り値

浮動小数を返します。

---

## Suika.setCharacterVolume()

特定のキャラクターボイスの音量を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| index     | 整数    | キャラクター名インデックス。               |
| vol       | 浮動小数 | 音量レベル。                               |

### 戻り値

なし。

---

## Suika.getCharacterVolume()

特定のキャラクターボイスの音量を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| ch_index  | 整数    | キャラクター名インデックス。               |

### 戻り値

浮動小数を返します。

---

## Suika.isMixerSoundFinished()

特定のトラックの再生が終了したかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 整数    | ミキサートラックインデックス。             |

### 戻り値

真偽値。

---

## Suika.getTrackFileName()

トラックに現在読み込まれているサウンドのファイル名を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| track     | 整数    | ミキサートラックインデックス。             |

### 戻り値

ファイルパスを表す文字列。

---

## Suika.applyCharacterVolume()

キャラクター固有の音量設定をVOICEトラックに適用します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                      |
|-----------|---------|-------------------------------------------|
| ch        | 整数    | キャラクター名インデックス。               |

### 戻り値

なし。

---

## Suika.getStringWidth()

UTF-8文字列の合計幅を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| fontType  | 整数    | フォント選択インデックス。 |
| fontSize  | 整数    | フォントサイズ。           |
| text      | 文字列  | テキスト。                 |

### 戻り値

ピクセル単位の幅を表す整数。

---

## Suika.getStringHeight()

UTF-8文字列の合計高さを取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                      |
|-----------|---------|---------------------------|
| fontType  | 整数    | フォント選択インデックス。 |
| fontSize  | 整数    | フォントサイズ。           |
| text      | 文字列  | テキスト。                 |

### 戻り値

ピクセル単位の高さを受け取る整数。

---

## Suika.drawGlyph()

画像上に単独のグリフ（文字）を描画します。

### 引数 (ディクショナリ形式)

| 引数          | 型      | 説明                                        |
|---------------|---------|---------------------------------------------|
| img           | オブジェクト | 描画先の画像。                              |
| font_type     | 整数    | フォント選択インデックス。                  |
| font_size     | 整数    | 描画するフォントサイズ。                    |
| base_font_size| 整数    | メトリクスの基準フォントサイズ。            |
| outline_size  | 整数    | 縁取りの幅。                                |
| x             | 整数    | X座標。                                     |
| y             | 整数    | Y座標。                                     |
| color         | Pixel   | メインのテキスト色。                        |
| outline_color | Pixel   | 縁取りの色。                                |
| codepoint     | 整数    | UTF-32コードポイント。                      |
| is_dim        | 真偽値  | 減光を適用するかどうか。                    |

### 戻り値

成功を表す真偽値。

---

## Suika.createDrawMsg()

高度なテキストレンダリングのための、複雑なメッセージ描画コンテキストを作成します。

### 引数 (ディクショナリ形式)

| 引数           | 型       | 説明                   |
|----------------|----------|------------------------|
| image          | 整数     | 描画先の画像。         |
| text           | 文字列   | 描画するメッセージ。   |
| fontType       | 整数     | フォントの選択。       |
| fontSize       | 整数     | フォントサイズ。       |
| baseFontSize   | 整数     | 基準フォントサイズ。   |
| rubySize       | 整数     | ルビのサイズ。         |
| outlineSize    | 整数     | 縁取りの幅。           |
| penX           | 整数     | ペンのX座標。          |
| penY           | 整数     | ペンのY座標。          |
| areaWidth      | 整数     | 描画エリアの幅。       |
| areaHeight     | 整数     | 描画エリアの高さ。     |
| leftMargin     | 整数     | 左余白。               |
| rightMargin    | 整数     | 右余白。               |
| topMargin      | 整数     | 上余白。               |
| bottomMargin   | 整数     | 下余白。               |
| lineMargin     | 整数     | 行間。                 |
| charMargin     | 整数     | 文字間。               |
| color          | 整数     | 色。                   |
| outlineColor   | 整数     | 縁取りの色。           |
| bgColor        | 整数     | 背景色。               |
| fillBg         | 真偽値   | 背景を塗りつぶすか？   |
| dim            | 真偽値   | 減光するか？           |
| ignoreLF       | 真偽値   | 改行を無視するか？     |
| ignoreFont     | 真偽値   | フォント変更を無視するか？ |
| ignoreOutline  | 真偽値   | 縁取り変更を無視するか？ |
| ignoreColor    | 真偽値   | 色変更を無視するか？   |
| ignoreSize     | 真偽値   | サイズ変更を無視するか？ |
| ignorePosition | 真偽値   | カーソル移動を無視するか？|
| ignoreRuby     | 真偽値   | ルビを無視するか？     |
| ignoreWait     | 真偽値   | インラインでの待ち時間を無視するか？|
| inlineWaitHook | 関数     | インライン待機時のフック。|
| tategaki       | 真偽値   | 縦書きを使用するか？   |

### 戻り値

メッセージ描画コンテキストオブジェクト。

---

## Suika.destroyDrawMsg()

メッセージ描画コンテキストを破棄します。

### 引数 (ディクショナリ形式)

| 引数           | 型       | 説明                   |
|----------------|----------|------------------------|
| context        | オブジェクト | メッセージ描画コンテキスト。 |

### 戻り値

なし。

---

## Suika.countDrawMsgChars()

エスケープシーケンスを除いた、残りの文字数をカウントします。

### 引数 (ディクショナリ形式)

| 引数           | 型       | 説明                   |
|----------------|----------|------------------------|
| context        | オブジェクト | メッセージ描画コンテキスト。 |

### 戻り値

整数値を返します。

---

## Suika.drawMessage()

メッセージ内の文字を最大 (maxChars) 文字まで描画します。

### 引数 (ディクショナリ形式)

| 引数           | 型       | 説明                   |
|----------------|----------|------------------------|
| context        | オブジェクト | メッセージ描画コンテキスト。 |
| maxChars       | 整数     | 最大文字数。           |

### 戻り値

この呼び出しで描画された文字数を示す整数を返します。

---

## Suika.getDrawMsgPenPosition()

描画コンテキストから現在のペン位置を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| context   | オブジェクト | 描画コンテキスト。       |

### 戻り値

`x` と `y` を含むオブジェクト。

---

## Suika.isEscapeSequenceChar()

文字がエスケープシーケンスの一部であるかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| c         | 文字列  | チェックする文字。       |

### 戻り値

真偽値。

---

## Suika.moveToTagFile()

新しいタグファイルを読み込み、実行ポイントをその先頭に移動します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| file      | 文字列  | .novel またはスクリプトファイルへのパス。 |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.getTagCount()

現在のスクリプトファイル内のタグの総数を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

タグの総数を表す整数。

---

## Suika.moveToTagIndex()

実行ポインタを特定のタグインデックスに移動します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| index     | 整数    | 対象のタグインデックス。   |

### 戻り値

真偽値。

---

## Suika.moveToNextTag()

実行ポインタを直後のタグに移動します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.moveToLabelTag()

特定のラベルにジャンプします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| name      | 文字列  | 対象のラベル名。         |

### 戻り値

真偽値。

---

## Suika.moveToMacroTag()

名前を指定して特定のマクロにジャンプします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| name      | 文字列  | 対象のマクロ名。         |

### 戻り値

真偽値。

---

## Suika.moveToElseTag()

対応する else/elseif/endif タグにジャンプします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| name      | 文字列  | 対象のマクロ名。         |

### 戻り値

真偽値。

---

## Suika.moveToEndIfTag()

対応する endif タグにジャンプします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| name      | 文字列  | 対象のマクロ名。         |

### 戻り値

真偽値。

---

## Suika.moveToEndMacroTag()

対応する endmacro タグにジャンプします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                     |
|-----------|---------|--------------------------|
| name      | 文字列  | 対象のマクロ名。         |

### 戻り値

真偽値。

---

## Suika.getTagFileName()

現在のスクリプトファイル名を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

ファイル名を表す文字列。

---

## Suika.getTagName()

現在のタグ名を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

タグ名を表す文字列。

---

## Suika.getTagPropertyCount()

現在のタグのプロパティ（引数）の数を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

プロパティの数を表す整数。

---

## Suika.getTagPropertyName()

現在のタグのプロパティ（引数）を名前またはインデックスで取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明               |
|-----------|---------|--------------------|
| index     | 整数    | プロパティのインデックス。 |

### 戻り値

名前を表す文字列。

---

## Suika.getTagPropertyValue()

現在のタグのプロパティ（値）を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明               |
|-----------|---------|--------------------|
| index     | 整数    | プロパティのインデックス。 |

### 戻り値

値を表す文字列。

---

## Suika.getTagArgBool()

現在のタグの特定の引数を取得します。デフォルト値や省略可能かどうかの指定をサポートしています。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                               |
|-----------|---------|------------------------------------|
| name      | 文字列  | 引数の名前。                       |
| omissible | 真偽値  | 引数が省略可能かどうか。           |
| defVal    | 真偽値  | 省略時のデフォルト値。             |

### 戻り値

要求された型の引数の値。

---

## Suika.getTagArgInt()

現在のタグの特定の引数を取得します。デフォルト値や省略可能かどうかの指定をサポートしています。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                               |
|-----------|---------|------------------------------------|
| name      | 文字列  | 引数の名前。                       |
| omissible | 真偽値  | 引数が省略可能かどうか。           |
| defVal    | 整数    | 省略時のデフォルト値。             |

### 戻り値

要求された型の引数の値。

---

## Suika.getTagArgFloat()

現在のタグの特定の引数を取得します。デフォルト値や省略可能かどうかの指定をサポートしています。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                               |
|-----------|---------|------------------------------------|
| name      | 文字列  | 引数の名前。                       |
| omissible | 真偽値  | 引数が省略可能かどうか。           |
| defVal    | 浮動小数点数 | 省略時のデフォルト値。             |

### 戻り値

要求された型の引数の値。

---

## Suika.getTagArgString()

現在のタグの特定の引数を取得します。デフォルト値や省略可能かどうかの指定をサポートしています。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                               |
|-----------|---------|------------------------------------|
| name      | 文字列  | 引数の名前。                       |
| omissible | 真偽値  | 引数が省略可能かどうか。           |
| defVal    | 文字列  | 省略時のデフォルト値。             |

### 戻り値

要求された型の引数の値。

---

## Suika.evaluateTag()

現在のタグのプロパティ値を評価し、インライン変数（`${varname}` 形式）を展開します。

このAPIを呼び出すと、プロパティ値のキャッシュが更新されます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.pushTagStackIf()

`[if]` 条件ブロック用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `if` ブロックの位置をマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.popTagStackIf()

`if` 条件ブロック用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `if` ブロックの終了をマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.pushTagStackWhile()

ループ（`while`）用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `while` ブロックをマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.popTagStackWhile()

ループ（`while`）用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `while` ブロックの終了をマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.pushTagStackFor()

ループ（`for`）用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `for` ブロックをマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.popTagStackFor()

ループ（`for`）用の内部スタックを管理します。

このAPIは、ネストされたブロック処理のために `for` ブロックの終了をマークします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.loadAnimeFromFile()

ファイルからアニメーション定義を読み込み、登録します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| file      | 文字列  | アニメーションファイルへのパス。            |
| reg_name  | 文字列  | アニメーションの登録名。                    |

### 戻り値

各レイヤーが読み込まれたかどうかを示す真偽値の配列。

---

## Suika.newAnimeSequence()

特定のレイヤーに対する新しいアニメーションシーケンスの記述を開始します。
このAPIは、手動でアニメーションを生成する場合に使用されます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| layer     | 整数    | 対象となるステージレイヤーのインデックス。   |

### 戻り値

成功を表す真偽値。

---

## Suika.addAnimeSequencePropertyF()

現在の手動アニメーションシーケンスに浮動小数点数プロパティ（例：位置、不透明度）を追加します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| key       | 文字列  | プロパティキー（例："x", "y", "a"）。       |
| val       | 浮動小数点数 | 目標値。                                    |

### 戻り値

真偽値。

---

## Suika.addAnimeSequencePropertyI()

現在の手動アニメーションシーケンスに整数プロパティ（例：位置、不透明度）を追加します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| key       | 文字列  | プロパティキー（例："x", "y", "a"）。       |
| val       | 整数    | 目標値。                                    |

### 戻り値

真偽値。

---

## Suika.startLayerAnime()

特定のレイヤーに対して登録されたアニメーションシーケンスを開始します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| layer     | 整数    | 対象となるステージレイヤーのインデックス。   |

### 戻り値

真偽値。

---

## Suika.isAnimeRunning()

全体のアニメーション状態をチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isAnimeFinishedForLayer()

特定のレイヤーのアニメーションが終了したかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| layer     | 整数    | 対象のステージレイヤー。   |

### 戻り値

真偽値。

---

## Suika.updateAnimeFrame()

アニメーションのフレーム状態を更新します。通常、1フレームに1回呼び出されます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.loadEyeImageIfExists()

キャラクターの立ち絵位置に対する瞬き（目パッチ）画像とアニメーションを管理します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| chpos     | 整数    | キャラ位置（左、中央など）。                |
| file      | 文字列  | 目パッチ画像ファイルへのパス。              |

### 戻り値

真偽値。

---

## Suika.reloadEyeAnime()

指定したキャラ位置の瞬き（目パッチ）アニメーションを再開します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| chpos     | 整数    | キャラ位置（左、中央など）。                |

### 戻り値

真偽値。

---

## Suika.runLipAnime()

メッセージ内容に基づいて、キャラクターの口パクアニメーションを開始します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| chpos     | 整数    | キャラクターの位置。                        |
| text      | 文字列  | 同期させるメッセージテキスト。              |

### 戻り値

なし。

---

## Suika.stopLipAnime()

口パクアニメーションを停止します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| chpos     | 整数    | キャラクターの位置。                        |

### 戻り値

なし。

---

## Suika.clearLayerAnimeSequence()

特定のレイヤーのアニメーションシーケンスをクリアします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| layer     | 整数    | 対象のステージレイヤー。   |

### 戻り値

なし。

---

## Suika.clearAllAnimeSequence()

すべてのレイヤーのアニメーションシーケンスをクリアします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.setVariableInt()

ローカル変数またはグローバル変数に値を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |
| value     | 整数    | 設定する値。           |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.setVariableFloat()

ローカル変数またはグローバル変数に値を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |
| value     | 浮動小数点数 | 設定する値。           |

### 戻り値

成功 or 失敗を表す真偽値。

---

## Suika.setVariableString()

ローカル変数またはグローバル変数に値を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |
| value     | 文字列  | 設定する値。           |

### 戻り値

成功 or 失敗を表す真偽値。

---

## Suika.getVariableInt()

変数の現在の値を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |

### 戻り値

整数の変数。

---

## Suika.getVariableFloat()

変数の現在の値を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |

### 戻り値

浮動小数点数の変数の値。

---

## Suika.getVariableString()

変数の現在の値を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | 変数名。               |

### 戻り値

文字列の変数の値。

---

## Suika.unsetVariable()

特定の変数を解除（削除）します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| name      | 文字列  | 解除する変数の名前。       |

### 戻り値

なし。

---

## Suika.unsetLocalVariables()

すべてのローカル変数を解除（削除）します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.makeVariableGlobal()

変数をグローバル（セーブを跨いで保持される）に設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| name      | 文字列  | 変数名。                   |
| is_global | 真偽値  | グローバルにするかどうか。 |

### 戻り値

真偽値。

---

## Suika.isGlobalVariable()

変数がグローバルかどうかをチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| name      | 文字列  | 変数名。                   |

### 戻り値

真偽値。

---

## Suika.getVariableCount()

変数の数を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

個数を表す整数。

---

## Suika.getVariableName()

登録されている変数を列挙します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | 変数のインデックス。   |

### 戻り値

名前を表す文字列。

---

## Suika.checkVariableExists()

指定された名前の変数が存在するかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| name      | 文字列  | チェックする名前。     |

### 戻り値

真偽値。

---

## Suika.executeSaveGlobal()

グローバルセーブを実行します。
グローバルデータには通常、永続的な設定などが含まれます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.executeLoadGlobal()

グローバルロードを実行します。
グローバルデータには通常、永続的な設定などが含まれます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.executeSaveLocal()

特定のスロットにゲームの進行状況をセーブします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.executeLoadLocal()

特定のスロットからゲームの進行状況をロードします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.checkSaveExists()

指定されたスロット番号にセーブデータが存在するかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

真偽値。

---

## Suika.deleteLocalSave()

ローカルセーブスロットを削除します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

なし。

---

## Suika.deleteGlobalSave()

グローバルセーブデータ全体を削除します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.checkRightAfterLoad()

現在のフレームがロード成功直後であるかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.getSaveTimestamp()

セーブデータが作成されたタイムスタンプ（Unix時間）を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

整数（タイムスタンプ）。

---

## Suika.getLatestSaveIndex()

最後に更新されたセーブスロットのインデックスを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

スロット番号を表す整数。

---

## Suika.getSaveChapterName()

セーブスロットに保存されている章（チャプター）名を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

章名を表す文字列。

---

## Suika.getSaveLastMessage()

セーブスロットに保存されている最後に表示されたメッセージを取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

メッセージを表す文字列。

---

## Suika.getSaveThumbnail()

セーブスロットに関連付けられたサムネイル画像を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | セーブスロットの番号。 |

### 戻り値

画像オブジェクト。

---

## Suika.clearHistory()

ヒストリ（バックログ）からすべてのメッセージを消去します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.addHistory()

履歴に新しいエントリを追加します。

### 引数 (ディクショナリ形式)

| 引数             | 型      | 説明                                        |
|------------------|---------|---------------------------------------------|
| name             | 文字列  | キャラクター名。                            |
| msg              | 文字列  | メッセージテキスト。                        |
| voice            | 文字列  | ボイスファイルへのパス。                    |
| bodyColor        | 整数    | 本体の色。                                  |
| bodyOutlineColor | 整数    | 本体の縁取り色。                            |
| nameColor        | 整数    | 名前の色。                                  |
| nameOutlineColor | 整数    | 名前の縁取り色。                            |

### 戻り値

成功を表す真偽値。

---

## Suika.getHistoryCount()

現在履歴にあるエントリの総数を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

履歴の総数を表す整数。

---

## Suika.getHistoryName()

指定したインデックスの履歴の名前を取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | 履歴のインデックス。   |

### 戻り値

文字列。

---

## Suika.getHistoryMessage()

指定したインデックスの履歴のメッセージを取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | 履歴のインデックス。   |

### 戻り値

文字列。

---

## Suika.getHistoryVoice()

指定したインデックスの履歴のボイスパスを取得します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| index     | 整数    | 履歴のインデックス。   |

### 戻り値

文字列。

---

## Suika.loadSeen()

現在のスクリプトファイルの既読フラグを読み込みます。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

成功を表す真偽値。

---

## Suika.saveSeen()

現在のスクリプトファイルの既読フラグを保存します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

成功を表す真偽値。

---

## Suika.getSeenFlags()

現在のタグの既読状態を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

整数。

`[text]` タグの場合、`0` は未読、`1` は既読を意味します。

`[choose]` タグの場合、各ビットはその選択肢が過去に選択されたかどうかを示します。

---

## Suika.setSeenFlags()

現在のタグの既読状態を設定します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| flag      | 整数    | 既読状態フラグ。       |

### 戻り値

なし。

---

## Suika.loadGUIFile()

GUI定義ファイルを読み込み、実行準備をします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                                        |
|-----------|---------|---------------------------------------------|
| file      | 文字列  | .gui ファイルへのパス。                     |
| sys       | 真偽値  | システムGUI（セーブ/ロード等）かどうか。    |

### システムGUIとは

システムGUIは通常、`[text]` や `[choose]` の実行中に呼び出され、
終了すると中断されていたタグに制御が戻ります。

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.startGUI()

読み込まれたGUIを開始します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.stopGUI()

現在実行中のGUIを停止します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isGUIRunning()

GUIが現在アクティブかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.isGUIFinished()

GUIが操作を完了したかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.getGUIResultLabel()

GUIを終了させた際に選択されたボタンのラベルを取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

結果のラベルを表す文字列。

---

## Suika.isGUIResultTitle()

GUIが「タイトルに戻る」アクションで閉じられたかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.checkIfSavedInGUI()

GUIがアクティブな間にセーブ操作が行われたかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.checkIfLoadedInGUI()

GUIがアクティブな間にロード操作が行われたかどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.checkRightAfterSysGUI()

現在のフレームが、システムGUIから戻った直後であるかチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.getMillisec()

ミリ秒単位での経過時間を取得します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

ミリ秒単位の整数。

---

## Suika.checkFileExists()

ファイルが存在するかチェックします。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| file      | 文字列  | ファイルへのパス。     |

### 戻り値

真偽値を返します。

---

## Suika.readFileContent()

ファイルの内容をすべて読み込みます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                   |
|-----------|---------|------------------------|
| file      | 文字列  | ファイルへのパス。     |

### 戻り値

文字列を返します。

---

## Suika.writeSaveData()

キーに関連付けられた生のセーブデータを直接書き込みます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| key       | 文字列  | データのユニークなキー。   |
| data      | 文字列  | 書き込むデータ。           |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.readSaveData()

キーに関連付けられた生のセーブデータを直接読み込みます。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| key       | 文字列  | データのユニークなキー。   |

### 戻り値

成功または失敗を表す真偽値。

---

## Suika.playVideo()

動画の再生を制御します。

### 引数 (ディクショナリ形式)

| 引数          | 型      | 説明                                        |
|---------------|---------|---------------------------------------------|
| file          | 文字列  | 動画ファイルへのパス。                      |
| is_skippable  | 真偽値  | ユーザーが動画をスキップ可能かどうか。      |

### 戻り値

再生には真偽値、再生中かどうかの確認には真偽値を返します。

---

## Suika.stopVideo()

動画の再生を停止します。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.isVideoPlaying()

動画が再生中かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値を返します。

---

## Suika.isFullScreenSupported()

フルスクリーンモードが可能かどうかをチェックします。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

真偽値。

---

## Suika.enterFullScreenMode()

フルスクリーンモードに入ります。

### 引数 (ディクショナリ形式)

なし。

### 戻り値

なし。

---

## Suika.speakText()

指定されたメッセージに対してテキスト読み上げ（TTS）を実行します。

### 引数 (ディクショナリ形式)

| 引数      | 型      | 説明                       |
|-----------|---------|----------------------------|
| msg       | 文字列  | 読み上げるテキスト。       |

### 戻り値

なし。
