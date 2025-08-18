#pragma once
#include <cstdint>
#include <array>
#include <KamataEngine.h>
#include <math/Vector3.h>
#define M_PI 3.14159265358979323846f
#include<numbers>


class DeathParticles {

	public:

	static inline const uint32_t kNumParticles = 8;

	/*void Initialize(Model* model, Camera* camera, const Vector3& position);*/

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	void Update();

	void Draw();

	static inline const float kDuration = 1.0f;

	static inline const float kSpeed = 0.1f;

	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;



	bool isFinished_ = false;

	float counter_ = 0.0f;

	KamataEngine::ObjectColor objectColor_;

	KamataEngine::Vector4 color_;

	private:

		KamataEngine::Camera* camera_;
	// ワールド変換データ
	

	// モデル
	KamataEngine::Model* model_ = nullptr;
};
