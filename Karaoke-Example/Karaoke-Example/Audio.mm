//
//  Audio.cpp
//  Karaoke-Example
//
//  Created by thealphanerd on 3/4/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#include "Audio.h"

//-----------------------------------------------------------------------------
// name: audio_callback()
// desc: audio callback, yeah
//-----------------------------------------------------------------------------

void audio_callback( Float32 * buffer, UInt32 numFrames, void * userData )
{
    SAMPLE realBufferSize = numFrames * 2 * sizeof(Float32);
    // zero!!!
    if (Globals::recording && Globals::recordingLength + numFrames * 2 < MAX_RECORD_LENGTH) {
        memcpy(Globals::recordingBuffer + Globals::recordingLength, buffer, realBufferSize);
        Globals::recordingLength += realBufferSize;
    }
    else
    {
        Globals::recording = false;
    }
    memset( buffer, 0, realBufferSize);
    if (Globals::playing && Globals::playHead < Globals::recordingLength) {
        memcpy(buffer, Globals::recordingBuffer + Globals::playHead, realBufferSize);
        Globals::playHead += realBufferSize;
    }
    else
    {
        Globals::playing = false;
    }
}

bool Audio::init()
{
    // init the audio layer
    bool result = MoAudio::init( SAMPRATE, FRAMESIZE, 2 );
    if( !result )
    {
        // something went wrong
        NSLog( @"cannot initialize real-time audio!" );
        // bail out
        return false;
    }
    return true;
}

bool Audio::start()
{
    // start the audio layer, registering a callback method
    bool result = MoAudio::start( audio_callback, NULL );
    if( !result )
    {
        // something went wrong
        NSLog( @"cannot start real-time audio!" );
        // bail out
        return false;
    }
    NSLog( @"starting real-time audio..." );
    return true;
}