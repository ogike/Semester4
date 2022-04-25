#version 330 core

out vec3 vs_out_col;

vec3 pos[6] = vec3[6](
  vec3(0,0,0),
  vec3(1,0,0),
  vec3(0,0,0),
  vec3(0,1,0),
  vec3(0,0,0),
  vec3(0,0,1)
);

vec3 col[3] = vec3[3] (
  vec3(1,0,0),
  vec3(0,1,0),
  vec3(0,0,1)
);

uniform mat4 MVP;

void main()
{
  gl_Position = MVP *  vec4( pos[gl_VertexID], 1 );
  vs_out_col = col[ gl_VertecID/2 ]; //egész osztás
}