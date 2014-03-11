//
//  Renderer.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__Renderer__
#define __Karaoke_Example__Renderer__

#import "mo_audio.h"
#import "mo_gfx.h"
#import "mo_touch.h"
#import "y-entity.h"
#import "Globals.h"
#import <vector>
#import "Simulation.h"

// initialize the engine (audio, grx, interaction)
void KaraokeInit();
// TODO: cleanup
// set graphics dimensions
void KaraokeSetDims( GLfloat width, GLfloat height );
// draw next frame of graphics
void KaraokeRender();

#endif /* defined(__Karaoke_Example__Renderer__) */
