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
// name: mo_gfx.cpp
// desc: MoPhO API for graphics
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
#include "mo_gfx.h"
#include <math.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



// vector static
GLfloat Vector3D::nowhere = 0.0f;
GLfloat Vector3D::zero = 0.0;




//-----------------------------------------------------------------------------
// name: Matrix4D()
// desc: constructor
//-----------------------------------------------------------------------------
Matrix4D::Matrix4D( GLfloat m0, GLfloat m4, GLfloat m8, GLfloat m12,
                   GLfloat m1, GLfloat m5, GLfloat m9, GLfloat m13,
                   GLfloat m2, GLfloat m6, GLfloat m10, GLfloat m14,
                   GLfloat m3, GLfloat m7, GLfloat m11, GLfloat m15 )
{
    m[0] = m0; m[1] = m1; m[2] = m2; m[3] = m3;
    m[4] = m4; m[5] = m5; m[6] = m6; m[7] = m7;
    m[8] = m8; m[9] = m9; m[10] = m10; m[11] = m11;
    m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}




#define QUAT_TOLERANCE (0.00001f)
//-----------------------------------------------------------------------------
// name: normalize()
// desc: normalize
//-----------------------------------------------------------------------------
void Quaternion4D::normalize()
{
    // don't normalize if we don't have to
    GLfloat mag2 = w * w + x * x + y * y + z * z;
    // check
    if( mag2 != 0.f && (::fabs(mag2 - 1.0f) > QUAT_TOLERANCE) )
    {
        GLfloat mag = sqrt(mag2);
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }
}




//-----------------------------------------------------------------------------
// name conjugate()
// desc: complex conjugate
//-----------------------------------------------------------------------------
Quaternion4D Quaternion4D::conjugate() const
{
    return Quaternion4D( w, -x, -y, -z );
}




//-----------------------------------------------------------------------------
// name: *
// desc: multiply
//-----------------------------------------------------------------------------
Quaternion4D Quaternion4D::operator *( const Quaternion4D & rhs ) const
{
    // the constructor takes its arguments as (x, y, z, w)
	return Quaternion4D( w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
                         w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
	                     w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
                         w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x );
}




//-----------------------------------------------------------------------------
// name: *
// desc: rotate a vector
//-----------------------------------------------------------------------------
Vector3D Quaternion4D::operator *( const Vector3D & rhs ) const
{
    // copy
	Vector3D vn( rhs );
    // normalize
	vn.normalize();
    
    // do some magic, aka math
	Quaternion4D vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;
    
    // more magic...
	resQuat = vecQuat * conjugate();
	resQuat = *this * resQuat;
    
	return Vector3D( resQuat.x, resQuat.y, resQuat.z );
}




//-----------------------------------------------------------------------------
// convert from angle
//-----------------------------------------------------------------------------
void Quaternion4D::fromAngle( const Vector3D & v, GLfloat angle )
{
    // copy
	Vector3D vn(v);
    // normalize
	vn.normalize();
    // angle
	angle *= 0.5f;
    
    // sine
	GLfloat sinAngle = sin(angle);
    
    // compute and set
	x = (vn.x * sinAngle);
	y = (vn.y * sinAngle);
	z = (vn.z * sinAngle);
	w = cos(angle);
}




//-----------------------------------------------------------------------------
// convert from Euler angle
//-----------------------------------------------------------------------------
void Quaternion4D::fromPitchYawRoll( GLfloat pitch, GLfloat yaw, GLfloat roll )
{
    // basically we create 3 quaternions, one each for pitch, yaw, and roll
    // and multiply those together.
    // the calculation below does the same, just shorter

    GLfloat p = pitch * PI_OVER_180 / 2.0;
    GLfloat y = yaw * PI_OVER_180 / 2.0;
    GLfloat r = roll * PI_OVER_180 / 2.0;

    GLfloat sinp = ::sin(p);
    GLfloat siny = ::sin(y);
    GLfloat sinr = ::sin(r);
    GLfloat cosp = ::cos(p);
    GLfloat cosy = ::cos(y);
    GLfloat cosr = ::cos(r);

    // compute and set
    this->x = sinr * cosp * cosy - cosr * sinp * siny;
    this->y = cosr * sinp * cosy + sinr * cosp * siny;
    this->z = cosr * cosp * siny - sinr * sinp * cosy;
    this->w = cosr * cosp * cosy + sinr * sinp * siny;

    // normalize
    this->normalize();
}




//-----------------------------------------------------------------------------
// name: toMatrix()
// desc: convert to matrix (4x4)
//-----------------------------------------------------------------------------
Matrix4D Quaternion4D::toMatrix()
{
    GLfloat x2 = x * x;
    GLfloat y2 = y * y;
    GLfloat z2 = z * z;
    GLfloat xy = x * y;
    GLfloat xz = x * z;
    GLfloat yz = y * z;
    GLfloat wx = w * x;
    GLfloat wy = w * y;
    GLfloat wz = w * z;

    // calculation would be a lot more complicated for non-unit length quaternions
    // note: constructor of Matrix4D expects the Matrix in column-major format 
    // like expected by OpenGL
    return Matrix4D( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
                     2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
                     2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);
}




//-----------------------------------------------------------------------------
// name: toAxisAngle()
// desc: to axis and angle
//-----------------------------------------------------------------------------
void Quaternion4D::toAxisAngle( Vector3D & axis, GLfloat & angle )
{
    // scale
    GLfloat scale = ::sqrt(x * x + y * y + z * z);
    // axis
	axis.x = x / scale;
	axis.y = y / scale;
	axis.z = z / scale;
    // angle
	angle = ::acos(w) * 2.0f;
}




//-----------------------------------------------------------------------------
// name: rotateAround()
// desc: rotate around axis (applies OpenGL transformation)
//-----------------------------------------------------------------------------
void Quaternion4D::rotateAround( const Vector3D & to, GLfloat angle )
{
    // quaternion
    Quaternion4D q;
    // from angle
    q.fromAngle( to, angle );
    // get matrix
    Matrix4D m = q.toMatrix();
    // apply tranformation
    m.applyTransform();
}




//-----------------------------------------------------------------------------
// name: rotateTo()
// desc: rotate using pitch, yaw, roll (applies OpenGL transformation)
//-----------------------------------------------------------------------------
void Quaternion4D::rotateTo( const Vector3D & pyr )
{
    // quaternion
    Quaternion4D q;
    // from angle
    q.fromPitchYawRoll( pyr.x, pyr.y, pyr.z);
    // get matrix
    Matrix4D m = q.toMatrix();
    // apply tranformation
    m.applyTransform();
}




//-----------------------------------------------------------------------------
// name: perspective()
// desc: set perspective projection
//       (from jshmrsn, macrumors forum)
//-----------------------------------------------------------------------------
void MoGfx::perspective( double fovy, double aspectRatio, double zNear, double zFar )
{
    double xmin, xmax, ymin, ymax;

    // projection
    glMatrixMode( GL_PROJECTION );
    // identity
    glLoadIdentity();

    // set field of view
    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspectRatio;
    xmax = ymax * aspectRatio;

    // set the frustum
    glFrustumf( xmin, xmax, ymin, ymax, zNear, zFar );

    // set hint
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    
    // modelview
    glMatrixMode( GL_MODELVIEW );
    // set to identity
    glLoadIdentity();

    // enable depth mask
    // glDepthMask( GL_TRUE );
}




//-----------------------------------------------------------------------------
// name: ortho()
// desc: set ortho projection
//       (from jshmrsn, macrumors forum)
//-----------------------------------------------------------------------------
void MoGfx::ortho( GLint width, GLint height, GLint landscape )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();                  
    glOrthof( 0, width, 0, height, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // landscape
    if( landscape == -1 || landscape == 1 )
    {
        // translate
        glTranslatef( width/2, height/2, 0 );
        // rotate around z
        glRotatef( landscape > 0 ? -90 : 90, 0, 0, 1 );
        // translate
        glTranslatef( -height/2, -width/2, 0 );
    }
    
    //flipped portrait
    // landscape
    if( landscape == 2 )
    {
        // translate
        glTranslatef( width/2, height/2, 0 );
        // rotate around z
        glRotatef( 180, 0, 0, 1 );
        // translate
        glTranslatef( -width/2, -height/2, 0 );
    }
}




//-----------------------------------------------------------------------------
// name: lookAt()
// desc: set eye, at, up vector
//       (from jshmrsn, macrumors forum)
//-----------------------------------------------------------------------------
void MoGfx::lookAt( double eye_x, double eye_y, double eye_z,
                    double at_x, double at_y, double at_z,
                    double up_x, double up_y, double up_z )
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eye_x - at_x;
    z[1] = eye_y - at_y;
    z[2] = eye_z - at_z;

    mag = sqrt( z[0] * z[0] + z[1] * z[1] + z[2] * z[2] );
    if( mag )
    {   /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = up_x;
    y[1] = up_y;
    y[2] = up_z;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    mag = sqrt( x[0] * x[0] + x[1] * x[1] + x[2] * x[2] );
    if( mag )
    {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    mag = sqrt( y[0] * y[0] + y[1] * y[1] + y[2] * y[2] );
    if( mag )
    {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0,0) = x[0];
    M(0,1) = x[1];
    M(0,2) = x[2];
    M(0,3) = 0.0;
    M(1,0) = y[0];
    M(1,1) = y[1];
    M(1,2) = y[2];
    M(1,3) = 0.0;
    M(2,0) = z[0];
    M(2,1) = z[1];
    M(2,2) = z[2];
    M(2,3) = 0.0;
    M(3,0) = 0.0;
    M(3,1) = 0.0;
    M(3,2) = 0.0;
    M(3,3) = 1.0;    
#undef M

    // multiply into m
    glMultMatrixf( m );

    // translate eye to origin
    glTranslatef( -eye_x, -eye_y, -eye_z );
}



//-----------------------------------------------------------------------------
// name: loadTexture()
// desc: loads an OpenGL ES texture
//       (from jshmrsn, macrumors forum)
//-----------------------------------------------------------------------------
bool MoGfx::loadTexture( NSString * name, NSString * ext )
{
    // load the resource
    NSString *path = [[NSBundle mainBundle] pathForResource:name ofType:ext];
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:path];
    if (image == nil)
    {
        NSLog( @"[mo_gfx]: cannot load file: %@.%@", name, ext );
        return FALSE;
    }
    
    // log
    NSLog( @"loading texture: %@.%@...", name, ext );

    // load image
    loadTexture( image );

    // cleanup
    // [image release];

    return true;
}


//-----------------------------------------------------------------------------
// name: loadTexture()
// desc: loads an OpenGL ES texture
//       (from jshmrsn, macrumors forum)
//-----------------------------------------------------------------------------
bool MoGfx::loadTexture( UIImage * image )
{
    if (image == nil)
    {
        NSLog( @"[mo_gfx]: error: UIImage == nil..." );
        return FALSE;
    }
    
    // convert to RGBA
    GLuint width = (unsigned int)CGImageGetWidth( image.CGImage );
    GLuint height = (unsigned int)CGImageGetHeight( image.CGImage );
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc( height * width * 4 );
    CGContextRef context = CGBitmapContextCreate( 
        imageData, width, height, 8, 4 * width, colorSpace, 
        kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, width, height ) );
    CGContextTranslateCTM( context, 0, height - height );
    CGContextDrawImage( context, CGRectMake( 0, 0, width, height ), image.CGImage );
    
    // load the texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                  GL_RGBA, GL_UNSIGNED_BYTE, imageData );
    
    // free resource - OpenGL keeps image internally
    CGContextRelease(context);
    free(imageData);
    
    return true;
}




//-----------------------------------------------------------------------------
// name: isPointInTriangle2D()
// desc: point in triangle test (2D)
//-----------------------------------------------------------------------------
bool MoGfx::isPointInTriangle2D( const Vector3D & p, const Vector3D & a, 
                                 const Vector3D & b, const Vector3D & c )
{
    Vector3D v0 = c - a;
    Vector3D v1 = b - a;
    Vector3D v2 = p - a;
    
    // Compute dot products
    GLfloat dot00 = v0*v0;
    GLfloat dot01 = v0*v1;
    GLfloat dot02 = v0*v2;
    GLfloat dot11 = v1*v1;
    GLfloat dot12 = v1*v2;
    
    // Compute barycentric coordinates
    GLfloat invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    GLfloat u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    GLfloat v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v <= 1);
}




//-----------------------------------------------------------------------------
// name: pointLineProject()
// desc: point line projection
//-----------------------------------------------------------------------------
bool MoGfx::pointLineProject( const Vector3D & pt, const Vector3D & a,
                             const Vector3D & b, Vector3D & result )
{
    // get magnitude
    GLfloat magnitude = (b - a).magnitude();
    // sanity check
    if( magnitude < .00001f )
    {
        // print
        // std::cerr << "[MoGfx]: point line projection - degenerate line!" << std::endl;
        
        // send back
        result.setAll( 0.0f );
        // return
        return false;
    }
    
    // u
    GLfloat u = (pt.x-a.x)*(b.x-a.x) + (pt.y-a.y)*(b.y-a.y) + (pt.z-a.z)*(b.z-a.z);
    u /= magnitude;
    
    // result
    result.x = a.x + u*(b.x-a.x);
    result.y = a.y + u*(b.y-a.y);
    result.z = a.z + u*(b.z-a.z);
    
    return true;
}




//-----------------------------------------------------------------------------
// name: getCurrentTime()
// desc: get current time for simulation
//-----------------------------------------------------------------------------
double MoGfx::getCurrentTime( bool fresh )
{
    if( fresh )
    {
        ourPrevTime = ourCurrTime;
        gettimeofday( &ourCurrTime, NULL );
    }
    
    return ourCurrTime.tv_sec + (double)ourCurrTime.tv_usec / 1000000;
}




//-----------------------------------------------------------------------------
// name: resetCurrentTime()
// desc: reset current time for simulation
//-----------------------------------------------------------------------------
void MoGfx::resetCurrentTime()
{
    ourPrevTime.tv_sec = 0;
    ourPrevTime.tv_usec = 0;
}




static bool bUseStaticTimeStep = false;
static float fStaticTimeStep = 1.0f / 30.0f;




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MoGfx::setStaticTimeStep( GLfloat timeStep )
{
    bUseStaticTimeStep = true;
    fStaticTimeStep = timeStep;
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void MoGfx::clearStaticTimeStep()
{
    bUseStaticTimeStep = false;
}




//-----------------------------------------------------------------------------
// name: delta()
// desc: get current time delta for simulation
//-----------------------------------------------------------------------------
GLfloat MoGfx::delta()
{
    if( bUseStaticTimeStep )
    {
        return fStaticTimeStep;
    }
    else 
    {
        double prev = ourPrevTime.tv_sec + (double)ourPrevTime.tv_usec / 1000000;
        double curr = ourCurrTime.tv_sec + (double)ourCurrTime.tv_usec / 1000000;
        // first 0
        GLfloat v = (prev == 0 ? 0.0f : (GLfloat)(curr - prev)) * ourDeltaFactor;
        // clamp
        if( v > 1.0f ) v = 1.0f;
        // return
        return v;
    }
}




//-----------------------------------------------------------------------------
// name: setDeltaFactor()
// desc: set time delta factor for simulation
//-----------------------------------------------------------------------------
void MoGfx::setDeltaFactor( GLfloat factor )
{
    ourDeltaFactor = factor;
}




// static instantiation
struct timeval MoGfx::ourCurrTime;
struct timeval MoGfx::ourPrevTime;
GLfloat MoGfx::ourDeltaFactor = 1.0f;
GLuint MoGfx::ourDesiredFrameRate = 30;
GLboolean MoGfx::ourUseFixedTimeStep = GL_TRUE;




//-----------------------------------------------------------------------------
// name: setColor()
// desc: set color in gl from a 3d vector (with optional alpha component)
//-----------------------------------------------------------------------------
void MoGfx::setColor( const Vector3D & v, GLfloat alpha )
{
    // call gl
    glColor4f( v.x, v.y, v.z, alpha );
}
