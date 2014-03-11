//
//  Renderer.cpp
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#include "Renderer.h"

// function prototypes
void KaraokeInitGraph();

// initialize the engine (audio, grx, interaction)
void KaraokeInit()
{
    NSLog( @"init..." );
    
    // generate texture name
//    glGenTextures( 1, Globals::texture );
    // bind the texture
//    glBindTexture( GL_TEXTURE_2D, *Globals::texture );
    // setting parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // Init simulation
    KaraokeInitGraph();
    Simulation::init();
    // load the texture
//    MoGfx::loadTexture( @"texture", @"png" );
}

// initialize the simulation()
void KaraokeInitGraph()
{
    // instantiate simulation
    Globals::graph = new SceneGraph();
    YEntity * root = &Globals::graph->root();
    KaraokeWave * wave = new KaraokeWave();
    wave->active = true;
    wave->col.setAll(0);
    wave->setWidth(1);
    wave->setHeight(0.5);
    root->addChild(wave);
}

// set graphics dimensions
void KaraokeSetDims( GLfloat width, GLfloat height )
{
    NSLog( @"set dims: %f %f", width, height );
    Globals::gfxWidth = width;
    Globals::gfxHeight = height;
}

// draw next frame of graphics
void KaraokeRender()
{
    // refresh current time reading
    MoGfx::getCurrentTime( true );
    
    
    // projection
    glMatrixMode( GL_PROJECTION );
    // reset
    glLoadIdentity();
    // alternate
    GLfloat ratio = Globals::gfxWidth / Globals::gfxHeight;
    glOrthof( -ratio, ratio, -1, 1, -1, 1 );
    // orthographic
    // glOrthof( -g_gfxWidth/2, g_gfxWidth/2, -g_gfxHeight/2, g_gfxHeight/2, -1.0f, 1.0f );
    // modelview
    glMatrixMode( GL_MODELVIEW );
    // reset
    // glLoadIdentity();
    
    glClearColor( 1, 1, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // push
    glPushMatrix();
    // cascade simulation
    Globals::graph->systemCascade();
    
    // pop
    glPopMatrix();
}
