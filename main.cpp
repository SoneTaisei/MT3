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

	Sphere sphere1 = { 0.0f,1.0f,0.0f,0.5f };
	Sphere sphere2 = { 1.0f,0.0f,0.0f,0.5f };
	sphere1.color = WHITE;
	sphere2.color = WHITE;

	// 点から始点へのベクトル
	Vector3 diffToPoint = {
	point.x - segment.origin.x,
	point.y - segment.origin.y,
	point.z - segment.origin.z
	};

	Vector3 project =
		Project(diffToPoint, segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

	Sphere pointSphere = { point,0.01f };
	Sphere clossPointSphere = { closestPoint,0.01f };

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
		
		if(IscollideSphere(sphere1, sphere2)) {
			sphere1.color = RED;
			sphere2.color = RED;
		} else {
			sphere1.color = WHITE;
			sphere2.color = WHITE;
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
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter1", &sphere1.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius1", &sphere1.radius, 0.01f);
		ImGui::DragFloat3("SphereCenter2", &sphere2.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius2", &sphere2.radius, 0.01f);
		ImGui::End();
#endif // _DEBUG

		ImGui::InputFloat3("Point", &point.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment origin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Segment diff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		//// 球体を描画する
		DrawSphere(sphere1, viewProjectionMatrix, viewportMatrix, sphere1.color);
		DrawSphere(sphere2, viewProjectionMatrix, viewportMatrix, sphere2.color);

		// グリッドを表示する
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(clossPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

		Vector3 start =
			Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end =
			Transform(
				Transform({ segment.origin.x + segment.diff.x,segment.origin.y + segment.diff.y,segment.origin.z + segment.diff.z },viewProjectionMatrix),
				viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		
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

