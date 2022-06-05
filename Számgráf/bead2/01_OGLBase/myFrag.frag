#version 330 core

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D texImage;
uniform sampler2D grassTexImage;
uniform sampler2D mudTexImage;
uniform sampler2D rockTexImage;

int eps = 2;


void main()
{
	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;

	vec4 col;
	vec4 grass = texture(grassTexImage, vs_out_tex);
	vec4 mud = texture(mudTexImage, vs_out_tex);
	vec4 rock = texture(rockTexImage, vs_out_tex);
	
	float deg = degrees(acos(normal.y));

	if(deg <= 90){
		col = mix(grass, mud, deg/90.f);
	} else{
		col = mix(mud, rock, (deg-90.f)/90.f);
	}

	fs_out_col = vec4(ambient + diffuse, 1) * col;
}