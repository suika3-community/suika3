/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * iOS Main
 */

#ifndef PLATFORM_UIMAIN_H
#define PLATFORM_UIMAIN_H

@import UIKit;
@import MetalKit;

#import "GameViewControllerProtocol.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@interface ViewController : UIViewController <GameViewControllerProtocol>
@end

@interface GameView : MTKView
@property float scale;
@property float left;
@property float top;
@end

#endif
