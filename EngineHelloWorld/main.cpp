#include <Application.hpp>

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string windowName = u8"Hello, World!";

Application* app = new Application(applicationName); //����Ӧ�ó���

/**
 * @brief ��갴ť�¼�
 * @param[in] sender Application
 * @param[in] eventArg �¼�����
 */
void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {

	if (eventArg->isDown == true) {
		app->SetWindow(windowName + u8"    MouseButton was down", 800, 600);
	}
	else app->SetWindow(windowName + u8"    MouseButton was up", 800, 600);

}

int main() {

	app->MakeWindow(windowName, 800, 600); //����һ������ΪHello, World�����Ϊ800���߶�Ϊ600�Ĵ���
	app->ShowWindow(); //��ʾ����

	app->MouseClick.push_back(OnMouseClick); //������갴���¼�

	app->RunLoop(); //������ѭ��

	Utility::Delete(app); //�ͷ���Դ
}