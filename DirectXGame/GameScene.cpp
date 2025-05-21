#include "GameScene.h"
#include "MyMath.h"

using namespace KamataEngine;


//デストラクタ
GameScene::~GameScene() { 
	delete model_; 

	delete modelSkydome_;
	

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
		
			delete worldTransformBlock;
		}
	}

	

	

	delete debugCamera_;

	delete modelBlock_;

	delete skydome_;
	//delete debugCamera_;

	delete mapChipField_;

	GenerateBlocks();

}


void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("uvChecker.png");
	model_ = Model::Create();
	camera_.Initialize();

	model_ = Model::CreateFromOBJ("player");

	modelBlock_ = Model::CreateFromOBJ("block");

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, &camera_);

	// 3Dモデル
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_, &camera_);

	debugCamera_ = new DebugCamera(1280, 720);

	modelBlock_ = Model::CreateFromOBJ("block");

	mapChipField_ = new MapChipField;

	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

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
}

void GameScene::Update() {
//自キャラの更新
	player_->Update();

	debugCamera_->Update();

	skydome_->Update();

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




}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	//
	// 自分キャラの描画
	player_->Draw();
	// ブロックの描画

	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)

				continue;

			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
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



}
//