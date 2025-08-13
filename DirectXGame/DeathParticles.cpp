#include "DeathParticles.h"

#include "MyMath.h"
using namespace KamataEngine;


void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	
	/*worldTransform_.translation_ = position;*/

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;

	}

	

}

void DeathParticles::Update() {
	/*worldTransformBlock->matWorld_=アフィン変更行列;*/
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();
	}

}
//
//void DeathParticles::Update() {
//model_->Draw(worldTransform_, *camera_
//}

void DeathParticles::Draw() {



	for (WorldTransform& worldTransform : worldTransforms_) {
		
		model_->Draw(worldTransform, *camera_); 
	}







}
