#include "glwidget.h"
#include "source/LeLib/util/util.h"
#include <QOpenGLFunctions>

GLWidget::GLWidget()
{
    setMouseTracking(true);
    installEventFilter(this);

}

GLWidget::GLWidget(QWidget *)
{
    setMouseTracking(true);
    installEventFilter(this);

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

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_work==nullptr)
        return;
    if (m_work->m_currentImage==nullptr)
        return;
    if (m_work->m_currentImage->m_image==nullptr)
        return;
    if (m_updateThread==nullptr)
        return;


    m_active=true;

    QPointF pos = QCursor::pos() -mapToGlobal(QPoint(0,0));
    pos.setX(pos.x()*(float)m_work->m_currentImage->m_image->m_width/width());
    pos.setY(pos.y()*(float)m_work->m_currentImage->m_image->m_height/height());

    m_updateThread->m_prevPos = m_updateThread->m_currentPos;
    m_updateThread->m_currentPos = QPointF(pos.x(), pos.y());

    if ((m_updateThread->m_prevPos-m_updateThread->m_currentPos).manhattanLength()>0.0)

        emit EmitMouseMove();

}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    emit EmitMouseMove();
}

bool GLWidget::eventFilter(QObject *object, QEvent *event) {
    if (m_cancel)
        return true;
    if(object==this) {
  //      qDebug() << event;
        mouseMoveEvent((QMouseEvent*)event);
        if (event->type()==QEvent::Enter) {
            Data::data.imageEvent = 1;
        }
        if (event->type()==QEvent::Leave) Data::data.imageEvent = 0;
        if (m_buttonDown)
            m_updateThread->m_currentButton=m_keepButton;

        return false;
    }
    return true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_prevButton = m_updateThread->m_currentButton;
    if (m_updateThread->m_currentButton==1) {
        m_updateThread->m_currentButton = -1;
    }
    else
        m_updateThread->m_currentButton = 0;

    m_buttonDown = false;

//    emit EmitMouseMove();
    emit EmitMouseRelease();

}

/*void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Space)  {
        emit EmitSwapDisplayMode();
    }
}
*/
void GLWidget::mousePressEvent(QMouseEvent *e)
{
    if (m_cancel)
        return;
    if (m_updateThread==nullptr)
        return;
    m_updateThread->m_work->m_currentImage->AddUndo();



    if(e->buttons() == Qt::RightButton) {
        m_imageChanged = true;
        m_updateThread->m_currentButton = 2;
        if (e->modifiers() & Qt::ShiftModifier) {
            m_updateThread->m_prevPos = m_updateThread->m_currentPos;
            m_updateThread->m_currentButton = 4;
        }
        m_keepButton = 2;
        m_buttonDown = true;
    }

    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
        m_keepButton = 1;
        m_buttonDown = true;
        m_imageChanged = true;
    }
/*    if(e->buttons() == Qt::RightButton) {
        m_updateThread->m_currentButton = 2;
        m_buttonDown = true;
        m_imageChanged = true;
    }
*/
    emit EmitMouseMove();

}

