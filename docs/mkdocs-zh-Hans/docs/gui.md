GUI
===

## 简介

GUI 是 Suika3 中用于创建用户界面（UI）和用户体验（UX）的功能。

在 Suika3 中，按钮是在专用的 GUI 模式下定义的，并以同步方式运行。也就是说，调用 GUI 会导致按钮点击或取消操作。

我们有意避免了在文本标签执行期间显示按钮等异步回调，因为初学者程序员往往难以理解和管理这些操作。

GUI 文件包含一系列按钮定义。每个按钮都包含行为类型、空闲（idle）和悬停（hover）图像以及附加属性。按钮被映射到动画层，当按钮状态改变时可以触发动画文件。

---

## GUI 示例

```
# 全局设置部分。
global {
    fadein:       0.2;            # 淡入时间（秒）。
    fadeout:      0.2;            # 淡出时间（秒）。
}
# 一个块描述一个按钮。
# 代码块的名称可以是您喜欢的任何内容，它不会影响任何功能。
button1 {
    # 层 ID (`1`-`32`)
    # `1` 表示 GUI 层中最顶层，`32` 表示最底层。
    id: 1;

    # 行为类型 (`goto` 表示点击后跳转到一个标记。)
    type: goto;
    # 要跳转到的标记。
    label: button1_clicked;
    # 位置
    x: 39;
    y: 99;
    # `width:` 和 `height:` 可以省略，因为它们可以从图像大小推断出来。
    # 图像
    image-idle:  gui/item-idle.png;    # 当按钮未被指向时显示。
    image-hover: gui/item-hover.png;   # 当按钮被指向时显示。
    image-press: gui/item-press.png;   # 当按钮被按下时显示。
    # 动画
    anime-idle:  gui/item-idle.anime;   # 当按钮状态变为 `idle` 时执行。
    anime-hover: gui/item-hover.anime;  # 当按钮状态变为 `hover` 时执行。
    anime-press: gui/item-press.anime;  # 当按钮被按下时执行。
    # 注意：`press` 不是一个独立的状态，它是 `hover` 状态的一个附加标志。
    # 当 `hover` 动画运行时，`idle` 动画将被取消。
    # 当 `idle` 动画运行时，`hover` 动画将被取消。
    # 但是，`press` 动画不会取消任何内容。
}
```

在 `global` 部分中，您可以指定 GUI 的选项。

其他部分被解释为按钮定义。
在这里，`button1` 在位置 `(39, 99)` 创建一个按钮。
如果点击该按钮，将发生所谓的 `goto` 跳转。

---

## 按钮类型

| 描述 | 含义 |
|---------------------------------|---------------------------------------------------|
| type: noaction; | 不可点击的图像。 |
| type: goto; | 点击时跳转到一个标记。 |
| type: gui; | 点击时跳转到一个 GUI。 |
| type: mastervol; | 显示为主音量滑块。 |
| type: bgmvol; | 显示为背景音乐（BGM）音量滑块。 |
| type: sevol; | 显示为音效（SE）音量滑块。 |
| type: voicevol; | 显示为语音音量滑块。 |
| type: charactervol; | 显示为角色音量滑块。 |
| type: textspeed; | 显示为文本速度滑块。 |
| type: autospeed; | 显示为自动模式速度滑块。 |
| type: preview; | 显示为文本预览区域。 |
| type: fullscreen; | 显示为全屏模式按钮。 |
| type: window; | 显示为窗口模式按钮。 |
| type: default; | 按下时重置设置。 |
| type: savepage; | 显示为存档/读档界面页面按钮。 |
| type: save; | 显示为存档槽位。 |
| type: load; | 显示为读档槽位。 |
| type: auto; | 显示为自动模式按钮。 |
| type: skip; | 显示为跳过模式按钮。 |
| type: title; | 显示为返回标题按钮。 |
| type: history; | 显示为历史记录按钮。 |
| type: historyscroll; | 显示为垂直历史记录滚动滑块。 |
| type: historyscroll-horizontal; | 显示为水平历史记录滚动滑块。 |
| type: cancel; | 显示为取消按钮。 |
| type: namevar; | 显示为预览名称变量值的区域。 |
| type: char; | 显示为输入字符的按钮。 |
| type: language | 点击时更改语言。 |

### `noaction`

一个不可点击的图像。

```
background {
    type: noaction;
    x: 0;
    y: 0;
    image-idle: idle.png;
}
```

### `goto`

一个可点击的按钮。点击时，标签执行将跳转到由 `label:` 参数指定的标记。

```
button1 {
    type: goto;
    label: label_clicked_button1;
    x: 0;
    y: 0;
    image-idle:  idle.png;
    image-hover: hover.png;
}
```

### `gui`

一个可点击的按钮。点击时，GUI 执行将链接到由 `file:` 参数指定的新 GUI。

```
button1 {
    type: gui;
    label: gui2.gui;
    x: 0;
    y: 0;
    image-idle:  idle.png;
    image-hover: hover.png;
}
```

### `mastervol`

用于设置主音量的滑块。

```
slider1 {
    type: mastervol;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `bgmvol`

用于设置背景音乐（BGM）音量的滑块。

此滑块设置的音量将存储在全局存档数据文件中。
这与通过 `[volume]` 标签设置并存储在本地存档数据中的音量不同。


```
slider1 {
    type: bgmvol;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `sevol`

用于设置音效（SE）音量的滑块。

此滑块设置的音量将存储在全局存档数据文件中。
这与通过 `[volume]` 标签设置并存储在本地存档数据中的音量不同。


```
slider1 {
    type: bgmvol;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `voicevol`

用于设置音效（SE）音量的滑块。

此滑块设置的音量将存储在全局存档数据文件中。
这与通过 `[volume]` 标签设置并存储在本地存档数据中的音量不同。


```
slider1 {
    type: bgmvol;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `charactervol`

用于设置每个角色音量的滑块。

角色索引通过 `index:` 参数传递。
索引 0 用于未定义的角色，1-32 用于已定义的角色。

```
slider1 {
    type: charactervol;
    index: 1;  # 角色索引
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `textspeed`

用于设置文本速度的滑块。

```
slider1 {
    type: textspeed;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `autospeed`

用于设置自动模式速度的滑块。

```
slider1 {
    type: textspeed;
    x: 0;
    y: 0;
    image-idle:  idle.png;   # 滑块底座条
    image-hover: hover.png;  # 滑块底座条（高亮）
    image-knob:  knob.png;   # 滑块旋钮
}
```

### `preview`

用于预览字体、语言和速度的文本区域。

```
preview1 {
    type: preview;
    x: 0;
    y: 0;
    image-idle: idle.png;
}
```

### `fullscreen`

一个可点击的按钮。
点击时，引擎将进入全屏模式。

```
fullscreen1 {
    type: fullscreen;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
    image-disable: disable.png;  # 当处于全屏模式时显示。
}
```

### `window`

一个可点击的按钮。
点击时，引擎将进入窗口模式。

```
window1 {
    type: window;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
    image-disable: disable.png;  # 当处于窗口模式时显示。
}
```

### `default`

一个可点击的按钮。
点击时，重置所有设置。

```
reset1 {
    type: default;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `savepage`

一个可点击的按钮。
点击时，切换存档界面的页码。

```
savepage1 {
    type: savepage;
    index: 0; # 页码索引 (0-)
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
    image-active:  active.png;  # 当该页面处于活动状态时显示。
}
```

### `save`

一个可点击的按钮。
点击时，执行存档操作。

```
save1 {
    type: save;
    index: 0; # 页面内的索引。实际存档槽位 = 页码 * 每页槽位数 + 索引
    x: 0;
    y: 0;
    image-idle: system/save/save-item-idle.png;
    image-hover: system/save/save-item-hover.png;
    index-x:   10;   # 编号
    index-y:   0;
    date-x:    60;   # 日期
    date-y:    0;
    thumb-x:   27;   # 缩略图
    thumb-y:   77;
    chapter-x: 260;  # 章节标题
    chapter-y: 48;
    msg-x:     260;  # 最后的消息
    msg-y:     96;
}
```

### `load`

一个可点击的按钮。
点击时，执行读档操作。

```
load1 {
    type: load;
    index: 0; # 页面内的索引。实际存档槽位 = 页码 * 每页槽位数 + 索引
    x: 0;
    y: 0;
    image-idle: system/load/load-item-idle.png;
    image-hover: system/load/load-item-hover.png;
    index-x:   10;   # 编号
    index-y:   0;
    date-x:    60;   # 日期
    date-y:    0;
    thumb-x:   27;   # 缩略图
    thumb-y:   77;
    chapter-x: 260;  # 章节标题
    chapter-y: 48;
    msg-x:     260;  # 最后的消息
    msg-y:     96;
}
```

### `auto`

一个可点击的按钮。
点击时，启动自动模式。

```
auto1 {
    type: auto;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `skip`

一个可点击的按钮。
点击时，启动跳过模式。

```
skip1 {
    type: skip;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `title`

一个可点击的按钮。
点击时，引擎将返回到指定的 NovelML 文件。

```
title1 {
    type: title;
    file: title.novel;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `history`

一个可点击的按钮。
显示一条历史消息项。

```
history {
    type: history;
    index: 0; # 页面内的索引
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
    name-x: 20;   # 名字
    name-y: 10;
    text-x:  20;  # 文本（当有名字时）
    text-y:  50;
    msg-x:  20;   # 消息（当没有名字时）
    msg-y:  10;
}
```

### `historyscroll`

历史记录界面的垂直滚动条。

```
scroll1 {
    type: historyscroll;
    x: 1200;
    y: 40;
    image-idle:  system/history/history-bar-idle.png;
    image-hover: system/history/history-bar-hover.png;
    image-knob:  system/history/history-bar-knob.png;
}
```

### `historyscroll-horizontal`

竖排文字历史记录界面的水平滚动条。

```
scroll1 {
    type: historyscroll-horizontal;
    x: 40;
    y: 650;
    image-idle:  system/history/history-bar-idle.png;
    image-hover: system/history/history-bar-hover.png;
    image-knob:  system/history/history-bar-knob.png;
}
```

### `cancel`

一个可点击的按钮。
点击时，GUI 将被取消（关闭）。

```
cancel1 {
    type: cancel;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `var`

用于显示变量值的文本区域。
它用于名字编辑功能。

```
var1 {
    type: var;
    variable: variable_name;
    x: 0;
    y: 0;
    image-idle:    idle.png;
}
```

### `char`

用于向变量追加字符的按钮。
它用于名字编辑功能。

```
char1 {
    type: char;
    variable: variable_name; # 变量名。
    msg: A;                  # 要追加的文本。
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
}
```

### `language`

用于切换语言的按钮。

```
language_english1 {
    type: language;
    langu: en;
    x: 0;
    y: 0;
    image-idle:    idle.png;
    image-hover:   hover.png;
    image-disable: disable.png; # 当英语为当前激活语言时显示。
}
```

---

## 通用按钮选项

| 描述 | 含义 |
|-----------------------|-------------------------------------------------------------|
| type: | 按钮的类型。 |
| x: | 按钮的 X 轴坐标。 |
| y: | 按钮的 Y 轴坐标。 |
| width: | 按钮的宽度。（默认设置为空闲状态图像的宽度） |
| height: | 按钮的高度。 |
| pointse: | 鼠标悬停在按钮上时播放的音效。 |
| clickse: | 点击按钮时播放的音效。 |

### pointse:

`pointse: btn-change.ogg;` 表示当鼠标光标进入按钮区域时播放指定的音效文件。

### clickse:

`clickse: click.ogg;` 表示当按钮被点击时播放指定的音效文件。
