precision mediump float;

varying vec3 v_color;
varying vec2 v_uv;

uniform vec3 camPos;
uniform sampler2D u_texture0;


void main()
{
	vec3 d = v_posW - u_campPos; // nu!!!! de fapt, iti trebuie modulul segmentului dintre cele 2 puncte. La SOMN
	vec4 color = texture2D(u_texture0, v_uv);
	if (color.a < 0.0001)
		discard;

	gl_FragColor = color;
}
