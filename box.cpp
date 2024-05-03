#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "box.hpp"

using namespace App;

Box::Box(int iid, Vector2 ipos, Vector2 isize, Color icolor) {
  id = iid;
  position = ipos;
  size = isize;
  color = icolor;
  body = { ipos.x - isize.x/2, ipos.y - isize.y/2, isize.x, isize.y };
  shader = LoadShader("assets/box.vs", "assets/box.fs");
  target = LoadRenderTexture(isize.x, isize.y);
  SetTextureWrap(target.texture, TEXTURE_WRAP_CLAMP);
}

Box::Box(int iid, Vector2 ipos, Vector2 isize, Color icolor, float irot) {
  id = iid;
  position = ipos;
  size = isize;
  rotation = irot;
  color = icolor;
  body = {
    ipos.x - isize.x/2 * std::cos(irot * DEG2RAD) + isize.y/2 * std::sin(irot * DEG2RAD),
    ipos.y - isize.x/2 * std::sin(irot * DEG2RAD) - isize.y/2 * std::cos(irot * DEG2RAD),
    isize.x,
    isize.y
  };
  shader = LoadShader("assets/box.vs", "assets/box.fs");
  target = LoadRenderTexture(isize.x, isize.y);
  SetTextureWrap(target.texture, TEXTURE_WRAP_CLAMP);
}

void Box::update(Vector2 pos, float rot) {
  position = pos;
  rotation = rot;
  body.x = pos.x - size.x/2 * std::cos(rot * DEG2RAD) + size.y/2 * std::sin(rot * DEG2RAD);
  body.y = pos.y - size.x/2 * std::sin(rot * DEG2RAD) - size.y/2 * std::cos(rot * DEG2RAD);
}

void Box::updateTexture() {
  BeginTextureMode(target);
    BeginShaderMode(shader);
      DrawRectangle(0, 0, size.x, size.y, color);
    EndShaderMode();
  EndTextureMode();
}

void Box::render() {
  DrawRectanglePro(body, (Vector2){0, 0}, rotation, color);
}

void Box::renderNormal() {
  Rectangle source = {0.0f, 0.0f, size.x, size.y};
  DrawTexturePro(target.texture, source, body, (Vector2){0.0f, 0.0f}, rotation, WHITE);
}

void Box::cleanup() {
  UnloadShader(shader);
  UnloadRenderTexture(target);
}
