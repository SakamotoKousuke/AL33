#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

enum class Scene {

	kUnknown = 0,

	kTitle,
	kGame,
};

Scene scene = Scene::kUnknown;
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {

			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {

			scene = Scene::kTitle;

			delete gameScene;
			gameScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();//
		}
		//lkjnm

		break;
	}
}

void UpdateScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;

	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}

}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	

	//エンジンの初期化
KamataEngine::Initialize(L"LE2D_11_サカモト_コウスケ_AL3");
	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	scene = Scene::kTitle;

	/*gameScene = new GameScene();

	gameScene->Initialize();*/

	

	titleScene = new TitleScene;

	titleScene->Initialize();

	

	

	//メインループ
	while (true) {

		//エンジンの更新
	if (KamataEngine::Update()) {
		break;

		


	}

	ChangeScene();

	UpdateScene();

	

	

	dxCommon->PreDraw();

	
	DrawScene();
	


	dxCommon->PostDraw();




}


	delete titleScene;




	delete gameScene;

	gameScene = nullptr;

KamataEngine::Finalize();
return 0;
}