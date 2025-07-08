#pragma once
#include "Struct.h"
#include<cstdint>

/*行列の描画
*********************************************************/

void MatrixScreenPrintf(int x, int y, const Matrix4x4 &matrix, const char *name);

void VectorScreenPrintf(int x, int y, Vector3 vector, const char *name);


/*行列の計算
*********************************************************/

// ベクトルの内積
float Dot(const Vector3 &a, const Vector3 &b);

// クロス積
Vector3 Cross(const Vector3 &v1, const Vector3 &v2);

// ベクトルを正規化する関数
Vector3 Normalize(const Vector3 &vector);

/* 加法*/
Matrix4x4 Add(const Matrix4x4 &matrix1, const Matrix4x4 &matrix2);

Vector3 AddV(const Vector3 a, const Vector3 b);

/* 減法*/
Matrix4x4 Subtract(const Matrix4x4 &matrix1, const Matrix4x4 &matrix2);

Vector3 SubtractV(const Vector3 &a, const Vector3 &b);

/* 転置行列*/
Matrix4x4 Transpose(const Matrix4x4 &m);

/* 単位行列の作成*/
Matrix4x4 MakeIdentity4x4();

// 1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

/*3次元回転行列
*********************************************************/

// X軸の回転行列
Matrix4x4 MakeRoteXMatrix(float radian);

// Y軸の回転行列
Matrix4x4 MakeRoteYMatrix(float radian);

// Z軸の回転行列
Matrix4x4 MakeRoteZMatrix(float radian);

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2);

Vector3 MultiplyV(float scalar, Vector3 vector);

/*行列の計算
*********************************************************/

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 &translate);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3 &scale);

// 座標変換
Vector3 Transform(const Vector3 &vector, const Matrix4x4 &matrix);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3 &scale, const Vector3 &rotate, const Vector3 &translate);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4 &m);

// 平面を法線と点から作成
Plane MakePlaneFromPointAndNormal(const Vector3 &point, const Vector3 &normal);

/*イージング
*********************************************************/

// 線形補間
Vector3 Leap(const Vector3 &start, const Vector3 &end, float t);

/*ばねの動き
*********************************************************/

void ApplySpringForce(Spring &spring, Ball &ball, float deltaTime);

/*円運動の計算
*********************************************************/

void CalculateCircularPosition(Vector3 &p,const Vector3 c,const float radius,float &angle,const float anglarVelocity,const float deltaTime);

/*振り子の角度の計算
*********************************************************/

void CalculatePendulumAngle(Pendulum &pendulum, const float deltaTime);

// 円錐振り子
void CalculateConicalPendulumAngle(ConicalPendulum &conicalPendulum, float deltaTime);

/*描画
*********************************************************/

// 球
void DrawSphere(const Sphere &sphere, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

// グリッド
void DrawGrid(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix);

// 平面の描画
void DrawPlane(const Plane &plane, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

// 三角形の描画
void DrawTriangle(const Triangle &triangle, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

// AABBの描画
void DrawAABB(const AABB &aabb, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

// ジベェ曲線の描画
void DrawBezier(const Vector3 &controlPoint0, const Vector3 &controlPoint1, const Vector3 &controlPoint2, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

/*当たり判定
*********************************************************/

// 長さ
float Length(const Vector3 &vector);

// 球と球
bool IsCollideSphere(const Sphere &s1, const Sphere &s2);

// 球と平面
bool IsCollideSpherePlane(const Sphere &sphere, const Plane &plane);

// 球と線分
bool IsCollideSegmentPlane(const Segment &segment, const Plane &plane);

// 線分と平面
bool IsCollisionTriangleSegment(const Triangle triangle, const Segment &segment);

// AABBとAABB
bool IsColliderAABB(const AABB &a, const AABB &b);

// AABBと球
bool IsColliderAABBSphere(const AABB &aabb, const Sphere &sphere);

// AABBと線分
bool IsColliderAABBSegment(const AABB &aabb, const Segment &segment);

// 平面の頂点を求める
Vector3 Perpendicular(const Vector3 &vector);

// 正射影ベクトル
Vector3 Project(const Vector3 &v1, const Vector3 &v2);

// 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Segment &segment);

/*二項演算子
*********************************************************/

Vector3 operator+(const Vector3 &v1, const Vector3 &v2);

Vector3 operator-(const Vector3 &v1, const Vector3 &v2);

Vector3 operator*(const float &s1, const Vector3 &v1);

Vector3 operator*(const Vector3 &v1, const float &s1);

Vector3 operator/(const Vector3 &v1, const float &s1);

Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2);

Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2);

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);
