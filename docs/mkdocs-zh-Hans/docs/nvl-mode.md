NVL Mode Reference
==================

Suika3 引擎支持全屏幕的小说风格模式，也就是通常所说的 NVL 模式。

## 概述

```
# 新的页面。
[text action="clear"]
# 文本块样式 1。（目前 VS Code 插件不会对此进行高亮显示）
[text text=
  """
  你好，这是 NVL 模式测试。
  NVL 模式拥有一个全屏样式的消息框。
  默认情况下，每个 text 标签都会自动换行。
  """]
# 文本块样式 2。（VS Code 插件会对此进行高亮显示）
[text text="""
  你好，这是 NVL 模式测试。
  NVL 模式拥有一个全屏样式的消息框。
  默认情况下，每个 text 标签都会自动换行。
"""]
# 内联段落续写。
[text action="inline" text=" 若要接续上一段文字，请使用 inline 行为。"]
# 新的页面。
[text action="clear"]
[text text="请显式地清除消息框。"]
[text text="这就是为什么在 Suika3 中 NVL 被称为页面模式！"]
```

## 切换

请在你的 `start.novel` 文件末尾添加以下两个宏：
```
#
# 用于启动 NVL 模式的宏。
#
[defmacro name="nvl-mode"]
   [text action="hide"]
   [wait time="0.3"] # Wait for the message box to hide.
   [config name="game.novel" value="true"]
   [config name="msgbox.image" value="system/message/msgbox-nvl.png"]
   [config name="msgbox.x" value="0"]
   [config name="msgbox.y" value="0"]
   [config name="msgbox.margin.line" value="60"]
   [config name="namebox.enable" value="false"]
   [config name="choose.box1.idle" value="system/choose/nvl.png"]
   [config name="choose.box1.hover" value="system/choose/nvl.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle-nvl.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover-nvl.anime"]
   [config name="choose.box1.idle" value="system/choose/nvl.png"]
   [config name="choose.box1.hover" value="system/choose/nvl.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle-nvl.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover-nvl.anime"]
   [config name="choose.box1.idle" value="system/choose/nvl.png"]
   [config name="choose.box1.hover" value="system/choose/nvl.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle-nvl.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover-nvl.anime"]
   [config name="click.move" value="true"]
   [text action="clear"]
[endmacro]
#
# 用于返回 ADV 模式的宏。
#
[defmacro name="adv-mode"]
   [text action="hide"]
   [wait time="0.3"] # Wait for the message box to hide.
   [config name="game.novel" value="false"]
   [config name="msgbox.image" value="system/message/msgbox.png"]
   [config name="msgbox.x" value="0"]
   [config name="msgbox.y" value="520"]
   [config name="msgbox.margin.line" value="60"]
   [config name="namebox.enable" value="true"]
   [config name="choose.box1.idle" value="system/choose/idle.png"]
   [config name="choose.box1.hover" value="system/choose/hover.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover.anime"]
   [config name="choose.box1.idle" value="system/choose/idle.png"]
   [config name="choose.box1.hover" value="system/choose/hover.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover.anime"]
   [config name="choose.box1.idle" value="system/choose/idle.png"]
   [config name="choose.box1.hover" value="system/choose/hover.png"]
   [config name="choose.box1.idle_anime" value="system/choose/idle.anime"]
   [config name="choose.box1.hover_anime" value="system/choose/hover.anime"]
   [config name="click.move" value="false"]
[endmacro]
```

然后调用 `nvl-mode` 宏即可切换到 NVL 模式。
```
[callmacro name="nvl-mode"]
```

如果你想切换回 ADV 模式（普通消息模式），请调用 `adv-mode` 宏：
```
[callmacro name="adv-mode"]
```
