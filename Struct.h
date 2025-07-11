﻿#pragma once
#define _USE_MATH_DEFINES
#include<cmath>

#include"assert.h"
#include <algorithm>
// 4x4の行列
struct Matrix4x4 {
	float m[4][4];
};

// 2x2の行列
struct Matrix2x2 {
	float m[2][2];
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

	// 算術演算子（ベクトル × スカラー）
	Vector3 operator+(const Vector3 &v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vector3 operator-(const Vector3 &v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
	Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }

	// 複合代入
	Vector3 &operator+=(const Vector3 &v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3 &operator-=(const Vector3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3 &operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3 &operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};

// スカラー × ベクトル
inline Vector3 operator*(float s, const Vector3 &v) {
	return { v.x * s, v.y * s, v.z * s };
}

struct Vector2 {
	float x;
	float y;
};

struct Sphere {
	Vector3 center;
	float radius;
	unsigned int color;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
	unsigned int color;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertices[3];// 頂点
};

struct AABB {
	Vector3 min;//!<最小点
	Vector3 max;//!<最大点
	unsigned int color;//<!色
};

struct Spring {
	Vector3 anchor;// 固定された端の位置
	float naturalLength;// 自然長
	float stiffness;// ばね定数k
	float dampingCoefficient;// 減衰係数
};

struct Ball {
	Vector3 position;// 位置
	Vector3 velocity;// 速度
	Vector3 acceleration;// 加速度
	float mass;// 質量
	float radius;// 半径
	unsigned int color;// 色
};

struct Pendulum {
	Vector3 anchor;// アンカーポイント。固定された端の位置
	float length;// 紐の長さ
	float angle;// 現在の角度
	float angularVelocity;// 角速度ω
	float angularAcceleration;// 角加速度
};

struct ConicalPendulum {
	Vector3 anchor;// アンカーポイント。固定された端の位置
	float length;// 紐の長さ
	float halfApexAngle;// 円錐の頂角の半分
	float angle;// 現在の角度
	float angularVelocity;// 角速度ω
};

static const int kRowHeight = 20;
static const int kColumnWidth = 70;
static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;