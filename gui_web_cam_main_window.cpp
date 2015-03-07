#include "gui_web_cam_main_window.h"
#include "ui_gui_web_cam_main_window.h"

#include <QtMultimedia/QCameraInfo>
#include <QtMultimediaWidgets/QCameraViewfinder>

namespace gui {

struct WebCamMainWindow::Impl
{
    Ui::WebCamMainWindow ui;
    std::unique_ptr<QCamera> cam;
};

WebCamMainWindow::WebCamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m(new Impl)
{
    m->ui.setupUi(this);
    const auto cams = QCameraInfo::availableCameras();
    m->ui.statusbar->showMessage(
                QString("%1 camera(s) found.").arg(cams.count()) );
    if ( !cams.empty() )
    {
        m->cam.reset( new QCamera(cams.front()) );
        m->cam->setViewfinder( m->ui.viewFinder );
        m->cam->start();
        m->cam->exposure()->setExposureMode(
                    QCameraExposure::ExposureAuto );
    }

}

WebCamMainWindow::~WebCamMainWindow()
{
}

} // namespace gui
