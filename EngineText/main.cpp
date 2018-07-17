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

const int textWidth = 200;
const int textHeight = 0;

Application* app = new Application(applicationName); //����Ӧ�ó���
PixelWorld* world = new PixelWorld(worldName, app); //��������
DataManager* dataManager = new DataManager(app); //�������ݹ�����
TextureManager* textureManager = new TextureManager(app); //�������������
MergeTexture* mergeTexture = new MergeTexture(app, 400, 400, PixelFormat::A8); //�����ϲ�������Ϊ�洢������ͼ

Font* font = nullptr;
Text* text = nullptr;

UIObject* object = nullptr;
UIObject* objectParent = nullptr;

Camera camera = Camera(PixelWorldEngine::RectangleF(0, 0, width, height)); //�����

/**
 * @brief ������Դ
 */
void MakeResource() {

	font = dataManager->RegisterFont("Consola.ttf", fontName, 32); //��ȡ�����ļ�
	
	text = new Text(u8"This is an example about render Text!", font, textWidth, textHeight); //�������������߶�����Ϊ0�Ļ���ʾ��ϵͳ����

	auto texture = text->GetTexture(); //��ȡ��������Ĺ�����Ҫ����

	mergeTexture->AddTexture(1, 0, 0, texture); //���������ϲ�������ȥ

	textureManager->AddMergeTexture(0, mergeTexture); //��Ӻϲ��������������
}

/**
 * @brief ��������
 */
void MakeObject() {
	object = new UIObject("UIObject");  //����UI����
	objectParent = new UIObject("UIObjectParent");

	float centerX = width * 0.5f; //��������λ��
	float centerY = height * 0.5f;

	objectParent->SetSize((float)text->GetWidth() * 2, (float)text->GetWidth() * 2); //���ø�������Ĵ�СΪ�ı���ȵ�����
	objectParent->SetPosition(centerX - objectParent->GetWidth() * 0.5f, centerY - objectParent->GetHeight() * 0.5f); //���ø��������λ��Ϊ����
	objectParent->SetAngle(glm::pi<float>() * 0.25f); //������ת�Ƕȣ���ת����Ϊ�������ģ�������������Ӱ��

	objectParent->SetRenderObjectID(0); //���ڲ���Ⱦ�����������Ϊ0
	objectParent->SetOpacity(0.7f); //���ò�͸���ȣ�������������Ӱ��

	objectParent->SetBorderWidth(1); //���ñ߿���
	objectParent->SetBorderColor(0, 1, 0); //���ñ߿���ɫ
	

	object->SetSize((float)text->GetWidth(), (float)text->GetHeight()); //���ô�С���ı�����һ����С
	object->SetPosition(objectParent->GetWidth() * 0.5f - object->GetWidth() * 0.5f, objectParent->GetHeight() * 0.5f - object->GetHeight() * 0.5f); //����λ��Ϊ����
	object->SetAngle(-glm::pi<float>() * 0.25f); //������ת�Ƕ�
	
	object->SetRenderObjectID(1); //����Ⱦ�������Ϊ�ı������
	object->SetEffectColor(1, 0, 0); //�����ı���ɫ��Ĭ��Ϊ(1, 1, 1)���䱾���Ƕ�������ɫ����һ���˷�
	
	object->SetBorderWidth(1); //���ñ߿��ȣ�Ĭ��Ϊ0
	object->SetBorderColor(1, 0, 0); //���ñ߿���ɫ



	objectParent->RegisterUIObject(object); //ע��������

	world->RegisterUIObject(objectParent); //ע��UI����

}

/**
 * @brief ��������
 */
void MakeWorld() {
	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //������������������������Ӱ�쵽UI����
	
	world->SetTextureManager(textureManager); //�������������

	app->SetWorld(world); //�������Ӧ�ó������
}

/**
 * @brief ������Դ�ͷ��ڴ�
 */
void DestoryResource() {
	dataManager->UnRegisterFont(fontName); //����Ҫ����ж��

	Utility::Delete(text);
	Utility::Delete(object);
	Utility::Delete(objectParent);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(mergeTexture);
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