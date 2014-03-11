//
//  KaraokeWave.cpp
//  Karaoke-Example
//
//  Created by thealphanerd on 3/10/14.
//  Copyright (c) 2014 thealphanerd. All rights reserved.
//

#include "KaraokeWave.h"

KaraokeWave::KaraokeWave()
{
    _vertices = NULL;
    //default width and height
    _width = 2;
    _height = 2;
    _numFrames = Globals::recordingLength;
    _vertices = new XPoint2D[_numFrames];
};

KaraokeWave::~KaraokeWave()
{

};

//-----------------------------------------------------------------------------
// name: cleanup()
// desc: clean up
//-----------------------------------------------------------------------------
void KaraokeWave::cleanup()
{
    // check
    if( _vertices != NULL ) SAFE_DELETE_ARRAY( _vertices );
    _numFrames = Globals::recordingLength;
    _vertices = new XPoint2D[_numFrames];
}

//-----------------------------------------------------------------------------
// name: update()
// desc: update
//-----------------------------------------------------------------------------
void KaraokeWave::update( YTimeInterval dt )
{
    if (_numFrames != Globals::recordingLength)
    {
        cleanup();
        generate();
    }
}

//-----------------------------------------------------------------------------
// name: render()
// desc: render
//-----------------------------------------------------------------------------
void KaraokeWave::render()
{
    // disable light
    glDisable( GL_LIGHTING );
    // set blend function
//    glBlendFunc( GL_ONE, GL_ONE );
    // enable blend
//    glEnable( GL_BLEND );
	
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // set pointer
    glVertexPointer( 2, GL_FLOAT, 0, _vertices );
    // draw it
    glDrawArrays( GL_LINE_STRIP, 0, _numFrames );
    
    // disable client state
    glDisableClientState( GL_VERTEX_ARRAY );
    // disable blend
//    glDisable( GL_BLEND );
}

//-----------------------------------------------------------------------------
// name: generate()
// desc: generate vertices
//-----------------------------------------------------------------------------
void KaraokeWave::generate()
{
    
    SAMPLE x = -_width/2;
    SAMPLE inc = _width / _numFrames;
    
    // iterate over buffer
    for( int i = 0; i < _numFrames; i++ )
    {
        // x coordinate
        _vertices[i].x = x; x += inc;
        // y coordinate
        _vertices[i].y = Globals::recordingBuffer[i] * _height;
    }
}