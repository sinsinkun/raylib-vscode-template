#pragma once
#include <raylib.h>

namespace App {
  class Light {
    public:
      Light(int iid, Vector2 ipos, float iradius, Color icolor, float iintensity);
      int id;
      Vector2 position;
      Color color;
      float intensity = 1.0f;
      float radius;
      void update(Vector2 pos);
      void updateShader(int screenW, int screenH, Vector2& cameraPos, Vector2& cameraOff);
      void render();
      void cleanup();
      Shader shader;
      int shaderLoc[5];
      RenderTexture2D lightBuffer;
  };
}