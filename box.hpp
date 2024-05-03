#pragma once
#include <raylib.h>

namespace App {
  class Box {
    public:
      Box(int iid, Vector2 ipos, Vector2 isize, Color icolor);
      Box(int iid, Vector2 ipos, Vector2 isize, Color icolor, float irot);
      int id;
      Vector2 position;
      Vector2 size;
      Rectangle body;
      float rotation = 0.0f;
      Color color;
      void update(Vector2 pos, float rot);
      void updateTexture();
      void render();
      void renderNormal();
      void cleanup();
    private:
      Shader shader;
      RenderTexture2D target;
  };
}