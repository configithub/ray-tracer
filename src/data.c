#include "data.h"
int current_ray = 0;
int current_ray_corner = 0;
int current_ray_obstacle = 0;
int current_quad = 0;
int current_triangle = 0;


float pi = 0;
float theta = 0;
float pov = 0;
bool is_pov = false;
bool mode_3d = false;

int pos_x = 0;
int pos_y = 0;

void register_ray_corner(int x, int y) {
  // we will potentially check every point registered here,
  // bad performances
  for (int i = 0; i < current_ray_corner; ++i) {
    if(ray_corners[i].x == x && ray_corners[i].y == y) { 
      return; }
  }
  ray_corners[current_ray_corner].x = x;
  ray_corners[current_ray_corner].y = y;
  ++current_ray_corner;
}

void register_ray_obstacle(int x1, int y1, int x2, int y2) {
  // we will potentially check every line registered here,
  // bad performances
  for (int i = 0; i < current_ray_obstacle; ++i) {
    if(ray_obstacles[i].a.x == x1 
    && ray_obstacles[i].a.y == y1
    && ray_obstacles[i].b.x == x2
    && ray_obstacles[i].b.y == y2) { return; }
  }
  register_ray_corner(x1, y1);
  register_ray_corner(x2, y2);
  ray_obstacles[current_ray_obstacle].a.x = x1;
  ray_obstacles[current_ray_obstacle].a.y = y1;
  ray_obstacles[current_ray_obstacle].b.x = x2;
  ray_obstacles[current_ray_obstacle].b.y = y2;
  ++current_ray_obstacle;
}


