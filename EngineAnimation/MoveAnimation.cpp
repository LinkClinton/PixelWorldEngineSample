#include "MoveAnimation.hpp"

KeyFrame MoveAnimation::LinearProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame)
{
	auto lastData = lastFrame.GetData<glm::vec2>();
	auto nextData = nextFrame.GetData<glm::vec2>();

	auto lastTime = lastFrame.GetTimePos();
	auto nextTime = nextFrame.GetTimePos();

	auto currentData = lastData + (nextData - lastData) * (timePos - lastTime) / (nextTime - lastTime);

	currentData.x = round(currentData.x);
	currentData.y = round(currentData.y);

	return KeyFrame(currentData, timePos);
}

void MoveAnimation::MoveSetFunction(void * sender, void * data)
{
	auto object = (PixelObject*)sender;
	auto position = *(glm::vec2*)data;

	object->Transform.SetPosition(position);
}

MoveAnimation::MoveAnimation(std::string name) : Animation(name, LinearProcessUnit)
{

}
