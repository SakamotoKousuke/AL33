#pragma once
#include <KamataEngine.h>

class TitleScene {

	public:

	bool finished_ = false;

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
	   


};












