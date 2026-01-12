// -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*-

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

//
// Apple Metal View
//

@import MetalKit;

#import "GameViewControllerProtocol.h"

extern bool gameRendererStartFlag;
extern bool gameRendererExitFlag;

@interface GameRenderer : NSObject <MTKViewDelegate>
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView andController:(nonnull id<GameViewControllerProtocol>)controller;
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size;
@end
