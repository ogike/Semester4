#version 130

// bemeneti változó - most a vertex shader-ből (vagyis ottani out)
in vec4 vs_out_col;
in vec4 vs_out_pos;

// kimeneti változó - a fragment színe
out vec4 fs_out_col;

//uniform változó: shader számára konstans
	//értékét kívülről (CPU oldalról) kell megadni
uniform float time;

//komplex számokra négyzetre emelés
vec2 sq(vec2 a){
	return vec2(a.x*a.x - a.y*a.y, 2*a.x*a.x);
}

//csak simán tovább adjuk a vertex kimenetét
void main()
{
	//				  R, G, B, A
	fs_out_col = vs_out_col;

	//				  R, G, B, A
	//fs_out_col = vec4(1, 1, 1, 1);

	//shorthands:
		//vec2(0) = vec2(0,0)
		//vec2(vs_out_pos.x, vs_out_pos.y) = vs_out_pos.xy
		//vec2(vs_out_pos.y, vs_out_pos.x) = vs_out_pos.yx
		//vec2(vs_out_pos.x, vs_out_pos.y) = vs_out_pos.xy
		//vec3(vs_out_pos.x, vs_out_pos.y, vs_out_pos.z) = vs_out_pos.xyz
		//vs_out_pos.xyzw = vs_out_pos.rgba
	//if(distance(vec2(0), vs_out_pos.xy) < 1)//: körlap

	float epsilon = 0.03;
	float radius = abs(sin(time));
	if(abs(distance(vec2(0), vs_out_pos.xy) - radius) < epsilon)
		fs_out_col = vec4(1,0,0,1);
		//legyenek a körön belüli pontok pirosak

	//4. feladat: komplex számsík mandelbizbasz halmazzal
	fs_out_col = vec4(1,0,0,1); //eddigi feladat felülfestése

	//közelítés: 1/time-al szorozni
	vec2 c = vs_out_pos.xy - vec2(0.5,0); //egésznek (orignónak) eltolása jobbra
	vec2 x = vec2(0);
	for(int i = 0; i < 100; ++i){
		x = sq(x) + c;
		if(length(x) > 2){ //ilyenkor tudjuk hogy nem korlátos lesz a fgv értéke
			discard; //szóval el lehet dobni
		}
	}
	
}


// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

// 2. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet
	//discard: voidot jeleníti meg ott gyakorlatilag

// 3. feladat: uniform változók - animált kör sugár!

// 4. feladat: komplex számok....
