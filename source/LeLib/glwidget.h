#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include "source/qlabellimage.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

    Q_OBJECT
    QOpenGLShaderProgram * m_program = nullptr;
    QOpenGLTexture       * m_texture = nullptr;
    QOpenGLTexture       * m_grid = nullptr;
public:



    bool m_imageChanged = false;
    int m_time=0;
    WorkerThread* m_updateThread = nullptr;
    ImageWorker* m_work = nullptr;
    bool m_active=false;
    bool m_cancel = false;



    GLWidget();
    GLWidget(QWidget* );
    void initializeGL() override;
    void paintGL() override ;
//    void paintEvent(QPaintEvent *event) override ;
    void setTexture(QImage& img, QImage& grid);
    void resizeGL(int width, int height) override;


    void mouseMoveEvent(QMouseEvent *e) override;

    void wheelEvent(QWheelEvent *e) override;
    void CancelAll() {m_cancel=true; m_active=false;}

    bool eventFilter(QObject *object, QEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *e) override;
//    void keyPressEvent(QKeyEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;


signals:
    void EmitMouseMove();
    void EmitMouseRelease();
    void EmitSwapDisplayMode();


};

#endif // GLWIDGET_H
