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

	// カメラ座標
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };
	Vector3 cameraScale = { 1.0f,1.0f,1.0f };

	Plane plane{
		.normal{Normalize({-0.2f,0.9f,-0.3f}) },
		.distance{0.0f}
	};

	Ball ball{
		.position{0.8f,1.2f,0.3f},
		.acceleration{0.0f,-9.8f,0.0f},
		.mass{2.0f},
		.radius{0.05f},
		.color{WHITE}
	};

	bool moveStart = false;

	float deltaTime = 1.0f / 60.0f;

	float e = 0.8f;


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

		// 各行列の計算
		Matrix4x4 cameraMatrix =
			MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix =
			Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix =
			MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix =
			Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix =
			MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

#ifdef _DEBUG
		ImGui::Begin("Window");

		if(ImGui::Button("start")) {
			moveStart = true;
		}

		ImGui::End();
		/*マウスでカメラ操作
		*********************************************************/

		ImGuiIO &io = ImGui::GetIO();

		// 右クリックドラッグで回転
		if(io.MouseDown[1]) {
			cameraRotate.y += io.MouseDelta.x * 0.001f;
			cameraRotate.x += io.MouseDelta.y * 0.001f;

			// clamp pitch
			cameraRotate.x = std::clamp(cameraRotate.x, -1.57f, 1.57f);  // ±90度
		}

		// 中クリックドラッグで平行移動
		if(io.MouseDown[2]) {
			Vector3 right = { cosf(cameraRotate.y), 0, -sinf(cameraRotate.y) };
			Vector3 up = { 0, 1, 0 };

			cameraTranslate = AddV(cameraTranslate, MultiplyV(-io.MouseDelta.x * 0.01f, right));
			cameraTranslate = AddV(cameraTranslate, MultiplyV(io.MouseDelta.y * 0.01f, up));
		}

		// ホイールでズーム
		cameraTranslate.z += io.MouseWheel * -0.5f;
		cameraTranslate.z = std::clamp(cameraTranslate.z, -6.49f, 50.0f);
#endif // _DEBUG

		// フレーム数で加算

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;
		if(IsCollideSpherePlane(Sphere{ ball.position,ball.radius }, plane)) {
			Vector3 reflected = Reflect(ball.velocity, plane.normal);
			Vector3 projectToNormal = Project(reflected, plane.normal);
			Vector3 movingDirection = reflected - projectToNormal;
			ball.velocity = projectToNormal * e + movingDirection;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		// グリッドを表示する
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);
		
		Sphere sphere = {
			.center = ball.position,
			.radius = ball.radius,
			.color = ball.color
		};

		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, sphere.color);

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

