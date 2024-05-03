#include <vector>
#include <raylib.h>
#include "box.hpp"

namespace App {
  class EventLoop {
    public:
      // global states
      Camera2D camera;
      int screenW = 800;
      int screenH = 600;
      Vector2 screenCenter = { 0.0, 0.0 };
      int fps = 0;
      double elapsed = 0.0;
      // assets
      std::vector<Box> boxes;
      RenderTexture2D colorBuffer;
      RenderTexture2D normalBuffer;
      // methods
      void init();
      void update();
      void render();
      void cleanup();
    private:
      void _updateSystem();
      void _drawFps();
  };
}