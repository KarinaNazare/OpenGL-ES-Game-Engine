attribute vec3 a_pos;
attribute vec3 a_color;
attribute vec2 a_uv;
attribute vec2 a_uvBlend;

varying vec3 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;

uniform mat4 u_mvp;
uniform sampler2D u_texture0;


void main()
{
	vec3 pos = a_pos;
	v_color = a_color;
	v_uv = a_uv;
	v_uvBlend = a_uvBlend;

	vec4 c_blend = texture2D(u_texture0, v_uvBlend);
	pos.y += c_blend.r*2.3+c_blend.g*3.3+c_blend.b*10.7;

	gl_Position = u_mvp * vec4(pos, 1.0);
}
