// -*- coding: utf-8; tab-width: 4; indent-tabs-mode: nil; -*-

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

//
// Apple Metal Shader Types
//

#ifndef PLATFORM_GAMESHADERTYPES_H
#define PLATFORM_GAMESHADERTYPES_H

#include <simd/simd.h>

enum GameVertexInputIndex {
    GameVertexInputIndexVertices = 0,
};

enum {
    GameTextureIndexColor = 0,
    GameTextureIndexRule = 1,
};

struct GameVertex {
    vector_float2 xy;
    vector_float2 uv;
    float alpha;
    float padding;  // This is absolutely needed for 64-bit alignments
};

#endif
