#define NOMINMAX
#include "Player.h"
// #include "GameScene.h"
#include "MapChipField.h"
#include "MyMath.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "Enemy.h"

using namespace KamataEngine;

using namespace MathUtility;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	worldTransform_.translation_ = position;

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 初期回転
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	// 移動入力
	InputMove();

	// 移動量を加味して衝突判定をする

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	// 判定結果を反映して移動させる
	// 移動
	// worldTransform_.translation_+=velocity_;
	CheckMapMove(collisionMapInfo);

	// 天井に接触してる場合の処理
	CheckMapCeiling(collisionMapInfo);



	// 壁に接触している場合の処理
	CheckMapWall(collisionMapInfo);

	// 接地状態の切り替え
	CheckMapLanding(collisionMapInfo);

	

	////着地フラグ
	// bool landing = false;

	// if (velocity_.y < 0) {

	//	if (worldTransform_.translation_.y <= 1.0f) {
	//		landing = true;
	//	}
	//}
	//// 接地判定
	// if (onGround_) {
	//	// ジャンプ開始
	//	if (velocity_.y > 0.0f) {
	//		// 空中状態に移行
	//		onGround_ = false;
	//	}
	// } else {
	//	// 着地
	//	if (landing) {
	//		// めり込み排斥
	//		worldTransform_.translation_.y = 1.0f;
	//		// 摩擦で横方向速度が減衰する
	//		velocity_.x *= (1.0f - kAttenuation);
	//		// 下方向速度をリセット
	//		velocity_.y = 0.0f;
	//		// 接地状態に移行
	//		onGround_ = true;
	//	}
	// }

	// Animate制御
	AnimateTurn();

	/*worldTransform_.translation_ += velocity_;*/

	// アフィン変更行列の作成
	/*worldTransformBlock->matWorld_=アフィン変更行列;*/
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_); };

void Player::InputMove() {

	// 移動入力
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {

					velocity_.x *= (1.0f - kAttenuation);
				}

				acceleration.x -= kAcceleration;
				/*velocity_.x *= (1.0f - kAttenuation);*/
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
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ加速
			velocity_ += Vector3(0, kJumpAccleration, 0);
		}
	} else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::CheckMapCollision(CollisionMapInfo& info) {

	CheckMapCollisionUP(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUP(CollisionMapInfo& info) {

	if (info.move.y <= 0) {
		return;
	}

	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	/*MapChipType mapChipTypeNext;*/

	bool hit = false;
	// 左上店の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex); // ← これが抜けていた
	
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点の判定

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		/*indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		MapChipField::IndexSet indexSetNow;
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
		    info.move.y = std::max(0.0f, info.move.y);
		    info.ceiling = true;*/

		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.ceiling = true;
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	bool hit = false;

	// 左下店の判定
	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex); // ← これが抜けていた
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex -1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext !=MapChipType::kBlock) {
		hit = true;
	}
	/*if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}*/
	// 右下点の判定

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] );
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	/*if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}*/
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex -1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext !=MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
			if (indexSetNow.yIndex != indexSet.yIndex) {
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			/*info.move.y = std::min(0.0f, rect.bottom-worldTransform_.translation_.y-(kHeight/2.0f+kBlank));
			 */
			// info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			info.landing = true;
			}
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	bool hit = false;

	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	/*if (mapChipType == MapChipType::kBlock) {*/
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock /*&& mapChipTypeNext != MapChipType::kBlock*/) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	/*if (mapChipType == MapChipType::kBlock) {*/
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock /*&& mapChipTypeNext != MapChipType::kBlock*/) {
		hit = true;
	}

	if (hit) {

		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kHeight / 2.0f, 0,0));

		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
			/*if (indexSetNow.yIndex != indexSet.yIndex) {*/
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			/*info.move.y = std::min(0.0f, rect.bottom-worldTransform_.translation_.y-(kHeight/2.0f+kBlank));
			 */
			// info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		/*}*/
	}


}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;

	bool hit = false;


	MapChipField::IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	/*if (mapChipType == MapChipType::kBlock) {*/
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex+1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock /*&& mapChipTypeNext != MapChipType::kBlock*/) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	/*if (mapChipType == MapChipType::kBlock) {*/
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex+1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock /*&& mapChipTypeNext != MapChipType::kBlock*/) {
		hit = true;
	}
	if (hit) {

		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth/ 2.0f, 0,0));

		/*MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));*/
			/*if (indexSetNow.yIndex != indexSet.yIndex) {*/
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			/*info.move.y = std::min(0.0f, rect.bottom-worldTransform_.translation_.y-(kHeight/2.0f+kBlank));
			 */
			// info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
			info.hitWall = true;
		/*}*/
	}


}

void Player::CheckMapWall(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}

}

Vector3 Player::GetWorldPosition() { 
	

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
	//return Vector3();

}

AABB Player::GetAABB() {
	
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;

	
	/*return AABB(); */
}



void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;

	velocity_ += Vector3(0.0f, 1.0f, 0.0f);
}

void Player::CheckMapMove(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.move;
}

void Player::CheckMapCeiling(const CollisionMapInfo& info) {
	// 天井に当たった?
	if (info.ceiling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0;
	}
}

//

////地面と接触している場合の処理
//   void Player::CheckCollisionMapInfo(const CollisionMapInfo& info) {

//    // 移動後の４つの角の座標
//    std::array<Vector3, kNumCorner> positionsNew;

//    MapChipType mapChipType;

//    bool hit = false;

//    // 左下店の判定
//    MapChipField::IndexSet indexSet;
//    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
//    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex); // ← これが抜けていた
//    if (mapChipType == MapChipType::kBlock) {
//	    hit = true;
//    }

//    // 左下店の判定
//    MapChipField::IndexSet indexSet;
//    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
//    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex); // ← これが抜けていた
//    if (mapChipType == MapChipType::kBlock) {
//	    hit = true;
//    }

//    if (onGround_) {
//	    if (info.landing) {

//		    onGround_ = true;

//		    velocity_.x *= (1.0f - kAttenuationLanding);

//		    velocity_.y = 0.0f;
//	    }
//	    if (velocity_.y > 0.0f) {
//		    onGround_ = false;
//	    } else {

//		    if (!hit) {
//			    onGround_ = false;
//		    }
//	    }
//    }
//   }

/*void Player::CheckMapCollision(CollisionMapInfo& info) {
    if (info.ceiling) {
        DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
        velocity_.y = 0;

    }



}*/

// 接地状態の切り替え
void Player::CheckMapLanding(const CollisionMapInfo& info) {
	// 自キャラが接地状態？
	if (onGround_) {

		// 接地状態の処理

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			// 落下判定

			// 移動後の４つの角の座標

			// 左下店の判定

			// 右下店の判定

			// 移動後の４つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			bool hit = false;

			// 左下店の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex-1, indexSet.yIndex); // ← これが抜けていた
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 左下店の判定

			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex-1, indexSet.yIndex); // ← これが抜けていた
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit) {
				// 空中状態に切り替える
					    onGround_ = false;
			}
		}
	} else {

		// 空中状態の処理
		if (info.landing) {

			onGround_ = true;

			velocity_.x *= (1.0f - kAttenuationLanding);

			velocity_.y = 0.0f;
		}
	}
}

void Player::AnimateTurn() {

	if (turnTimer_ > 0.0f) {
		turnTimer_ = 1.0f / 60.0f;

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		/*worldTransform_.rotation_.y = desttinationRotationY;*/
		/*worldTransform_.rotation_.y = turnFirstRotationY_;*/
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

// 指定した角の座標計算

// Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {
//  Vector3 offsetTable[kNumCorner] = {
//      {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //kRightBottom
//{-kWidth / 2.0f, -kHeight / 2.0f, 0}, //kLeftBottom
//{+kWidth / 2.0f, +kHeight / 2.0f, 0}, //kRightTop
//{-kWidth / 2.0f, +kHeight / 2.0f, 0},  //kLeftTop
//     };
//  return center + offsetTable[static_cast<uint32_t>(corner)];
// }
Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {
	static Vector3 offsetTable[static_cast<size_t>(Corner::kNumCorner)] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, //  kLeftTop
	};
	return center + offsetTable[static_cast<size_t>(corner)];
}

void Player::ApplyMapCollisionInfo(const CollisionMapInfo& info) {
	worldTransform_.translation_ += info.move;
	velocity_.y = 0;
};

