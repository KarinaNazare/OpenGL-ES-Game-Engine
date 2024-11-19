#pragma once
precision mediump float;

varying vec3 v_color;
varying vec2 v_uv;
//insemana ceva interpolat

uniform float u_time;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform float u_dispMax;
uniform sampler2D u_texture0;

void main()
{
	vec2 disp = texture2D(u_texture0,vec2(v_uv.x, v_uv.y + u_time)).rg;

	vec2 offset = 2.0 * u_dispMax * (disp - vec2(0.5,0.5));
	vec2 v_uv_displaced = v_uv + offset;
	
	vec4 c_fire = texture2D(u_texture1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture2, v_uv);
	c_fire.a = c_fire.a * c_alpha.r;
	

	if (c_fire.a < 0.0001)
		discard;

	gl_FragColor = c_fire;
	
}

