precision mediump float;

varying vec3 v_color;
varying vec3 v_coord;


uniform samplerCube u_texture0;

void main()
{

	vec4 color = textureCube(u_texture0, v_coord);
	if (color.a < 0.0001)
		discard;
		//gl_FragColor = vec4(v_coord,1.0);
	gl_FragColor = color;
}
