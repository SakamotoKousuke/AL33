#pragma once
#include "KamataEngine.h"
#include <math/Vector3.h>

class MapChipField;

enum class LRDirection {
	kRight,
	kLeft,
};


enum Corner {
	kRightBottom,     //右下 
	kLeftBottom, //左下 
	kRightTop,//右上 
	kLeftTop,// 左上

	kNumCorner// 要素


};



class Player {
public:

	
struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突
		bool landing = false; // 天井衝突
		bool hitWall = false; // 壁接触
		KamataEngine::Vector3 move; // 移動量
	};

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

	static inline const float kWidth = 0.8f;

	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.1f;

	static inline const float kAttenuationLanding = 1.0f;

	enum class LRDirection {
		kRight,
		kLeft,
	};

	

	/*void Initialize(KamataEngine::Model* model, uint32_t textureHandle,KamataEngine::Camera* camera);*/
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void InputMove();

	//void Initialize(KamataEngine::Model model, KamataEngine::Camera camera, const KamataEngine::Vector3& position);

	void AnimateTurn();

	/*Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);*/
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);


	void Update();


	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_=mapChipField; }

	

	void CheckMapCollisionUP(CollisionMapInfo& info);

	void CheckMapCollisionDown(CollisionMapInfo& info);

	void CheckMapMove(const CollisionMapInfo& info);

	void CheckMapCeiling(const CollisionMapInfo& info);

	void CheckMapCollision(CollisionMapInfo& info);

	void ApplyMapCollisionInfo(const CollisionMapInfo& info);

	/*void CheckCollisionMapInfo(const CollisionMapInfo& info);*/

	void CheckMapLanding(const CollisionMapInfo& info);

	//void ApplyCollisionMapInfo(const CollisionMapInfo& info);

	


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

		MapChipField* mapChipField_ = nullptr;

};


