#version 140

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

uniform sampler2D texImage;

uniform float time;

void main()
{
	fs_out_col = texture(texImage, vs_out_tex0);

	float r = 0.3;
	float len = length(vs_out_tex0 - vec2(0.5, 0.5));

	if(len < r){
	float a = len / r;
		fs_out_col = mix( vec4(1,0,0,1), fs_out_col, a);
	}

	
	//fs_out_col = texture(texImage, vs_out_tex0 * (1+sin(time)));
}