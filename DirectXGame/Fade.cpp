#include "Fade.h"
#include <algorithm>
using namespace KamataEngine;

// 初期化
void Fade::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle = TextureManager::Load("white1x1.png");
	// スプライトインスタンスの生成
	sprite_ = Sprite::Create(textureHandle, {0, 0});
	sprite_->SetSize(Vector2(1280, 720));
	// 画面サイズ
	sprite_->SetColor(Vector4(0, 0, 0, 1));
	// 黑色
}

// 更新
void Fade::Update() {
// フェード状態による分岐
switch (status_) {
case Status::None:
	// 何もしない
	break;
case Status::FadeIn:
	// 1フレーム分の秒数をカウントアップ counter += 1.0f / 60.0f;
	// フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
	}
	counter_ = duration_;
	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	sprite_->SetColor(Vector4(0, 0, 0, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)));
	break;
case Status::FadeOut:
	// 1フレーム分の秒数をカウントアップ counter += 1.0f / 60.0f;
	// フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
		counter_ = duration_;
	} // 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
	sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
	break;
}
}

// 描画
void Fade::Draw() {
	if (status_== Status::None) {
		return;
	}
	
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon =DirectXCommon::GetInstance();
	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}
// フェード開始
void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

// フェード終了
void Fade::Stop() { 
	status_ = Status::None;
}
// フェード終了判定
bool Fade::IsFinished() const {
	// フェード状態による分岐
	switch (status_) {
	case Status::FadeIn:
	
	case Status::FadeOut:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}