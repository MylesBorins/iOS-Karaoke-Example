//
//  KaraokeWave.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__KaraokeWave__
#define __Karaoke_Example__KaraokeWave__

#include <iostream>
#import "y-entity.h"
#import "y-waveform.h"
#include "mo_gfx.h"
#include "Globals.h"

using namespace std;

class KaraokeWave: public YEntity
{
public:
    KaraokeWave();
    ~KaraokeWave();
    
public:
    // clean up
    void cleanup();
    // set width
    void setWidth( GLfloat width ) { _width = width; generate(); }
    // set height
    void setHeight( GLfloat height ) { _height = height; generate(); }
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
    
protected:
    // generate vertices
    void generate();
protected:
    // vertex array
    XPoint2D * _vertices;
    // width of waveform
    GLfloat _width;
    // height of waveform
    GLfloat _height;
    unsigned int _numFrames;
};


#endif /* defined(__Karaoke_Example__KaraokeWave__) */
