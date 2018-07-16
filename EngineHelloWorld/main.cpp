#include <Application.hpp>

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string windowName = u8"Hello, World!";

Application* app = new Application(applicationName); //构建应用程序

/**
 * @brief 鼠标按钮事件
 * @param[in] sender Application
 * @param[in] eventArg 事件参数
 */
void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {

	if (eventArg->isDown == true) {
		app->SetWindow(windowName + u8"    MouseButton was down", 800, 600);
	}
	else app->SetWindow(windowName + u8"    MouseButton was up", 800, 600);

}

int main() {

	app->MakeWindow(windowName, 800, 600); //创建一个标题为Hello, World，宽度为800，高度为600的窗口
	app->ShowWindow(); //显示窗口

	app->MouseClick.push_back(OnMouseClick); //加入鼠标按下事件

	app->RunLoop(); //进入主循环

	Utility::Delete(app); //释放资源
}