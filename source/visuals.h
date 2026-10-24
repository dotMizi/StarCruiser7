#ifndef STARCRUISER_VISUALS_H
#define STARCRUISER_VISUALS_H

#include <3ds.h>
/* Include after vector.h, 3dmodels.h and game.h in implementation files. */
void visuals_reset(void);
void visuals_begin_frame(void);
void visuals_explosion(float x, float y, float z);
void visuals_player_hit(Vector3 source, bool protected_hit);
void visuals_advance(float distance);
void visuals_rotate(float alpha, float beta);
void visuals_draw_world(u8 *left, u8 *right);
void visuals_draw_impact(u8 *left, u8 *right);
void visuals_draw_target(u8 *left, u8 *right, int index, bool locked);
void visuals_object_glow(Matrix3 world, enum object_type type, u8 *left, u8 *right);

#endif
