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

	Vector3 translates[3]{
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};

	Vector3 rotates[3]{
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};

	Vector3 scales[3]{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};

	Matrix4x4 Ls = {};
	Matrix4x4 Le = {};
	Matrix4x4 Lh = {};

	


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
		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);
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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Ls = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		Le = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		Lh = MakeAffineMatrix(scales[2], rotates[2], translates[2]);

		// ワールド行列の作成
		Matrix4x4 Ws = Ls;
		Matrix4x4 We = Multiply(Le, Ws);
		Matrix4x4 Wh = Multiply(Lh, We);

		// 制御点をワールド座標に変換
		Vector3 worldP[3] = {
			Transform({0.0f, 0.0f, 0.0f}, Ws),
			Transform({ 0.0f, 0.0f, 0.0f }, We),
			Transform({ 0.0f, 0.0f, 0.0f }, Wh)
		};

		// グリッドを表示する
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		Sphere sphereControlPositions[3] = {};
		for(uint32_t i = 0; i < 3; ++i) {
			sphereControlPositions[i].center = worldP[i];
			sphereControlPositions[i].radius = 0.1f;
		}

		// 点の場所
		DrawSphere(sphereControlPositions[0], viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphereControlPositions[1], viewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(sphereControlPositions[2], viewProjectionMatrix, viewportMatrix, BLUE);


		// 描画用
		for(int i = 0; i < 2; ++i) {
			Vector3 start = Transform(Transform(sphereControlPositions[i].center, viewProjectionMatrix), viewportMatrix);
			Vector3 end = Transform(Transform(sphereControlPositions[i+1].center, viewProjectionMatrix), viewportMatrix);
			// 線を描画する
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);
		}
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

