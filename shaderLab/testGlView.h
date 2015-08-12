#ifndef TESTGLVIEW_H
#define TESTGLVIEW_H

#include <QtWidgets>
#include <QGLWidget>
class TestGlView:public QGLWidget
{
  Q_OBJECT

    public :
    TestGlView(QWidget* parent):QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    {
        timer = new QTimer(this);
        timer->start(10);
        radius  = 0.4;
        angle   = 1.0;
        connect(timer,SIGNAL(timeout()),this,SLOT(repaint()));
    }
    ~ TestGlView()
    {}

    void initializeGL()
    {
        glClearColor(0.0, 0.0, 102.0/255.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    }

    void resizeGL(int width, int height)
    {
        glViewport(0, 0, (GLint)width, (GLint)height);
    }

    void  paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINES);

        glVertex2f((float) 0.5, (float) 0.5);
        float x2 = 0.5+sin(angle)*radius;
        float y2 = 0.5+cos(angle)*radius;
        glVertex2f(x2,y2);
        angle+=0.03;

        glEnd();
    }

    void repaint()
    {
        paintGL();
    }

public:
  QTimer* timer;
  double radius;
  float angle;
};
#endif // TESTGLVIEW_H
