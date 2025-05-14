#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include <vector>
#include "Skydome.h"

class GameScene
{



	public:

		~GameScene() ;
		void Initialize();

		void Update();

		void Draw();

		uint32_t textureHandle_ = 0;

		//3Dモデルデータ
		KamataEngine::Model* model_ = nullptr;
		//カメラ
		KamataEngine::Camera camera_;
	    // 3Dモデルデータ
	    KamataEngine::Model* modelSkydome_ = nullptr;


		Player* player_ = nullptr;

		Skydome* skydome_ = nullptr;


		std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

		bool isDebugCameraActive_ = false;

		KamataEngine::DebugCamera* debugCamera_ = nullptr;

		//モデルブロック
		KamataEngine::Model* modelBlock_ = nullptr;
		//3Dモデル
		/*KamataEngine::Model* modelSkydome_ = nullptr;*/


		//KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);
		//
	 /*   bool isDebugCameraActive_ = false;*/

		//KamataEngine::DebugCamera* debugCamera_ = nullptr;
};