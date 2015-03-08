#include "gui_web_cam_main_window.h"
#include "ui_gui_web_cam_main_window.h"

#include "../qt_utils/event_filter.h"
#include "../qt_utils/event_handling_graphics_item.h"
#include "../qt_utils/invoke_in_thread.h"
#include "../qt_utils/loop_thread.h"

#include "../cpp_utils/exception.h"

#include <cassert>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
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
    qu::EventHandlingGraphicsItem<QGraphicsRectItem> rect;
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

    m->rect.setParentItem( &m->videoItem );
    const auto updateRectSize = [this]()
    {
        const QRectF parentRect = m->rect.parentItem()->boundingRect();

        const auto rectX = parentRect.center().x();
        const auto rectY = parentRect.center().y() + 0.4*parentRect.height();
        const auto rectSize = std::min(
            parentRect.width()*2,
            parentRect.height()) * 0.15;
        m->rect.setRect(
            rectX-rectSize*0.8,
            rectY-rectSize*0.5,
            rectSize*1.6,
            rectSize );
    };
    connect( &m->videoItem, &QGraphicsVideoItem::nativeSizeChanged,
             this, updateRectSize );

    m->connectWebCamToVideoItemAsync();

    // put mirrored item into a QGraphicsScene.
    m->videoItem.setMatrix( QMatrix(-1,0,0,1,0,0),true );
    m->scene.addItem( &m->videoItem );

    m->makeSceneFitToView();

    // put scene into graphicsView.
    m->ui.graphicsView->setScene(&m->scene);

    m->rect.setFlag( QGraphicsItem::ItemIsSelectable );
    m->rect.setMousePressEventHandler(
        [this]( qu::EventHandlingGraphicsItem<QGraphicsRectItem> *,
                QGraphicsSceneMouseEvent * event)
    {
        if ( event->button() != Qt::LeftButton )
            return;

        m->videoItem.setMatrix( QMatrix(-1,0,0,1,m->videoItem.boundingRect().width(),0),true );
    });
}


WebCamMainWindow::~WebCamMainWindow()
{
}


void WebCamMainWindow::Impl::makeSceneFitToView()
{
    auto view = QPointer<QGraphicsView>(ui.graphicsView);
    qu::installEventFilter( ui.graphicsView,
                            [view,this]( QObject *, QEvent * event )
    {
        if ( event->type() == QEvent::Paint
             && !view.isNull()
             && view->scene() )
        {
            view->fitInView( videoItem.boundingRect(),
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
