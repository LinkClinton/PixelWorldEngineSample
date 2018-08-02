#pragma once

#include <Animation.hpp>
#include <PixelObject.hpp>

using namespace PixelWorldEngine;

class RotateAnimation : public Animation {
private:
	/**
	 * @brief 线性插值
	 */
	static KeyFrame LinearProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame);
public:
	/**
	 * @brief 设置物体的角度
	 */
	static void RotateSetFunction(void* sender, void* data);

	RotateAnimation(std::string name);
};