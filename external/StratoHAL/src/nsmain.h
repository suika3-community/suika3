/* -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * macOS Main
 */

#ifndef PLATFORM_NSMAIN_H
#define PLATFORM_NSMAIN_H

#import "GameViewControllerProtocol.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@interface ViewController : NSViewController <NSWindowDelegate, GameViewControllerProtocol>
@end

@interface GameView : MTKView
@end

#endif
