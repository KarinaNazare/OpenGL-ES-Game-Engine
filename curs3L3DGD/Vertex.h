#pragma once

#include "Math.h"

struct Vertex
{
	Vector3 pos;
	Vector3 norm;
	Vector3 color;
	Vector2 uv;
	Vector2 uvBlend; //offfset-ul 3*size(Vector3)+size(Vector2)
	Vector2 posL; //CUM IAU ASTA
	Vertex();
	Vertex(Vector3 &pos, Vector3 &color, Vector2 &uv) : pos(pos), color(color), uv(uv) {}
	Vertex(Vector3 &pos, Vector3 &color, Vector2 &uv, Vector2 &uvBlend) : pos(pos), color(color), uv(uv), uvBlend(uvBlend) {}
	Vertex(Vector3 &pos, Vector3 &color) : Vertex(pos, color, Vector2(0.0f, 0.0f)) {}
	Vertex(const Vertex &vertex) : pos(vertex.pos), color(vertex.color), uv(vertex.uv) , uvBlend(vertex.uvBlend), norm(vertex.norm){}
};
