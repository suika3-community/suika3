// -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*-

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

//
// GameViewControllerProtocol
//  - This is a protocol shared by the macOS and iOS versions of GameViewController's.
//

#import <Foundation/Foundation.h>

@protocol GameViewControllerProtocol <NSObject>

// Screen scaling
- (float)screenScale;
- (CGPoint)screenOffset;
- (CGSize)screenSize;
- (CGPoint)windowPointToScreenPoint:(CGPoint)windowPoint;

// Video playback implementation
- (BOOL)isVideoPlaying;
- (void)playVideoWithPath:(NSString *)path skippable:(BOOL)isSkippable;
- (void)stopVideo;

// Set a window title
- (void)setWindowTitle:(NSString *)name;

// Full screen implementation
- (BOOL)isFullScreen;
- (void)enterFullScreen;
- (void)leaveFullScreen;

@end
