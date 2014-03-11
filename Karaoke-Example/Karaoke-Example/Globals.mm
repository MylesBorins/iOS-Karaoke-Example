//
//  Globals.cpp
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#include "Globals.h"

// global variables
GLfloat Globals::waveformWidth = 2;
GLfloat Globals::gfxWidth = 1024;
GLfloat Globals::gfxHeight = 640;

// Scene Graph
SceneGraph * Globals::graph = NULL;

// Physics stuff
GLfloat Globals::scaler = 1;
GLfloat Globals::boundVelocity = 1.5;

// Set defaults for scheduler
UInt32 Globals::clock = 0;
double Globals::tempo = 128;
double beat = (SAMPRATE * 60) / Globals::tempo;
UInt32 Globals::whole = beat * 4;
UInt32 Globals::half = beat * 2;
UInt32 Globals::quarter = beat;
UInt32 Globals::eigth = beat / 2;
UInt32 Globals::sixteenth = beat / 4;
UInt32 Globals::thirtysecond = beat / 8;

// colors
Vector3D Globals::ourWhite( 1, 1, 1 );
Vector3D Globals::ourRed( 1, .5, .5 );
Vector3D Globals::ourBlue( 102.0f/255, 204.0f/255, 1.0f );
Vector3D Globals::ourOrange( 1, .75, .25 );
Vector3D Globals::ourGreen( .7, 1, .45 );
Vector3D Globals::ourGray( .4, .4, .4 );
Vector3D Globals::ourYellow( 1, 1, .25 );
Vector3D Globals::ourSoftYellow( .7, .7, .1 );
Vector3D Globals::ourPurple( .6, .25, .6 );

// globals for recording
BOOL Globals::recording = false;
BOOL Globals::playing = false;
int Globals::recordingLength = 0;
int Globals::playHead = 0;

Float32 * Globals::recordingBuffer = new float[MAX_RECORD_LENGTH];