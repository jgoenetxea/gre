#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void updateEditorWithShaderCode();
    void updateSceneWithNewShaderCode();
    void initialize();

private slots:
    void sltSystemInitialized();

public slots:
    void sltUpdateEditorContent();
    void sltRunProgram();
    void sltShowHideCodeEditor(bool status);
};

#endif // MAINWINDOW_H
