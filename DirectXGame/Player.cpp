#include "Player.h"
#include "GameScene.h"
#include "MyMath.h"




using namespace KamataEngine;

void Player::Initialize(Model* model, uint32_t textureHandle,Camera* camera) { 
	assert(model);
	camera_ = camera;
	//model_->Draw(worldTransform_, camera_, textureHandle);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

}

void Player::Update() {
	// アフィン変更行列の作成
	/*worldTransformBlock->matWorld_=アフィン変更行列;*/
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	

	worldTransform_.TransferMatrix(); 


	
}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_); 
};
