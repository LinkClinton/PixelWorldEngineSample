#include "RotateAnimation.hpp"

KeyFrame RotateAnimation::LinearProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame)
{
	auto lastData = lastFrame.GetData<float>();
	auto nextData = nextFrame.GetData<float>();

	auto lastTime = lastFrame.GetTimePos();
	auto nextTime = nextFrame.GetTimePos();

	auto currentData = lastData + (nextData - lastData) * (timePos - lastTime) / (nextTime - lastTime);

	return KeyFrame(currentData, timePos);
}

void RotateAnimation::RotateSetFunction(void * sender, void * data)
{
	auto object = (PixelObject*)sender;
	auto angle = *(float*)data;

	object->Transform.SetRotate(angle);
}

RotateAnimation::RotateAnimation(std::string name) : Animation(name, LinearProcessUnit)
{
}
