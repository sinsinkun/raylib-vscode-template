#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in vec2 fragPos;

out vec4 finalColor;

void main() {
  float r = 0.5 * step(1.0-fragPos.x, fragPos.y) + 0.5 * step(fragPos.y, fragPos.x);
  float g = 0.5 * step(fragPos.y, 1.0-fragPos.x) + 0.5 * step(fragPos.y, fragPos.x);
  float b = 0.5;
  
  vec3 color = vec3(r,g,b);
  if (fragPos.x > 0.2 && fragPos.x < 0.8 && fragPos.y > 0.2 && fragPos.y < 0.8) {
    color = vec3(0.5,0.5,1.0);
  }

  finalColor = vec4(color, 1.0);
}