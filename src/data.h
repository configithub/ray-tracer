#ifndef _DATA_H_
#define _DATA_H_

#define WWIDTH 400
#define WHEIGHT 300
#define BPP_DEPTH 16

typedef struct {
  int w;
  int h;
} wh;

#define max_texture_sheets 50
wh wh_array[max_texture_sheets]; 

typedef struct {
  int x;
  int y;
} point;

#define max_ray_corners 200
point ray_corners[max_ray_corners]; 
int current_ray_corner;

typedef struct {
  point a;
  point b;
} segment;

#define max_ray_obstacles 200
segment ray_obstacles[max_ray_obstacles];
int current_ray_obstacle;

typedef struct {
  point a;
  point b;
  point c;
  point d;
} quad;
#define max_quad 200
quad quads[max_quad];
int current_quad;

typedef struct {
  point center;
  point out;
  float angle; 
} ray;

#define max_rays 200
ray rays[max_rays];
int current_ray;

void register_ray_corner(int x, int y);
void register_ray_obstacle(int x1, int y1, int x2, int y2);

#endif
