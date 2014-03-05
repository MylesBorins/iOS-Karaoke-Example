/*----------------------------------------------------------------------------
  MoMu: A Mobile Music Toolkit
  Copyright (c) 2010 Nicholas J. Bryan, Jorge Herrera, Jieun Oh, and Ge Wang
  All rights reserved.
    http://momu.stanford.edu/toolkit/
 
  Mobile Music Research @ CCRMA
  Music, Computing, Design Group
  Stanford University
    http://momu.stanford.edu/
    http://ccrma.stanford.edu/groups/mcd/
 
 MoMu is distributed under the following BSD style open source license:
 
 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The authors encourage users of MoMu to include this copyright notice,
 and to let us know that you are using MoMu. Any person wishing to 
 distribute modifications to the Software is encouraged to send the 
 modifications to the original authors so that they can be incorporated 
 into the canonical version.
 
 The Software is provided "as is", WITHOUT ANY WARRANTY, express or implied,
 including but not limited to the warranties of MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE and NONINFRINGEMENT.  In no event shall the authors
 or copyright holders by liable for any claim, damages, or other liability,
 whether in an actino of a contract, tort or otherwise, arising from, out of
 or in connection with the Software or the use or other dealings in the 
 software.
 -----------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// name: mo_gfx.h
// desc: MoPhO API for graphics routines
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Nick Bryan
//          Jieun Oh
//          Jorge Hererra
//
//    date: Fall 2009
//    version: 1.0.0
//
// Mobile Music research @ CCRMA, Stanford University:
//     http://momu.stanford.edu/
//-----------------------------------------------------------------------------
#ifndef __MO_GFX_H__
#define __MO_GFX_H__


#include "mo_def.h"
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <OpenGLES/ES1/gl.h>
#include <CoreFoundation/CoreFoundation.h>




//-----------------------------------------------------------------------------
// name: class Vector3D
// desc: 3d vector
//-----------------------------------------------------------------------------
class Vector3D
{
public:
    Vector3D( ) : x(0), y(0), z(0) { }
    Vector3D( GLfloat _x, GLfloat _y, GLfloat _z ) { set( _x, _y, _z ); }
    Vector3D( const Vector3D & other ) { *this = other; }
    ~Vector3D() { }
    
public:
    void set( GLfloat _x, GLfloat _y, GLfloat _z ) { x = _x; y = _y; z = _z; }
    void setAll( GLfloat val ) { x = y = z = val; }

public:
    GLfloat & operator []( int index )
    { if( index == 0 ) return x; if( index == 1 ) return y; 
      if( index == 2 ) return z; return nowhere; }
    const GLfloat & operator []( int index ) const
    { if( index == 0 ) return x; if( index == 1 ) return y; 
      if( index == 2 ) return z; return zero; }
    const Vector3D & operator =( const Vector3D & rhs )
    { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }

    Vector3D operator +( const Vector3D & rhs ) const
    { Vector3D result = *this; result += rhs; return result; }
    Vector3D operator -( const Vector3D & rhs ) const
    { Vector3D result = *this; result -= rhs; return result; }
    Vector3D operator *( GLfloat scalar ) const
    { Vector3D result = *this; result *= scalar; return result; }

    inline void operator +=( const Vector3D & rhs )
    { x += rhs.x; y += rhs.y; z += rhs.z; }
    inline void operator -=( const Vector3D & rhs )
    { x -= rhs.x; y -= rhs.y; z -= rhs.z; }
    inline void operator *=( GLfloat scalar )
    { x *= scalar; y *= scalar; z *= scalar; }

    // dot product
    inline GLfloat operator *( const Vector3D & rhs ) const
    { GLfloat result = x*rhs.x + y*rhs.y + z*rhs.z; return result; }
    // cross product
    inline Vector3D operator ^( const Vector3D & rhs ) const
    { return Vector3D( y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x ); }
    // magnitude
    inline GLfloat magnitude() const
    { return ::sqrt( x*x + y*y + z*z ); }
    inline GLfloat magnitudeSqr() const
    { return x*x + y*y + z*z; }
    // normalize
    inline void normalize()
    { GLfloat mag = magnitude(); if( mag == 0 ) return; *this *= 1/mag; }
    // 2d angles
    inline GLfloat angleXY() const
    { return ::atan2( y, x ); }
    inline GLfloat angleYZ() const
    { return ::atan2( z, y ); }
    inline GLfloat angleXZ() const
    { return ::atan2( z, x ); }
    
    // 2d: set from polar
    void setXYFromPolar( float magnitude, float degrees )
    { x = magnitude * ::cos( degrees * M_PI / 180.0 );
      y = magnitude * ::sin( degrees * M_PI / 180.0 ); }

public: // using the 3-tuple for interpolation
    inline void interp()
    { value = (goal-value)*slew + value; }
    inline void interp( GLfloat delta )
    { value = (goal-value)*slew*delta + value; }
    inline void interp2( GLfloat delta ) // time-invariant
    { value = (goal - value) * (1.0f - ::powf(1.0f - slew, delta)) + value; }
    inline void update( GLfloat _goal )
    { goal = _goal; }
    inline void update( GLfloat _goal, GLfloat _slew )
    { goal = _goal; slew = _slew; }
    inline void updateSet( GLfloat _goalAndValue )
    { goal = value = _goalAndValue; }
    inline void updateSet( GLfloat _goalAndValue, GLfloat _slew )
    { goal = value = _goalAndValue; slew = _slew; }

public:
    // either use as .x, .y, .z OR .value, .goal, .slew
    union { GLfloat x; GLfloat value; };
    union { GLfloat y; GLfloat goal; };
    union { GLfloat z; GLfloat slew; };
    
public:
    static GLfloat nowhere;
    static GLfloat zero;
};




//-----------------------------------------------------------------------------
// name: class iSlew3D
// desc: contains a slew of vectors for slewing a vector, accessible as .actual
//-----------------------------------------------------------------------------
class iSlew3D
{
public:
    iSlew3D( float slew = 0.5 ) : m_slewX(0, 0, slew), m_slewY(0, 0, slew), m_slewZ(0, 0, slew) { }
    iSlew3D( const Vector3D & o, float slew = 0.5 ) : m_slewX(o.x, o.x, slew), m_slewY(o.y, o.y, slew), m_slewZ(o.z, o.z, slew), m_actual(o) { }
    iSlew3D( GLfloat x, GLfloat y, GLfloat z, float slew = 0.5 ) : m_slewX(x, x, slew), m_slewY(y, y, slew), m_slewZ(z, z, slew), m_actual(x, y, z) { }
    
public:
    void set( const Vector3D & o )
    { m_actual = o; m_slewX.value = o.x; m_slewY.value = o.y; m_slewZ.value = o.z; }
    void setSlew( GLfloat slew )
    { m_slewX.slew = m_slewY.slew = m_slewZ.slew = slew; }
    
public:
    void interp()
    { m_slewX.interp(); m_slewY.interp(); m_slewZ.interp();
      m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void interp( float delta )
    { m_slewX.interp( delta ); m_slewY.interp( delta ); m_slewZ.interp( delta );
      m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void interp2( float delta )
    { m_slewX.interp2( delta ); m_slewY.interp2( delta ); m_slewZ.interp2( delta );
      m_actual.x = m_slewX.value; m_actual.y = m_slewY.value; m_actual.z = m_slewZ.value; }
    void update( Vector3D goal )
    { m_slewX.update( goal.x ); m_slewY.update( goal.y ); m_slewZ.update( goal.z ); }
    void update( Vector3D goal, float slew )
    { m_slewX.update( goal.x, slew ); m_slewY.update( goal.y, slew ); m_slewZ.update( goal.z, slew ); }
    void updateSet( Vector3D goalAndValue )
    { m_slewX.updateSet( goalAndValue.x ); m_slewY.updateSet( goalAndValue.y ); m_slewZ.updateSet( goalAndValue.z ); }
    void updateSet( Vector3D goalAndValue, float slew )
    { m_slewX.updateSet( goalAndValue.x, slew ); m_slewY.updateSet( goalAndValue.y, slew ); m_slewZ.updateSet( goalAndValue.z, slew ); }
    
public:
    const Vector3D & actual() const { return m_actual; }
    Vector3D & actual() { return m_actual; }
    
    const Vector3D & slewX() const { return m_slewX; }
    const Vector3D & slewY() const { return m_slewY; }
    const Vector3D & slewZ() const { return m_slewZ; }
    
public:
    static float slewForDuration( float duration, float margin = 0.05 )
    { if( duration > 0 ) return 1 - ::pow( margin, 1.0 / duration );
      else return 1; }
    
private:
    Vector3D m_slewX;
    Vector3D m_slewY;
    Vector3D m_slewZ;
    Vector3D m_actual;
};




//-----------------------------------------------------------------------------
// name: class Matrix4D
// desc: ...
//-----------------------------------------------------------------------------
class Matrix4D
{
public:
    // constructor
    Matrix4D() { memset( m, 0, sizeof(GLfloat) * 16 ); }
    // copy constructor
    Matrix4D( const Matrix4D & rhs )
    { *this = rhs; }
    // constructor
    Matrix4D( GLfloat m0, GLfloat m4, GLfloat m8, GLfloat m12,
              GLfloat m1, GLfloat m5, GLfloat m9, GLfloat m13,
              GLfloat m2, GLfloat m6, GLfloat m10, GLfloat m14,
              GLfloat m3, GLfloat m7, GLfloat m11, GLfloat m15 );

public:
    // copy
    void operator=( const Matrix4D & rhs )
    { memcpy( m, rhs.m, sizeof(GLfloat)*16 ); }
             
public:
    // access indexed element
    GLfloat & operator []( int index )
    { if( index < 0 || index >= 16 ) assert( false ); return m[index]; }
    // access indexed element (read-only)
    const GLfloat & operator []( int index ) const
    { if( index < 0 || index >= 16 ) assert( false ); return m[index]; }
   
public:
    // multiply by vector
    Vector3D operator *( const Vector3D & rhs ) const;
    // multiply by matrix
    Matrix4D operator *( const Matrix4D & rhs ) const;

public:
    // apply as GL transformation
    void applyTransform()
    { glMultMatrixf( m ); }

protected:
    // column-major (like OpenGL)
    GLfloat m[16];
};




//-----------------------------------------------------------------------------
// name: class Quaternion4D
// desc: ...
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//         much code from Game Programming Wiki and the internet
//         much inspiration from Mattias Ljungstrom
//-----------------------------------------------------------------------------
class Quaternion4D
{
public:
    // constructor (identity)
    Quaternion4D() : w(1), x(0), y(0), z(0) { }
    // constructor
    Quaternion4D( GLfloat ww, GLfloat xx, GLfloat yy, GLfloat zz )
    : w(ww), x(xx), y(yy), z(zz) { }
    // set to identity
    void identity() { w = 1; x = y = z = 0; }
    
public:
    // normalize
    void normalize();
    // complex conjugate
    Quaternion4D conjugate() const;
    // multiply
    Quaternion4D operator *( const Quaternion4D & rhs ) const;
    // rotate a vector
    Vector3D operator *( const Vector3D & rhs ) const;

public:
    // convert from angle
    void fromAngle( const Vector3D & v, GLfloat angle );
    // convert from Euler angle
    void fromPitchYawRoll( GLfloat pitch, GLfloat yaw, GLfloat roll );
    
public:
    // convert to matrix (4x4)
    Matrix4D toMatrix();
    // to axis and angle
    void toAxisAngle( Vector3D & axis, GLfloat & angle );
    
public:
    // rotate around axis (applies OpenGL transformation)
    static void rotateAround( const Vector3D & axis, GLfloat angle );
    // rotate using pitch, yaw, roll
    static void rotateTo( const Vector3D & pyr );

protected: // static
    static Quaternion4D ourQuaternion;
    static Matrix4D ourMatrix;
    
protected:
    GLfloat w;
    GLfloat x;
    GLfloat y;
    GLfloat z;
};




//-----------------------------------------------------------------------------
// name: class MoGfx
// desc: MoPhO graphics functions
//-----------------------------------------------------------------------------
class MoGfx
{
public: // GLU-like stuff
    // perspective projection
    static void perspective( double fovy, double aspectRatio, double zNear, double zFar );
    // orthographic projection
    static void ortho( GLint width = 320, GLint height = 480, GLint landscape = 0 );
    // look at
    static void lookAt( double eye_x, double eye_y, double eye_z,
                        double at_x, double at_y, double at_z,
                        double up_x, double up_y, double up_z );
    
    // load texture (call this with a texture bound)
    static bool loadTexture( NSString * name, NSString * ext );

    // load texture from a UIImage
    static bool loadTexture( UIImage * image );
    
    // point in triangle test (2D)
    static bool isPointInTriangle2D( const Vector3D & pt, const Vector3D & a, 
                                     const Vector3D & b, const Vector3D & c );
    // point line projection
    static bool pointLineProject( const Vector3D & pt, const Vector3D & a,
                                  const Vector3D & b, Vector3D & result );

    // get current time
    static double getCurrentTime( bool fresh );
    // reset current time tracking
    static void resetCurrentTime();
    // get delta
    static GLfloat delta();
    // set delta factor
    static void setDeltaFactor( GLfloat factor );
    
    // get desired frame rate
    static GLuint getDesiredFrameRate() { return ourDesiredFrameRate; }
    // set desired frame rate
    static void setDesiredFrameRate( GLuint rate ) { ourDesiredFrameRate = rate; }

    // use fixed time step?
    static GLboolean getUseFixedTimeStep() { return ourUseFixedTimeStep; }
    // set fixed time step
    static void setUseFixedTimeStep( GLboolean value ) { ourUseFixedTimeStep = value; }

public: // helper
    static void setColor( const Vector3D & v, GLfloat alpha = 1.0f );
    
public:
    static struct timeval ourPrevTime;
    static struct timeval ourCurrTime;
    static GLfloat ourDeltaFactor;
    static GLuint ourDesiredFrameRate;
    static GLboolean ourUseFixedTimeStep;
    
    static void setStaticTimeStep(float timeStep);
    static void clearStaticTimeStep();
};




#endif
