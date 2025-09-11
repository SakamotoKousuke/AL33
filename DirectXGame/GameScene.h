#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "MyMath.h"
#include "DeathParticles.h"


class GameScene
{

	enum class Phase {
		kPlay,
		kdeath,
	};
	Phase phase_;


	public:


		/*GameScene(); */      // ← 明示的に追加する
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

		KamataEngine::Model* modelEnemy_ = nullptr;

		bool finished_ = false;

		bool IsFinished() const { return finished_; }


		Player* player_ = nullptr;

		Skydome* skydome_ = nullptr;

		/*Enemy* enemy_ = nullptr;*/
	    std::list<Enemy*> enemies_;
		

		std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

		bool isDebugCameraActive_ = false;

		KamataEngine::DebugCamera* debugCamera_ = nullptr;

		//モデルブロック
		KamataEngine::Model* modelBlock_ = nullptr;

		KamataEngine::Model* modelDeathParticles_ = nullptr;

		//3Dモデル
		/*KamataEngine::Model* modelSkydome_ = nullptr;*/


		//KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);
		//
	 /*   bool isDebugCameraActive_ = false;*/

		//KamataEngine::DebugCamera* debugCamera_ = nullptr;

		MapChipField* mapChipField_;

		CameraController* cameraController_;

		

		static constexpr uint32_t kNumBlockVirtical = 10;
	    static constexpr uint32_t kNumBlockHorizontal = 20;
	    static constexpr float kBlockWidth = 2.0f;
	    static constexpr float kBlockHeight = 2.0f;

		//worldTransformBlocks_.resize(kNumBlockVirtical); // OK！

		void GenerateBlocks();


		void CheckAllCollisions();
		 /*MapChipField* mapChipField_ = nullptr;*/    // ヘッダ内で安全に初期化（C++11以上）
	    void ChangePhase();

		

		DeathParticles* deathParticles_ = nullptr;
};