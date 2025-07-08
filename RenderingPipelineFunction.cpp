#include"RenderingPipelineFunction.h"
#include"Novice.h"


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

// ベクトルの内積
float Dot(const Vector3 &a, const Vector3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// クロス積
Vector3 Cross(const Vector3 &v1, const Vector3 &v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

// ベクトルを正規化する関数
Vector3 Normalize(const Vector3 &vector) {
	float length = Length(vector);
	// 長さが0の場合はゼロベクトルを返す（エラー防止）
	if(length == 0.0f) {
		return { 0.0f, 0.0f, 0.0f };
	}
	return {
		vector.x / length,
		vector.y / length,
		vector.z / length
	};
}

/* 加法*/
Matrix4x4 Add(const Matrix4x4 &matrix1, const Matrix4x4 &matrix2) {
	Matrix4x4 result;

	result.m[0][0] = matrix1.m[0][0] + matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] + matrix2.m[0][1];
	result.m[0][2] = matrix1.m[0][2] + matrix2.m[0][2];
	result.m[0][3] = matrix1.m[0][3] + matrix2.m[0][3];
	result.m[1][0] = matrix1.m[1][0] + matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] + matrix2.m[1][1];
	result.m[1][2] = matrix1.m[1][2] + matrix2.m[1][2];
	result.m[1][3] = matrix1.m[1][3] + matrix2.m[1][3];
	result.m[2][0] = matrix1.m[2][0] + matrix2.m[2][0];
	result.m[2][1] = matrix1.m[2][1] + matrix2.m[2][1];
	result.m[2][2] = matrix1.m[2][2] + matrix2.m[2][2];
	result.m[2][3] = matrix1.m[2][3] + matrix2.m[2][3];
	result.m[3][0] = matrix1.m[3][0] + matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][1] + matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][2] + matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][3] + matrix2.m[3][3];

	return result;
}

Vector3 AddV(const Vector3 a, const Vector3 b) {
	Vector3 result = {};

	result = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	};

	return result;
}

/* 減法*/
Matrix4x4 Subtract(const Matrix4x4 &matrix1, const Matrix4x4 &matrix2) {
	Matrix4x4 result;

	result.m[0][0] = matrix1.m[0][0] - matrix2.m[0][0];
	result.m[0][1] = matrix1.m[0][1] - matrix2.m[0][1];
	result.m[0][2] = matrix1.m[0][2] - matrix2.m[0][2];
	result.m[0][3] = matrix1.m[0][3] - matrix2.m[0][3];
	result.m[1][0] = matrix1.m[1][0] - matrix2.m[1][0];
	result.m[1][1] = matrix1.m[1][1] - matrix2.m[1][1];
	result.m[1][2] = matrix1.m[1][2] - matrix2.m[1][2];
	result.m[1][3] = matrix1.m[1][3] - matrix2.m[1][3];
	result.m[2][0] = matrix1.m[2][0] - matrix2.m[2][0];
	result.m[2][1] = matrix1.m[2][1] - matrix2.m[2][1];
	result.m[2][2] = matrix1.m[2][2] - matrix2.m[2][2];
	result.m[2][3] = matrix1.m[2][3] - matrix2.m[2][3];
	result.m[3][0] = matrix1.m[3][0] - matrix2.m[3][0];
	result.m[3][1] = matrix1.m[3][1] - matrix2.m[3][1];
	result.m[3][2] = matrix1.m[3][2] - matrix2.m[3][2];
	result.m[3][3] = matrix1.m[3][3] - matrix2.m[3][3];

	return result;
}

Vector3 SubtractV(const Vector3 &a, const Vector3 &b) {
	return {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	};
}

/* 転置行列*/
Matrix4x4 Transpose(const Matrix4x4 &matrix) {
	Matrix4x4 result;

	result.m[0][0] = matrix.m[0][0];
	result.m[0][1] = matrix.m[1][0];
	result.m[0][2] = matrix.m[2][0];
	result.m[0][3] = matrix.m[3][0];
	result.m[1][0] = matrix.m[0][1];
	result.m[1][1] = matrix.m[1][1];
	result.m[1][2] = matrix.m[2][1];
	result.m[1][3] = matrix.m[3][1];
	result.m[2][0] = matrix.m[0][2];
	result.m[2][1] = matrix.m[1][2];
	result.m[2][2] = matrix.m[2][2];
	result.m[2][3] = matrix.m[3][2];
	result.m[3][0] = matrix.m[0][3];
	result.m[3][1] = matrix.m[1][3];
	result.m[3][2] = matrix.m[2][3];
	result.m[3][3] = matrix.m[3][3];

	return result;
}

/* 単位行列の作成*/
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	return result;
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

Vector3 MultiplyV(float scalar, Vector3 vector) {
	Vector3 result = {};

	result = {
		vector.x * scalar,
		vector.y * scalar,
		vector.z * scalar
	};

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


			Vector3 clipPosA = Transform(a, viewProjectionMatrix);
			Vector3 clipPosB = Transform(b, viewProjectionMatrix);
			Vector3 clipPosC = Transform(c, viewProjectionMatrix);

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
		float lineX = kGridEvery * xIndex - kGridHalfWidth;
		Vector3 startPos = { lineX,0.0f,-kGridHalfWidth };
		Vector3 endPos = { lineX,0.0f,kGridHalfWidth };

		Vector3 clipStartPos = Transform(startPos, viewProjectionMatrix);
		Vector3 clipEndPosB = Transform(endPos, viewProjectionMatrix);

		Vector3 screenStartPos = Transform(clipStartPos, viewportMatrix);
		Vector3 screenEndPos = Transform(clipEndPosB, viewportMatrix);

		if(xIndex == 5) {
			Novice::DrawLine(
				int(screenStartPos.x), int(screenStartPos.y),
				int(screenEndPos.x), int(screenEndPos.y),
				BLACK
			);
		} else {
			Novice::DrawLine(
				int(screenStartPos.x), int(screenStartPos.y),
				int(screenEndPos.x), int(screenEndPos.y),
				0xAAAAAAFF
			);
		}
	}

	for(uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float lineZ = kGridEvery * zIndex - kGridHalfWidth;
		Vector3 startPos = { -kGridHalfWidth,0.0f,lineZ };
		Vector3 endPos = { kGridHalfWidth,0.0f,lineZ };

		Vector3 clipStartPos = Transform(startPos, viewProjectionMatrix);
		Vector3 clipEndPos = Transform(endPos, viewProjectionMatrix);

		Vector3 screenStartPos = Transform(clipStartPos, viewportMatrix);
		Vector3 screenEndPos = Transform(clipEndPos, viewportMatrix);

		if(zIndex == 5) {
			Novice::DrawLine(
				int(screenStartPos.x), int(screenStartPos.y),
				int(screenEndPos.x), int(screenEndPos.y),
				BLACK
			);
		} else {
			Novice::DrawLine(
				int(screenStartPos.x), int(screenStartPos.y),
				int(screenEndPos.x), int(screenEndPos.y),
				0xAAAAAAFF
			);
		}
	}

}

// 平面の描画
void DrawPlane(const Plane &plane, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	// 正規化された法線（念のため）
	Vector3 normal = Normalize(plane.normal);

	// 平面上の中心点（-distance × normal）
	Vector3 center = MultiplyV(-plane.distance, normal);

	// 平面上にある2つの垂直方向ベクトルを作成
	Vector3 u = Normalize(Perpendicular(normal));
	Vector3 v = Cross(normal, u);

	// 平面の範囲（長さを調整できる）
	float scale = 2.0f;

	// 4頂点を作成（平面上の四角形）
	Vector3 points[4] = {
		AddV(center, AddV(MultiplyV(scale, u), MultiplyV(scale, v))), // +u +v
		AddV(center, AddV(MultiplyV(scale, u), MultiplyV(-scale, v))), // +u -v
		AddV(center, AddV(MultiplyV(-scale, u), MultiplyV(-scale, v))), // -u -v
		AddV(center, AddV(MultiplyV(-scale, u), MultiplyV(scale, v))), // -u +v
	};

	// 変換してスクリーン座標へ
	for(int i = 0; i < 4; ++i) {
		points[i] = Transform(Transform(points[i], viewProjectionMatrix), viewportMatrix);
	}

	// 線で囲む
	for(int i = 0; i < 4; ++i) {
		int j = (i + 1) % 4;
		Novice::DrawLine(int(points[i].x), int(points[i].y), int(points[j].x), int(points[j].y), color);
	}


}

// 三角形の描画
void DrawTriangle(const Triangle &triangle, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	Vector3 screenVertices[3];

	// 各頂点を ビュー射影 → ビューポート の順で変換
	for(int i = 0; i < 3; ++i) {
		Vector3 ndc = Transform(triangle.vertices[i], viewProjectionMatrix);   // 1. クリップ空間→NDC
		screenVertices[i] = Transform(ndc, viewportMatrix);             // 2. NDC→スクリーン
	}

	Novice::DrawTriangle(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		color, kFillModeWireFrame
	);


}

void DrawAABB(const AABB &aabb, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	Vector3 min = aabb.min;
	Vector3 max = aabb.max;
	Vector3 vertices[8] = {
		{min.x,min.y,min.z},
		{min.x,min.y,max.z},
		{min.x,max.y,max.z},
		{min.x,max.y,min.z},
		{max.x,min.y,min.z},
		{max.x,min.y,max.z},
		{max.x,max.y,max.z},
		{max.x,max.y,min.z},
	};

	// スクリーン座標に変換
	for(int i = 0; i < 8; ++i) {
		vertices[i] = Transform(vertices[i], viewProjectionMatrix);
		vertices[i] = Transform(vertices[i], viewportMatrix);
	}

	// エッジを描画
	for(int i = 0; i < 4; ++i) {
		int next = (i + 1) % 4;
		// 底面のエッジ
		Novice::DrawLine(
			static_cast<int>(vertices[i].x), static_cast<int>(vertices[i].y),
			static_cast<int>(vertices[next].x), static_cast<int>(vertices[next].y),
			color
		);
		// 上面のエッジ
		Novice::DrawLine(
			static_cast<int>(vertices[i + 4].x), static_cast<int>(vertices[i + 4].y),
			static_cast<int>(vertices[next + 4].x), static_cast<int>(vertices[next + 4].y),
			color
		);
		// 垂直エッジ
		Novice::DrawLine(
			static_cast<int>(vertices[i].x), static_cast<int>(vertices[i].y),
			static_cast<int>(vertices[i + 4].x), static_cast<int>(vertices[i + 4].y),
			color
		);

	};
}

void DrawBezier(const Vector3 &controlPoint0, const Vector3 &controlPoint1, const Vector3 &controlPoint2, const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color) {
	float t = 0;
	Vector3 prevPos = controlPoint0;
	for(uint32_t i = 0; i < 16; ++i) {
		t += 1.0f / 16.0f;
		// 制御点p0,p1を線形補間
		Vector3 p0p1 = Leap(controlPoint0, controlPoint1, t);
		// 制御点p1,p2を線形補間
		Vector3 p1p2 = Leap(controlPoint1, controlPoint2, t);
		// 補間店p0p1,p1p2をさらに線形補間
		Vector3 p = Leap(p0p1, p1p2, t);

		// ワールド→スクリーン座標に変換
		Vector3 screenPrev = Transform(Transform(prevPos, viewProjectionMatrix), viewportMatrix);
		Vector3 screenP = Transform(Transform(p, viewProjectionMatrix), viewportMatrix);

		// ライン描画
		Novice::DrawLine((int)screenPrev.x, (int)screenPrev.y,
			(int)screenP.x, (int)screenP.y, color);

		prevPos = p;
	}
}

// 平面を法線と点から作成
Plane MakePlaneFromPointAndNormal(const Vector3 &point, const Vector3 &normal) {
	Vector3 normalized = Normalize(normal);
	float distance = Dot(normalized, point);
	return { normalized, distance };
}

// 線形補間
Vector3 Leap(const Vector3 &start, const Vector3 &end, float t) {
	return {
	start.x + (end.x - start.x) * t,
	start.y + (end.y - start.y) * t,
	start.z + (end.z - start.z) * t
	};
}

void ApplySpringForce(Spring &spring, Ball &ball, float deltaTime) {
	Vector3 diff = ball.position - spring.anchor;
	float length = Length(diff);

	if(length != 0.0f) {
		Vector3 direction = Normalize(diff);

		// ばねの自然長に沿った理想的な位置
		Vector3 restPosition = spring.anchor + direction * spring.naturalLength;

		// 変位（今の位置と自然な位置との差）
		Vector3 displacement = length * (ball.position - restPosition);

		// フックの法則による復元力: F = -k * x
		Vector3 restoringForce = -spring.stiffness * displacement;

		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;

		// 減衰力: F = -d * v
		Vector3 force = restoringForce + dampingForce;


		ball.cceleration = force / ball.mass;

	}

	// 加速度を速度に、速度を位置に反映
	ball.velocity = ball.velocity + ball.cceleration * deltaTime;
	ball.position = ball.position + ball.velocity * deltaTime;


}

/*円運動の計算
*********************************************************/

void CalculateCircularPosition(Vector3 &p, const Vector3 c, const float radius, float &angle, const float anglarVelocity, const float deltaTime) {
	angle += anglarVelocity * deltaTime;

	p.x = c.x + std::cos(angle) * radius;
	p.y = c.y + std::sin(angle);
	p.z = c.z;

}

/*振り子の角度の計算
*********************************************************/

void CalculatePendulumAngle(Pendulum &pendulum, const float deltaTime) {
	pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sin(pendulum.angle);
	pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
	pendulum.angle += pendulum.angularVelocity * deltaTime;

}

// 円錐振り子
void CalculateConicalPendulumAngle(ConicalPendulum &conicalPendulum, float deltaTime) {
	conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
	conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;

}

/*当たり判定
*********************************************************/

// 長さ
float Length(const Vector3 &vector) {
	return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

// 球と球
bool IsCollideSphere(const Sphere &s1, const Sphere &s2) {
	// 2つの球の中心の距離を求める
	float distance = Length(s2.center - s1.center);

	// 半径の合計よりも短ければ衝突
	if(distance < s1.radius + s2.radius) {
		return true;
	}
	return false;
}

// 球と平面
bool IsCollideSpherePlane(const Sphere &sphere, const Plane &plane) {
	Vector3 normalizedNormal = Normalize(plane.normal); // 法線を正規化
	float distance = std::abs(Dot(normalizedNormal, sphere.center) + plane.distance);

	return distance <= sphere.radius;
}

// 線分と平面
bool IsCollideSegmentPlane(const Segment &segment, const Plane &plane) {
	Vector3 A = segment.origin;
	Vector3 B = AddV(segment.origin, segment.diff);
	Vector3 dir = { B.x - A.x, B.y - A.y, B.z - A.z };

	float denom = Dot(plane.normal, dir);
	if(fabs(denom) < 1e-6f) {
		// 平行 or nearly parallel
		return false;
	}
	// solve n·(A + t dir) + D = 0  =>  t = -(n·A + D) / (n·dir)
	float t = -(Dot(plane.normal, A) + plane.distance) / denom;
	return (t >= 0.0f && t <= 1.0f);
}

// 三角形と線分
bool IsCollisionTriangleSegment(const Triangle triangle, const Segment &segment) {
	const float EPSILON = 1e-5f;

	Vector3 edge1 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 edge2 = triangle.vertices[2] - triangle.vertices[0];
	Vector3 dir = segment.diff;
	Vector3 h = Cross(dir, edge2);
	float a = Dot(edge1, h);

	if(std::abs(a) < EPSILON) {
		return false;  // 平行で交差しない
	}

	float f = 1.0f / a;
	Vector3 s = segment.origin - triangle.vertices[0];
	float u = f * Dot(s, h);
	if(u < 0.0f || u > 1.0f) {
		return false;
	}

	Vector3 q = Cross(s, edge1);
	float v = f * Dot(dir, q);
	if(v < 0.0f || u + v > 1.0f) {
		return false;
	}

	float t = f * Dot(edge2, q);
	if(t < 0.0f || t > 1.0f) {
		return false;  // 線分の範囲外
	}

	return true;
}

bool IsColliderAABB(const AABB &a, const AABB &b) {
	if((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z)) {
		return true;
	}

	return false;
}

bool IsColliderAABBSphere(const AABB &aabb, const Sphere &sphere) {
	// 最近接点を求める
	Vector3 closetPoint = {
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};

	// 最近接点と球の中心との距離を求める
	float distance = Length(closetPoint - sphere.center);
	// 距離が半径うよりも小さければ衝突
	if(distance < sphere.radius) {
		return true;
	}
	return false;
}

bool IsColliderAABBSegment(const AABB &aabb, const Segment &segment) {
	Vector3 d = segment.diff;  // p1 - p0
	Vector3 p0 = segment.origin;

	float tmin = 0.0f;
	float tmax = 1.0f;

	for(int i = 0; i < 3; ++i) {
		float start = (&p0.x)[i];
		float dir = (&d.x)[i];
		float minB = (&aabb.min.x)[i];
		float maxB = (&aabb.max.x)[i];

		if(fabs(dir) < 1e-6f) {
			// 線分がこの軸方向に平行な場合
			if(start < minB || start > maxB) {
				return false;
			}
		} else {
			float t1 = (minB - start) / dir;
			float t2 = (maxB - start) / dir;
			if(t1 > t2) std::swap(t1, t2);
			tmin = max(tmin, t1);
			tmax = min(tmax, t2);
			if(tmin > tmax) return false;
		}
	}

	return true;
}

// 平面の頂点を求める
Vector3 Perpendicular(const Vector3 &vector) {
	if(vector.x != 0.0f || vector.y != 0.0f) {
		return{ -vector.y,vector.x,0.0f };
	}
	return{ 0.0f, -vector.z, vector.y };
}


// 正射影ベクトル
Vector3 Project(const Vector3 &v1, const Vector3 &v2) {
	Vector3 result;

	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	float lenSq = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;
	float scalar = dot / lenSq;

	result = { scalar * v2.x, scalar * v2.y, scalar * v2.z };

	return result;
}

// 最近接点
Vector3 ClosestPoint(const Vector3 &point, const Segment &segment) {
	Vector3 result;

	Vector3 vecToPoint = { point.x - segment.origin.x,point.y - segment.origin.y,point.z - segment.origin.z };
	// 正射影ベクトルをする
	Vector3 proj = Project(vecToPoint, segment.diff);
	result = { segment.origin.x + proj.x,segment.origin.y + proj.y,segment.origin.z + proj.z };


	return result;
}


/*二項演算子
*********************************************************/

Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
	return AddV(v1, v2);
}

Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
	return SubtractV(v1, v2);
}

Vector3 operator*(const float &s1, const Vector3 &v1) {
	return MultiplyV(s1, v1);
}

Vector3 operator*(const Vector3 &v1, const float &s1) {
	return s1 * v1;
}

Vector3 operator/(const Vector3 &v1, const float &s1) {
	return MultiplyV(1.0f / s1, v1);
}

Matrix4x4 operator+(const Matrix4x4 &m1, const Matrix4x4 &m2) {
	return Add(m1, m2);
}

Matrix4x4 operator-(const Matrix4x4 &m1, const Matrix4x4 &m2) {
	return Subtract(m1, m2);
}

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2) {
	return Multiply(m1, m2);
}





