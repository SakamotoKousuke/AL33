#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	


	//エンジンの初期化
KamataEngine::Initialize(L"LE2D_11_サカモト_コウスケ_AL3");
	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	GameScene* gameScene = new GameScene();

	gameScene->Initialize();

	//メインループ
	while (true) {

		//エンジンの更新
	if (KamataEngine::Update()) {
		break;




	}


	gameScene->Update();

	dxCommon->PreDraw();

	gameScene->Draw();

	


	dxCommon->PostDraw();




}

	delete gameScene;

	gameScene = nullptr;

KamataEngine::Finalize();
return 0;
}