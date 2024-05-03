#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "light.hpp"

using namespace App;

Light::Light(int iid, Vector2 ipos, float iradius, Color icolor, float iintensity) {
  id = iid;
  position = ipos;
  radius = iradius;
  color = icolor;
  intensity = iintensity;
  // load shader
  shader = LoadShader("assets/light.vs", "assets/light.fs");
  shaderLoc[0] = GetShaderLocation(shader, "lightPos");
  shaderLoc[1] = GetShaderLocation(shader, "lightIntensity");
  shaderLoc[2] = GetShaderLocation(shader, "lightRadius");
  shaderLoc[3] = GetShaderLocation(shader, "lightColor");
  // load buffer
  lightBuffer = LoadRenderTexture(800, 600);
  SetTextureWrap(lightBuffer.texture, TEXTURE_WRAP_CLAMP);
}

void Light::update(Vector2 pos) {
  position = pos;
}

void Light::updateShader(int screenW, int screenH, Vector2& cameraPos, Vector2& cameraOff) {
  // update render textures
  if (lightBuffer.texture.width != screenW || lightBuffer.texture.height != screenH) {
    UnloadRenderTexture(lightBuffer);
    lightBuffer = LoadRenderTexture(screenW, screenH);
    SetTextureWrap(lightBuffer.texture, TEXTURE_WRAP_CLAMP);
  }
  Vector2 camPos = Vector2Subtract(cameraPos, cameraOff);
  float pos[2] = { position.x - camPos.x, (float)screenH - position.y + camPos.y };
  float clr[3] = { (float)color.r / 255, (float)color.g / 255, (float)color.b / 255 };
  SetShaderValue(shader, shaderLoc[0], pos, SHADER_UNIFORM_VEC2);
  SetShaderValue(shader, shaderLoc[1], &intensity, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, shaderLoc[2], &radius, SHADER_UNIFORM_FLOAT);
  SetShaderValue(shader, shaderLoc[3], clr, SHADER_UNIFORM_VEC3);
}

void Light::render() {
  DrawCircle(position.x, position.y, 4.0f, color);
  DrawCircleLines(position.x, position.y, radius, (Color){color.r, color.g, color.b, 50});
}

void Light::cleanup() {
  UnloadShader(shader);
  UnloadRenderTexture(lightBuffer);
}
