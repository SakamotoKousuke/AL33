#pragma once
#include "KamataEngine.h"

class Skydome {
	public:

		void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);


		void Update();


		void Draw();

	private:

			KamataEngine::WorldTransform worldTransform_;

			KamataEngine::Model* model_ = nullptr;

			KamataEngine::Camera* camera_ = nullptr;
};