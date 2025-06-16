#pragma once
#include "KamataEngine.h"
#include <math/Vector3.h>


enum class LRDirection {
	kRight,
	kLeft,
};


class Player {
public:

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; } 

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 0.1f;

	static inline const float kLimitFallSpeed = 2.0f;

	static inline const float kAcceleration = 0.05f;

	static inline const float kAttenuation = 0.1f;

	static inline const float kJumpAccleration = 1.0f;

	float turnFirstRotationY_ = 0.0f;

	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }


	static inline const float kLimitRunSpeed = 1.0f;

	

	enum class LRDirection {
		kRight,
		kLeft,
	};

	

	/*void Initialize(KamataEngine::Model* model, uint32_t textureHandle,KamataEngine::Camera* camera);*/
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	//void Initialize(KamataEngine::Model model, KamataEngine::Camera camera, const KamataEngine::Vector3& position);

	void Update();


	void Draw();


	private:
		//ワールド変換データ
		KamataEngine::WorldTransform worldTransform_;
		//モデル
	    KamataEngine::Model* model_ = nullptr;
		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;


		KamataEngine::Camera* camera_;

		LRDirection lrDirection_ = LRDirection::kRight;
		
		KamataEngine::Vector3 velocity_ = {};
};


