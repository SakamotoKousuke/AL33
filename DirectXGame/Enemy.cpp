#include <KamataEngine.h>
#include "Enemy.h"
#include "MyMath.h"
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;


void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) { 
	velocity_ = {-kWalkSpeed, 0, 0}; 

	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	
	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	walkTimer_ = 0.0f;
}

void Enemy::Update() { 
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.rotation_.x = std::sin(walkTimer_ * 5.0f);
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;


	worldTransform_.TransferMatrix();
}



void Enemy::Draw() { model_->Draw(worldTransform_, *camera_); }
