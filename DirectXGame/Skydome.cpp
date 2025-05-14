#include "Skydome.h"

using namespace KamataEngine;

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	assert(model);
	camera_ = camera;
	// model_->Draw(worldTransform_, camera_, textureHandle);
	model_ = model;

	worldTransform_.Initialize();
}
void Skydome::Update() {};
void Skydome::Draw() {
	model_->Draw(worldTransform_, *camera_); 
};