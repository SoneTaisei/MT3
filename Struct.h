#pragma once
// 4x4の行列
struct Matrix4x4 {
	float m[4][4];
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

// vector3
struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector2 {
	float x;
	float y;
};

struct Sphere {
	Vector3 center;
	float radius;
};

static const int kRowHeight = 20;
static const int kColumnWidth = 70;
static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;