#include "main.h"
#include <tgmath.h>
#include <stdbool.h>

SDL_Event events;
bool running;

int mouse_x, mouse_y;


void manage_inputs() {
  while(SDL_PollEvent(&events)) {
    if(events.type == SDL_QUIT)
      running = false;
  }
  SDL_GetMouseState(&mouse_x, &mouse_y);
}

void init_position() {
  mouse_x = 0;
  mouse_y = 0;
  pos_x = WMAP / 2; 
  pos_y = HMAP / 2;
}

void init_math_constants() { 
  pi = acos(0) * 2;
  pov = pi / 4;
  is_pov = true;
  theta = pi / 2;
  // printf("%f\n", halfpi);
}


void init_quad(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  quads[current_quad].a.x = x1;
  quads[current_quad].a.y = y1;
  quads[current_quad].b.x = x2;
  quads[current_quad].b.y = y2;
  quads[current_quad].c.x = x3;
  quads[current_quad].c.y = y3;
  quads[current_quad].d.x = x4;
  quads[current_quad].d.y = y4;
  register_ray_obstacle(x1, y1, x2, y2);
  register_ray_obstacle(x2, y2, x3, y3);
  register_ray_obstacle(x3, y3, x4, y4);
  register_ray_obstacle(x4, y4, x1, y1);
  ++current_quad;
}

void init_shapes() {
  init_quad(WMAP/2,-40+HMAP/2, 40+WMAP/2,HMAP/2,
            WMAP/2,40+HMAP/2, -40+WMAP/2,HMAP/2);
  init_quad(250, 100, 300, 100, 300, 150, 250, 150);
  init_quad(50, 90, 120, 70, 120, 250, 50, 150);
}

void render() { 
  clear_screen();
  draw_all_squares();
  //draw_ray_light_surface(mouse_x, mouse_y);
  //draw_all_rays(&mouse_x, &mouse_y);
  draw_surfaces();
  SDL_GL_SwapBuffers();
}


void loop() {
  running = true;
  while(running) { 
    manage_inputs();
    calculate_ray_triangle(mouse_x, mouse_y);
    render();
  }
}


int main(int argc, char **argv) {
  init_sdl();
  init_shapes();
  init_position();
  init_math_constants();
  loop();
}
