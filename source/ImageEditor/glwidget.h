#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
//#include "qlabellimage.h"
#include "abstractimageeditor.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions, public AbstractImageEditor
{

    Q_OBJECT
    QOpenGLShaderProgram * m_program = nullptr;
    QOpenGLTexture       * m_texture = nullptr;
    QOpenGLTexture       * m_grid = nullptr;
public:




    GLWidget();
    GLWidget(QWidget* );
    void initializeGL() override;
    void paintGL() override ;
//    void paintEvent(QPaintEvent *event) override ;
    void setTexture(QImage& img, QImage& grid);
    void resizeGL(int width, int height) override;


    void mouseMoveEvent(QMouseEvent *e) override;

    void wheelEvent(QWheelEvent *e) override;

    bool eventFilter(QObject *object, QEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *e) override;
//    void keyPressEvent(QKeyEvent *e) override;

    void mousePressEvent(QMouseEvent *e) override;


signals:
    void EmitMouseMove(QEvent* e);
    void EmitMouseRelease();
    void EmitSwapDisplayMode();


};

#endif // GLWIDGET_H
