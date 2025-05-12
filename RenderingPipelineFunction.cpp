#include"RenderingPipelineFunction.h"
#include"Novice.h"

#define _USE_MATH_DEFINES
#include<cmath>

#include"assert.h"

/*行列の描画
*********************************************************/

void MatrixScreenPrintf(int x, int y, const Matrix4x4 &matrix, const char *name) {
	// 作ったものの名前
	Novice::ScreenPrintf(x, y, "%s", name);
	// 表示したい数字
	for(int row = 0; row < 4; ++row) {
		for(int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth,
				y + row * kRowHeight + kRowHeight,
				"%7.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, Vector3 vector, const char *name) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", name);
}

/*行列の計算
*********************************************************/

Vector3 Cross(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// 1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result = {
		1.0f / aspectRatio * (1.0f / std::tan(fovY / 2.0f)),0.0f,0.0f,0.0f,
		0.0f,(1 / std::tan(fovY / 2.0f)),0.0f,0.0f,
		0.0f,0.0f,farClip / (farClip - nearClip),1.0f,
		0.0f,0.0f,(-nearClip * farClip) / (farClip - nearClip),0.0f
	};

	return result;
}

// 2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};

	result = {
		2.0f / (right - left),0.0f,0.0f,0.0f,
		0.0f,2.0f / (top - bottom),0.0f,0.0f,
		0.0f,0.0f,1.0f / (farClip - nearClip),0.0f,
		(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1.0f
	};

	return result;
}

// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {};

	result = {
		width / 2.0f,0.0f,0.0f,0.0f,
		0.0f,-height / 2.0f,0.0f,0.0f,
		0.0f,0.0f,maxDepth - minDepth,0.0f,
		left + width / 2.0f,top + height / 2.0f,minDepth,1.0f
	};

	return result;
}

Matrix4x4 MakeRoteXMatrix(float radian) {
	Matrix4x4 result{};

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,std::cos(radian),std::sin(radian),0.0f,
		0.0f,-std::sin(radian),std::cos(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4x4 MakeRoteYMatrix(float radian) {
	Matrix4x4 result{};

	result = {
		std::cos(radian),0.0f,-std::sin(radian),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		std::sin(radian),0.0f,std::cos(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4x4 MakeRoteZMatrix(float radian) {
	Matrix4x4 result{};

	result = {
		std::cos(radian),std::sin(radian),0.0f,0.0f,
		-std::sin(radian),std::cos(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

/*行列の計算
*********************************************************/

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 &translate) {
	Matrix4x4 result = {};

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate.x,translate.y,translate.z,1.0f
	};

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3 &scale) {
	Matrix4x4 result = {};

	result = {
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

Matrix4x4 Multiply(Matrix4x4 matrix1, Matrix4x4 matrix2) {
	Matrix4x4 result = {};

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			for(int k = 0; k < 4; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}

	return result;
}

// 座標変換
Vector3 Transform(const Vector3 &vector, const Matrix4x4 &matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3 &scale, const Vector3 &rotate, const Vector3 &translate) {
	Matrix4x4 result{};

	// 回転行列
	Matrix4x4 rotZ = MakeRoteZMatrix(rotate.z);
	Matrix4x4 rotX = MakeRoteXMatrix(rotate.x);
	Matrix4x4 rotY = MakeRoteYMatrix(rotate.y);

	// 回転行列の合成
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotX, rotY), rotZ);

	// 拡大縮小
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// 平行移動
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return result;

}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4 &matrix) {
	float determinant =
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
		matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
		matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
		matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
		matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
		matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
		matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
		matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] -
		matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
		matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
		matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
		matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
		matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
		matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	float determinantRecp = 1.0f / determinant;
	Matrix4x4 result;
	result.m[0][0] =
		determinantRecp * (
			matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]);
	result.m[0][1] =
		determinantRecp * (
			-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]);
	result.m[0][2] =
		determinantRecp * (
			matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]);
	result.m[0][3] =
		determinantRecp * (
			-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]);
	result.m[1][0] =
		determinantRecp * (
			-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
			matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]);
	result.m[1][1] =
		determinantRecp * (
			matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
			matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]);
	result.m[1][2] =
		determinantRecp * (
			-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]);
	result.m[1][3] =
		determinantRecp * (
			matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
			matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]);
	result.m[2][0] =
		determinantRecp * (
			matrix.m[1][0] * matrix.m[2][1] * matrix.m[2][3] + matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]);
	result.m[2][1] =
		determinantRecp * (
			-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]);
	result.m[2][2] =
		determinantRecp * (
			matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]);
	result.m[2][3] =
		determinantRecp * (
			-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
			matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]);
	result.m[3][0] =
		determinantRecp * (
			-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
			matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]);
	result.m[3][1] =
		determinantRecp * (
			matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
			matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]);
	result.m[3][2] =
		determinantRecp * (
			-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]);
	result.m[3][3] =
		determinantRecp * (
			matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
			matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]);

	return result;
}

/*描画
*********************************************************/

// 球
void DrawSphere(const Sphere &sphere, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 12;
	const float kLonEvery = 2.0f * float(M_PI) / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;
	// 緯度の方向に分割 -π/2 ~ π/2
	for(uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;// 現在の緯度
		// 緯度の方向に分割 0 ~ 2π
		for(uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;// 現在の経度
			Vector3 a, b, c;
			a.x = sphere.center.x + std::cos(lat) * std::cos(lon) * sphere.radius;
			a.y = sphere.center.y + std::sin(lat) * sphere.radius;
			a.z = sphere.center.z + std::cos(lat) * std::sin(lon) * sphere.radius;

			b.x = sphere.center.x + std::cos(lat + kLatEvery) * std::cos(lon) * sphere.radius;
			b.y = sphere.center.y + std::sin(lat + kLatEvery) * sphere.radius;
			b.z = sphere.center.z + std::cos(lat + kLatEvery) * std::sin(lon) * sphere.radius;

			c.x = sphere.center.x + std::cos(lat) * std::cos(lon + kLonEvery) * sphere.radius;
			c.y = sphere.center.y + std::sin(lat) * sphere.radius;
			c.z = sphere.center.z + std::cos(lat) * std::sin(lon + kLonEvery) * sphere.radius;

			// 座標変換をする
			Vector3 localVertexA = { a.x,a.y,a.z };
			Vector3 localVertexB = { b.x,b.y,b.z };
			Vector3 localVertexC = { c.x,c.y,c.z };

			Vector3 clipPosA = Transform(localVertexA, viewProjectionMatrix);
			Vector3 clipPosB = Transform(localVertexB, viewProjectionMatrix);
			Vector3 clipPosC = Transform(localVertexC, viewProjectionMatrix);

			Vector2 ndcPosA = {
				clipPosA.x / clipPosA.z,
				clipPosA.y / clipPosA.z
			};

			Vector2 ndcPosB = {
				clipPosB.x / clipPosB.z,
				clipPosB.y / clipPosB.z
			};

			Vector2 ndcPosC = {
				clipPosC.x / clipPosC.z,
				clipPosC.y / clipPosC.z
			};

			Vector3 screenPosA = Transform({ ndcPosA.x,ndcPosA.y,1.0f }, viewportMatrix);
			Vector3 screenPosB = Transform({ ndcPosB.x,ndcPosB.y,1.0f }, viewportMatrix);
			Vector3 screenPosC = Transform({ ndcPosC.x,ndcPosC.y,1.0f }, viewportMatrix);

			Novice::DrawLine(int(screenPosA.x), int(screenPosA.y), int(screenPosB.x), int(screenPosB.y), color);
			Novice::DrawLine(int(screenPosB.x), int(screenPosB.y), int(screenPosC.x), int(screenPosC.y), color);

		}
	}

}

// グリッド
void DrawGrid(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);
	// 奥から手前への線を順々に引いていく
	for(uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		Vector3 startPos = { float(xIndex),0.0f,-kGridEvery };
		Vector3 endPos = { float(xIndex),0.0f,kGridEvery };

		Vector3 clipPosA = Transform(startPos, viewProjectionMatrix);
		Vector3 clipPosB = Transform(endPos, viewProjectionMatrix);

		Vector2 ndcStartPos = {
			clipPosA.x,
			clipPosA.y
		};

		Vector2 ndcEndPos = {
			clipPosB.x,
			clipPosB.y
		};

		Vector3 screenStartPos = Transform({ ndcStartPos.x,ndcStartPos.y,1.0f }, viewportMatrix);
		Vector3 screenEndPos = Transform({ ndcEndPos.x,ndcEndPos.y,1.0f }, viewportMatrix);

		Novice::DrawLine(
			int(screenStartPos.x), int(screenStartPos.y),
			int(screenEndPos.x), int(screenEndPos.y),
			0xAAAAAAFF
		);
	}

	for(uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 startPos = { -kGridEvery,0.0f,float(zIndex) };
		Vector3 endPos = { kGridEvery,0.0f,float(zIndex) };

		Vector3 clipStartPos = Transform(startPos, viewProjectionMatrix);
		Vector3 clipEndPos = Transform(endPos, viewProjectionMatrix);

		Vector2 ndcStartPos = {
			clipStartPos.x,
			clipStartPos.y
		};

		Vector2 ndcEndPos = {
			clipEndPos.x,
			clipEndPos.y
		};

		Vector3 screenStartPos = Transform({ ndcStartPos.x,ndcStartPos.y,1.0f }, viewportMatrix);
		Vector3 screenEndPos = Transform({ ndcEndPos.x,ndcEndPos.y,1.0f }, viewportMatrix);

		Novice::DrawLine(
			int(screenStartPos.x), int(screenStartPos.y),
			int(screenEndPos.x), int(screenEndPos.y),
			0xAAAAAAFF
		);
	}

}