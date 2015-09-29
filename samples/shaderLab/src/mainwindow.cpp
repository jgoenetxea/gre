#include "mainwindow.h"
#include "ui_mainwindow.h"

const string defaultFragmentShader = "#version 330 core\n \
// Interpolated values from the vertex shaders\n \
in vec2 UV;\n \
// Ouput data\n \
out vec3 fragColor;\n \
// Values that stay constant for the whole mesh.\n \
uniform sampler2D iChannel0;\n \
uniform vec3 iResolution;		// Viewport resolution (in pixels)\n \
uniform float iGlobalTime;		// shader playback time (in seconds)\n \
uniform vec4 iMouse;			// mouse pixel coords. xy: current (if MLB down), zw: click\n \
uniform vec4 iDate;				// (year, month, day, time in seconds)\n \
uniform float iSampleRate;		// sound sample rate (i.e., 44100)\n \
\n \
void main(){\n \
    fragColor = vec3(255,0,0);\n \
}\n";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->glview->setAnimationFrameRate(60);

    connect(ui->glview, SIGNAL(initialized()), this, SLOT(sltSystemInitialized()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateEditorWithShaderCode()
{
    string& fragmentCode = ui->glview->getCurrentFragmentShaderCode();

    cout << "Code:" << endl << fragmentCode << endl;

    ui->codeEditor->setPlainText( QString(fragmentCode.c_str()) );
}

void MainWindow::updateSceneWithNewShaderCode()
{
    string newCode = ui->codeEditor->toPlainText().toStdString();

    cout << "New Code:" << endl << newCode << endl;

    ui->glview->updateShader( newCode );
}

void MainWindow::initialize()
{
    ui->glview->updateShader( defaultFragmentShader );
    updateEditorWithShaderCode();
}





void MainWindow::sltUpdateEditorContent()
{
    updateEditorWithShaderCode();
}

void MainWindow::sltRunProgram()
{
    updateSceneWithNewShaderCode();
}

void MainWindow::sltSystemInitialized()
{
    initialize();
}

void MainWindow::sltShowHideCodeEditor( bool status )
{
    ui->codeEditor->setVisible( !status );
}
