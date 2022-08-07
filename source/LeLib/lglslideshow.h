#ifndef LGLSlideshow_H
#define LGLSlideshow_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

//#include "qlabellimage.h"

class GLSlide {
public:
    GLSlide() {

    }
    GLSlide(QString img, QString text, QString project);

    QImage m_image;
    QString m_text;
    QString m_project;
};



class LGLSlideshow : public QOpenGLWidget, protected QOpenGLFunctions
{

    Q_OBJECT
    QOpenGLShaderProgram * m_program = nullptr;
    QOpenGLTexture       * m_texture1 = nullptr;
    QOpenGLTexture       * m_texture2 = nullptr;
public:





    QVector<GLSlide> m_slides;
    int m_curSlide = 0;
    float m_time=0,m_time2=0;
    int m_curType, m_prevType = 0;
    bool m_randomize = true;
    LGLSlideshow();
    LGLSlideshow(QWidget* );
    void Init();
    void initializeGL() override;
    void paintGL() override ;
//    void paintEvent(QPaintEvent *event) override ;
    void setCurrentTexture();
//    void resizeGL(int width, int height) override;


    void mouseMoveEvent(QMouseEvent *e) override;

    void wheelEvent(QWheelEvent *e) override;

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
