#include "gui_web_cam_main_window.h"
#include "ui_gui_web_cam_main_window.h"

#include "../qt_utils/event_filter.h"
#include "../qt_utils/invoke_in_thread.h"
#include "../qt_utils/loop_thread.h"

#include <cassert>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QPointer>
#include <QtMultimedia/QCameraInfo>

namespace gui {

struct WebCamMainWindow::Impl
{
    Ui::WebCamMainWindow ui;
    std::unique_ptr<QCamera> cam;
    QGraphicsScene scene;
    qu::LoopThread worker;

    // Makes sure the scene is always shown as big as possible.
    void makeVideoFitToView();

    // Puts a video display into the scene.
    void putVideoItemIntoScene();
};


WebCamMainWindow::WebCamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m(new Impl)
{
    m->ui.setupUi(this);
    m->makeVideoFitToView();
    m->putVideoItemIntoScene();
    m->ui.graphicsView->setScene(&m->scene);
}


WebCamMainWindow::~WebCamMainWindow()
{
}


void WebCamMainWindow::Impl::makeVideoFitToView()
{
    auto view = QPointer<QGraphicsView>(ui.graphicsView);
    qu::installEventFilter( ui.graphicsView,
                            [view]( QObject *, QEvent * event )
    {
        if ( event->type() == QEvent::Paint
             && view->scene() )
        {
            view->fitInView( view->sceneRect(),
                             Qt::KeepAspectRatio );
        }

        return false;
    });
}


void WebCamMainWindow::Impl::putVideoItemIntoScene()
{
    // switch to worker thread.
    qu::invokeInThread( &worker, [this](){

    const auto cams = QCameraInfo::availableCameras();
    const auto nCams = cams.count();

    // show number of cameras in statusbar.
    qu::invokeInGuiThread( [this,nCams](){
        ui.statusbar->showMessage(
                    QString("Found %1 camera(s).").arg(nCams) );
    } );

    if ( nCams > 0 )
    {
        // initialize camera.
        cam.reset( new QCamera(cams.front()) );
        cam->moveToThread( QCoreApplication::instance()->thread() );

        // switch to gui thread
        qu::invokeInGuiThread( [this,nCams](){

        // set QGraphicsItem as view finder and start capturing images.
        auto item = std::make_unique<QGraphicsVideoItem>();
        cam->setViewfinder( item.get() );
        cam->start();

        // put mirrored item into a QGraphicsScene.
        item->setMatrix( QMatrix(-1,0,0,1,0,0),true );
        scene.addItem(item.get());
        item.release();

        } ); // end of gui thread execution
    }
    } ); // end of worker thread execution
}


} // namespace gui
