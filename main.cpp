#include "gui_web_cam_main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gui::WebCamMainWindow w;
    w.show();

    return a.exec();
}
