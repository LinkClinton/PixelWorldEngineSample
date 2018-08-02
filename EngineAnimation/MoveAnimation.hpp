#pragma once

#include <Animation.hpp>
#include <PixelObject.hpp>

using namespace PixelWorldEngine;

class MoveAnimation : public Animation {
private:
	static KeyFrame LinearProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame);
public:
	static void MoveSetFunction(void* sender, void* data);

	MoveAnimation(std::string name);
};