attribute vec2 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv;
attribute vec3 a_pos;

varying vec3 v_color;
varying vec2 v_uv;
varying vec3 v_coord;

uniform mat4 u_mvp;


void main()
{
	vec3 pos = a_pos;
	v_color = a_color;
	v_uv = a_uv;
	v_coord=a_pos;

	gl_Position = u_mvp * vec4(pos, 1.0);
}
