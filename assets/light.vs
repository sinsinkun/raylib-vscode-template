#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 mvp;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec2 fragPos;

void main() {
  fragTexCoord = texCoord;
  fragColor = color;
  fragNormal = normal;
  vec4 clipPos = mvp * vec4(position, 1.0);
  fragPos = vec2(0.5 * (clipPos.xy + 1.0));

  gl_Position = mvp * vec4(position, 1.0);
}