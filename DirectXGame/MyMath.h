#pragma once
#include "KamataEngine.h"

float EaseInOut(float x1, float x2, float t);

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);