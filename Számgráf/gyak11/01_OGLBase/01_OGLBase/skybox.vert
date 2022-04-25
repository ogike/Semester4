#version 330 core

// VBO-ból érkező változók
in vec3 vs_in_pos;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;

// shader külső paraméterei - most a három transzformációs mátrixot külön-külön vesszük át
uniform mat4 MVP;

void main()
{
	//z helyett is 4. koordinátát vesszsük itt
		//homogén osztást megcsinálja magának úgyis, hogy w = 1 legyen
		//így a z értéke is = 1 lesz
		//így a csúcsok sokkal inkább egy gömb szerű alakot vesznek fel, ha ugyanolyan távolságra vannak mindd
	gl_Position = (MVP * vec4( vs_in_pos, 1 )).xyww;	// [x,y,w,w] => homogén osztás után [x/w, y/w, 1]

	vs_out_pos = vs_in_pos;
}