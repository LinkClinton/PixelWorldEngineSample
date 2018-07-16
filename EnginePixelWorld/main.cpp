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

Application* app = new Application(applicationName); //构建Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //构建世界
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //构建地图

DataManager* dataManager = new DataManager(app); //构建数据管理器
TextureManager* textureManager = new TextureManager(app); //构建纹理管理器

Camera camera = Camera(RectangleF(0, 0, width, height)); //构建摄像机

MergeTexture* mergeTexture = new MergeTexture(app, 512, 256, Graphics::PixelFormat::R8G8B8A8); //构建合并纹理

/**
 * @brief 构建好纹理管理器
 */
void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //读取纹理

	int renderObjectID = 0; //渲染编号，即将纹理映射成整数，方便我们使用

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //创建纹理，从原有纹理中复制出一部分出来

			mergeTexture->AddTexture(++renderObjectID, j * 64, i * 64, subTexture); //将这一块纹理添加到合并纹理中去，并为其设置好渲染编号

			Utility::Delete(subTexture); //释放资源
		}
	}

	textureManager->AddMergeTexture(0, mergeTexture); //添加合并纹理到纹理管理器
}

/**
 * @brief 构建世界
 */
void MakeWorld() {
	
	//将地图使用渲染编号为18的纹理覆盖
	for (int x = 0; x < mapWidth; x++) {//地图宽度
		for (int y = 0; y < mapHeight; y++) {//地图高度
			auto data = new MapData(); //创建地图块数据，记住最后释放资源

			data->RenderObjectID = 18; //设置这一块地图渲染的时候使用的纹理

			worldMap->SetMapData(x, y, data); //设置好地图数据
		}
	}

	world->SetResolution(width, height); //设置分辨率

	world->SetCamera(&camera); //设置好摄像机
	world->SetWorldMap(worldMap); //设置好地图

	world->SetTextureManager(textureManager); //设置好纹理管理器
}

int main() {

	MakeTextureManager(); //构建好纹理管理器
	
	MakeWorld(); //构建世界
	
	app->SetWorld(world); //设置世界

	app->MakeWindow(windowName, width, height); //创建窗口
	app->ShowWindow(); //显示窗口
	app->RunLoop(); //主循环

	//释放资源
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