// -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; -*-

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */
 
//
// Apple Metal View
//

@import simd;
@import MetalKit;

#import "GameRenderer.h"
#import "GameShaderTypes.h"
#import "GameViewControllerProtocol.h"

// Base
#import "stratohal/platform.h"

// POSIX
#import <sys/time.h>

// HAL implementation
#import "aunit.h"

//
// nsmain.m or uimain.m
//
extern int screen_width;
extern int screen_height;

//
// Shader
//
extern const char *gameShader;

//
// GameView Objects (accessed by the HAL functions)
//
static id<GameViewControllerProtocol> theViewController;
static MTKView *theMTKView;
static id<MTLDevice> theDevice;
static id<MTLRenderPipelineState> theNormalPipelineState;
static id<MTLRenderPipelineState> theAddPipelineState;
static id<MTLRenderPipelineState> theSubPipelineState;
static id<MTLRenderPipelineState> theDimPipelineState;
static id<MTLRenderPipelineState> theRulePipelineState;
static id<MTLRenderPipelineState> theMeltPipelineState;
static id<MTLCommandBuffer> theCommandBuffer;
static id<MTLBlitCommandEncoder> theBlitEncoder;
static id<MTLRenderCommandEncoder> theRenderEncoder;
static struct image *theInitialUploadArray[128];
static int theInitialUploadArrayCount;
static id<MTLTexture> thePurgeArray[128];
static int thePurgeArrayCount;
static dispatch_semaphore_t in_flight_semaphore;

//
// Start/Quit flags.
//
bool gameRendererStartFlag;
bool gameRendererExitFlag;

//
// Forward declarations
//
static BOOL runFrame(void);
static void drawPrimitives(int dst_left, int dst_top, int dst_width, int dst_height,
                           struct image *src_image,
                           struct image *rule_image,
                           int src_left, int src_top, int src_width, int src_height,
                           int alpha,
                           id<MTLRenderPipelineState> pipeline);
static void drawPrimitives3D(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                             struct image *src_image,
                             struct image *rule_image,
                             int src_left, int src_top, int src_width, int src_height,
                             int alpha,
                             id<MTLRenderPipelineState> pipeline);

//
// GameRender
//

@interface GameRenderer ()
@end

@implementation GameRenderer
{
    id<MTLCommandQueue> _commandQueue;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView andController:(nonnull id<GameViewControllerProtocol>)controller {
    NSError *error = NULL;

    self = [super init];
    if(self == nil)
        return nil;

    theMTKView = mtkView;
    theDevice = mtkView.device;
    theViewController = controller;

    // Load shaders.
    id<MTLLibrary> defaultLibrary = [theDevice newLibraryWithSource:[NSString stringWithUTF8String:gameShader] options:nil error:&error];
    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    if (!vertexFunction) {
        NSLog(@"vertexFunction not found!");
    }
#if 0
    id<MTLLibrary> defaultLibrary = [theDevice newDefaultLibrary];
#endif
    NSAssert(defaultLibrary, @"Failed to create library: %@", error);

    // Construct the normal shader pipeline.
    MTLRenderPipelineDescriptor *normalPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    normalPipelineStateDescriptor.label = @"Normal Texturing Pipeline";
    normalPipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    normalPipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentNormalShader"];
    normalPipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    normalPipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
    normalPipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    normalPipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    normalPipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    normalPipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
    normalPipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    normalPipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor =  MTLBlendFactorOne;
    normalPipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theNormalPipelineState = [theDevice newRenderPipelineStateWithDescriptor:normalPipelineStateDescriptor error:&error];
    NSAssert(theNormalPipelineState, @"Failed to create pipeline state: %@", error);

    // Construct the add shader pipeline.
    MTLRenderPipelineDescriptor *addPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    addPipelineStateDescriptor.label = @"Add Texturing Pipeline";
    addPipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    addPipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentNormalShader"];
    addPipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    addPipelineStateDescriptor.colorAttachments[0].blendingEnabled = TRUE;
    addPipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    addPipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    addPipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    addPipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
    addPipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOne;
    addPipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor =  MTLBlendFactorOne;
    addPipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theAddPipelineState = [theDevice newRenderPipelineStateWithDescriptor:addPipelineStateDescriptor error:&error];
    NSAssert(theAddPipelineState, @"Failed to create pipeline state: %@", error);

    // Construct the sub shader pipeline.
    MTLRenderPipelineDescriptor *subPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    subPipelineStateDescriptor.label = @"Add Texturing Pipeline";
    subPipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    subPipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentNormalShader"];
    subPipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    subPipelineStateDescriptor.colorAttachments[0].blendingEnabled = TRUE;
    subPipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationReverseSubtract;
    subPipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    subPipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    subPipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorZero;
    subPipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOne;
    subPipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor =  MTLBlendFactorOne;
    subPipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theSubPipelineState = [theDevice newRenderPipelineStateWithDescriptor:subPipelineStateDescriptor error:&error];
    NSAssert(theSubPipelineState, @"Failed to create pipeline state: %@", error);

    // Construct a dim shader pipeline.
    MTLRenderPipelineDescriptor *dimPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    dimPipelineStateDescriptor.label = @"Dim Texturing Pipeline";
    dimPipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    dimPipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentDimShader"];
    dimPipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    dimPipelineStateDescriptor.colorAttachments[0].blendingEnabled = TRUE;
    dimPipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    dimPipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    dimPipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    dimPipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
    dimPipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    dimPipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor =  MTLBlendFactorOne;
    dimPipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theDimPipelineState = [theDevice newRenderPipelineStateWithDescriptor:dimPipelineStateDescriptor error:&error];
    NSAssert(theDimPipelineState, @"Failed to create pipeline state: %@", error);

    // Construct a rule shader pipeline.
    MTLRenderPipelineDescriptor *rulePipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    rulePipelineStateDescriptor.label = @"Rule Texturing Pipeline";
    rulePipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    rulePipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentRuleShader"];
    rulePipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    rulePipelineStateDescriptor.colorAttachments[0].blendingEnabled = TRUE;
    rulePipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    rulePipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    rulePipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    rulePipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
    rulePipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    rulePipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
    rulePipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theRulePipelineState = [theDevice newRenderPipelineStateWithDescriptor:rulePipelineStateDescriptor error:&error];
    NSAssert(theRulePipelineState, @"Failed to create pipeline state: %@", error);

    // Construct a melt shader pipeline.
    MTLRenderPipelineDescriptor *meltPipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    meltPipelineStateDescriptor.label = @"Melt Texturing Pipeline";
    meltPipelineStateDescriptor.vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];
    meltPipelineStateDescriptor.fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentMeltShader"];
    meltPipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
    meltPipelineStateDescriptor.colorAttachments[0].blendingEnabled = TRUE;
    meltPipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    meltPipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    meltPipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    meltPipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorOne;
    meltPipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    meltPipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
    meltPipelineStateDescriptor.depthAttachmentPixelFormat = theMTKView.depthStencilPixelFormat;
    theMeltPipelineState = [theDevice newRenderPipelineStateWithDescriptor:meltPipelineStateDescriptor error:&error];
    NSAssert(theMeltPipelineState, @"Failed to create pipeline state: %@", error);

    // Create a command queue.
    _commandQueue = [theDevice newCommandQueue];

    // TODO:
    in_flight_semaphore = dispatch_semaphore_create(1);

    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    if (!gameRendererStartFlag)
        return;
    if (gameRendererExitFlag)
        return;
    if (view.currentRenderPassDescriptor == nil)
        return;
    if ([theViewController isVideoPlaying]) {
        if(!runFrame())
            gameRendererExitFlag = true;
        return;
    }

    // Create a command buffer.
    theCommandBuffer = [_commandQueue commandBuffer];
    theCommandBuffer.label = @"MyCommand";

    // Nil-ify the encoders.
    theBlitEncoder = nil;
    theRenderEncoder = nil;

    // Create textures for images that are loaded before the first rendering.
    for (int i = 0; i < theInitialUploadArrayCount; i++)
        if (theInitialUploadArray[i] != NULL)
            notify_image_update(theInitialUploadArray[i]);
    theInitialUploadArrayCount = 0;

    // Make a barrier.
    dispatch_semaphore_wait(in_flight_semaphore, DISPATCH_TIME_FOREVER);
    __block dispatch_semaphore_t block_sema = in_flight_semaphore;
    [theCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
            (void)buffer;
        dispatch_semaphore_signal(block_sema);
    }];

    // Create an array for textures to be destroyed.
    thePurgeArrayCount = 0;

    // Run a frame event and do rendering.
    if (!gameRendererExitFlag) {
        if(!runFrame())
            gameRendererExitFlag = true;
    }

    // End encodings.
    if (theBlitEncoder != nil)
        [theBlitEncoder endEncoding];
    if (theRenderEncoder != nil)
        [theRenderEncoder endEncoding];

    // Schedule a rendering to the current drawable.
    [theCommandBuffer presentDrawable:view.currentDrawable];
    
    // Push the command buffer to the GPU.
    [theCommandBuffer commit];

    // Synchronize.
    [theCommandBuffer waitUntilCompleted];
    
    // Set the destroyed textures purgeable.
    for (int i = 0; i < thePurgeArrayCount; i++) {
        if (thePurgeArray[i] == NULL)
            continue;
        [thePurgeArray[i] setPurgeableState:MTLPurgeableStateEmpty];
        thePurgeArray[i] = NULL;
    }
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
}

@end

//
// HAL
//

//
// Run a frame.
//
static BOOL runFrame(void)
{
    if(!on_event_frame())
        return FALSE;

    return TRUE;
}

//
// テクスチャの更新を通知する
//
void notify_image_update(struct image *img)
{
    if (theCommandBuffer == nil) {
        assert(theInitialUploadArrayCount < 128);
        assert(img->width > 0 && img->width < 4096);
        assert(img->height > 0 && img->height < 4096);
        theInitialUploadArray[theInitialUploadArrayCount++] = img;
        return;
    }

    MTLRegion region = {{ 0, 0, 0 }, {(NSUInteger)img->width, (NSUInteger)img->height, 1}};
    id<MTLTexture> texture = nil;
    
    if (img->texture == NULL) {
        // For the first time, create a texture.
        MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
        textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
        textureDescriptor.width = (NSUInteger)img->width;
        textureDescriptor.height = (NSUInteger)img->height;
        texture = [theDevice newTextureWithDescriptor:textureDescriptor];
        img->texture = (__bridge void * _Nonnull)(texture);
        CFBridgingRetain(texture);
    } else {
        // Get the existing texture.
        texture = (__bridge id<MTLTexture>)(img->texture);
    }
    
    // Upload the pixels.
    assert(theRenderEncoder == nil);
    if (theBlitEncoder == nil) {
        theBlitEncoder = [theCommandBuffer blitCommandEncoder];
        theBlitEncoder.label = @"Texture Encoder";
    }
    [texture replaceRegion:region mipmapLevel:0 withBytes:img->pixels bytesPerRow:(NSUInteger)(img->width * 4)];
}

//
// テクスチャの破棄を通知する
//
void notify_image_free(struct image *img)
{
    // Metal初期化前に作成され、Metal初期化前に削除されるイメージを、theInitialUploadArrayから削除する
    for (int i = 0; i < theInitialUploadArrayCount; i++) {
        if (theInitialUploadArray[i] == img) {
            theInitialUploadArray[i] = NULL;
            return;
        }
    }
    
    thePurgeArray[thePurgeArrayCount++] = (__bridge id<MTLTexture>)(img->texture);
    CFBridgingRelease(img->texture);
}

//
// Render an image to the screen with the "normal" pipeline.
//
void render_image_normal(int dst_left, int dst_top, int dst_width, int dst_height, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    if (dst_width == -1)
        dst_width = src_image->width;
    if (dst_height == -1)
        dst_height = src_image->height;
    if (src_width == -1)
        src_width = src_image->width;
    if (src_height == -1)
        src_height = src_image->height;

    drawPrimitives(dst_left, dst_top, dst_width, dst_height, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theNormalPipelineState);
}

//
// Render an image to the screen with the "add" pipeline.
//
void render_image_add(int dst_left, int dst_top, int dst_width, int dst_height, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    if (dst_width == -1)
        dst_width = src_image->width;
    if (dst_height == -1)
        dst_height = src_image->height;
    if (src_width == -1)
        src_width = src_image->width;
    if (src_height == -1)
        src_height = src_image->height;

    drawPrimitives(dst_left, dst_top, dst_width, dst_height, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theAddPipelineState);
}

//
// Render an image to the screen with the "sub" pipeline.
//
void render_image_sub(int dst_left, int dst_top, int dst_width, int dst_height, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    if (dst_width == -1)
        dst_width = src_image->width;
    if (dst_height == -1)
        dst_height = src_image->height;
    if (src_width == -1)
        src_width = src_image->width;
    if (src_height == -1)
        src_height = src_image->height;

    drawPrimitives(dst_left, dst_top, dst_width, dst_height, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theSubPipelineState);
}


//
// Render an image to the screen with the "dim" pipeline.
//
void render_image_dim(int dst_left, int dst_top, int dst_width, int dst_height, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    if (dst_width == -1)
        dst_width = src_image->width;
    if (dst_height == -1)
        dst_height = src_image->height;
    if (src_width == -1)
        src_width = src_image->width;
    if (src_height == -1)
        src_height = src_image->height;

    drawPrimitives(dst_left, dst_top, dst_width, dst_height, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theDimPipelineState);
}

//
// Render an image to the screen with the "rule" pipeline.
//
void render_image_rule(struct image *src_img, struct image *rule_img, int threshold)
{
    drawPrimitives(0, 0, src_img->width, src_img->height, src_img, rule_img, 0, 0, src_img->width, src_img->height, threshold, theRulePipelineState);
}

//
// Render an image to the screen with the "melt" pipeline.
//
void render_image_melt(struct image *src_img, struct image *rule_img, int progress)
{
    drawPrimitives(0, 0, src_img->width, src_img->height, src_img, rule_img, 0, 0, src_img->width, src_img->height, progress, theMeltPipelineState);
}

//
// Draw a rectangle with a specified pipeline.
//
static void drawPrimitives(int dst_left, int dst_top, int dst_width, int dst_height,
                           struct image *src_image,
                           struct image *rule_image,
                           int src_left, int src_top, int src_width, int src_height,
                           int alpha,
                           id<MTLRenderPipelineState> pipeline)
{
    // Calc the half size of the window.
    float hw = (float)screen_width / 2.0f;
    float hh = (float)screen_height / 2.0f;

    // Get the texture size.
    float tw = (float)src_image->width;
    float th = (float)src_image->height;
    
    // The vertex shader input
    float vsIn[24];

    // Set the left top vertex.
    vsIn[0] = ((float)dst_left - hw) / hw;   // X (-1.0 to 1.0, left to right)
    vsIn[1] = -((float)dst_top - hh) / hh;   // Y (-1.0 to 1.0, bottom to top)
    vsIn[2] = (float)src_left / tw;          // U (0.0 to 1.0, left to right)
    vsIn[3] = (float)src_top / th;           // V (0.0 to 1.0, top to bottom)
    vsIn[4] = (float)alpha / 255.0f;         // Alpha (0.0 to 1.0)
    vsIn[5] = 0;                             // Padding for a 64-bit boundary

    // Set the right top vertex.
    vsIn[6] = ((float)dst_left + (float)dst_width - hw) / hw;    // X (-1.0 to 1.0, left to right)
    vsIn[7] = -((float)dst_top - hh) / hh;                   // Y (-1.0 to 1.0, bottom to top)
    vsIn[8] = (float)(src_left + src_width) / tw;                // U (0.0 to 1.0, left to right)
    vsIn[9] = (float)(src_top) / th;                         // V (0.0 to 1.0, top to bottom)
    vsIn[10] = (float)alpha / 255.0f;                        // Alpha (0.0 to 1.0)
    vsIn[11] = 0;                                            // Padding for a 64-bit boundary
    
    // Set the left bottom vertex.
    vsIn[12] = ((float)dst_left - hw) / hw;                  // X (-1.0 to 1.0, left to right)
    vsIn[13] = -((float)dst_top + (float)dst_height - hh) / hh;  // Y (-1.0 to 1.0, bottom to top)
    vsIn[14] = (float)src_left / tw;                         // U (0.0 to 1.0, left to right)
    vsIn[15] = (float)(src_top + src_height) / th;               // V (0.0 to 1.0, top to bottom)
    vsIn[16] = (float)alpha / 255.0f;                        // Alpha (0.0 to 1.0)
    vsIn[17] = 0;                                            // Padding for a 64-bit boundary

    // Set the right bottom vertex.
    vsIn[18] = ((float)dst_left + (float)dst_width - hw) / hw;   // X (-1.0 to 1.0, left to right)
    vsIn[19] = -((float)dst_top + (float)dst_height - hh) / hh;  // Y (-1.0 to 1.0, bottom to top)
    vsIn[20] = (float)(src_left + src_width) / tw;               // U (0.0 to 1.0, left to right)
    vsIn[21] = (float)(src_top + src_height) / th;               // V (0.0 to 1.0, top to bottom)
    vsIn[22] = (float)alpha / 255.0f;                        // Alpha (0.0 to 1.0)
    vsIn[23] = 0;                                            // Padding for a 64-bit boundary

    // Upload textures if they are pending.
    if (theBlitEncoder != nil) {
        [theBlitEncoder endEncoding];
        theBlitEncoder = nil;
    }

    // Draw two triangles.
    if (theRenderEncoder == nil) {
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
        theRenderEncoder = [theCommandBuffer renderCommandEncoderWithDescriptor:theMTKView.currentRenderPassDescriptor];
        theRenderEncoder.label = @"MyRenderEncoder";

        MTLViewport viewport;
        viewport.originX = [theViewController screenOffset].x;
        viewport.originY = [theViewController screenOffset].y;
        viewport.width = [theViewController screenSize].width;
        viewport.height = [theViewController screenSize].height;
        viewport.zfar = 0;
        viewport.znear = 0;
        [theRenderEncoder setViewport:viewport];
    }
    [theRenderEncoder setRenderPipelineState:pipeline];
    id<MTLTexture> tex1 = (__bridge id<MTLTexture> _Nullable)(src_image->texture);
    id<MTLTexture> tex2 = rule_image != NULL ? (__bridge id<MTLTexture> _Nullable)(rule_image->texture) : nil;
    [theRenderEncoder setVertexBytes:vsIn length:sizeof(vsIn) atIndex:GameVertexInputIndexVertices];
    [theRenderEncoder setFragmentTexture:tex1 atIndex:GameTextureIndexColor];
    if (tex2 != nil)
        [theRenderEncoder setFragmentTexture:tex2 atIndex:GameTextureIndexRule];
    [theRenderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
}

//
// Renders an image to the screen with the "normal" shader pipeline.
//
void render_image_3d_normal(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    drawPrimitives3D(x1, y1, x2, y2, x3, y3, x4, y4, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theNormalPipelineState);
}

//
// Renders an image to the screen with the "add" shader pipeline.
//
void render_image_3d_add(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    drawPrimitives3D(x1, y1, x2, y2, x3, y3, x4, y4, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theAddPipelineState);
}

//
// Renders an image to the screen with the "sub" shader pipeline.
//
void render_image_3d_sub(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    drawPrimitives3D(x1, y1, x2, y2, x3, y3, x4, y4, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theSubPipelineState);
}

//
// Renders an image to the screen with the "dim" shader pipeline.
//
void render_image_3d_dim(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, struct image *src_image, int src_left, int src_top, int src_width, int src_height, int alpha)
{
    drawPrimitives3D(x1, y1, x2, y2, x3, y3, x4, y4, src_image, NULL, src_left, src_top, src_width, src_height, alpha, theDimPipelineState);
}

//
// Draw a rectangle with a specified pipeline.
//
static void drawPrimitives3D(float x1, float y1,
                             float x2, float y2,
                             float x3, float y3,
                             float x4, float y4,
                             struct image *src_image,
                             struct image *rule_image,
                             int src_left, int src_top, int src_width, int src_height,
                             int alpha,
                             id<MTLRenderPipelineState> pipeline)
{
    // Calc the half size of the window.
    float hw = (float)screen_width / 2.0f;
    float hh = (float)screen_height / 2.0f;

    // Get the texture size.
    float tw = (float)src_image->width;
    float th = (float)src_image->height;
    
    // The vertex shader input
    float vsIn[24];

    // Set the left top vertex.
    vsIn[0] = ((float)x1 - hw) / hw;        // X (-1.0 to 1.0, left to right)
    vsIn[1] = -((float)y1 - hh) / hh;       // Y (-1.0 to 1.0, bottom to top)
    vsIn[2] = (float)src_left / tw;         // U (0.0 to 1.0, left to right)
    vsIn[3] = (float)src_top / th;          // V (0.0 to 1.0, top to bottom)
    vsIn[4] = (float)alpha / 255.0f;        // Alpha (0.0 to 1.0)
    vsIn[5] = 0;                            // Padding for a 64-bit boundary

    // Set the right top vertex.
    vsIn[6] = ((float)x2 - hw) / hw;               // X (-1.0 to 1.0, left to right)
    vsIn[7] = -((float)y2 - hh) / hh;              // Y (-1.0 to 1.0, bottom to top)
    vsIn[8] = (float)(src_left + src_width) / tw;  // U (0.0 to 1.0, left to right)
    vsIn[9] = (float)(src_top) / th;               // V (0.0 to 1.0, top to bottom)
    vsIn[10] = (float)alpha / 255.0f;              // Alpha (0.0 to 1.0)
    vsIn[11] = 0;                                  // Padding for a 64-bit boundary

    // Set the left bottom vertex.
    vsIn[12] = ((float)x3 - hw) / hw;               // X (-1.0 to 1.0, left to right)
    vsIn[13] = -((float)y3 - hh) / hh;              // Y (-1.0 to 1.0, bottom to top)
    vsIn[14] = (float)src_left / tw;                // U (0.0 to 1.0, left to right)
    vsIn[15] = (float)(src_top + src_height) / th;  // V (0.0 to 1.0, top to bottom)
    vsIn[16] = (float)alpha / 255.0f;               // Alpha (0.0 to 1.0)
    vsIn[17] = 0;                                   // Padding for a 64-bit boundary

    // Set the right bottom vertex.
    vsIn[18] = ((float)x4 - hw) / hw;               // X (-1.0 to 1.0, left to right)
    vsIn[19] = -((float)y4 - hh) / hh;              // Y (-1.0 to 1.0, bottom to top)
    vsIn[20] = (float)(src_left + src_width) / tw;  // U (0.0 to 1.0, left to right)
    vsIn[21] = (float)(src_top + src_height) / th;  // V (0.0 to 1.0, top to bottom)
    vsIn[22] = (float)alpha / 255.0f;               // Alpha (0.0 to 1.0)
    vsIn[23] = 0;                                   // Padding for a 64-bit boundary

    // Upload textures if they are pending.
    if (theBlitEncoder != nil) {
        [theBlitEncoder endEncoding];
        theBlitEncoder = nil;
    }

    // Draw two triangles.
    if (theRenderEncoder == nil) {
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        theMTKView.currentRenderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
        theRenderEncoder = [theCommandBuffer renderCommandEncoderWithDescriptor:theMTKView.currentRenderPassDescriptor];
        theRenderEncoder.label = @"MyRenderEncoder";

        MTLViewport viewport;
        viewport.originX = [theViewController screenOffset].x;
        viewport.originY = [theViewController screenOffset].y;
        viewport.width = [theViewController screenSize].width;
        viewport.height = [theViewController screenSize].height;
        viewport.zfar = 0;
        viewport.znear = 0;
        [theRenderEncoder setViewport:viewport];
    }
    [theRenderEncoder setRenderPipelineState:pipeline];
    id<MTLTexture> tex1 = (__bridge id<MTLTexture> _Nullable)(src_image->texture);
    id<MTLTexture> tex2 = rule_image != NULL ? (__bridge id<MTLTexture> _Nullable)(rule_image->texture) : nil;
    [theRenderEncoder setVertexBytes:vsIn length:sizeof(vsIn) atIndex:GameVertexInputIndexVertices];
    [theRenderEncoder setFragmentTexture:tex1 atIndex:GameTextureIndexColor];
    if (tex2 != nil)
        [theRenderEncoder setFragmentTexture:tex2 atIndex:GameTextureIndexRule];
    [theRenderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
}
