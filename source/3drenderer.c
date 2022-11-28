#include <3ds.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/param.h>

#include "vector.h"
#include "3dmodels.h"
#include "game.h"
#include "render.h"
#include "colors.h"

#include "chars.h"

/**
 * very simple software 3d renderer
 * based on articles and blogs written by Matthijs Hollemans, Bernd Frassek and David Rousset
 * fast enough for small objects, so there ist no z-buffer, no shading or back-face culling
 * implemented
 */

u8* fbAdr_L; 
u8* fbAdr_R;

void setFrameBuffer(u8* fbAdr_l, u8* fbAdr_r)
{
	fbAdr_L = fbAdr_l;
	fbAdr_R = fbAdr_r;
}

void Clear (u32 color)
{
	int x,y;
	u8 r, g, b;
	
	r = RGBAToRed(color);
	g = RGBAToGreen(color);
	b = RGBAToBlue(color);
	
	for (x=0; x<WIDTH_TOP; x++)
		for(y=0; y<HEIGHT; y++)
		{
					fbAdr_L[((x*HEIGHT)+y)*3+0] = b;
					fbAdr_L[((x*HEIGHT)+y)*3+1] = g;
					fbAdr_L[((x*HEIGHT)+y)*3+2] = r;
					fbAdr_R[((x*HEIGHT)+y)*3+0] = b;
					fbAdr_R[((x*HEIGHT)+y)*3+1] = g;
					fbAdr_R[((x*HEIGHT)+y)*3+2] = r;
		}
}

// Puts a pixel on physical screen at the specified X,Y coordinate, regarding the stereo separation
// based on the Z value.
void PutPixel(int x, int y, int z, u32 color)
{
	u32 bg_color = ColorsToRGBA(fbAdr_L[((x*HEIGHT)+y)*3+2], fbAdr_L[((x*HEIGHT)+y)*3+1], fbAdr_L[((x*HEIGHT)+y)*3+0], 1.0);
	u32 new_color = RGBAToRGB(bg_color, color);
	u8 r = RGBAToRed(new_color);
	u8 g = RGBAToGreen(new_color);
	u8 b = RGBAToBlue(new_color);
	int separation;
				
	separation = get_stereo_separation(z*2);
	
	if ((x + separation/2 < WIDTH_TOP) && (x - separation/2 > 0))
	{
		fbAdr_L[(((x-separation/2)*HEIGHT)+y)*3+0] = b;
		fbAdr_L[(((x-separation/2)*HEIGHT)+y)*3+1] = g;
		fbAdr_L[(((x-separation/2)*HEIGHT)+y)*3+2] = r;
		fbAdr_R[(((x+separation/2)*HEIGHT)+y)*3+0] = b;
		fbAdr_R[(((x+separation/2)*HEIGHT)+y)*3+1] = g;
		fbAdr_R[(((x+separation/2)*HEIGHT)+y)*3+2] = r;
	}
}

// Project takes some 3D coordinates and transform them
// in 2D coordinates using the transformation matrix
Vector3 Project(Vertex vertex, Matrix3 transMat)
{
	// transforming the coordinates
	Vector3 v;
	Vector3 w;
	Vector3 coord;
	coord.X = vertex.x;
	coord.Y = vertex.y;
	coord.Z = vertex.z;
	coord.W = 1;
	
	
	w = TransformCoordinate(transMat, coord);
	v.X = (int)(FOCAL_DISTANCE * w.X / w.Z)+WIDTH_TOP/2;	
	v.Y = (int)(FOCAL_DISTANCE * w.Y / w.Z)+HEIGHT/2;
	v.Z = w.Z;
	
	return v;
}

void DrawLine (Vector3 v1, Vector3 v2)
{
	int x0 = (int)v1.X;
	int y0 = (int)v1.Y;
	int x1 = (int)v2.X;
	int y1 = (int)v2.Y;
	
	int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = dx+dy, e2; /* error value e_xy */

	while (1) {
		DrawPoint((Vector3){x0,y0,v1.Z});
		if (x0==x1 && y0==y1) break;
		e2 = 2*err;
		if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	}
}

// DrawPoint calls PutPixel but does the clipping operation before
void DrawPoint(Vector3 point, u32 color)
{
	// Clipping what's visible on screen
	if (point.X >= 0 && point.Y >= 0 && point.X < WIDTH_TOP && point.Y < HEIGHT)
	{
		// Drawing a yellow point
		
		PutPixel((int)point.X, (int)point.Y, (int)point.Z, color); 
	}
}


// The main method of the engine that re-compute each vertex projection
// during each frame
void Render (Mesh mesh, bool view, enum object_type type)
{
	int i;

	mesh.BackPosition.X = mesh.Position.X; 
	mesh.BackPosition.Z = mesh.Position.Z * (-1);
	mesh.BackPosition.Y = mesh.Position.Y;	
	if (view?(mesh.Position.Z <= 0):(mesh.BackPosition.Z <= 0)) return;
	
	// Beware to apply rotation before translation 
	Matrix3 worldMatrix = MMMultiply(RotationYawPitchRoll(mesh.Rotation.X, mesh.Rotation.Y, mesh.Rotation.Z), (view?Translation(mesh.Position):Translation(mesh.BackPosition)));
	
	for (i=0; i< mesh.num_of_faces; i++)
	{
		// First, we project the 3D coordinates into the 2D space
		Vector3 point1 = Project(mesh.vertexes[mesh.faces[i].a], worldMatrix);
		Vector3 point2 = Project(mesh.vertexes[mesh.faces[i].b], worldMatrix);
		Vector3 point3 = Project(mesh.vertexes[mesh.faces[i].c], worldMatrix);
		// Then we can draw on screen
		float r,g,b;
		r = g = b = (0.25f + (i % mesh.num_of_faces) * 0.75f / mesh.num_of_faces)*255;
		
		// colorcorrection by object type
		if ((type == PHOTONT)||(type == STARBASE))
		{
			if (r >172)
			{
				r = 255;
			} else r += 83;
			
			if (b <52)
			{
				b = 0;
			} else b -= 52;

		} else if (type == ENEMYPHOTONT)
		{
			if (r >185)
			{
				r = 255;
			} else r += 70;
			
			if (b >185)
			{
				b = 255;
			} else b += 70;
			
			if (g <70)
			{
				g = 0;
			} else g -= 70;
		
		} else	if (shield_active)
		{ 
			if (g >205)
			{
				g = 255;
			} else g += 50;
		}
		
		u32 color = (u32)ColorsToRGBA((u8)r,(u8)g,(u8)b,1); 
		if(
			(abs(point1.X) < 1200) &&
			(abs(point1.Y) < 1200) &&
			(abs(point1.Z) < 1200) &&
			(abs(point2.X) < 1200) &&
			(abs(point2.Y) < 1200) &&
			(abs(point2.Z) < 1200) &&
			(abs(point3.X) < 1200) &&
			(abs(point3.Y) < 1200) &&
			(abs(point3.Z) < 1200)
		)
			DrawTriangle(point1, point2, point3, color);
	}
}

// Clamping values to keep them between 0 and 1
float Clamp(float value)
{
	float fmin = 0;
	float fmax = 1;
    return MAX(fmin, MIN(value, fmax));
}

// Interpolating the value between 2 vertices 
// min is the starting point, max the ending point
// and gradient the % between the 2 points
float Interpolate(float fmin, float fmax, float gradient)
{
    return fmin + (fmax - fmin) * Clamp(gradient);
}

// drawing line between 2 points from left to right
// papb -> pcpd
// pa, pb, pc, pd must then be sorted before
void ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, u32 color)
{
	int x;
	
    // Thanks to current Y, we can compute the gradient to compute others values like
    // the starting X (sx) and ending X (ex) to draw between
    // if pa.Y == pb.Y or pc.Y == pd.Y, gradient is forced to 1
    float gradient1 = pa.Y != pb.Y ? (y - pa.Y) / (pb.Y - pa.Y) : 1;
    float gradient2 = pc.Y != pd.Y ? (y - pc.Y) / (pd.Y - pc.Y) : 1;
            
    int sx = (int)Interpolate(pa.X, pb.X, gradient1);
    int ex = (int)Interpolate(pc.X, pd.X, gradient2);

    // drawing a line from left (sx) to right (ex) 
    for (x = sx; x < ex; x++)
    {
        DrawPoint((Vector3){x, y, pa.Z}, color);
    }
}

void DrawTriangle(Vector3 p1, Vector3 p2, Vector3 p3, u32 color)
{
	Vector3 temp;
	int y;
	
    // Sorting the points in order to always have this order on screen p1, p2 & p3
    // with p1 always up (thus having the Y the lowest possible to be near the top screen)
    // then p2 between p1 & p3
    if (p1.Y > p2.Y)
    {
        temp = p2;
        p2 = p1;
        p1 = temp;
    }

    if (p2.Y > p3.Y)
    {
        temp = p2;
        p2 = p3;
        p3 = temp;
    }

    if (p1.Y > p2.Y)
    {
        temp = p2;
        p2 = p1;
        p1 = temp;
    }

    // inverse slopes
    float dP1P2, dP1P3;

    // http://en.wikipedia.org/wiki/Slope
    // Computing inverse slopes
    if (p2.Y - p1.Y > 0)
        dP1P2 = (p2.X - p1.X) / (p2.Y - p1.Y);
    else
        dP1P2 = 0;

    if (p3.Y - p1.Y > 0)
        dP1P3 = (p3.X - p1.X) / (p3.Y - p1.Y);
    else
        dP1P3 = 0;

    // First case where triangles are like that:
    // P1
    // -
    // -- 
    // - -
    // -  -
    // -   - P2
    // -  -
    // - -
    // -
    // P3
	
    if (dP1P2 > dP1P3)
    {
        for (y = (int)p1.Y; y <= (int)p3.Y; y++)
        {
            if (y < p2.Y)
            {
                ProcessScanLine(y, p1, p3, p1, p2, color);
            }
            else
            {
                ProcessScanLine(y, p1, p3, p2, p3, color);
            }
        }
    }
	
    // Second case where triangles are like that:
    //       P1
    //        -
    //       -- 
    //      - -
    //     -  -
    // P2 -   - 
    //     -  -
    //      - -
    //        -
    //       P3
	
    else
    {
        for (y = (int)p1.Y; y <= (int)p3.Y; y++)
        {
            if (y < p2.Y)
            {
                ProcessScanLine(y, p1, p2, p1, p3, color);
            }
            else
            {
                ProcessScanLine(y, p2, p3, p1, p3, color);
            }
        }
    }
	
}



