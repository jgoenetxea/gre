#include "glView.h"

#include <iostream>

GLView::GLView(QWidget *parent) :
    QGLWidget(parent)
{
    m_fps_timer = NULL;
}

GLView::~GLView()
{
}

void GLView::initializeGL()
{
    setAnimationFrameRate( 60 );

    m_mc.initialize();
    m_sq.initialize();
}

void GLView::paintGL()
{
    //m_mc.paint();
    m_sq.paint();
}

void GLView::resizeGL(int width, int height)
{
    //int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);

    glViewport(0, 0, (GLint)width, (GLint)height);
}

void GLView::setAnimationFrameRate( const unsigned int fps )
{
    if( fps == 0 )
    {
        if( m_fps_timer != NULL )
        {
            m_fps_timer->stop();
            m_fps_timer->disconnect();
            delete m_fps_timer;
            m_fps_timer = NULL;
        }
    }
    else
    {
        if( m_fps_timer == NULL )
        {
            m_fps_timer = new QTimer(this);
        }
        int timeInterval = static_cast<int>((1./fps) * 1000);
        std::cout << "Timer initialized with " << timeInterval << "ms as interval value" << std::endl;
        m_fps_timer->start(timeInterval);
        connect(m_fps_timer,SIGNAL(timeout()),this,SLOT(sltTimerEvent()));
    }
}

void GLView::sltTimerEvent()
{
    //std::cout << "Timer event generated" << std::endl;
    paintGL();
    update();
}
