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

Application* app = new Application(applicationName); //构建Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //构建世界
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //构建地图

DataManager* dataManager = new DataManager(app); //构建数据管理器
TextureManager* textureManager = new TextureManager(app, Graphics::PixelFormat::R8G8B8A8); //构建纹理管理器

Camera camera = Camera(RectangleF(0, 0, cameraWidth, cameraHeight)); //构建摄像机

GameObject* player; //操控的物体
GameObject* object[objectCount]; //世界中的物体

float cameraTimes = 1.0f;
float cameraSpeed = 1.0f;

void OnPlayerUpdate(void* sender) {
	
	auto deltaTime = app->GetDeltaTime(); //获取经过的时间

	glm::vec2 translate = glm::vec2(0, 0); //位移

	float speed = 200; //速度

					   //根据WSAD移动
	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;
	
	//位移
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		player->Transform.SetPosition(player->Transform.GetPosition() + translate);
	}
}

void OnPlayerAfterUpdate(void* sender) {
	auto deltaTime = app->GetDeltaTime(); //获取经过的时间

	//移动摄像机，将物体作为中心点
	camera.SetFocus(player->Transform.GetPosition().x + player->GetSize().width * 0.5f,
		player->Transform.GetPosition().y + player->GetSize().height * 0.5f,
		RectangleF(cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes, cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes));
}

/**
 * @brief 构建好纹理管理器
 */
void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //读取纹理

	int renderObjectID = 0; //渲染编号，即将纹理映射成整数，方便我们使用

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //创建纹理，从原有纹理中复制出一部分出来

			textureManager->RegisterTexture(++renderObjectID, subTexture); //将这一块纹理添加到纹理管理器中去，并为其设置好渲染编号

			Utility::Delete(subTexture); //释放资源
		}
	}
}

/**
* @brief 构建世界
*/
void MakeWorld() {

	//将地图使用渲染编号为18的纹理覆盖
	for (int x = 0; x < mapWidth; x++) {//地图宽度
		for (int y = 0; y < mapHeight; y++) {//地图高度
			auto data = MapData(); //创建地图块数据

			data.RenderObjectID = 18; //设置这一块地图渲染的时候使用的纹理

			worldMap->SetMapData(x, y, data); //设置好地图数据
		}
	}

	worldMap->SetMapBlockSize(48); //设置地图格子

	world->SetResolution(width, height, 2); //设置分辨率

	world->SetCamera(&camera); //设置好摄像机
	world->SetWorldMap(worldMap); //设置好地图

	world->SetTextureManager(textureManager); //设置好纹理管理器

}

void MakeObject() {

	float mapCenterX = worldMap->GetMapBlockSize() * worldMap->GetWidth() * 0.5f; //地图中心点X坐标
	float mapCenterY = worldMap->GetMapBlockSize() * worldMap->GetHeight() * 0.5f; //地图中心点Y坐标

	player = new GameObject(u8"Player"); //玩家

	player->SetSize(64, 64); //设置玩家操控块大小
	player->Transform.SetPosition(glm::vec2(mapCenterX - player->GetSize().width * 0.5f, mapCenterY - player->GetSize().height * 0.5f)); //设置位置为中心
	player->RenderObjectID = 10; //设置渲染编号
	player->IsEnablePhysicsCollide = true;

	player->Update.push_back(OnPlayerUpdate);
	player->AfterUpdate.push_back(OnPlayerAfterUpdate);

	world->SetPixelObject(player, Layer::World); //注册玩家
	
	float objectSize = 48; //物体大小

	float distanceX = objectSize * 2; //距离玩家距离X分量
	float distanceY = objectSize * 2; //距离玩家距离Y分量

	float Left = mapCenterX - distanceX - objectSize; //左上角块X位置
	float Top = mapCenterY - distanceY - objectSize; //左上角块Y位置

	int id = 0; //编号

	for (int xPosition = 0; xPosition < 6; xPosition++) { //以此构建物体
		for (int yPosition = 0; yPosition < 6; yPosition++) {
			if (xPosition != 0 && yPosition != 0 && xPosition != 5 && yPosition != 5) //如果处于实心位置，不构造
				continue;

			object[id] = new GameObject(); //匿名构造物体

			object[id]->SetSize(objectSize, objectSize); //设置大小
			object[id]->Transform.SetPosition(glm::vec2(Left + xPosition * objectSize + xPosition, Top + yPosition * objectSize + yPosition)); //设置位置
			object[id]->RenderObjectID = 10; //设置渲染编号

			world->SetPixelObject(object[id], Layer::World); //注册物体

			id++;
		}
	}
}

void OnUpdate(void* sender) {

	auto deltaTime = app->GetDeltaTime(); //获取经过的时间

	app->SetWindow(windowName + " FPS: " + Utility::ToString(app->GetFramePerSecond()), width, height);	

	if (Input::GetKeyCodeDown(KeyCode::Up) == true)
		cameraTimes += deltaTime * cameraSpeed;
	if (Input::GetKeyCodeDown(KeyCode::Down) == true)
		cameraTimes -= deltaTime * cameraSpeed;

	cameraTimes = Utility::Limit(cameraTimes, 0.5f, 2.0f);
}

int main() {

	MakeTextureManager(); //构建好纹理管理器

	MakeWorld(); //构建世界

	MakeObject(); //构建物体

	app->SetWorld(world); //设置世界

	app->Update.push_back(OnUpdate); //添加更新事件

	app->MakeWindow(windowName, width, height); //创建窗口
	app->ShowWindow(); //显示窗口
	app->RunLoop(); //主循环

	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(app);
}