动画
=====

Anime 是一项通过 anime 标签来播放基于图层动画的功能。

## Anime 文件

Anime 文件是一种文本文件，用于描述一系列图层变换。

## 序列

若要在 1 秒内将消息框向右移动 100 像素，请在 anime 文件中编写以下序列。

```
# 一个代码块描述了一个动画序列。
# 代码块的名称可以是您喜欢的任何内容，它不会影响任何功能。
move {
    # 这是图层指定符。
    layer: msg;
    # 这是时间指定符。（单位：秒）
    start: 0.0;
    end: 1.0;
    # 这是起始位置指定符。'r0' 表示相对 '0'。
    from-x: r0;
    from-y: r0;
    # 这是起始透明度指定符。
    from-a: 255;
    # 这是最终位置指定符。'r100' 表示相对 '100'。
    to-x: r100;
    to-y: r0;
    # 这是最终透明度指定符。
    to-a: 255;
}
```

## 图层结构

以下是我们自下而上的图层结构。

| 图层名称 | 描述 |
|------------------|--------------------------------------------|
| bg | 背景图层。 |
| bg2 | 第二背景（用于无缝滚动） |
| efb1 ~ efb4 | 后方特效 1~4 |
| chb | 后方居中角色 |
| chb-eye | 后方居中角色 |
| chb-lip | 后方居中角色 |
| chl | 左侧角色 |
| chl-eye | 左侧角色 |
| chl-lip | 左侧角色 |
| chlc | 左中角色 |
| chlc-eye | 左中角色 |
| chlc-lip | 左中角色 |
| chr | 右侧角色 |
| chr-eye | 右侧角色 |
| chr-lip | 右侧角色 |
| chrc | 右中角色 |
| chrc-eye | 右中角色 |
| chrc-lip | 右中角色 |
| eff1 ~ eff4 | 前方特效 1 ~ 4 |
| msgbox | 消息框 |
| namebox | 姓名框 |
| choose1-idle ~ choose8-idle | 选项框 1 ~ 8（静止） |
| choose1-hover ~choose8-hover | 选项框 1 ~ 8（悬停） |
| chf | 角色面部 |
| chf-eye | 角色面部 |
| chf-lip | 角色面部 |
| click | 点击动画 |
| auto | 自动模式横幅 |
| skip | 跳过模式横幅 |
| text1 ~ text8 | 文本图层 1 ~ 8 |
| gui-button-1 ~ gui-button-32 | GUI 按钮 ID 1 ~ 32 |

## 缩放与旋转

```
# 在 3 秒内将 `effect1` 图层放大至 2.0 倍并旋转 360 度。
test1 {
    layer: effect1;

    start: 0.0;
    end: 3.0;
    from-x: 0;
    from-y: 400;
    from-a: 255;
    to-x: 0;
    to-y: 400;
    to-a: 255;
    # 缩放与旋转中心点
    center-x: 600;
    center-y: 100;
    # 缩放比例
    from-scale-x: 1.0;
    from-scale-y: 1.0;
    to-scale-x: 2.0;
    to-scale-y: 2.0;

    # 旋转角度（正数为向右，负数为向左）
    from-rotate: 0.0;
    to-rotate: -360;
}
# 反向操作。
test2 {
    layer: effect1;
    start: 3.0;
    end: 6.0;
    from-x: 0;
    from-y: 400;
    from-a: 255;
    to-x: 0;
    to-y: 400;
    to-a: 255;
    center-x: 600;
    center-y: 100;
    from-scale-x: 2.0;
    to-scale-x: 1.0;
    from-scale-y: 2.0;
    to-scale-y: 1.0;
    from-rotate: -360;
    to-rotate: 0;
}
```
