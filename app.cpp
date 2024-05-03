#include <iostream>
#include <string>
#include <raylib.h>
#include <raymath.h>
#include "app.hpp"

using namespace App;

void EventLoop::init() {
  // initialize assets
  camera = { 0 };
  camera.zoom = 1.0f;
  camera.target = { 0.0f, 0.0f };

  // initialize buffers
  colorBuffer = LoadRenderTexture(screenW, screenH);
  normalBuffer = LoadRenderTexture(screenW, screenH);

  // initialize boxes
  std::vector<Vector2> boxPositions = {
    (Vector2){-200.0f, 100.0f}, (Vector2){-150.0f, 100.0f}, (Vector2){-100.0f, 100.0f}, (Vector2){-50.0f, 100.0f},
    (Vector2){0.0f, 100.0f}, (Vector2){50.0f, 100.0f}, (Vector2){100.0f, 100.0f}, (Vector2){150.0f, 100.0f},
    (Vector2){200.0f, 100.0f}
  };
  for (int i=0; i<boxPositions.size(); i++) {
    Color clr = GREEN;
    if (i%3 == 0) clr = BLUE;
    else if (i%2 == 0) clr = GRAY;
    Box b = {i, boxPositions[i], (Vector2){50.0f, 50.0f}, clr };
    boxes.push_back(b);
  }

  // initialize lights
  Light light1 = {1, (Vector2){ 0.0f, 200.0f }, 1200.0f, BLUE, 0.6};
  lights.push_back(light1);
  Light light2 = {2, (Vector2){ -400.0f, -200.0f }, 400.0f, YELLOW, 0.4};
  lights.push_back(light2);
  Light light3 = {3, (Vector2){ 400.0f, -200.0f }, 400.0f, RED, 0.8};
  lights.push_back(light3);
}

void EventLoop::update() {
  _updateSystem();
  // update camera
  if (IsKeyDown(KEY_A)) camera.target.x -= 4.0f;
  if (IsKeyDown(KEY_D)) camera.target.x += 4.0f;
  if (IsKeyDown(KEY_W)) camera.target.y -= 2.0f;
  if (IsKeyDown(KEY_S)) camera.target.y += 2.0f;

  // update assets
  for (Box& b: boxes) b.updateTexture();
  for (Light& l: lights) {
    if (l.id == 2) l.update((Vector2){ -400.0f, -100.0f + 100.0f * (float)std::sin(elapsed * 2)});
    else if (l.id == 3) l.update((Vector2){ 400.0f, -100.0f - 100.0f * (float)std::cos(elapsed * 2)});
    l.updateShader(screenW, screenH, camera.target, camera.offset);
  }

  // update render textures
  if (colorBuffer.texture.width != screenW || colorBuffer.texture.height != screenH) {
    UnloadRenderTexture(colorBuffer);
    colorBuffer = LoadRenderTexture(screenW, screenH);
    SetTextureWrap(colorBuffer.texture, TEXTURE_WRAP_CLAMP);
    UnloadRenderTexture(normalBuffer);
    normalBuffer = LoadRenderTexture(screenW, screenH);
    SetTextureWrap(normalBuffer.texture, TEXTURE_WRAP_CLAMP);
  }
}

void EventLoop::render() {
  Rectangle src = {0, 0, (float)screenW, -(float)screenH};
  Rectangle dest = {0, 0, (float)screenW, (float)screenH};
  _updateBuffers(src, dest);

  BeginDrawing();
    ClearBackground(BLACK);

    // draw color texture
    DrawTexturePro(colorBuffer.texture, src, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    // multiply by light
    BeginBlendMode(BLEND_MULTIPLIED);
      DrawTexturePro(normalBuffer.texture, src, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    EndBlendMode();

    // draw reticle
    // DrawCircle(screenCenter.x, screenCenter.y, 1.0f, WHITE);
    // DrawCircleLines(screenCenter.x, screenCenter.y, 5.0f, WHITE);

    // draw FPS overlay
    _drawFps();
  EndDrawing();
}

void EventLoop::cleanup() {
  // destroy instantiated resources
  for (Box& b: boxes) b.cleanup();
  for (Light& l: lights) l.cleanup();
  // destroy global resources
  UnloadRenderTexture(colorBuffer);
  UnloadRenderTexture(normalBuffer);
}

void EventLoop::_updateSystem() {
  fps = GetFPS();
  screenW = GetScreenWidth();
  screenH = GetScreenHeight();
  elapsed = GetTime();
  screenCenter = { (float)screenW/2, (float)screenH/2 };
  camera.offset = screenCenter;
}

void EventLoop::_drawFps() {
  std::string fpst = std::to_string(fps);
  std::string fpstxt = "FPS: ";
  fpstxt.append(fpst);
  DrawText(fpstxt.c_str(), 10.0, 10.0, 16, GREEN);
}

void EventLoop::_updateBuffers(Rectangle& src, Rectangle& dest) {
  // generate color buffer
  BeginTextureMode(colorBuffer);
    ClearBackground(BLACK);
    // draw to camera
    BeginMode2D(camera);
      DrawRectangle(-800, -500, 1600, 1000, WHITE);
      for (Box& b: boxes) b.render();
      // for (Light& l: lights) l.render();
    EndMode2D();
  EndTextureMode();

  // generate normal buffer
  BeginTextureMode(normalBuffer);
    ClearBackground(BLACK);
    // draw to camera
    BeginMode2D(camera);
      for (Box& b: boxes) b.renderNormal();
    EndMode2D();
  EndTextureMode();

  // generate light buffers
  for (Light& l: lights) {
    BeginTextureMode(l.lightBuffer);
      BeginShaderMode(l.shader);
        DrawTexturePro(normalBuffer.texture, src, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
      EndShaderMode();
    EndTextureMode();
  }

  // clear normal buffer
  BeginTextureMode(normalBuffer);
    ClearBackground(BLACK);
  EndTextureMode();

  // add all light buffers together into normal buffer
  for (Light& l: lights) {
    BeginTextureMode(normalBuffer);
      DrawTexturePro(normalBuffer.texture, src, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
      BeginBlendMode(BLEND_ADDITIVE);
        DrawTexturePro(l.lightBuffer.texture, src, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
      EndBlendMode();
    EndTextureMode();
  }
}