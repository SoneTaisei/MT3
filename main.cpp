#include <Novice.h>
#define _USE_MATH_DEFINES
#include<cmath>

#include"assert.h"
#include"RenderingPipelineFunction.h"

#include"imgui.h"


const char kWindowTitle[] = "LC1C_10_ソネ_タイセイ_タイトル";


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 v1 = { 1.2f,-3.9f,2.5f };
	Vector3 v2 = { 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);

	Vector3 cameraRotate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraTranslate = { 0.26f,0.0f,0.0f };
	Vector3 cameraPosition = { 0.0f,0.0f,0.0f };

	Sphere sphere = { 0.0f,0.0f,0.0f,0.05f };

	const Vector3 kLocalVertices[3] = {
		{-0.5f,-0.5f,5.0f},
		{0.0f,0.5f,5.0f},
		{0.5f,-0.5f,5.0f},
	};

	// ウィンドウの×ボタンが押されるまでループ
	while(Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// worldMatrixを中心回転にあわせる
		Matrix4x4 worldMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, Vector3{}, Vector3{});

		// 各行列の計算
		Matrix4x4 cameraMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix =
			Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix =
			MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix =
			Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix =
			MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

#ifdef _DEBUG
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.1f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.1f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.1f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.1f);
		ImGui::End();
#endif // _DEBUG


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// 球体を描画する
		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, BLACK);

		// グリッドを表示する
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if(preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

