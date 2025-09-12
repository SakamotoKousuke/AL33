#pragma once
#include <KamataEngine.h>

#include "Fade.h"




class TitleScene {

	public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};


	

	

	bool IsFinished() const { return finished_; }

	void Initialize();

	void Update();

	void Draw();

	~TitleScene();
	
	private:




		KamataEngine::Model* model_ = nullptr;

		KamataEngine::Model* modelPlayer_ = nullptr;
	    // カメラ
	    KamataEngine::Camera camera_;
	    // ワールド変換データ
	    KamataEngine::WorldTransform worldTransform_;
	    KamataEngine::WorldTransform worldTransformPlayer_;
	    // 回転
	    float rotate = 0.0f;

	    // 終了フラグ (変数名に注意)
		bool finished_ = false;
		//ll
		//フェード
	    Fade* fade_ = nullptr;

		//現在のフェーズ
	    Phase phase_ = Phase::kFadeIn;

};












