#include "source/LeLib/LGLShowcases.h"
#include <QDesktopServices>
#include "source/LeLib/util/util.h"
#include <QOpenGLFunctions>
#include <QTimer>
#include <QUrl>
#include <time.h>

LGLShowcases::LGLShowcases()
{
//    setMouseTracking(true);
  //  QOpenGLWidget::installEventFilter((QOpenGLWidget*)this);

    m_randomize = true;
}

LGLShowcases::LGLShowcases(QWidget *)
{
    setMouseTracking(true);
    QOpenGLWidget::installEventFilter((QOpenGLWidget*)this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);
}

void LGLShowcases::Init()
{
    srand(time(nullptr));
    m_curSlide = 0;
    m_randomize = true;
    m_prevType = rand()%m_slides.count();
    m_curType = (m_prevType+1)%m_slides.count();

}


void LGLShowcases::initializeGL()
{
//    qDebug() << "LGLShowcases HERE";
    //LGLShowcases::initializeGL();
    initializeOpenGLFunctions();
 //   initializeGL()
    QImage image(320,200,QImage::Format_RGB32);
    setCurrentTexture();
  //  image.fill(QColor(0,255,0).rgb());
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, Util::loadTextFile(":/resources/shaders/screen_showcases.vert"));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, Util::loadTextFile(":/resources/shaders/screen_showcases.frag"));
    m_program->bindAttributeLocation("vPosition", 0);
    m_program->link();



}

//void LGLShowcases::paintEvent(QPaintEvent *event)
void LGLShowcases::paintGL()
{
    if (m_time2>=1) {
        m_time2 = 0;
 //       m_time = 0;
        m_curSlide++;
        m_prevType = m_curType;
        if (m_randomize) {
            int i = m_curType;
            while (i==m_curType)
                m_curType = rand()%m_slides.count();

        }
        else
            m_curType++;

        setCurrentTexture();
    }
    // celar the framebuffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // bind the texture
    uint texture_unit = 1;
    m_texture1->bind( texture_unit );
    m_texture1->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture1->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_texture2->bind( texture_unit+1 );
    m_texture2->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture2->setMagnificationFilter(QOpenGLTexture::Nearest);
    // activate the shader
    m_program->bind();


    m_program->setUniformValue( "type1", (m_curSlide) );
    m_program->setUniformValue( "type2", (m_curSlide+1) );

    m_program->setUniformValue( "t1", texture_unit );
    m_program->setUniformValue( "t2", texture_unit+1 );
    m_program->setUniformValue( "time", m_time );
    m_program->setUniformValue( "time2", m_time2 );

    m_time += 0.0031;
    m_time2 += 0.0031;
//    qDebug() << "TIME "<<m_time;
    /*m_program->setUniformValue("CD", CD.x(), CD.y());
    m_program->setUniformValue("barrelScale", barrelScale.x(), barrelScale.y());
    m_program->setUniformValue("chromatic",chromatic);
    m_program->setUniformValue("lsca",lsca);
    m_program->setUniformValue("lamp",lamp);
    m_program->setUniformValue("saturation",saturation);
    m_program->setUniformValue("gamma",gamma);
    m_program->setUniformValue("time",time);
*/
//    time++;
    GLfloat vertices[]{ -1.0f, -1.0f,
                        1.0f, -1.0f,
                        -1.0f,  1.0f,
                        1.0f, 1.0f };

    m_program->enableAttributeArray(0);
    m_program->setAttributeArray(0, GL_FLOAT, vertices, 2);
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    m_program->disableAttributeArray(0);





//    qDebug() <<"TEST " <<rand()%100;

    // release the shader
    //    m_program->release();
}

void LGLShowcases::setCurrentTexture()
{
    if (m_texture1!=nullptr)
        delete m_texture1;
    if (m_texture2!=nullptr)
        delete m_texture2;


    m_texture1 = new QOpenGLTexture( m_slides[m_prevType%m_slides.count()].m_image );
    m_texture2 = new QOpenGLTexture( m_slides[m_curType%m_slides.count()].m_image );

}


void LGLShowcases::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void LGLShowcases::mouseMoveEvent(QMouseEvent *e)
{

    emit EmitMouseMove();

}

void LGLShowcases::wheelEvent(QWheelEvent *e)
{
    emit EmitMouseMove();
}

bool LGLShowcases::eventFilter(QObject *object, QEvent *event)
{
/*    if (m_cancel)
        return true;
    if(object==this) {
        if (AIE_eventFilter(object,event,this)) {
            emit EmitMouseMove();

        }
        return false;
    }*/
    return false;

}

void LGLShowcases::mouseReleaseEvent(QMouseEvent *e)
{
    emit EmitMouseRelease();
}

void LGLShowcases::mousePressEvent(QMouseEvent *e)
{
    QString url = m_slides[m_prevType%m_slides.count()].m_text;
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));

    emit EmitMouseMove();
}



