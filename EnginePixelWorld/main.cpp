#include <Application.hpp>
#include <PixelWorld.hpp>

#include <cstring>

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string pixelWorldName = u8"PixelWorld";
const std::string windowName = u8"PixelWorld";
const std::string worldMapName = u8"WorldMap";

const int width = 800;
const int height = 600;

const int mapWidth = 15;
const int mapHeight = 15;

Application* app = new Application(applicationName); //����Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //��������
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //������ͼ

DataManager* dataManager = new DataManager(app); //�������ݹ�����
TextureManager* textureManager = new TextureManager(app); //�������������

Camera camera = Camera(RectangleF(0, 0, width, height)); //���������

MergeTexture* mergeTexture = new MergeTexture(app, 512, 256, Graphics::PixelFormat::R8G8B8A8); //�����ϲ�����

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

	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //���ú������
	world->SetWorldMap(worldMap); //���úõ�ͼ

	world->SetTextureManager(textureManager); //���ú����������
}

int main() {

	MakeTextureManager(); //���������������
	
	MakeWorld(); //��������
	
	app->SetWorld(world); //��������

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