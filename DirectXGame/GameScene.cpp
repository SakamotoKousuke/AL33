#include "GameScene.h"
#include "MyMath.h"
#include "Enemy.h"
#include "player.h"
#include "DeathParticles.h"



using namespace KamataEngine;


//デストラクタ
GameScene::~GameScene() { 
	delete model_; 

	delete modelSkydome_;
	
	delete modelEnemy_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		
			if (!worldTransformBlock)

				continue;

			delete worldTransformBlock;
		}
	}

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete debugCamera_;

	delete modelBlock_;

	delete skydome_;
	//delete debugCamera_;

	delete mapChipField_;

	delete cameraController_;

	delete deathParticles_;
}


void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	camera_.Initialize();

	model_ = Model::CreateFromOBJ("player");

	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle");

	modelEnemy_ = Model::CreateFromOBJ("enemy");

	modelBlock_ = Model::CreateFromOBJ("block");

	//カメラを持たせる

	
	mapChipField_ = new MapChipField;

	
	
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");



	// 自キャラの生成
	player_ = new Player();

	/*enemy_ = new Enemy();*/
	for (int32_t i = 0; i < 10; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6 + i, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// 自キャラの初期化
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);

	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(5, 18);

	player_->Initialize(model_, &camera_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	// カメラコントローラーの生成
	cameraController_ = new CameraController();



	// カメラコントローラーの初期化
	cameraController_->Initialize();


	
// プレイヤーのWorldTransformを追従対象に設定
	cameraController_->SetTargrt(player_); // ← OK

	// カメラの位置をプレイヤーに瞬時に追従させる（初期配置）
	cameraController_->Reset();
	

	// 3Dモデル
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_, &camera_);

	debugCamera_ = new DebugCamera(1280, 720);

	modelBlock_ = Model::CreateFromOBJ("block");

	
	

	GenerateBlocks();

	
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};

	cameraController_->SetMovableArea(cameraArea);

	// 要素数

	/* const uint32_t kNumBlockVirtical = 10;
	 const uint32_t kNumBlockHorizontal = 20;*/

	// ブロック一個分の横幅

	// const float kBlockWidth = 2.0f;
	// const float kBlockHeight = 2.0f;

	// 要素数を変更する
	/*  worldTransformBlocks_.resize(kNumBlockHorizontal);*/

	// 要素数を変更する
	// 列数を設定(縦方向のブロック数)

	// worldTransformBlocks_.resize(kNumBlockVirtical);
	// for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	////一列の要素数を設定（横方向のブロック数）
	// worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}

	/*
	ブロックの生成*/

	// for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//  for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	//   // チェッカー状に配置（i+j が偶数のときだけ配置）
	//  /* if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
	//	  WorldTransform* worldTransform = new WorldTransform();
	//	  worldTransform->Initialize();
	//	  worldTransformBlocks_[i][j] = worldTransform;
	//	  worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);*/
	//	   if ((i + j) % 2 == 1) {
	//	    worldTransformBlocks_[i][j] = new WorldTransform();
	//	    worldTransformBlocks_[i][j]->Initialize();
	//	    worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//	    worldTransformBlocks_[i][j]->translation_.y = -kBlockHeight * i; // Y軸反転（左下スタート）
	//	  }
	//	   else {
	//	   worldTransformBlocks_[i][j] = nullptr; // 穴を開ける
	//	  }
	//	  /* worldTransformBlocks_[i][j] = new WorldTransform();
	//	   worldTransformBlocks_[i][j]->Initialize();
	//	   worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
	//	   worldTransformBlocks_[i][j]->translation_.y = kBlockHeight*i;*/
	//   }
	// }
	//}

	// worldTransformBlocks_.resize(kNumBlockVirtical);
	// for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//  worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}

	// ここで WorldTransform の new と配置などを行う

	//
	//}


	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticles_, &camera_, playerPosition);

}

void GameScene::Update() {
//自キャラの更新
	player_->Update();

	/*enemy_->Update();*/

	CheckAllCollisions();

	debugCamera_->Update();

	skydome_->Update();

	cameraController_->Update();

	#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	/*	OutputDebugStringA(isDebugCameraActive_ ? "DebugCamera ON\n" : "DebugCamera OFF\n");*/
	
	}
	#endif
	
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();

	} else {
		camera_.UpdateMatrix();
		
			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;
			// ビュープロじぇぅション行列の転送
			camera_.TransferMatrix();
		
	}

	// ブロックの更新

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) 

				continue;
				// アフィン変更行列の作成
				/*worldTransformBlock->matWorld_=アフィン変更行列;*/
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			

		}
	}

	
	for (Enemy* enemy : enemies_) {
	
		enemy->Update();

	}
	
	if (deathParticles_){
		deathParticles_->Update();
	}

	

}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	//
	// 自分キャラの描画
	player_->Draw();
	// ブロックの描画

	/*enemy_->Draw();*/

	for (Enemy* enemy : enemies_) {
	
	enemy->Draw();

	}


	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)

				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}


	Model::PostDraw();

	
}


	void GameScene::GenerateBlocks() {


	// 要素数
	uint32_t numBlockVirtical =mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal =mapChipField_->GetNumBlockHorizontal();


	// 要素数を変更する
	// 列数を設定 (縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		
			// 1列の要素数を設定 (横方向のブロック数)

			worldTransformBlocks_[i].resize(numBlockHorizontal);
			
		
	}

	//ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_=mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}

	cameraController_->Update();


}
    void GameScene::CheckAllCollisions() {

	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
	
		aabb2 = enemy->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);

			enemy->OnCollision(player_);
		}

	
	}


	
	}
    //