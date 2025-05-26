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

	Segment segment = { {-0.45f,-1.45f,0.0f},{1.0f,0.58f,0.0f} };

	Triangle triangle;
	triangle.vertices[0] = { -1.0f,0.0f,0.0f };
	triangle.vertices[1] = { 1.0f,0.0f,0.0f };
	triangle.vertices[2] = { 0.0f,1.0f,0.0f };


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

		if(IsCollisionTriangleSegment(triangle, segment)) {
			segment.color = RED;
		} else {
			segment.color = WHITE;
		}

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
		ImGui::DragFloat3("Triangle[0]", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("Triangle[1]", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("Triangle[2]", &triangle.vertices[2].x, 0.01f);
		ImGui::DragFloat3("SegmentOrigin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("SegmentDiff", &segment.diff.x, 0.01f);
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

		// グリッドを表示する
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		// 三角形の描画
		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, WHITE);

		// 描画用
		Vector3 endPos = AddV(segment.origin, segment.diff); // 1か所で一貫
		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(endPos, viewProjectionMatrix), viewportMatrix);

		// 線を描画する
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment.color);


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

