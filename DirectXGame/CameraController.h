#pragma once
#include "KamataEngine.h"
#include <vector>
#include "Skydome.h"
#include <vector>

//using namespace MathUtility;
using namespace KamataEngine;

// Playerの前方宣言（定義はcppまたは別ヘッダにある前提）

class Player;





class CameraController {

public:

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(Rect area) { movableArea_ = area; }


	
	void Initialize();

	void Update();

	void Reset();

	void SetTargrt(Player* target) { target_ = target; }

	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	static inline const float kInterpolationRate = 1.0f;


	
	static inline const float kVelocityBias = 1.0f;

	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};



	private:

		 //Camera camera_; // ← ★ここを「実体」に変更！ ポインタではなくなる

		 Camera camera_ ; // ← ポインタに変更
		/*KamataEngine::Camera camera_;*/

		KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};

		Player* target_ = nullptr;

		KamataEngine::Vector3 targetPosition_={100, 100};



	
};