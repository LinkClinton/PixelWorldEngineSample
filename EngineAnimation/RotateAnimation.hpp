#pragma once

#include <Animation.hpp>
#include <PixelObject.hpp>

using namespace PixelWorldEngine;

class RotateAnimation : public Animation {
private:
	/**
	 * @brief ���Բ�ֵ
	 */
	static KeyFrame LinearProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame);
public:
	/**
	 * @brief ��������ĽǶ�
	 */
	static void RotateSetFunction(void* sender, void* data);

	RotateAnimation(std::string name);
};