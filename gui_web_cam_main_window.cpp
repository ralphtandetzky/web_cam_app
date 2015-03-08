#include "gui_web_cam_main_window.h"
#include "ui_gui_web_cam_main_window.h"

#include "../qt_utils/event_filter.h"
#include "../qt_utils/invoke_in_thread.h"
#include "../qt_utils/loop_thread.h"

#include "../cpp_utils/exception.h"

#include <cassert>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QPointer>
#include <QtMultimedia/QCameraInfo>

namespace gui {

struct WebCamMainWindow::Impl
{
    Ui::WebCamMainWindow ui;
    std::unique_ptr<QCamera> webCam;
    QGraphicsScene scene;
    QGraphicsVideoItem videoItem;
    qu::LoopThread worker;

    // Makes sure the scene is always shown as big as possible.
    void makeSceneFitToView();

    // Puts a video display into the scene.
    void connectWebCamToVideoItemAsync();
};


WebCamMainWindow::WebCamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m(new Impl)
{
    m->ui.setupUi(this);
    m->connectWebCamToVideoItemAsync();

    // put mirrored item into a QGraphicsScene.
    m->videoItem.setMatrix( QMatrix(-1,0,0,1,0,0),true );
    m->scene.addItem( &m->videoItem );

    m->makeSceneFitToView();

    // put scene into graphicsView.
    m->ui.graphicsView->setScene(&m->scene);
}


WebCamMainWindow::~WebCamMainWindow()
{
}


void WebCamMainWindow::Impl::makeSceneFitToView()
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


void WebCamMainWindow::Impl::connectWebCamToVideoItemAsync()
{
    // switch to worker thread.
    qu::invokeInThread( &worker, [this](){

    const auto cams = QCameraInfo::availableCameras();
    const auto nCams = cams.count();

    CU_ENFORCE( nCams > 0, "No cameras could be detected." )

    // initialize camera.
    webCam.reset( new QCamera(cams.front()) );
    webCam->moveToThread( QCoreApplication::instance()->thread() );

    // switch back to gui thread
    qu::invokeInGuiThread( [this,nCams](){

    // set QGraphicsItem as view finder and start capturing images.
    webCam->setViewfinder( &videoItem );
    webCam->start();

    } ); // end of gui thread execution
    } ); // end of worker thread execution
}


} // namespace gui
