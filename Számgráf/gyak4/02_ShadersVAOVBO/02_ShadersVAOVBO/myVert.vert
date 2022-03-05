#version 130

// VBO-ból érkező változók
in vec3 vs_in_pos;
in vec3 vs_in_col;

uniform float time;
uniform int circle;


// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_col;

void main()
{
	gl_Position = vec4( vs_in_pos, 1 );

	//if(distance(gl_Position, vec4(0,0,0,1)) < 1)		 
	if(circle == 1)
		gl_Position += vec4(sin(time) * 0.5, 0, 0, 0);
	vs_out_pos = vs_in_pos;
	vs_out_col = vs_in_col;
}