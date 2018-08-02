#include <Application.hpp>
#include <PixelWorld.hpp>

#include "MoveAnimation.hpp"
#include "RotateAnimation.hpp"

#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string pixelWorldName = u8"PixelWorld";
const std::string windowName = u8"PixelWorld";
const std::string worldMapName = u8"WorldMap";

const int width = 800;
const int height = 600;

const int mapWidth = 30;
const int mapHeight = 30;

const int objectCount = 10;
const int objectSize = 10;

Application* app = new Application(applicationName); //����Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //��������
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //������ͼ

DataManager* dataManager = new DataManager(app); //�������ݹ�����
TextureManager* textureManager = new TextureManager(app, Graphics::PixelFormat::R8G8B8A8); //�������������

Camera camera = Camera(RectangleF(0, 0, width, height)); //���������

PixelObject* subObject[objectCount * objectCount]; //����
Animation* animation[objectCount * objectCount]; //����
Animation* rotateAnimation; //��ת����
Animator* animator = new Animator("Animator"); //������������

int Rand(int l, int r) { //�������
	return rand() % (r - l + 1) + l;
}

int Rand(int x) { //�������
	return Rand(0, x);
}

/**
* @brief ���������������
*/
void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //��ȡ����

	int renderObjectID = 0; //��Ⱦ��ţ���������ӳ�����������������ʹ��

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //����������ԭ�������и��Ƴ�һ���ֳ���

			textureManager->RegisterTexture(++renderObjectID, subTexture); //����һ��������ӵ������������ȥ����Ϊ�����ú���Ⱦ���

			Utility::Delete(subTexture); //�ͷ���Դ
		}
	}

}

/**
 * @brief ����������
 */
void MakeObject() {

	float startX = width * 0.5f - objectCount * objectSize * 0.5f + objectSize * 0.5f; //������ʼλ��
	float startY = height * 0.5f - objectCount * objectSize * 0.5f + objectSize * 0.5f;

	int id = 0;
	
	float rotateSpeed = glm::pi<float>() * 2; //��ת�ٶ�

	rotateAnimation = new RotateAnimation("RotateAnimation");

	rotateAnimation->SetKeyFrame(0, 0);
	rotateAnimation->SetKeyFrame(5 * rotateSpeed, 5);
	rotateAnimation->SetKeyFrame(5 * rotateSpeed, 8);
	rotateAnimation->SetKeyFrame(0, 13);
	rotateAnimation->Sort(); //������ת����

	for (int x = 0; x < objectCount; x++) {
		for (int y = 0; y < objectCount; y++) {
			//��������
			subObject[id] = new PixelObject();
			animation[id] = new MoveAnimation("MoveAnimation" + Utility::ToString(id));

			subObject[id]->SetSize(objectSize, objectSize);
			subObject[id]->Transform.SetPosition(glm::vec2(startX + x * objectSize, startY + y * objectSize));

			subObject[id]->RenderObjectID = 2;

			world->SetPixelObject(subObject[id], Layer::UI);

			int startXpos = Rand(width);
			int startYpos = Rand(height);
			//������϶���
			animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 0);
			animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 5);
			animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 8);
			animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 13);
			animation[id]->Sort();

			//��Ӷ���
			animator->AddAnimation(subObject[id], MoveAnimation::MoveSetFunction, animation[id]);
			animator->AddAnimation(subObject[id], RotateAnimation::RotateSetFunction, rotateAnimation);

			id++;
		}
	}
	//�����ظ��������Լ����ж���
	animator->EnableRepeat(true);
	animator->Run();
}

/**
* @brief ��������
*/
void MakeWorld() {

	//����ͼʹ����Ⱦ���Ϊ18��������
	for (int x = 0; x < mapWidth; x++) {//��ͼ���
		for (int y = 0; y < mapHeight; y++) {//��ͼ�߶�
			auto data = MapData(); //������ͼ������

			data.RenderObjectID = 18; //������һ���ͼ��Ⱦ��ʱ��ʹ�õ�����

			worldMap->SetMapData(x, y, data); //���úõ�ͼ����
		}
	}

	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //���ú������
	world->SetWorldMap(worldMap); //���úõ�ͼ

	world->SetTextureManager(textureManager); //���ú����������
}

int main() {
	srand((unsigned int)time(0));

	MakeTextureManager(); //���������������

	MakeWorld(); //��������

	MakeObject(); //��������

	app->SetWorld(world); //��������

	app->RegisterAnimator(animator); //ע�ᶯ��

	app->MakeWindow(windowName, width, height); //��������
	app->ShowWindow(); //��ʾ����
	app->RunLoop(); //��ѭ��

	for (int i = 0; i < objectCount*objectCount; i++) {
		Utility::Delete(subObject[i]);
		Utility::Delete(animation[i]);
	}

	Utility::Delete(rotateAnimation);
	Utility::Delete(animator);
	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(app);
}