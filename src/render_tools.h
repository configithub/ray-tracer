#ifndef _RENDER_TOOLS_H_
#define _RENDER_TOOLS_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <stdbool.h>
#include "data.h"

typedef struct {
  int x; int y;
} fpoint;

bool init_sdl();

void clear_screen();

void load_png(char* name, int* texture_id);

void draw_line(int x1, int y1, int x2, int y2);

void draw_triangle(int x1, int x2, int y1, int y2, 
                                        int x3, int y3);

void draw_all_squares();

void draw_hollow_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4);

void draw_square_gradient(int x1, int y1, int x2, int y2, 
                        int x3, int y3, int x4, int y4,
                        float r1, float g1, float b1,
                        float r2, float g2, float b2,
                        float r3, float g3, float b3,
                        float r4, float g4, float b4);

void draw_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4);

void draw_all_rays(int *x, int *y);

void draw_ray_light_surface();

void draw_surfaces();

void draw_rectangle_texture(GLuint texture, int x, int y, 
                  int xtex, int ytex, int wtex, int htex, 
                GLfloat r, GLfloat g, GLfloat b, GLfloat a);

void draw_point(int x, int y, int r);
void draw_point_rayob(int x, int y, int r);
void draw_ngon(fpoint* center, int r, int n);
void draw_ngon2(fpoint* center, int r, int n, int d);


#endif
