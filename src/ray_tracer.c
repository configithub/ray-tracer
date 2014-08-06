#include "ray_tracer.h"
#include <tgmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int compare_ray(const void *va, const void *vb) {
  const ray *a = va;
  const ray *b = vb;
  if((*a).angle < (*b).angle) { return -1; }
  else if((*a).angle > (*b).angle) { return 1; }
  return 0;
}

bool line_intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, point *result) {
  float s;
  int d =  (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
  if(d == 0) {
    return false;
  }else{
    int n = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    s = (float)n / d;
    result->x = (long) (x1 + s * (x2 - x1) + 0.5);
    result->y = (long) (y1 + s * (y2 - y1) + 0.5); 
    return true;
  }
}

bool is_intersect_in_ray(ray *r, point *p) {
  return ( ( ( r->center.x <= p->x && p->x <= r->out.x )
          || ( r->out.x <= p->x && p->x <= r->center.x ) )
            &&
           ( ( r->center.y <= p->y && p->y <= r->out.y )
          || ( r->out.y <= p->y && p->y <= r->center.y ) ) );
}

bool is_intersect_in_segment(segment *s, point *p) {
  return ( ( ( s->b.x <= p->x && p->x <= s->a.x )
          || ( s->a.x <= p->x && p->x <= s->b.x ) )
            &&
           ( ( s->b.y <= p->y && p->y <= s->a.y )
          || ( s->a.y <= p->y && p->y <= s->b.y ) ) );
}

int last_x = 0;
int last_y = 0;

void calculate_ray_triangle(int x, int y) {
  // if we already have done this for the coordinates,
  // don't do it again
  if(last_x == x && last_y == y) { return; }else{
    last_x = x; last_y = y;
  }
  // find each ray and order them by angle
  current_ray = 0;
  for (int i = 0; i < current_ray_corner; ++i) {
    if(x == ray_corners[i].x && y == ray_corners[i].y) {
      continue;
    }
    rays[current_ray].center.x = x;
    rays[current_ray].center.y = y;
    rays[current_ray].out = ray_corners[i];
    while(rays[current_ray].out.x > 0 
        && rays[current_ray].out.x < WWIDTH 
        && rays[current_ray].out.y > 0 
        && rays[current_ray].out.y < WHEIGHT) {
      rays[current_ray].out.x += 4*(ray_corners[i].x - x);
      rays[current_ray].out.y += 4*(ray_corners[i].y - y);
    }
    rays[current_ray].angle = atan2( (ray_corners[i].y - y), ( ray_corners[i].x - x) );
    ++current_ray;
  }

  // sort ray by angle
  qsort(rays, current_ray, sizeof(*rays), compare_ray);
  // find all intersects
  for (int i = 0; i < current_ray_obstacle; ++i) {
    for (int j = 0; j < current_ray; ++j) {
      point intersect;
      if(!line_intersection(rays[j].center.x, rays[j].center.y,
                        rays[j].out.x, rays[j].out.y,
                        ray_obstacles[i].a.x, ray_obstacles[i].a.y,
                        ray_obstacles[i].b.x, ray_obstacles[i].b.y,
                        &intersect)) { continue; }
      if(is_intersect_in_ray(&rays[j], &intersect)  &&
         is_intersect_in_segment(&ray_obstacles[i], &intersect)) {
        rays[j].out = intersect;
      }
    }
  }
}


