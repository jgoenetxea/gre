#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QTimer>

#include "scenes/movingCube.h"

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);
    ~GLView();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void setAnimationFrameRate( const unsigned int fps );

private:
    QTimer* m_fps_timer;
    unsigned int m_fps;

    MovingCube m_mc;

protected slots:
    void sltTimerEvent();
};

#endif // GLVIEW_H
