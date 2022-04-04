#version 140

// pipeline-ból bejövő per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenő érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya ("jobb fent")
uniform vec3 light_dir = vec3(-1,-1,-1);

//point light_dir
uniform vec3 light_pos = vec3(3,0,0);

// fénytulajdonságok: ambiens, diffúz, spekuláris
	//"a fény milyen színű"
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.4, 0.6, 0.6);
uniform vec3 Ls = vec3(0.9, 0.9, 0.9);

// anyagtulajdonságok: ambiens, diffúz, spekuláris
	//erősség, "hány százalékát verjük vissza a fénynek"
//uniform vec3 Ka = vec3(0.2, 0.4, 0.6);
//uniform vec3 Kd = vec3(0.2, 0.4, 0.6);
//uniform vec3 Ks = vec3(0.4, 0.8, 1.0);
uniform vec3 Ka = vec3(1); //texrúrához ne függjön anyagtól
uniform vec3 Kd = vec3(1);
uniform vec3 Ks = vec3(1);

uniform vec3 eye; //eye pos

uniform sampler2D texImage;

void main()
{	
	//
	// ambiens szín számítása
	//
	vec3 ambient = La * Ka;

	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/
	vec3 n = normalize(vs_out_norm);
	vec3 toLight_dir = normalize(-light_dir);
	vec3 toLight_point = normalize(light_pos - vs_out_pos); //átírás point lighthoz (dir light helyett)
	float di_dir = clamp( dot(n, toLight_dir), 0, 1 ); //diffuse intensity
	float di_point = clamp( dot(n, toLight_point), 0, 1 ); //diffuse intensity
	vec3 diffuse = (di_dir + di_point) * Ld * Kd;

	//
	// fényfoltképző szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
				reflect(beérkező_vektor, normálvektor);
		- pow: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
				pow(alap, kitevő);
	*/

	vec3 ref_dir = reflect( normalize(light_dir), n );
	vec3 ref_point = reflect( -toLight_point, n );

	vec3 toEye = normalize(eye - vs_out_pos);
	//alapból ez a visszacsillanó folt nagyon eloszlana, de nekünk csak az kell, ahol tényleg a legerősebb
		//ezárt felerősítjük (pow), így ahol kevesebb mint 1, eltűnik, ahol 1 közeli, ott lesz csak visszacsillanás
	float si_dir = pow( clamp( dot(toEye, ref_dir), 0, 1), 256);
	float si_point = pow( clamp( dot(toEye, ref_point), 0, 1), 256);
	vec3 specular = (si_dir + si_point) * Ls * Ks;
	
	//
	// a fragment végső színének meghatározása
	//
	fs_out_col = vec4(ambient + diffuse + specular, 1);

	// felületi normális
	//fs_out_col = vec4(vs_out_norm, 1);



	// textúrával
	vec4 textureColor = texture(texImage, vs_out_tex);
	fs_out_col = vec4(ambient + diffuse + specular, 1) * textureColor;
}
