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
#include "visuals.h"

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

void Clear(u32 color)
{
    u8 r = RGBAToRed(color);
    u8 g = RGBAToGreen(color);
    u8 b = RGBAToBlue(color);

    u32 packedPixel = (b) | (g << 8) | (r << 16);
    u32* fb_L = (u32*)fbAdr_L;
    u32* fb_R = (u32*)fbAdr_R;

    int totalPixels = WIDTH_TOP * HEIGHT;

    for (int i = 0; i < totalPixels; i++)
    {
        fb_L[i] = packedPixel;
        fb_R[i] = packedPixel;
    }
}

// Puts a pixel on physical screen at the specified X,Y coordinate, regarding the stereo separation
// based on the Z value.

void PutPixel(int x, int y, int z, u32 color)
{
    // Hintergrundfarbe auslesen und neue Farbe berechnen
    u32 bg_color = ColorsToRGBA(
        fbAdr_L[((x * HEIGHT) + y) * 3 + 2],
        fbAdr_L[((x * HEIGHT) + y) * 3 + 1],
        fbAdr_L[((x * HEIGHT) + y) * 3 + 0],
        1.0
    );
    u32 new_color = RGBAToRGB(bg_color, color);

    // Extrahiere Farbkomponenten
    u8 r = RGBAToRed(new_color);
    u8 g = RGBAToGreen(new_color);
    u8 b = RGBAToBlue(new_color);

    // Berechne die Stereo-Separation einmal
    int half_separation = get_stereo_separation(z * 2) / 2;

    // Berechnung der Positionen für das linke und rechte Auge
    int left_x = x - half_separation;
    int right_x = x + half_separation;

    // Vermeide unnötige Berechnungen durch Adressprüfung vorher
    if (left_x >= 0 && left_x < WIDTH_TOP && right_x >= 0 && right_x < WIDTH_TOP)
    {
        // Berechnung der Basisadresse für den aktuellen Pixel
        int left_offset = ((left_x * HEIGHT) + y) * 3;
        int right_offset = ((right_x * HEIGHT) + y) * 3;

        // Schreiben der Farbwerte in die Framebuffer
        fbAdr_L[left_offset + 0] = b;
        fbAdr_L[left_offset + 1] = g;
        fbAdr_L[left_offset + 2] = r;

        fbAdr_R[right_offset + 0] = b;
        fbAdr_R[right_offset + 1] = g;
        fbAdr_R[right_offset + 2] = r;
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

// Clamping values to keep them between 0 and 1
float Clamp(float value)
{
    float fmin = 0;
    float fmax = 1;
    return MAX(fmin, MIN(value, fmax));
}

// drawing line between 2 points from left to right
// papb -> pcpd
// pa, pb, pc, pd must then be sorted before
void ProcessScanLine(int y, Vector3 pa, Vector3 pb, Vector3 pc, Vector3 pd, u32 color)
{
    if (y < 0 || y >= HEIGHT) return;

    // Berechnung der Gradienten
    float dy1 = pb.Y - pa.Y;
    float dy2 = pd.Y - pc.Y;

    // Berechnung der Start- und Endpunkte der Linie
    float gradient1 = (dy1 != 0) ? (y - pa.Y) / dy1 : 1;
    float gradient2 = (dy2 != 0) ? (y - pc.Y) / dy2 : 1;

    int sx = (int)(pa.X + gradient1 * (pb.X - pa.X));
    int ex = (int)(pc.X + gradient2 * (pd.X - pc.X));

    // Sicherstellen, dass sx < ex für konsistente Rasterisierung
    if (sx > ex)
    {
        int temp = sx;
        sx = ex;
        ex = temp;
    }

    // Clip once and reuse depth and color values for the whole scanline.
    int half_separation = get_stereo_separation((int)pa.Z * 2) / 2;
    int margin = abs(half_separation);
    sx = MAX(sx, margin);
    ex = MIN(ex, WIDTH_TOP - margin);
    if (sx >= ex) return;

    unsigned alpha = color & 255;
    unsigned inverse_alpha = 256 - alpha;
    unsigned r = ((color >> 24) & 255) * alpha;
    unsigned g = ((color >> 16) & 255) * alpha;
    unsigned b = ((color >> 8) & 255) * alpha;
    for (int x = sx; x < ex; x++)
    {
        int offset = (x * HEIGHT + y) * 3;
        int left = offset - half_separation * HEIGHT * 3;
        int right = offset + half_separation * HEIGHT * 3;
        // Preserve the existing blend against the unshifted left buffer.
        u8 blue = (inverse_alpha * fbAdr_L[offset] + b) >> 8;
        u8 green = (inverse_alpha * fbAdr_L[offset + 1] + g) >> 8;
        u8 red = (inverse_alpha * fbAdr_L[offset + 2] + r) >> 8;
        fbAdr_L[left] = fbAdr_R[right] = blue;
        fbAdr_L[left + 1] = fbAdr_R[right + 1] = green;
        fbAdr_L[left + 2] = fbAdr_R[right + 2] = red;
    }
}

void DrawTriangle(Vector3 p1, Vector3 p2, Vector3 p3, u32 color)
{
    // Sortiere die Punkte nach Y-Wert
    if (p1.Y > p2.Y) { Vector3 temp = p1; p1 = p2; p2 = temp; }
    if (p2.Y > p3.Y) { Vector3 temp = p2; p2 = p3; p3 = temp; }
    if (p1.Y > p2.Y) { Vector3 temp = p1; p1 = p2; p2 = temp; }

    // Berechne inverse Steigungen nur einmal
    float dP1P2 = (p2.Y > p1.Y) ? (p2.X - p1.X) / (p2.Y - p1.Y) : 0;
    float dP1P3 = (p3.Y > p1.Y) ? (p3.X - p1.X) / (p3.Y - p1.Y) : 0;

    // Wähle die Reihenfolge der Scanline-Verarbeitung basierend auf Steigung
    bool isRightHanded = dP1P2 > dP1P3;

    for (int y = MAX((int)p1.Y, 0); y <= MIN((int)p3.Y, HEIGHT - 1); y++)
    {
        if (y < p2.Y)
        {
            // Verarbeite den oberen Teil des Dreiecks
            if (isRightHanded)
                ProcessScanLine(y, p1, p3, p1, p2, color);
            else
                ProcessScanLine(y, p1, p2, p1, p3, color);
        }
        else
        {
            // Verarbeite den unteren Teil des Dreiecks
            if (isRightHanded)
                ProcessScanLine(y, p1, p3, p2, p3, color);
            else
                ProcessScanLine(y, p2, p3, p1, p3, color);
        }
    }
}

/* Static model normals are built once. No allocations occur while drawing. */
#define LIGHT_MODELS 8
#define LIGHT_FACES 128
static struct {
    const Vertex *vertices;
    const Face *faces;
    int count;
    Vector3 normals[LIGHT_FACES];
} lighting_cache[LIGHT_MODELS];

static Vector3 face_normal(Mesh mesh, int index)
{
    Face f = mesh.faces[index];
    Vertex a = mesh.vertexes[f.a], b = mesh.vertexes[f.b], c = mesh.vertexes[f.c];
    Vector3 ab = {b.x-a.x, b.y-a.y, b.z-a.z, 0};
    Vector3 ac = {c.x-a.x, c.y-a.y, c.z-a.z, 0};
    Vector3 normal = VectorCross(ab, ac);
    normal.W = 0;
    return VectorNormal(normal);
}

static Vector3 *model_normals(Mesh mesh)
{
    if (mesh.num_of_faces <= 0 || mesh.num_of_faces > LIGHT_FACES) return NULL;
    for (int slot = 0; slot < LIGHT_MODELS; slot++)
    {
        if (lighting_cache[slot].vertices == mesh.vertexes &&
            lighting_cache[slot].faces == mesh.faces &&
            lighting_cache[slot].count == mesh.num_of_faces)
            return lighting_cache[slot].normals;
        if (!lighting_cache[slot].vertices)
        {
            lighting_cache[slot].vertices = mesh.vertexes;
            lighting_cache[slot].faces = mesh.faces;
            lighting_cache[slot].count = mesh.num_of_faces;
            for (int i = 0; i < mesh.num_of_faces; i++)
                lighting_cache[slot].normals[i] = face_normal(mesh, i);
            return lighting_cache[slot].normals;
        }
    }
    return NULL;
}

// The main method of the engine that re-compute each vertex projection
// during each frame
void Render(Mesh mesh, bool view, enum object_type type)
{
    // Transformation der Positionen vorbereiten
    mesh.BackPosition.X = mesh.Position.X;
    mesh.BackPosition.Z = -mesh.Position.Z;
    mesh.BackPosition.Y = mesh.Position.Y;

    if (view ? (mesh.Position.Z <= 0) : (mesh.BackPosition.Z <= 0))
        return;

    // Weltmatrix vor der Schleife berechnen
    Matrix3 worldMatrix = MMMultiply(
        RotationYawPitchRoll(mesh.Rotation.X, mesh.Rotation.Y, mesh.Rotation.Z),
        view ? Translation(mesh.Position) : Translation(mesh.BackPosition)
    );

    // Cache shared vertices once per object; retain a fallback for larger meshes.
    Vector3 projected[128];
    bool cached = mesh.num_of_vertexes > 0 && mesh.num_of_vertexes <= 128;
    if (cached)
        for (int v = 0; v < mesh.num_of_vertexes; v++)
            projected[v] = Project(mesh.vertexes[v], worldMatrix);

    // Häufig verwendete Werte zwischenspeichern
    Vector3 *normals = model_normals(mesh);
    // Rotate a fixed, normalized light into model space once per object.
    Vector3 light = {
        worldMatrix.a1 * 0.36f + worldMatrix.a2 * 0.48f - worldMatrix.a3 * 0.8f,
        worldMatrix.b1 * 0.36f + worldMatrix.b2 * 0.48f - worldMatrix.b3 * 0.8f,
        worldMatrix.c1 * 0.36f + worldMatrix.c2 * 0.48f - worldMatrix.c3 * 0.8f, 0
    };
    bool isPhotonOrStarbase = (type == PHOTONT) || (type == STARBASE);
    bool isEnemyPhoton = (type == ENEMYPHOTONT);

    // Schleifenoptimierung
    for (int i = 0; i < mesh.num_of_faces; i++)
    {
        // Projektion von Eckpunkten vorbereiten
        Vector3 point1 = cached ? projected[mesh.faces[i].a] :
            Project(mesh.vertexes[mesh.faces[i].a], worldMatrix);
        Vector3 point2 = cached ? projected[mesh.faces[i].b] :
            Project(mesh.vertexes[mesh.faces[i].b], worldMatrix);
        Vector3 point3 = cached ? projected[mesh.faces[i].c] :
            Project(mesh.vertexes[mesh.faces[i].c], worldMatrix);

        // Farbberechnung
        Vector3 normal = normals ? normals[i] : face_normal(mesh, i);
        // Meshes contain both windings: light both sides without hiding faces.
        float intensity = 0.28f + 0.72f * fminf(1.0f, fabsf(VectorDot(normal, light)));
        if (type == PHOTONT || type == ENEMYPHOTONT) intensity = 1.0f;
        float r = intensity * 255, g = intensity * 255, b = intensity * 255;

        // Farbanpassung basierend auf dem Objekttyp
        if (isPhotonOrStarbase)
        {
            r = (r > 172) ? 255 : (r + 83);
            b = (b < 52) ? 0 : (b - 52);
        }
        else if (isEnemyPhoton)
        {
            r = (r > 185) ? 255 : (r + 70);
            b = (b > 185) ? 255 : (b + 70);
            g = (g < 70) ? 0 : (g - 70);
        }
        else if (shield_active)
        {
            g = (g > 205) ? 255 : (g + 50);
        }

        // Farbe in 32-Bit-RGBA kodieren
        u32 color = (u32)ColorsToRGBA((u8)r, (u8)g, (u8)b, 1);

        // Sichtbarkeit testen
        if (
            abs(point1.X) < 1200 && abs(point1.Y) < 1200 && abs(point1.Z) < 1200 &&
            abs(point2.X) < 1200 && abs(point2.Y) < 1200 && abs(point2.Z) < 1200 &&
            abs(point3.X) < 1200 && abs(point3.Y) < 1200 && abs(point3.Z) < 1200
        )
        {
            DrawTriangle(point1, point2, point3, color);
        }
    }
    visuals_object_glow(worldMatrix, type, fbAdr_L, fbAdr_R);
}
