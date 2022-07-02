#include "glwidget.h"
#include "source/LeLib/util/util.h"
#include <QOpenGLFunctions>
#include <QDebug>
GLWidget::GLWidget()
{
    setMouseTracking(true);
    QOpenGLWidget::installEventFilter((QOpenGLWidget*)this);


}

GLWidget::GLWidget(QWidget *)
{
    setMouseTracking(true);
    QOpenGLWidget::installEventFilter((QOpenGLWidget*)this);

}


void GLWidget::initializeGL()
{

    //GLWidget::initializeGL();
    initializeOpenGLFunctions();
 //   initializeGL()
    QImage image(320,200,QImage::Format_RGB32);
    m_texture = new QOpenGLTexture( image );
    m_grid = new QOpenGLTexture( image );

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, Util::loadTextFile(":/resources/shaders/screen.vert"));

    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, Util::loadTextFile(":/resources/shaders/screen.frag"));
    m_program->bindAttributeLocation("vPosition", 0);
    m_program->link();
//    qDebug() << "HERE";




}

//void GLWidget::paintEvent(QPaintEvent *event)
void GLWidget::paintGL()
{
    // celar the framebuffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind the texture
    uint texture_unit = 1;
    m_texture->bind( texture_unit );
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_grid->bind( texture_unit+1 );
    m_grid->setMinificationFilter(QOpenGLTexture::Nearest);
    m_grid->setMagnificationFilter(QOpenGLTexture::Nearest);
    // activate the shader
    m_program->bind();

    m_program->setUniformValue( "screen", texture_unit );
    m_program->setUniformValue( "grid", texture_unit+1 );
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

void GLWidget::setTexture(QImage &img, QImage& grid)
{
    if (m_texture!=nullptr)
        delete m_texture;
    if (m_grid!=nullptr)
        delete m_grid;
    m_texture = new QOpenGLTexture( img );
    m_grid = new QOpenGLTexture(grid);
    update();
  //  update();
}

void GLWidget::resizeGL(int width, int height)
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

void GLWidget::resizeEvent(QResizeEvent *event)
{
    if (event!=nullptr)
        event->accept();

    float scale = 1;
    if (m_aspectType==2)
        scale = 1.5;

//    qDebug() << m_aspectType;



    if (m_aspectType!=0) {
        // Fixed 1:1

        int m = std::min((float)event->size().width(),event->size().height()*scale);

        QWidget::resize(m,m/scale);

    }
    QOpenGLWidget::resizeEvent(event);
}


void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (AIE_mouseMoveEvent(e,this))


        //  qDebug() << (m_updateThread->m_prevPos- m_updateThread->m_currentPos);

        //    if ((m_updateThread->m_prevPos-m_updateThread->m_currentPos).manhattanLength()>0.0)
//        qDebug() << "HERE "<<rand()%100 ;
        emit EmitMouseMove(e);


    //    emit EmitMouseMove();

}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    emit EmitMouseMove(e);
}

bool GLWidget::eventFilter(QObject *object, QEvent *event)
{

    if (m_cancel)
        return true;
    if(object==this) {
        if (AIE_eventFilter(object,event,this)) {
            emit EmitMouseMove(event);

        }
        return false;
    }
    return true;

}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    AIE_mouseReleaseEvent(e);
    emit EmitMouseRelease();
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    AIE_mousePressEvent(e);
    emit EmitMouseMove(e);
}



