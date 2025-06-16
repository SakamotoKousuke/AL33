#define NOMINMAX
#include "Player.h"
//#include "GameScene.h"
#include <cassert>
#include "MyMath.h"
#include<numbers>
#include <algorithm>


using namespace KamataEngine;

using namespace MathUtility;

void Player::Initialize( Model* model, Camera* camera, const Vector3& position) { 
	assert(model);
	camera_ = camera;
	//model_->Draw(worldTransform_, camera_, textureHandle);
	model_ = model;
	//textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

}

void Player::Update() {
	



	/*worldTransform_.translation_ += velocity_;*/

	//// アフィン変更行列の作成
	///*worldTransformBlock->matWorld_=アフィン変更行列;*/
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//worldTransform_.TransferMatrix(); 



	//移動入力
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
				velocity_.x *= (1.0f - kAttenuation);
				if (lrDirection_ != LRDirection::kLeft)
					lrDirection_ = LRDirection::kLeft;

				// 開始時の角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// タイマーに時間を設定する
				turnTimer_ = kTimeTurn;
			}

			

			// 加速/減速
			velocity_ += acceleration;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ加速
			velocity_ += Vector3(0, kJumpAccleration, 0);
		}
	}	
	else {
			// 落下速度
			velocity_ += Vector3(0, -kGravityAcceleration, 0);
			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	bool landing = false;

	if (velocity_.y < 0) {

		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}
	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	} 

	worldTransform_.translation_ += velocity_;

	if (turnTimer_>0.0f)
	{
		turnTimer_ = 1.0f / 60.0f;


		//左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float>*3.0f/2.0f
		};
		//状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		/*worldTransform_.rotation_.y = desttinationRotationY;*/
		/*worldTransform_.rotation_.y = turnFirstRotationY_;*/
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
		
	}
	

	// アフィン変更行列の作成
	/*worldTransformBlock->matWorld_=アフィン変更行列;*/
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix(); 

}

void Player::Draw() {
	model_->Draw(worldTransform_, *camera_); 
};
