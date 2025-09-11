#include "MyMath.h"
#include "TitleScene.h"
#include <numbers>
using namespace KamataEngine;
// デストラクタ
TitleScene::~TitleScene() {
	// モデル
	delete model_;
	delete modelPlayer_;
}
// 初期化
void TitleScene::Initialize() {
	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");
	// カメラの初期化
	camera_.Initialize();

// ワールド変換の初期化
worldTransform_.Initialize();
worldTransform_.scale_ = {2, 2, 2};
worldTransform_.translation_ = {0, 8, 0};
worldTransformPlayer_.Initialize();
worldTransformPlayer_.scale_ = {10, 10, 10};
worldTransformPlayer_.translation_ = {0, -8, 0};
worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
}



	//modeltitleFont_ = Model::CreateFromOBJ("titleFont");

void TitleScene::Update() {

	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	// 回転
	rotate += 0.1f;
	worldTransformPlayer_.rotation_.y = sin(rotate) + std::numbers::pi_v<float>;
	// アフィン変換行列の作成
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_, worldTransformPlayer_.translation_);
	// 行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
	// タイトルシーンの終了条件
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
 		finished_ = true;
	}

	/*finished_ = true;*/
}

void TitleScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon =DirectXCommon::GetInstance();
	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());
	// ここに3Dモデルインスタンスの描画処理を記述する
	//
	//  描画
	model_->Draw(worldTransform_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	// 3Dモデル描画後処理
	Model::PostDraw();

}