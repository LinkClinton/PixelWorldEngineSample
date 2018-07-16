#include <Application.hpp>
#include <PixelWorld.hpp>
#include <Input.hpp>

#include <cstring>

#include "GameObject.hpp"

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string pixelWorldName = u8"PixelWorld";
const std::string windowName = u8"PixelWorld";
const std::string worldMapName = u8"WorldMap";

const int width = 1280;
const int height = 720;

const int mapWidth = 100;
const int mapHeight = 100;

const int cameraWidth = 1280;
const int cameraHeight = 720;

const int objectCount = 20;

Application* app = new Application(applicationName); //����Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //��������
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //������ͼ

DataManager* dataManager = new DataManager(app); //�������ݹ�����
TextureManager* textureManager = new TextureManager(app); //�������������

Camera camera = Camera(RectangleF(0, 0, cameraWidth, cameraHeight)); //���������

MergeTexture* mergeTexture = new MergeTexture(app, 512, 256, Graphics::PixelFormat::R8G8B8A8); //�����ϲ�����

GameObject* player; //�ٿص�����
GameObject* object[objectCount]; //�����е�����

float cameraTimes = 1.0f;
float cameraSpeed = 1.0f;

/**
 * @brief ���������������
 */
void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //��ȡ����

	int renderObjectID = 0; //��Ⱦ��ţ���������ӳ�����������������ʹ��

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //����������ԭ�������и��Ƴ�һ���ֳ���

			mergeTexture->AddTexture(++renderObjectID, j * 64, i * 64, subTexture); //����һ��������ӵ��ϲ�������ȥ����Ϊ�����ú���Ⱦ���

			Utility::Delete(subTexture); //�ͷ���Դ
		}
	}

	textureManager->AddMergeTexture(0, mergeTexture); //��Ӻϲ��������������
}

/**
* @brief ��������
*/
void MakeWorld() {

	//����ͼʹ����Ⱦ���Ϊ18��������
	for (int x = 0; x < mapWidth; x++) {//��ͼ���
		for (int y = 0; y < mapHeight; y++) {//��ͼ�߶�
			auto data = new MapData(); //������ͼ�����ݣ���ס����ͷ���Դ

			data->RenderObjectID = 18; //������һ���ͼ��Ⱦ��ʱ��ʹ�õ�����

			worldMap->SetMapData(x, y, data); //���úõ�ͼ����
		}
	}

	worldMap->SetMapBlockSize(48); //���õ�ͼ����

	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //���ú������
	world->SetWorldMap(worldMap); //���úõ�ͼ

	world->SetTextureManager(textureManager); //���ú����������
}

void MakeObject() {

	float mapCenterX = worldMap->GetMapBlockSize() * worldMap->GetWidth() * 0.5f; //��ͼ���ĵ�X����
	float mapCenterY = worldMap->GetMapBlockSize() * worldMap->GetHeight() * 0.5f; //��ͼ���ĵ�Y����

	player = new GameObject(u8"Player"); //���

	player->SetSize(64, 64); //������Ҳٿؿ��С
	player->SetPosition(mapCenterX - player->GetWidth() * 0.5f, mapCenterY - player->GetHeight() * 0.5f); //����λ��Ϊ����
	player->SetRenderObjectID(10); //������Ⱦ���

	world->RegisterPixelObject(player); //ע�����
	
	float objectSize = 48; //�����С

	float distanceX = objectSize * 2; //������Ҿ���X����
	float distanceY = objectSize * 2; //������Ҿ���Y����

	float Left = mapCenterX - distanceX - objectSize; //���Ͻǿ�Xλ��
	float Top = mapCenterY - distanceY - objectSize; //���Ͻǿ�Yλ��

	int id = 0; //���

	for (int xPosition = 0; xPosition < 6; xPosition++) { //�Դ˹�������
		for (int yPosition = 0; yPosition < 6; yPosition++) {
			if (xPosition != 0 && yPosition != 0 && xPosition != 5 && yPosition != 5) //�������ʵ��λ�ã�������
				continue;

			object[id] = new GameObject("Object" + Utility::ToString(id)); //��������

			object[id]->SetSize(objectSize, objectSize); //���ô�С
			object[id]->SetPosition(Left + xPosition * objectSize, Top + yPosition * objectSize); //����λ��
			object[id]->SetRenderObjectID(10); //������Ⱦ���

			world->RegisterPixelObject(object[id]); //ע������

			id++;
		}
	}
}

void OnUpdate(void* sender) {

	auto deltaTime = app->GetDeltaTime(); //��ȡ������ʱ��

	glm::vec2 translate = glm::vec2(0, 0); //λ��

	float speed = 200; //�ٶ�

	//����WSAD�ƶ�
	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;
	if (Input::GetKeyCodeDown(KeyCode::Up) == true)
		cameraTimes += deltaTime * cameraSpeed;
	if (Input::GetKeyCodeDown(KeyCode::Down) == true)
		cameraTimes -= deltaTime * cameraSpeed;

	cameraTimes = Utility::Limit(cameraTimes, 0.5f, 2.0f);

	//λ��
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		player->SetNeedTranslate(translate);
		player->Move(translate.x, translate.y);
	}

	//�ƶ����������������Ϊ���ĵ�
	camera.SetFocus(player->GetPositionX() + player->GetWidth() * 0.5f,
		player->GetPositionY() + player->GetHeight() * 0.5f,
		RectangleF(cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes, cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes));
}

int main() {

	MakeTextureManager(); //���������������

	MakeWorld(); //��������

	MakeObject(); //��������

	app->SetWorld(world); //��������

	app->Update.push_back(OnUpdate); //��Ӹ����¼�

	app->MakeWindow(windowName, width, height); //��������
	app->ShowWindow(); //��ʾ����
	app->RunLoop(); //��ѭ��

	//�ͷ���Դ
	for (int x = 0; x < mapWidth; x++)
		for (int y = 0; y > mapHeight; y++) {
			auto data = worldMap->GetMapData(x, y);

			Utility::Delete(data);
		}

	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(mergeTexture);
	Utility::Delete(app);
}