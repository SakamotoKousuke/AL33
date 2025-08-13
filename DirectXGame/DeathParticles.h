#pragma once
#include <cstdint>
#include <array>
#include <KamataEngine.h>
#include <math/Vector3.h>

class DeathParticles {

	public:

	static inline const uint32_t kNumParticles = 8;

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	void Update();

	void Draw();

	private:

		KamataEngine::Camera* camera_;
	// ワールド変換データ
	

	// モデル
	KamataEngine::Model* model_ = nullptr;
};
