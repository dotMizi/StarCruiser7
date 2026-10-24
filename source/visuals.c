#include <3ds.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "vector.h"
#include "3dmodels.h"
#include "game.h"
#include "render.h"
#include "colors.h"
#include "visuals.h"

/* Fixed pools and small, clipped sprites keep the extra work bounded. */
#define BURST_COUNT 8
#define BURST_MS 450
#define IMPACT_MS 280

typedef struct { Vector3 position; u64 started; bool active; } Burst;
static Burst bursts[BURST_COUNT];
static unsigned next_burst;
static u64 frame_time, impact_started, acquired_at;
static bool impact_active, impact_protected, previous_lock;
static int impact_side, previous_target = -1;
extern void draw_text(char *, u8 *, int, int, int, int);

static int clampi(int value, int low, int high)
{
    return value < low ? low : value > high ? high : value;
}

static void add_pixel(u8 *buffer, int x, int y, int r, int g, int b)
{
    if ((unsigned)x >= WIDTH_TOP || (unsigned)y >= HEIGHT) return;
    u8 *pixel = buffer + (x * HEIGHT + y) * 3;
    pixel[0] = clampi(pixel[0] + b, 0, 255);
    pixel[1] = clampi(pixel[1] + g, 0, 255);
    pixel[2] = clampi(pixel[2] + r, 0, 255);
}

static bool project_position_near(Vector3 p, int *x, int *y, int *separation, float near_depth)
{
    float depth = front_view ? p.Z : -p.Z;
    if (!isfinite(depth) || depth <= near_depth || depth > MAX_DIST * 8.0f) return false;
    float px = FOCAL_DISTANCE * p.X / depth + WIDTH_TOP / 2;
    float py = FOCAL_DISTANCE * p.Y / depth + HEIGHT / 2;
    if (!isfinite(px) || !isfinite(py) || px < -40 || px > WIDTH_TOP + 40 ||
        py < -40 || py > HEIGHT + 40) return false;
    *x = (int)px;
    *y = (int)py;
    *separation = clampi(get_stereo_separation((int)depth * 2) / 2, 0, 6);
    return true;
}

static bool project_position(Vector3 p, int *x, int *y, int *separation)
{
    return project_position_near(p, x, y, separation, 20.0f);
}

static void glow(u8 *left, u8 *right, int x, int y, int separation,
                 int radius, int r, int g, int b)
{
    radius = clampi(radius, 1, 18);
    int radius_squared = radius * radius + 1;
    for (int dx = -radius; dx <= radius; dx++)
        for (int dy = -radius; dy <= radius; dy++)
        {
            int weight = radius_squared - dx * dx - dy * dy;
            if (weight <= 0) continue;
            int red = r * weight / radius_squared;
            int green = g * weight / radius_squared;
            int blue = b * weight / radius_squared;
            add_pixel(left, x + dx - separation, y + dy, red, green, blue);
            add_pixel(right, x + dx + separation, y + dy, red, green, blue);
        }
}

void visuals_reset(void)
{
    memset(bursts, 0, sizeof(bursts));
    next_burst = 0;
    impact_active = false;
    previous_target = -1;
    previous_lock = false;
}

void visuals_begin_frame(void)
{
    frame_time = osGetTime();
}

void visuals_explosion(float x, float y, float z)
{
    bursts[next_burst] = (Burst){ {x, y, z, 1}, osGetTime(), true };
    next_burst = (next_burst + 1) % BURST_COUNT;
}

void visuals_player_hit(Vector3 source, bool protected_hit)
{
    impact_started = osGetTime();
    impact_active = true;
    impact_protected = protected_hit;
    if (fabsf(source.X) + fabsf(source.Y) < 5 || source.Z < 0)
        impact_side = 0; /* Central or rear hit: use a narrow frame. */
    else if (fabsf(source.X) > fabsf(source.Y))
        impact_side = source.X < 0 ? 1 : 2;
    else
        impact_side = source.Y < 0 ? 3 : 4;
}

void visuals_advance(float distance)
{
    for (int i = 0; i < BURST_COUNT; i++)
        if (bursts[i].active) bursts[i].position.Z -= distance;
}

void visuals_rotate(float alpha, float beta)
{
    Matrix3 rotation = RotationYawPitchRoll(-alpha, -beta, 0);
    for (int i = 0; i < BURST_COUNT; i++)
        if (bursts[i].active)
            bursts[i].position = TransformCoordinate(rotation, bursts[i].position);
}

void visuals_draw_world(u8 *left, u8 *right)
{
    for (int i = 0; i < BURST_COUNT; i++)
    {
        if (!bursts[i].active) continue;
        u64 age = frame_time - bursts[i].started;
        if (age >= BURST_MS || warp_state == IN_HYPERSPACE)
        {
            bursts[i].active = false;
            continue;
        }
        int x, y, separation;
        if (!project_position(bursts[i].position, &x, &y, &separation)) continue;
        float depth = fabsf(bursts[i].position.Z);
        int radius = clampi((int)((8 + age * 0.04f) * FOCAL_DISTANCE / depth), 2, 18);
        int fade = (BURST_MS - age) * 255 / BURST_MS;
        /* Brief white impact, followed by an expanding warm burst. */
        glow(left, right, x, y, separation, radius, fade,
             age < 75 ? fade : fade * 3 / 5, age < 75 ? fade : fade / 8);
    }

    int stereo = get_stereo_separation(MAX_DIST);
    for (int i = 0; i < MAX_NUM_DEBRIS; i++)
    {
        if (!debris[i].active || debris[i].ttl <= 0) continue;
        Vector3 p = {debris[i].pos.x, debris[i].pos.y, debris[i].pos.z, 1};
        int x, y, separation;
        // Keep fragments visible almost up to the camera's passage plane.
        if (!project_position_near(p, &x, &y, &separation, 1.0f)) continue;
        float depth = fabsf(p.Z);
        // Restore the original cloud's stronger near-field stereo depth.
        separation = stereo / 2;
        if (depth > MAX_DIST / 10) separation = stereo * 3 / 8;
        if (depth > MAX_DIST / 7) separation = stereo / 4;
        if (depth > MAX_DIST / 2) separation = stereo / 8;
        if (depth > 2 * MAX_DIST / 3) separation = 0;
        // Solid fragments retain the original four-pixel minimum size.
        int size = DEBRIS_WIDTH + clampi((int)(FOCAL_DISTANCE * 0.4f / depth), 0, 4);
        int fade = clampi(debris[i].ttl * 255 / 20, 0, 255);
        int brightness = clampi((int)(255.0f * (1.0f - depth / MAX_DIST) + 55), 55, 255);
        int r = brightness * fade / 255;
        int g = r;
        int b = r;
        if (shield_active)
        {
            g = clampi(brightness + 30, 0, 255) * fade / 255;
            r /= 2;
        }
        for (int dx = -size / 2; dx < size - size / 2; dx++)
            for (int dy = -size / 2; dy < size - size / 2; dy++)
            {
                add_pixel(left, x + dx - separation, y + dy, r, g, b);
                add_pixel(right, x + dx + separation, y + dy, r, g, b);
            }
    }
}

void visuals_draw_impact(u8 *left, u8 *right)
{
    if (!impact_active) return;
    u64 age = frame_time - impact_started;
    if (age >= IMPACT_MS) { impact_active = false; return; }
    int fade = (IMPACT_MS - age) * 150 / IMPACT_MS;
    for (int layer = 0; layer < 6; layer++)
    {
        int strength = fade * (6 - layer) / 6;
        int r = impact_protected ? strength / 5 : strength;
        int g = impact_protected ? strength * 2 / 3 : strength / 5;
        int b = impact_protected ? strength : strength / 12;
        for (int x = 0; x < WIDTH_TOP; x++)
        {
            if (impact_side == 0 || impact_side == 3)
            { add_pixel(left,x,layer,r,g,b); add_pixel(right,x,layer,r,g,b); }
            if (impact_side == 0 || impact_side == 4)
            { add_pixel(left,x,HEIGHT-1-layer,r,g,b); add_pixel(right,x,HEIGHT-1-layer,r,g,b); }
        }
        for (int y = 6; y < HEIGHT - 6; y++)
        {
            if (impact_side == 0 || impact_side == 1)
            { add_pixel(left,layer,y,r,g,b); add_pixel(right,layer,y,r,g,b); }
            if (impact_side == 0 || impact_side == 2)
            { add_pixel(left,WIDTH_TOP-1-layer,y,r,g,b); add_pixel(right,WIDTH_TOP-1-layer,y,r,g,b); }
        }
    }
}

void visuals_draw_target(u8 *left, u8 *right, int index, bool locked)
{
    if (index < 0 || index >= MAX_NUM_OF_ENEMIES || !render_object[index].active)
    { previous_target = -1; previous_lock = false; return; }
    if (index != previous_target || (locked && !previous_lock)) acquired_at = frame_time;
    previous_target = index;
    previous_lock = locked;
    Vector3 position = render_object[index].mesh.Position;
    int x, y, ignored;
    if (!project_position(position, &x, &y, &ignored)) return;
    if (x < 16 || x >= WIDTH_TOP - 16 || y < 16 || y >= HEIGHT - 16) return;
    /* HUD brackets stay at screen depth to avoid rapid stereo jumps. */
    int radius = clampi((int)(FOCAL_DISTANCE * 12.0f / fabsf(position.Z)) + 5, 14, 36);
    bool acquired = frame_time - acquired_at < 180;
    int r = acquired ? 255 : locked ? 70 : 235;
    int g = locked ? 255 : 190;
    int b = acquired ? 255 : 70;
    for (int corner_x = -1; corner_x <= 1; corner_x += 2)
        for (int corner_y = -1; corner_y <= 1; corner_y += 2)
            for (int length = 0; length < 6; length++)
            {
                int xx = x + corner_x * radius, yy = y + corner_y * radius;
                add_pixel(left,xx-corner_x*length,yy,r,g,b);
                add_pixel(right,xx-corner_x*length,yy,r,g,b);
                add_pixel(left,xx,yy-corner_y*length,r,g,b);
                add_pixel(right,xx,yy-corner_y*length,r,g,b);
            }
    char label[32];
    float distance = VectorLength(position);
    if (!isfinite(distance) || distance > 999999) return;
    snprintf(label, sizeof(label), "%s %d METRONS", locked ? "LOCK" : "TARGET", (int)distance);
    int label_width = (int)strlen(label) * 8;
    int label_x = clampi(x - label_width / 2, 4, WIDTH_TOP - label_width - 4);
    int label_y = y - radius - 13;
    if (label_y < 4) label_y = y + radius + 6;
    label_y = clampi(label_y, 4, HEIGHT - 12);
    draw_text(label,left,label_x,label_y,locked ? C_GREEN : C_YELLOW,WIDTH_TOP);
    draw_text(label,right,label_x,label_y,locked ? C_GREEN : C_YELLOW,WIDTH_TOP);
}

void visuals_object_glow(Matrix3 world, enum object_type type, u8 *left, u8 *right)
{
    bool photon = type == PHOTONT || type == ENEMYPHOTONT;
    bool engine = type == FIGHTER || type == RAIDER || type == SHUTTLE;
    if (!photon && (!engine || world.c3 > -0.15f)) return;
    Vector3 local = photon ? (Vector3){0,0,0,1} : (Vector3){0,0,6,1};
    Vector3 position = TransformCoordinate(world, local);
    /* world already contains the selected front/back view translation. */
    if (!front_view) position.Z = -position.Z;
    int x, y, separation;
    if (!project_position(position, &x, &y, &separation)) return;
    int radius = clampi((int)((photon ? 5.0f : 3.0f) * FOCAL_DISTANCE / fabsf(position.Z)), 2, 8);
    int pulse = 190 + (int)((frame_time / 45) % 3) * 20;
    glow(left, right, x, y, separation, radius,
         photon ? pulse : 40, photon ? (type == PHOTONT ? pulse * 3 / 4 : 60) : 120,
         photon ? (type == PHOTONT ? 30 : pulse) : pulse);
    glow(left, right, x, y, separation, 1, 200, 220, 255);
}
