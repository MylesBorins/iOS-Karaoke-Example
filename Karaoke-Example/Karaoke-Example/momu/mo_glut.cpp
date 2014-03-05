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
// name: mo_glut.h
// desc: MoPhO API for graphics routines based on GLUT|ES
//
// authors: Ge Wang (ge@ccrma.stanford.edu)
//          Nick Bryan
//          Jieun Oh
//          Jorge Hererra
//
// original GLUT by Mark Kilgard
// GLUT|ES port by Joachim Pouderoux with the help of Jean-Eudes Marvie
//
//    date: Fall 2009
//    version: 1.0.0
//
// Mobile Music research @ CCRMA, Stanford University:
//     http://momu.stanford.edu/
//-----------------------------------------------------------------------------
#include "mo_glut.h"
#include <stdlib.h>
#include <math.h>


// glut's PI
#define PI_ 3.14159265358979323846


//-----------------------------------------------------------------------------
// name: glutSolidTorus()
// desc: ...
//-----------------------------------------------------------------------------
void glutSolidTorus( GLfloat ir, GLfloat _or, GLint sides, GLint rings )
{
	GLint i, j, k, triangles;
	GLfloat s, t, x, y, z, twopi, nx, ny, nz;
	GLfloat sin_s, cos_s, cos_t, sin_t, twopi_s, twopi_t;
	GLfloat twopi_sides, twopi_rings;
	static GLfloat* v, *n;
	static GLfloat parms[4];
	GLfloat* p, *q;
    
	if (v) 
	{
		if (parms[0] != ir || parms[1] != _or || parms[2] != sides || parms[3] != rings) 
		{
			free(v);
			free(n);
			n = v = 0;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if (!v) 
	{
		parms[0] = ir; 
		parms[1] = _or; 
		parms[2] = (GLfloat)sides; 
		parms[3] = (GLfloat)rings;
        
		p = v = (GLfloat*)malloc(sides*(rings+1)*2*3*sizeof *v);
		q = n = (GLfloat*)malloc(sides*(rings+1)*2*3*sizeof *n);
        
		twopi = 2.0f * (GLfloat)PI_;
		twopi_sides = twopi/sides;
		twopi_rings = twopi/rings;
        
		for (i = 0; i < sides; i++) 
		{
			for (j = 0; j <= rings; j++) 
			{
				for (k = 1; k >= 0; k--) 
				{
					s = (i + k) % sides + 0.5f;
					t = (GLfloat)( j % rings);
                    
					twopi_s= s*twopi_sides;
					twopi_t = t*twopi_rings;
                    
					cos_s = (GLfloat)cos(twopi_s);
					sin_s = (GLfloat)sin(twopi_s);
                    
					cos_t = (GLfloat)cos(twopi_t);
					sin_t = (GLfloat)sin(twopi_t);
                    
					x = (_or+ir*(GLfloat)cos_s)*(GLfloat)cos_t;
					y = (_or+ir*(GLfloat)cos_s)*(GLfloat)sin_t;
					z = ir * (GLfloat)sin_s;
                    
					*p++ = x;
					*p++ = y;
					*p++ = z;
                    
					nx = (GLfloat)cos_s*(GLfloat)cos_t;
					ny = (GLfloat)cos_s*(GLfloat)sin_t;
					nz = (GLfloat)sin_s;
                    
					*q++ = nx;
					*q++ = ny;
					*q++ = nz;
				}
			}
		}
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	triangles = (rings + 1) * 2;
    
	for(i = 0; i < sides; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, triangles * i, triangles);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}


//-----------------------------------------------------------------------------
static GLfloat boxvec[6][3] =
{
	{-1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0}
};

static GLushort boxndex [12][3] = 
{
	{0, 1, 2},
	{0, 2, 3},
	{3, 2, 6},
	{3, 6, 7},
	{6, 4, 7},
	{6, 5, 4},
	{4, 5, 1},
	{4, 1, 0},
	{2, 1, 5},
	{2, 5, 6},
	{3, 7, 4},
	{3, 4, 0}
};

static GLushort wireboxndex[6][4] = 
{
	{0, 1, 2, 3},
	{3, 2, 6, 7},
	{7, 6, 5, 4},
	{4, 5, 1, 0},
	{5, 6, 2, 1},
	{7, 4, 0, 3}
};




//-----------------------------------------------------------------------------
// name: glutSolidBox()
// desc: ...
//-----------------------------------------------------------------------------
void glutSolidBox( GLfloat Width, GLfloat Depth, GLfloat Height )  // x y z
{
	int i;
	GLfloat v[8][3];
    
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = - Width/ 2.0f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = Width / 2.0f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -Depth / 2.0f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = Depth / 2.0f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -Height / 2.0f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = Height / 2.0f;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState (GL_VERTEX_ARRAY);
    
	for (i = 0; i < 6; i++)
	{
		glNormal3f(boxvec[i][0], boxvec[i][1], boxvec[i][2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, boxndex[i*2]);
	}
    
	glDisableClientState (GL_VERTEX_ARRAY);
}




//-----------------------------------------------------------------------------
// name: glutWireBox()
// desc: ...
//-----------------------------------------------------------------------------
void glutWireBox(GLfloat Width, GLfloat Depth, GLfloat Height)
{
	GLfloat v[8][3];
	int i;
    
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = - Width/ 2.0f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = Width / 2.0f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -Depth / 2.0f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = Depth / 2.0f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -Height / 2.0f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = Height / 2.0f;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glEnableClientState (GL_VERTEX_ARRAY);
    
	for ( i = 0; i < 6; i++)
	{
		glNormal3f(boxvec[i][0], boxvec[i][1], boxvec[i][2]);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, wireboxndex[i]);
	}
	glDisableClientState (GL_VERTEX_ARRAY);
}




//-----------------------------------------------------------------------------
void PlotSpherePoints(GLfloat radius, GLint stacks, GLint slices, GLfloat* v, GLfloat* n)
{
    
	GLint i, j; 
	GLfloat slicestep, stackstep;
    
	stackstep = ((GLfloat)PI_) / stacks;
	slicestep = 2.0f * ((GLfloat)PI_) / slices;
    
	for (i = 0; i < stacks; ++i)		
	{
		GLfloat a = i * stackstep;
		GLfloat b = a + stackstep;
        
		GLfloat s0 =  (GLfloat)sin(a);
		GLfloat s1 =  (GLfloat)sin(b);
        
		GLfloat c0 =  (GLfloat)cos(a);
		GLfloat c1 =  (GLfloat)cos(b);
        
		for (j = 0; j <= slices; ++j)		
		{
			GLfloat c = j * slicestep;
			GLfloat x = (GLfloat)cos(c);
			GLfloat y = (GLfloat)sin(c);
            
			*n = x * s0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = y * s0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = c0;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = x * s1;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = y * s1;
			*v = *n * radius;
            
			n++;
			v++;
            
			*n = c1;
			*v = *n * radius;
            
			n++;
			v++;
            
		}
	}
}




//-----------------------------------------------------------------------------
// name: glutSolidSphere()
// desc: ...
//-----------------------------------------------------------------------------
void glutSolidSphere(GLfloat radius, GLint slices, GLint stacks) 
{
	GLint i, triangles; 
	static GLfloat *v=NULL, *n=NULL;
	static GLfloat parms[3];
 
	if (v) 
	{
		if (parms[0] != radius || parms[1] != slices || parms[2] != stacks) 
		{
			free(v); 
			free(n);
            
			n = v = NULL;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if (!v) 
	{
		parms[0] = radius; 
		parms[1] = (GLfloat)slices; 
		parms[2] = (GLfloat)stacks;
        
		v = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof(GLfloat) );
		n = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof(GLfloat) );
        
		PlotSpherePoints(radius, stacks, slices, v, n);
        
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	triangles = (slices + 1) * 2;
    
	for(i = 0; i < stacks; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i * triangles, triangles);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}




//-----------------------------------------------------------------------------
// name: glutWireSphere()
// desc: ...
//-----------------------------------------------------------------------------
void glutWireSphere(GLfloat radius, GLint slices, GLint stacks) 
{
	GLint i, j, f; 
	static GLfloat* v, *n;
	static GLfloat parms[3];
    
	if (v) 
	{
		if (parms[0] != radius || parms[1] != slices || parms[2] != stacks) 
		{
			free(v); 
			free(n);
            
			n = v = 0;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if (!v) 
	{
		parms[0] = radius; 
		parms[1] = (GLfloat)slices; 
		parms[2] = (GLfloat)stacks;
        
		v = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *v);
		n = (GLfloat*)malloc(stacks*(slices+1)*2*3*sizeof *n);
        
		PlotSpherePoints(radius, stacks, slices, v, n);
        
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	for(i = 0; i < stacks; ++i)
	{
		f = i * (slices + 1);
        
		for (j = 0; j <= slices; ++j)
			glDrawArrays(GL_LINE_LOOP, (f + j)*2, 3);
	}
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}




//-----------------------------------------------------------------------------
// name: glutSolidCone()
// desc: ...
//-----------------------------------------------------------------------------
void glutSolidCone(GLfloat base, GLfloat height, GLint slices, GLint stacks) 
{
	GLint i, j;
	GLfloat twopi, nx, ny, nz;
	static GLfloat * v = NULL, * n = NULL;
	static GLfloat parms[4];
	GLfloat * p = NULL, * q = NULL;
    
    // ge: added
    if( base == 0.0f || height == 0.0f )
        return;
    
	if (v) 
	{
		if (parms[0] != base || parms[1] != height || parms[2] != slices || parms[3] != stacks) 
		{
			free(v); 
			free(n);
            
			n = v = 0;
            
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glNormalPointer(GL_FLOAT, 0, 0);
		}
	}
    
	if ((!v) && (height != 0.0f))
	{
		float phi = (float)atan(base/height);
		float cphi = (GLfloat)cos(phi);
		float sphi= (GLfloat)sin(phi);
        
		parms[0] = base; 
		parms[1] = height; 
		parms[2] = (GLfloat)slices;
		parms[3] = (GLfloat)stacks;
        
		p = v = (float*)malloc(stacks*(slices+1)*2*3*sizeof *v);
		q = n = (float*)malloc(stacks*(slices+1)*2*3*sizeof *n);
        
		twopi = 2.0f * ((float)PI_);
        
		for (i = 0; i < stacks; i++) 
		{
			GLfloat r = base*(1.0f - (GLfloat)i /stacks);
			GLfloat r1 = base*(1.0f - (GLfloat)(i+1.0)/stacks);
			GLfloat z = height*i /stacks;
			GLfloat z1 = height*(1.0f+i) /stacks;
            
			for (j = 0; j <= slices; j++) 
			{
				GLfloat theta = j == slices ? 0.f : (GLfloat) j /slices*twopi;
				GLfloat ctheta = (GLfloat)cos(theta);
				GLfloat stheta = (GLfloat)sin(theta);
                
				nx = ctheta;
				ny = stheta;
				nz = sphi;
                
				*p++ = r1*nx;
				*p++ = r1*ny;
				*p++ = z1;
                
				*q++ = nx*cphi;
				*q++ = ny*cphi;
				*q++ = nz;
                
				*p++ = r*nx;
				*p++ = r*ny;
				*p++ = z;
                
                
				*q++ = nx*cphi;
				*q++ = ny*cphi;
				*q++ = nz;
                
			}
		}
	}
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	for(i = 0; i < stacks; i++)
		glDrawArrays(GL_TRIANGLE_STRIP, i*(slices+1)*2, (slices+1)*2);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}




//-----------------------------------------------------------------------------
// name: glutSolidCube()
// desc: ...
//-----------------------------------------------------------------------------
void glutSolidCube(GLfloat size) 
{
	static GLfloat v[108];	   // 108 =  6*18
    
	static const GLfloat cubev[108] = 
	{
		-1., -1., 1.,	/* front */
		1., -1., 1.,
		-1.,  1., 1.,
        
		1., -1., 1.,
		1.,  1., 1.,
		-1.,  1., 1.,
        
		-1.,  1., -1.,	/* back */
		1., -1., -1.,
		-1., -1., -1.,
        
		-1.,  1., -1.,
		1.,  1., -1.,
		1., -1., -1.,
        
		-1., -1., -1.,	/* left */
		-1., -1.,  1.,
		-1.,  1., -1.,
        
		-1., -1.,  1.,
		-1.,  1.,  1.,
		-1.,  1., -1.,
        
		1., -1.,  1.,	/* right */
		1., -1., -1.,
		1.,  1.,  1.,
        
		1., -1., -1.,
		1.,  1., -1.,
		1.,  1.,  1.,
        
		-1.,  1.,  1.,	/* top */
		1.,  1.,  1.,
		-1.,  1., -1.,
        
		1.,  1.,  1.,
		1.,  1., -1.,
		-1.,  1., -1.,
        
		-1., -1., -1.,	/* bottom */
		1., -1., -1.,
		-1., -1.,  1.,
        
		1., -1., -1.,
		1., -1.,  1.,
		-1., -1.,  1.,
	};
    
	static const GLfloat cuben[108] = 
	{
		0., 0., 1.,	/* front */
		0., 0., 1.,
		0., 0., 1.,
        
		0., 0., 1.,
		0., 0., 1.,
		0., 0., 1.,
        
		0., 0., -1.,	/* back */
		0., 0., -1.,
		0., 0., -1.,
        
		0., 0., -1.,
		0., 0., -1.,
		0., 0., -1.,
        
		-1., 0., 0.,	/* left */
		-1., 0., 0.,
		-1., 0., 0.,
        
		-1., 0., 0.,
		-1., 0., 0.,
		-1., 0., 0.,
        
		1., 0., 0.,	/* right */
		1., 0., 0.,
		1., 0., 0.,
        
		1., 0., 0.,
		1., 0., 0.,
		1., 0., 0.,
        
		0., 1., 0.,	/* top */
		0., 1., 0.,
		0., 1., 0.,
        
		0., 1., 0.,
		0., 1., 0.,
		0., 1., 0.,
        
		0., -1., 0.,	/* bottom */
		0., -1., 0.,
		0., -1., 0.,
        
		0., -1., 0.,
		0., -1., 0.,
		0., -1., 0.,
	};
    
	int i;
	size /= 2;
    
	for(i = 0; i < 108; i++) 
		v[i] = cubev[i] * size;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, cuben);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	glDrawArrays(GL_TRIANGLES, 0, 36);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}




//-----------------------------------------------------------------------------
// name: glutWireCube()
// desc: ...
//-----------------------------------------------------------------------------
void glutWireCube(GLfloat size) 
{
	static GLfloat v[72];
    
	static const GLfloat cubev[72] = 	  // 72 = 3*6*4
	{
		-1., -1., 1.,	/* front */
		1., -1., 1.,
		1.,  1., 1.,
		-1.,  1., 1.,
        
		-1.,  1., -1.,	/* back */
		1.,  1., -1.,
		1., -1., -1.,
		-1., -1., -1.,
        
		-1., -1., -1.,	/* left */
		-1., -1.,  1.,
		-1.,  1.,  1.,
		-1.,  1., -1.,
        
		1., -1.,  1.,	/* right */
		1., -1., -1.,
		1.,  1., -1.,
		1.,  1.,  1.,
        
		-1.,  1.,  1.,	/* top */
		1.,  1.,  1.,
		1.,  1., -1.,
		-1.,  1., -1.,
        
		-1., -1., -1.,	/* bottom */
		1., -1., -1.,
		1., -1.,  1.,
		-1., -1.,  1.,
	};
    
	static const GLfloat cuben[72] = 
	{
		0., 0., 1.,	/* front */
		0., 0., 1.,
		0., 0., 1.,
		0., 0., 1.,
        
		0., 0., -1.,	/* back */
		0., 0., -1.,
		0., 0., -1.,
		0., 0., -1.,
        
		-1., 0., 0.,	/* left */
		-1., 0., 0.,
		-1., 0., 0.,
		-1., 0., 0.,
        
		1., 0., 0.,	/* right */
		1., 0., 0.,
		1., 0., 0.,
		1., 0., 0.,
        
		0., 1., 0.,	/* top */
		0., 1., 0.,
		0., 1., 0.,
		0., 1., 0.,
        
		0., -1., 0.,	/* bottom */
		0., -1., 0.,
		0., -1., 0.,
		0., -1., 0.,
	};
    
	int i;
	size /= 2;
    
	for(i = 0; i < 72; i++) 
		v[i] = cubev[i] * size;
    
	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, cuben);
    
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
    
	for(i = 0; i < 6; i++)
		glDrawArrays(GL_LINE_LOOP, 4*i, 4);
    
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
    
}
