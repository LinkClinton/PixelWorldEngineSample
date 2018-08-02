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

Application* app = new Application(applicationName); //构建Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //构建世界
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //构建地图

DataManager* dataManager = new DataManager(app); //构建数据管理器
TextureManager* textureManager = new TextureManager(app, Graphics::PixelFormat::R8G8B8A8); //构建纹理管理器

Camera camera = Camera(RectangleF(0, 0, width, height)); //构建摄像机

PixelObject* subObject[objectCount * objectCount]; //物体
Animation* animation[objectCount * objectCount]; //动画
Animation* rotateAnimation; //旋转动画
Animator* animator = new Animator("Animator"); //动画管理类型

int Rand(int l, int r) { //随机函数
	return rand() % (r - l + 1) + l;
}

int Rand(int x) { //随机函数
	return Rand(0, x);
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
 * @brief 构建好物体
 */
void MakeObject() {

	float startX = width * 0.5f - objectCount * objectSize * 0.5f + objectSize * 0.5f; //计算起始位置
	float startY = height * 0.5f - objectCount * objectSize * 0.5f + objectSize * 0.5f;

	int id = 0;
	
	float rotateSpeed = glm::pi<float>() * 2; //旋转速度

	rotateAnimation = new RotateAnimation("RotateAnimation");

	rotateAnimation->SetKeyFrame(0, 0);
	rotateAnimation->SetKeyFrame(5 * rotateSpeed, 5);
	rotateAnimation->SetKeyFrame(5 * rotateSpeed, 8);
	rotateAnimation->SetKeyFrame(0, 13);
	rotateAnimation->Sort(); //构建旋转动画

	for (int x = 0; x < objectCount; x++) {
		for (int y = 0; y < objectCount; y++) {
			//构建物体
			subObject[id] = new PixelObject();
			animation[id] = new MoveAnimation("MoveAnimation" + Utility::ToString(id));

			subObject[id]->SetSize(objectSize, objectSize);
			subObject[id]->Transform.SetPosition(glm::vec2(startX + x * objectSize, startY + y * objectSize));

			subObject[id]->RenderObjectID = 2;

			world->SetPixelObject(subObject[id], Layer::UI);

			int startXpos = Rand(width);
			int startYpos = Rand(height);
			//构建组合动画
			animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 0);
			animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 5);
			animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 8);
			animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 13);
			animation[id]->Sort();

			//添加动画
			animator->AddAnimation(subObject[id], MoveAnimation::MoveSetFunction, animation[id]);
			animator->AddAnimation(subObject[id], RotateAnimation::RotateSetFunction, rotateAnimation);

			id++;
		}
	}
	//允许重复动画，以及运行动画
	animator->EnableRepeat(true);
	animator->Run();
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

	world->SetResolution(width, height); //设置分辨率

	world->SetCamera(&camera); //设置好摄像机
	world->SetWorldMap(worldMap); //设置好地图

	world->SetTextureManager(textureManager); //设置好纹理管理器
}

int main() {
	srand((unsigned int)time(0));

	MakeTextureManager(); //构建好纹理管理器

	MakeWorld(); //构建世界

	MakeObject(); //构建物体

	app->SetWorld(world); //设置世界

	app->RegisterAnimator(animator); //注册动画

	app->MakeWindow(windowName, width, height); //创建窗口
	app->ShowWindow(); //显示窗口
	app->RunLoop(); //主循环

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