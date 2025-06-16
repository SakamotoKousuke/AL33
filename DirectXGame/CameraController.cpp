#include "CameraController.h"
#include "Player.h" // ← ここを追加！！（これだけでOK）


using namespace KamataEngine;

using namespace MathUtility;


// デストラクタ
void CameraController::Initialize() { 
	/*camera_ = *camera; */

	 

	/*camera_.Initialize();*/
	
	 camera_.Initialize();   

}
//
//void CameraController::SetTargrt(Player* target) { target_ = target; }

void CameraController::Update() {

	const Vector3& targetVelocity = target_->GetVelocity();
	//if (!target_ || !camera_)
	//	return; 
	//  追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	
	// 追従対象とオフセットからカメラの座標を計算
	targetPosition_ = targetWorldTransform.translation_ + targetOffset_+targetVelocity*kVelocityBias;



	//Camera& cam = *camera_;

	/*cam.translation_ = targetWorldTransform.translation_ + targetOffset_;*/
//cam.UpdateMatrix();




	/*const Vector3& targetVelocity = target_->GetVelocity();

	targetPosition_ = targetWorldTransform.translation_ + targetVelocity * kVelocityBias;*/






	//追従対象とオフセットからcameraの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	// 安全チェック（NULLポインタ防_

	camera_.translation_.x = max(camera_.translation_.x, camera_.translation_.x + targetMargin.left);
	camera_.translation_.x = min(camera_.translation_.x, camera_.translation_.x + targetMargin.right);
	camera_.translation_.y = min(camera_.translation_.y, camera_.translation_.y + targetMargin.bottom);
	camera_.translation_.y = max(camera_.translation_.y, camera_.translation_.y + targetMargin.top);
	

	camera_.translation_.x = max(camera_.translation_.x, movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.top);
	
	
	
	
	// 行列を更新する
	camera_.UpdateMatrix();
}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;

};