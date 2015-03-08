#include "gui_web_cam_main_window.h"
#include "ui_gui_web_cam_main_window.h"

#include "../qt_utils/invoke_in_thread.h"
#include "../qt_utils/loop_thread.h"

#include <cassert>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QtMultimedia/QCameraInfo>

namespace gui {

struct WebCamMainWindow::Impl
{
    Ui::WebCamMainWindow ui;
    std::unique_ptr<QCamera> cam;
    QGraphicsScene scene;

    qu::LoopThread worker;
};

WebCamMainWindow::WebCamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m(new Impl)
{
    m->ui.setupUi(this);

    // switch to worker thread.
    qu::invokeInThread( &m->worker, [this](){

    const auto cams = QCameraInfo::availableCameras();
    const auto nCams = cams.count();

    // show number of cameras in statusbar.
    qu::invokeInGuiThread( [this,nCams](){
        m->ui.statusbar->showMessage(
                    QString("Found %1 camera(s).").arg(nCams) );
    } );

    if ( nCams > 0 )
    {
        // initialize camera.
        m->cam.reset( new QCamera(cams.front()) );
        m->cam->moveToThread( QCoreApplication::instance()->thread() );

        // switch to gui thread
        qu::invokeInGuiThread( [this,nCams](){

        // set QGraphicsItem as view finder and start capturing images.
        auto item = std::make_unique<QGraphicsVideoItem>();
        m->cam->setViewfinder( item.get() );
        m->cam->start();

        // put mirrored item into a QGraphicsScene.
        item->setMatrix( QMatrix(-1,0,0,1,0,0),true );
        m->scene.addItem(item.get());
        item.release();
        m->ui.graphicsView->setScene(&m->scene);

        } ); // end of gui thread execution
    }
    } ); // end of worker thread execution
}

WebCamMainWindow::~WebCamMainWindow()
{
}

} // namespace gui
