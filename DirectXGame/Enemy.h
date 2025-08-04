#pragma once
#include <KamataEngine.h>

class Enemy {
public:

	static inline const float kWalkSpeed = 0.1f;
	KamataEngine::Vector3 velocity_ = {};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	

	//static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera* camera_;
};