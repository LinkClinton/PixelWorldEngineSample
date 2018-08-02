#include <Application.hpp>
#include <Text.hpp>

using namespace PixelWorldEngine;
using namespace Graphics;

const std::string applicationName = u8"Application";
const std::string worldName = u8"PixelWorld";
const std::string windowName = u8"Text";

const std::string fontName = u8"Consola";

const int width = 1280;
const int height = 720;


Application* app = new Application(applicationName); //构建应用程序
PixelWorld* world = new PixelWorld(worldName, app); //构建世界
DataManager* dataManager = new DataManager(app); //构建数据管理器

TextureManager* textureManager = new TextureManager(app, PixelFormat::R8G8B8A8);

Font* font = nullptr;

PixelObject* object = nullptr;

Camera camera = Camera(PixelWorldEngine::RectangleF(0, 0, width, height)); //摄像机

/**
 * @brief 构建资源
 */
void MakeResource() {

	font = dataManager->RegisterFont("Consola.ttf", fontName, 32); //读取字体文件
	
}

/**
 * @brief 构建物体
 */
void MakeObject() {
	object = new PixelObject("UIObject");  //构建UI物体
	
	float centerX = width * 0.5f; //计算中心位置
	float centerY = height * 0.5f;

	int size = 300;

	object->SetSize((float)size, (float)size); //设置大小和文本纹理一样大小
	object->Transform.SetPosition(glm::vec2(centerX, centerY)); //设置位置为中心
	object->Transform.SetRotate(-glm::pi<float>() * 0.25f); //设置旋转角度
	
	object->RenderObjectID = 0; //将渲染编号设置为文本纹理的

	object->TextColor = glm::vec3(1, 0, 0); //设置文本颜色，默认为(0, 0, 0)
	object->Font = font;
	object->Text = u8"This is an example about render Text!";

	world->SetPixelObject(object, Layer::UI); //注册UI物体

}

/**
 * @brief 构建世界
 */
void MakeWorld() {
	world->SetResolution(width, height); //设置分辨率

	world->SetCamera(&camera);

	world->SetTextureManager(textureManager);

	app->SetWorld(world); //将世界和应用程序关联
}

/**
 * @brief 销毁资源释放内存
 */
void DestoryResource() {
	dataManager->UnRegisterFont(fontName); //字体要进行卸载

	Utility::Delete(object);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(app);
}

int main() {
	MakeResource(); //构建资源
	
	MakeObject(); //构建物体
	
	MakeWorld(); //构建世界

	//构建窗口并显示
	app->MakeWindow(windowName, width, height);
	app->ShowWindow();

	app->RunLoop(); //主循环
}