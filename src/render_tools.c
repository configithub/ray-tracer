#include "render_tools.h"


bool init_sdl() {
  if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK) == -1 ) {
    return false;
  }
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  if(! SDL_SetVideoMode(WWIDTH, WHEIGHT, BPP_DEPTH, SDL_OPENGL|SDL_DOUBLEBUF)  ) {
    return false;
  }
  glEnable(GL_BLEND);
  glViewport(0, 0, WWIDTH, WHEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WWIDTH, WHEIGHT, 0, -1, 1);
  glScalef(1, 1, 1);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);
  // window corner as ray corners
  register_ray_obstacle(0, 0, WMAP, 0);
  register_ray_obstacle(WMAP, 0, WMAP, HMAP);
  register_ray_obstacle(0, HMAP, WMAP, HMAP);
  register_ray_obstacle(0, HMAP, 0, 0);
  return true;
}


void clear_screen() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void load_png(char* name, int* texture_id) {
  SDL_Surface *pic = IMG_Load(name);
  GLenum glFormat = (pic->format->BitsPerPixel == 32 ? GL_RGBA : GL_RGB);
  GLuint currentTexture = 0;
  glGenTextures(1, &currentTexture);

  glBindTexture(GL_TEXTURE_2D, currentTexture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, pic->w, pic->h, 0, glFormat, GL_UNSIGNED_BYTE, NULL);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pic->w, pic->h, glFormat, GL_UNSIGNED_BYTE, pic->pixels);

  wh_array[currentTexture].w = pic->w;
  wh_array[currentTexture].h = pic->h;
  SDL_FreeSurface(pic);
  *texture_id = currentTexture;
}

void draw_line(int x1, int y1, int x2, int y2) {
  glColor4f(0.0f,1.0f,0.0f,1.0f);
  GLfloat vertices[] = {x1,y1, x2,y2};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_VERTEX_ARRAY);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void draw_white_line(int x1, int y1, int x2, int y2) {
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  GLfloat vertices[] = {x1,y1, x2,y2};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_VERTEX_ARRAY);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void draw_triangle(int x1, int y1, int x2, int y2, 
                                        int x3, int y3) {
  GLfloat vertices[] = {x1,y1, x2,y2, x3,y3};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}


void draw_all_squares() {
  for (int i = 0; i < current_quad; ++i) {
    draw_hollow_square(quads[i].a.x, quads[i].a.y,
                quads[i].b.x, quads[i].b.y,
                quads[i].c.x, quads[i].c.y,
                quads[i].d.x, quads[i].d.y);
  }
}


void draw_hollow_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4) {
  draw_line(x1, y1, x2, y2);
  draw_line(x2, y2, x3, y3);
  draw_line(x3, y3, x4, y4);
  draw_line(x4, y4, x1, y1);
}

void draw_square(int x1, int y1, int x2, int y2, 
                 int x3, int y3, int x4, int y4) {
  GLfloat vertices[] = {x1,y1, x2,y2, x3,y3, x4,y4};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_all_rays(int *x, int *y) {
  for (int i = 0; i < current_ray; ++i) {
    draw_line(*x, *y, rays[i].out.x, rays[i].out.y);
  }
}

void draw_ray_light_surface(int x, int y) {
  if(current_triangle == 0) { return; }
  for (int i = 0; i < current_triangle; ++i) {
    draw_triangle(x, y, triangles[i].a.x, triangles[i].a.y,
              triangles[i].b.x, triangles[i].b.y);
    // cover aliasing artifacts between triangles
    draw_white_line(x, y, triangles[i].a.x, triangles[i].a.y);
    draw_white_line(x, y, triangles[i].b.x, triangles[i].b.y);
  }
}

void draw_rectangle_texture(GLuint texture, int x, int y, 
                    int xtex, int ytex, int wtex, int htex, 
                GLfloat r, GLfloat g, GLfloat b, GLfloat a) {

  int totalTexW = wh_array[texture].w;
  int totalTexH = wh_array[texture].h;

  float x1 = xtex / (float)totalTexW;  
  float y1 = ytex / (float)totalTexH;  

  float x2 = (xtex + wtex) / (float)totalTexW;  
  float y2 = (ytex + htex) / (float)totalTexH;  

  GLfloat textureCoordinates[] = { x1, y1, // top left
    x2, y1,  // top right
    x2, y2,  // bottom right
    x1, y2}; // bottom left
  GLfloat vertices[] = { (float)x, (float)y, // top left
    (float)x + wtex, (float)y, // top right
    (float)x + wtex, (float)y + htex, // bottom right
    (float)x, (float)y + htex }; // bottom left
  glColor4f(r, g, b, a);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinates);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}




