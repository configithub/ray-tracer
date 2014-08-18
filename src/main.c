#include "main.h"
#include <tgmath.h>
#include <stdbool.h>

SDL_Event events;
bool running;

int mouse_x, mouse_y;



void key_down3d(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
    case SDLK_UP:
      accel_y = 5;
    break;
    case SDLK_DOWN:
      accel_y = -5;
    break;
    case SDLK_RIGHT:
      accel_x = 5;
    break;
    case SDLK_LEFT:
      accel_x = -5;
    break;
  }
}

void key_up(SDLKey sym, SDLMod mod, Uint16 unicode) { 
  switch(sym) {
    case SDLK_SPACE:
      if(mode_3d) {mode_3d = false;}else{mode_3d = true;}
    break;
  }
}

void manage_speed() {
  int max_speed = 5;
  int friction = 1;
  int boundary = 3;
  // do accel
  speed_x += accel_x;
  speed_y += accel_y;
  // speed friction
  if(speed_x > 0) { speed_x -= friction; }
  else if(speed_x < 0) { speed_x += friction; }
  if(speed_y > 0) { speed_y -= friction; }
  else if(speed_y < 0) { speed_y += friction; }
  // accel friction
  if(accel_x > 0) { accel_x -= friction; }
  else if(accel_x < 0) { accel_x += friction; }
  if(accel_y > 0) { accel_y -= friction; }
  else if(accel_y < 0) { accel_y += friction; }
  // cap speed
  if(speed_x > max_speed) { speed_x = max_speed; }
  else if(speed_x < -max_speed) { speed_x = -max_speed; }
  if(speed_y > max_speed) { speed_y = max_speed; }
  else if(speed_y < -max_speed) { speed_y = -max_speed; }
  // do motion
  pos_y +=(int) (speed_x * cos(theta) + speed_y * sin(theta)+0.5);
  pos_x +=(int) (speed_y * cos(theta) - speed_x * sin(theta)+0.5);
  // stay in map
  if(pos_x < boundary) { pos_x = boundary; }
  else if(pos_x > WMAP-boundary) { pos_x = WMAP-boundary; }
  if(pos_y < boundary) { pos_y = boundary; }
  else if(pos_y > HMAP-boundary) { pos_y = HMAP-boundary; }
}


void manage_inputs3d() {
  while(SDL_PollEvent(&events)) {
    if(events.type == SDL_QUIT)
      running = false;
    if(events.type ==  SDL_KEYDOWN) 
      key_down3d(events.key.keysym.sym,events.key.keysym.mod,events.key.keysym.unicode);
    if(events.type ==  SDL_KEYUP) 
      key_up(events.key.keysym.sym,events.key.keysym.mod,events.key.keysym.unicode);
  }
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  theta += (float)mouse_x / 20; 
  if(theta < -pi) { theta += 2*pi; }
  else if(theta > pi) { theta -= 2*pi; }
}

void manage_inputs() {
  while(SDL_PollEvent(&events)) {
    if(events.type == SDL_QUIT)
      running = false;
    if(events.type ==  SDL_KEYUP) 
      key_up(events.key.keysym.sym,events.key.keysym.mod,events.key.keysym.unicode);
  }
  SDL_GetMouseState(&mouse_x, &mouse_y);
}

void init_position() {
  mouse_x = 0;
  mouse_y = 0;
  pos_x = WMAP / 2; 
  pos_y = HMAP / 2;
  accel_x = 0;
  accel_y = 0;
  speed_x = 0;
  speed_y = 0;
}

void init_math_constants() { 
  pi = acos(0) * 2;
  pov = pi / 4;
  is_pov = true;
  mode_3d = true;
  theta = - pi / 2;
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
  draw_ray_light_surface(pos_x, pos_y);
  draw_all_rays(&pos_x, &pos_y);
  SDL_GL_SwapBuffers();
}

void render3d() { 
  clear_screen();
  draw_surfaces();
  SDL_GL_SwapBuffers();
}

void loop() {
  running = true;
  while(running) { 
    manage_inputs3d();
    manage_speed();
    calculate_ray_triangle(pos_x, pos_y);
    if(mode_3d) {
        render3d();
    }else{
        render();
    }
  }
}


int main(int argc, char **argv) {
  init_sdl();
  init_shapes();
  init_position();
  init_math_constants();
  loop();
}
