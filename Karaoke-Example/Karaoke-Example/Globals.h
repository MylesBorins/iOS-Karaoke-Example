//
//  Globals.h
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#ifndef __Karaoke_Example__Globals__
#define __Karaoke_Example__Globals__

#include <iostream>
#import "mo_audio.h"
#import "mo_gfx.h"
#import "SceneGraph.h"

using namespace std;

#define SAMPRATE 24000
#define FRAMESIZE 1024
#define NUM_CHANNELS 2
#define NUM_ENTITIES 200
#define MAX_RECORD_LENGTH 1440000

class Globals
{
public:
    // global variables
    static GLfloat waveformWidth;
    static GLfloat gfxWidth;
    static GLfloat gfxHeight;
    
    // globals for phsyics
    static GLfloat scaler;
    static GLfloat boundVelocity;
    
    // top level root simulation
    static SceneGraph * graph;
    
    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;
    
    // globals for recording
    static BOOL recording;
    static BOOL playing;
    static int recordingLength;
    static int playHead;
    static Float32 * recordingBuffer;
    
    // simple scheduler
    static UInt32 clock;
    static double tempo;
    static UInt32 thirtysecond;
    static UInt32 sixteenth;
    static UInt32 eigth;
    static UInt32 quarter;
    static UInt32 half;
    static UInt32 whole;
};

#endif /* defined(__Karaoke_Example__Globals__) */
