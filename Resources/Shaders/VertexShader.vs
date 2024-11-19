attribute vec3 a_pos;
attribute vec3 a_color;
attribute vec2 a_uv;

varying vec3 v_color;
varying vec2 v_uv;
varying vec3 v_posW;

uniform mat4 u_mvp;


void main()
{
	v_color = a_color;
	v_uv = a_uv;
	v_posW = a_posL * u_world;

	gl_Position = u_mvp * vec4(a_pos, 1.0);
}
 