#include "DeathParticles.h"
#include <algorithm>
#include "MyMath.h"
using namespace KamataEngine;
using namespace MathUtility;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	
	/*worldTransform_.translation_ = position;*/

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;

	}
	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	
	// 引数の内容をメンバ変数に記録
	camera_ = camera;
}

void DeathParticles::Update() {
	/*worldTransformBlock->matWorld_=アフィン変更行列;*/
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};
	
		float angle = kAngleUnit * i;

		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);

		velocity = Transform(velocity, matrixRotation);

		worldTransforms_[i].translation_ += velocity;

	}

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration) {
		counter_ = kDuration;
	
		isFinished_ = true;
	}

	if (isFinished_) {
		return;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);

	objectColor_.SetColor(color_);
}
//
//void DeathParticles::Update() {
//model_->Draw(worldTransform_, *camera_
//}

void DeathParticles::Draw() {



	for (WorldTransform& worldTransform : worldTransforms_) {
		
		model_->Draw(worldTransform, *camera_,&objectColor_); 
	}







}
