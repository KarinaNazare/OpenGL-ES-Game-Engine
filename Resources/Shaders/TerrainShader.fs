precision mediump float;

varying vec3 v_color;
varying vec2 v_uv;
varying vec2 v_uvBlend;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

void main()
{
	vec4 colorBlend = texture2D(u_texture0, v_uvBlend);
	vec4 dirt = texture2D(u_texture1, v_uv);
	vec4 rock = texture2D(u_texture2, v_uv);
	vec4 grass = texture2D(u_texture3, v_uv);
	vec4 color;


	color = colorBlend.r * grass + colorBlend.g *dirt +colorBlend.b * rock;
		

	if (color.a < 0.0001)
		discard;

	gl_FragColor = color;
}
