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

YScoreReader * Audio::_scoreReader = nullptr;
stk::Mandolin ** Audio::_mandolins = new stk::Mandolin*[5];

void audio_callback( Float32 * buffer, UInt32 numFrames, void * userData )
{
    SAMPLE realBufferSize = numFrames * 2 * sizeof(Float32);
    if (Globals::recording && Globals::recordingLength + numFrames * 2 < MAX_RECORD_LENGTH) {
        for (int i = 0; i < numFrames; i ++)
        {
            Globals::recordingBuffer[i + Globals::recordingLength] = buffer[i * 2];
        }
        Globals::recordingLength += numFrames;
    }
    else if (Globals::recording)
    {
        // Fire an event... isn't that neat?
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter]
             postNotificationName:@"ToggleRecord"
             object:NULL];
        });
    }
    
    // zero!!!
    memset( buffer, 0, realBufferSize);
    
    // NASTY MIDI OMG
    
    for (int j = 0; j < 5; j++)
    {
        for (int i =0; i < numFrames; i ++)
        {
            float srate = SAMPRATE;
            const NoteEvent * ne = Audio::_scoreReader->current(j + 3);
            if(ne && ne->time - (Globals::clock + i) / srate <= 0) {
                Audio::_mandolins[j]->noteOn(MoFun::midi2freq(ne->data2), 0.5);
                Audio::_scoreReader->nextNoteOn(j + 3);
            }
            buffer[i*2] += Audio::_mandolins[j]->tick();
            buffer[i*2 + 1] += buffer[i*2];
        }
    }
    
    for (int i =0; i < numFrames; i ++)
    {
        buffer[i*2] /= 5;
        buffer[i*2 + 1] /= 5;
    }
    
    
    if (Globals::playing && Globals::playHead < Globals::recordingLength) {
        for (int i = 0; i < numFrames; i++)
        {
            buffer[i * 2] = buffer[i*2 +1] = Globals::recordingBuffer[i + Globals::playHead];
        }
        Globals::playHead += numFrames;
    }
    else if (Globals::playing)
    {
        // Stop Playback and reset playhead
        Globals::playHead = 0;
        // Use dispatch_async to send message from main queue
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter]
             postNotificationName:@"TogglePlay"
             object:NULL];
        });
    }
    Globals::clock += numFrames;
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
    
    for (int i = 0; i < 5; i++)
    {
        _mandolins[i] = new stk::Mandolin(50);
    }
    _scoreReader = new YScoreReader();
    
    NSString *midiFilePath = [[NSBundle mainBundle]
                              pathForResource:@"thing_called_love"
                              ofType:@"kar"];
    const char *path = [midiFilePath UTF8String];
    _scoreReader->load(path);
    Globals::tempo = _scoreReader->getBPM();
    std::vector<LyricEvent *> lyricEvents = _scoreReader->getLyricEvents(2);
    std::cerr << lyricEvents[0]->lyric << std::endl;
    std::cerr << lyricEvents[0]->time << std::endl;
    std::cerr << lyricEvents[0]->endTime << std::endl;
    
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