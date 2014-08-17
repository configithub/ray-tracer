#include "ray_tracer.h"
#include <tgmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int compare_ray(const void *va, const void *vb) {
  const ray *a = va; const ray *b = vb;
  if(a->angle < b->angle) { return -1; }
  else if(a->angle > b->angle) { return 1; }
  return 0;
}

bool line_intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, point *result) {
  int d =  (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
  if(d == 0) {
    return false;
  }else{
    int n = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
    float s = (float)n / d;
    result->x = (long) (x1 + s * (x2 - x1) + 0.5);
    result->y = (long) (y1 + s * (y2 - y1) + 0.5); 
    return true;
  }
}

bool is_segment_boundary(segment *s, point *p) {
  return ((s->a.x == p->x && s->a.y == p->y) ||
       (s->b.x == p->x && s->b.y == p->y));
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

bool ray_crosses_segment(segment *s, ray *r, point *intersect) {
  if(line_intersection(s->a.x, s->a.y, s->b.x, s->b.y, 
          r->center.x, r->center.y, r->out.x, r->out.y, intersect)) {
    if(is_intersect_in_ray(r, intersect) && is_intersect_in_segment(s, intersect)) {
      return true;
    }
  }
  return false;
}

void add_edge_rays(float angle, int x, int y) {
  point edge; int d = 20;
  edge.x = (int) (x + d*cos(angle)+0.5);
  edge.y = (int) (y + d*sin(angle)+0.5);
  rays[current_ray].center.x = x;
  rays[current_ray].center.y = y;
  rays[current_ray].out = edge;
  rays[current_ray].angle = angle - theta;
  while(rays[current_ray].out.x > 0 
      && rays[current_ray].out.x < WMAP 
      && rays[current_ray].out.y > 0 
      && rays[current_ray].out.y < HMAP) {
    rays[current_ray].out.x += 10*(edge.x - x);
    rays[current_ray].out.y += 10*(edge.y - y);
  }
  ++current_ray;
} 

void extend_ray(ray *r) {
  while(r->out.x > 0 && r->out.x < WMAP 
      && r->out.y > 0 && r->out.y < HMAP) {
    r->out.x += 10*(r->out.x - r->center.x);
    r->out.y += 10*(r->out.y - r->center.y);
  }
}

float distance(point *a, point *b) {
  return sqrt( (a->x - b->x)*(a->x - b->x)
         + (a->y - b->y)*(a->y - b->y));
}

int last_x = 0; int last_y = 0;

void calculate_ray_triangle(int x, int y) {
  // if we already have done this for the coordinates,
  // don't do it again
  if(last_x == x && last_y == y) { return; }else{
    last_x = x; last_y = y;
  }
  // find each ray and order them by angle
  current_ray = 0;
  current_triangle = 0;
  if(is_pov) {
    add_edge_rays(theta + pov, x, y);
    add_edge_rays(theta - pov, x, y);
  } 
  for (int i = 0; i < current_ray_corner; ++i) {
    if(x == ray_corners[i].x && y == ray_corners[i].y) {
      continue;
    }
    float angle = atan2( (ray_corners[i].y - y), ( ray_corners[i].x - x) ) - theta;
    if(angle < -pi) { angle += 2 * pi; } else if(angle > pi) { angle -= 2 * pi; }
    if(is_pov && ((angle < -pov) || (angle > pov))) { continue; }
    else{
      // initialize new ray
      rays[current_ray].angle = angle;
      rays[current_ray].center.x = x;
      rays[current_ray].center.y = y;
      rays[current_ray].out = ray_corners[i];
      extend_ray(&rays[current_ray]);
      ++current_ray;
    }
  }

  // sort ray by angle
  qsort(rays, current_ray, sizeof(*rays), compare_ray);
  // find all intersects
  for (int j = 0; j < current_ray; ++j) {
    int next = (j+1) % current_ray;
    point inter1, inter2;
    float distance1 = INF, distance2 = INF;
    for (int i = 0; i < current_ray_obstacle; ++i) {
      point i1 = {0,0}, i2 = {0,0};
      if(ray_crosses_segment(&ray_obstacles[i], &rays[j], &i1)
        && ray_crosses_segment(&ray_obstacles[i], &rays[next], &i2)) {
        float d1 = distance(&rays[j].center, &i1);
        float d2 = distance(&rays[next].center, &i2);
        if(d1 <= distance1 && d2 <= distance2) {
          inter1 = i1; inter2 = i2;
          distance1 = d1; distance2 = d2;
        }
      }  
    }
    triangles[current_triangle].a = inter1;
    triangles[current_triangle].b = inter2;
    ++current_triangle;
  }
}

