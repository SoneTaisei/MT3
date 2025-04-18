#include <Novice.h>

const char kWindowTitle[] = "LC1C_10_ソネ_タイセイ_タイトル";

/*********************************************************
*構造体
*********************************************************/

// 4x4の行列
struct Matrix4x4 {
	float m[4][4];
};

// vector3
struct Vector3 {
	float x;
	float y;
	float z;
};

static const int kRowHeight = 20;
static const int kColumnWidth = 44;

/*********************************************************
*関数
*********************************************************/

/*行列の描画
*********************************************************/

void MatrixScreenPrintf(int x, int y, const Matrix4x4 &matrix);

void VectorScreenPrintf(int x, int y, Vector3 vector);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

/*行列の描画
*********************************************************/

void MatrixScreenPrintf(int x, int y, const Matrix4x4 &matrix) {
	for(int row = 0; row < 4; ++row) {
		for(int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth,
				y + row * kRowHeight,
				"%7.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, Vector3 vector) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
}

