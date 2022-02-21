#version 130

// lokális változók: két tömb
vec4 positions[6] = vec4[6](
	//2 x 2 négyzetnek a csúcspontjai
	vec4(-1, -1, 0, 1), //4. koordináta: homogén koordináta, egyenlőre jó 1
	vec4( 1, -1, 0, 1),
	vec4(-1,  1, 0, 1),

	vec4(-1,  1, 0, 1),
	vec4( 1, -1, 0, 1),
	vec4( 1,  1, 0, 1)
);

vec4 colors[6] = vec4[6](
	vec4(1, 0, 0, 1), //piros
	vec4(0, 1, 0, 1), //zöld
	vec4(0, 0, 1, 1), //kék

	vec4(0, 0, 1, 1),
	vec4(0, 1, 0, 1),
	vec4(1, 1, 1, 1) //fehér
);

// kimeneti változó: a csúcspont színe
out vec4 vs_out_col;
//feladat 2:
out vec4 vs_out_pos;

void main()
{
	// gl_VertexID: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_VertexID.xhtml
		//hanyadik vertexed dolgozzuk fel
		//tehát itt most minden egyes vertexID-ra lefut a program (valószínüleg párhuzamosan)

	//gl_position: vertexnek a poziciója (0 és 1 közé interpolálva)
	gl_Position = positions[gl_VertexID];
	//vertex sharedből a kimeneti változó
	//grafikus szerelőszalagon tovább adódik, fragment shader felé
	vs_out_col	= colors[gl_VertexID];

	//feladat2:
	vs_out_pos = gl_Position; //tvább adjuk a vert pozicióját
}