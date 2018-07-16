#pragma once

#include <PixelWorld.hpp>

using namespace PixelWorldEngine;

class GameObject : public PixelObject {
private:
	glm::vec2 needTranslate; //��¼��Ҫ�ƶ�����
protected:
	virtual void OnMove(float translationX, float translationY)override; //�ƶ��¼�

	virtual void OnCollide(void* sender, PixelObject* object)override; //��ײ�¼�
public:
	GameObject(std::string name); //���캯��

	void SetNeedTranslate(glm::vec2 translate); //������Ҫ�ƶ�����
};