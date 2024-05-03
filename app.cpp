#include <iostream>
#include <string>
#include <raylib.h>
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
    Box b = {i, boxPositions[i], (Vector2){50.0f, 50.0f}, GREEN };
    boxes.push_back(b);
  }
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
}

void EventLoop::render() {
  BeginDrawing();
    ClearBackground(BLACK);
    // draw to camera
    BeginMode2D(camera);
      DrawRectangle(-800, -200, 1600, 400, BLUE);
      for (Box& b: boxes) b.renderNormal();
    EndMode2D();

    // draw reticle
    DrawCircle(screenCenter.x, screenCenter.y, 1.0f, WHITE);
    DrawCircleLines(screenCenter.x, screenCenter.y, 5.0f, WHITE);

    // draw FPS overlay
    _drawFps();
  EndDrawing();
}

void EventLoop::cleanup() {
  // destroy instantiated resources
  for (Box& b: boxes) b.cleanup();

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
