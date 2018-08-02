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


Application* app = new Application(applicationName); //����Ӧ�ó���
PixelWorld* world = new PixelWorld(worldName, app); //��������
DataManager* dataManager = new DataManager(app); //�������ݹ�����

TextureManager* textureManager = new TextureManager(app, PixelFormat::R8G8B8A8);

Font* font = nullptr;

PixelObject* object = nullptr;

Camera camera = Camera(PixelWorldEngine::RectangleF(0, 0, width, height)); //�����

/**
 * @brief ������Դ
 */
void MakeResource() {

	font = dataManager->RegisterFont("Consola.ttf", fontName, 32); //��ȡ�����ļ�
	
}

/**
 * @brief ��������
 */
void MakeObject() {
	object = new PixelObject("UIObject");  //����UI����
	
	float centerX = width * 0.5f; //��������λ��
	float centerY = height * 0.5f;

	int size = 300;

	object->SetSize((float)size, (float)size); //���ô�С���ı�����һ����С
	object->Transform.SetPosition(glm::vec2(centerX, centerY)); //����λ��Ϊ����
	object->Transform.SetRotate(-glm::pi<float>() * 0.25f); //������ת�Ƕ�
	
	object->RenderObjectID = 0; //����Ⱦ�������Ϊ�ı������

	object->TextColor = glm::vec3(1, 0, 0); //�����ı���ɫ��Ĭ��Ϊ(0, 0, 0)
	object->Font = font;
	object->Text = u8"This is an example about render Text!";

	world->SetPixelObject(object, Layer::UI); //ע��UI����

}

/**
 * @brief ��������
 */
void MakeWorld() {
	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera);

	world->SetTextureManager(textureManager);

	app->SetWorld(world); //�������Ӧ�ó������
}

/**
 * @brief ������Դ�ͷ��ڴ�
 */
void DestoryResource() {
	dataManager->UnRegisterFont(fontName); //����Ҫ����ж��

	Utility::Delete(object);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(app);
}

int main() {
	MakeResource(); //������Դ
	
	MakeObject(); //��������
	
	MakeWorld(); //��������

	//�������ڲ���ʾ
	app->MakeWindow(windowName, width, height);
	app->ShowWindow();

	app->RunLoop(); //��ѭ��
}