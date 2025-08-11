#pragma once
#include <KamataEngine.h>
#include "MyMath.h"
//
//enum class LRDirection {
//	kRight,
//	kLeft,
//};

class Player;

class Enemy {
public:

	static inline const float kWalkSpeed = 0.1f;
	KamataEngine::Vector3 velocity_ = {};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	static inline const float kWidth = 0.8f;

	static inline const float kHeight = 0.8f;

	KamataEngine::Vector3 GetWorldPosition();

	//static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera* camera_;

	AABB GetAABB();

	void OnCollision(const Player* player);
};