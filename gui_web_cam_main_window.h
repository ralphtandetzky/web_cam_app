#pragma once

#include <QMainWindow>
#include <memory>

namespace gui {

class WebCamMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WebCamMainWindow(QWidget *parent = 0);
    ~WebCamMainWindow();

private:
    struct Impl;
    std::unique_ptr<Impl> m;
};


} // namespace gui
