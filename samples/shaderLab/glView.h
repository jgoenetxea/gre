#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QTimer>

#include "scenes/movingCube.h"
#include "scenes/simpleQuad.h"

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLView(QWidget *parent = 0);
    ~GLView();

    void setAnimationFrameRate( const unsigned int fps );
    void updateShader( const string& fragmentShader );
    std::string& getCurrentFragmentShaderCode();
    std::string& getCurrentVertexShaderCode();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer* m_fps_timer;
    unsigned int m_fps;

    SimpleQuad m_sq;

signals:
    void initialized();

protected slots:
    void sltTimerEvent();
};

#endif // GLVIEW_H
