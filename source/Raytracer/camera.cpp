#include "camera.h"
#include <algorithm>
#include <math.h>
#include <QMatrix2x2>
#include <QVector2D>

Camera::Camera()
{

}

void Camera::TranslateXY(QVector3D &delta) {
    QVector3D right = QVector3D::crossProduct((m_camera-m_target), m_up).normalized();
    QVector3D d =  -delta.y()*m_up.normalized() + right*delta.x();
    m_camera = m_camera + d;
    m_target = m_target + d;
}

void Camera::ZoomXY(double delta) {
    m_camera = m_camera - (m_camera-m_target).normalized()*delta;
}

void Camera::RotateUp(double r) {
    QVector3D d = (m_camera-m_target).normalized();
    QVector3D right = QVector3D::crossProduct(d, m_up).normalized();
    m_up = QVector3D::crossProduct(right, d).normalized();
    QQuaternion q = QQuaternion::fromAxisAndAngle(d, r);
    m_up = q*m_up;
}

QMatrix4x4 Camera::GetRotationMatrix() {
    /*    QVector3D zaxis = (m_camera-m_target).normalized();
        QVector3D xaxis = (QVector3D::crossProduct(m_up, zaxis)).normalized()*-1;
        QVector3D yaxis = (QVector3D::crossProduct(zaxis, xaxis)).normalized();

        QMatrix4x4 M;
        M.setToIdentity();
        M(0,0) = xaxis.x();
        M(1, 0) = yaxis.x();
        M(2, 0) = zaxis.x();

        M(0, 1) = xaxis.y();
        M(1, 1) = yaxis.y();
        M(2, 1) = zaxis.y();

        M(0, 2) = xaxis.z();
        M(1, 2) = yaxis.z();
        M(2, 2) = zaxis.z();

        M(0, 3) = 0;
        M(1, 3) = 0;
        M(2, 3) = 0;
    */
    //    return M;
    QMatrix4x4 M;
    M.setToIdentity();
    //    M.lookAt( (m_camera-m_target),QVector3D(0,0,0),m_up);
    M.lookAt( QVector3D(0,0,0),(m_target-m_camera),m_up);
    return M;
    M = m_viewMatrix;
    //    M.translate(m_camera);

//    qDebug() << M;


    //    M = m_viewMatrix.inverted();
    /*    M(3, 0) = 0;
        M(3, 1) = 0;
        M(3, 2) = 0;
    */
    return M;

}

void Camera::RotateVertical(double angle) {
    QVector3D d = m_camera - m_target;
    QVector3D side = QVector3D::crossProduct( m_up, d);
    QQuaternion q = QQuaternion::fromAxisAndAngle(side,angle);
    m_camera = q*(m_camera-m_target) + m_target;

    d = m_camera - m_target;
    m_up = QVector3D::crossProduct(d, side).normalized();
}

void Camera::setupViewmatrix() {


    /*    glm::mat4 proj = glm::perspective(FoV, AspectRatio, Near, Far);
           glm::mat4 view = glm::lookAt(glm::vec3(0.0f), CameraDirection, CameraUpVector);

           glm::mat4 invVP = glm::inverse(proj * view);
           glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
           glm::vec4 worldPos = invVP * screenPos;
      */

    m_projection.setToIdentity();
    m_projection.perspective(m_fov,m_aspect,1,10000.0);
    m_viewMatrix.setToIdentity();
    //    m_viewMatrix.lookAt(m_target, m_camera, m_up);
//    m_viewMatrix.lookAt(m_rotMatrix* m_target, m_rotMatrix* m_camera, m_rotMatrix* m_up);
    m_viewMatrix.lookAt(m_camera, m_target, m_up);
    //    m_viewMatrix.lookAt(m_camera, m_target, m_up);
    //    qDebug() << m_camera;
    //    m_viewMatrix = m_rotMatrix* m_viewMatrix;
    // Pre-calculate mvp
    //    m_invVP = (m_projection.inverted()*m_viewMatrix).inverted();
    m_invVP = (m_projection*m_viewMatrix).inverted();

}

QVector3D Camera::coord2ray(double x, double y, double width, double height) {

    double xx = x / (width  * 0.5) - 1.0;
    double yy = y / (height * 0.5) - 1.0;

//    double sx = (xx > 0) - (xx < 0);
//;
 //   double sy = (yy > 0) - (yy < 0);


//    xx = pow(abs(xx),0.8)*sx;
//    yy = pow(abs(yy),0.8)*sy;

    QVector4D screenPos = QVector4D(xx, -yy, 1.0, 1.0);
    QVector4D worldPos = m_invVP * screenPos;
    return worldPos.toVector3D().normalized();
}

QVector3D Camera::fisheye(double x, double y, double width, double height)
{

    double xx = x / (width  * 0.5) - 1.0;
    double yy = y / (height * 0.5) - 1.0;




    double th = 0;//x/width*3.14159*2.0;

    QVector2D UV(xx,yy);
//    QVector3D fvcam(xx,yy, th);
    //    double th = cos(time/100.0)*2+1;
    QMatrix2x2 rmat;// =  QMatrix2x2(cos(th),-sin(th),sin(th),cos(th));
    //        QVector2D p2 = rmat*UV;
    QVector2D p2 = QVector2D(cos(th)*UV.x() - sin(th)*UV.y(),sin(th)*UV.x() + cos(th)*UV.y());

//    QVector2D p = QVector2D(p2.x()*0.5, p2.y()*1);
 //   p2 = (p)-QVector2D(0.5,1);

    QVector2D thetaphi = ((p2) - QVector2D(3.1415926535897932384626433832795, 1.5707963267948966192313216916398));
    thetaphi.setY(-thetaphi.y());

    return QVector3D(cos(thetaphi.y()) * cos(thetaphi.x()), sin(thetaphi.y()), cos(thetaphi.y()) * sin(thetaphi.x()));


}

void Camera::setRotation(QVector3D &v) {
    QMatrix3x3 rot = QQuaternion::fromEulerAngles(v).toRotationMatrix();
    m_rotMatrix.setToIdentity();
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
            m_rotMatrix(i,j) = rot(i,j);
    //    m_rotMatrix = m_rotMatrix*rot;
    //QMatrix4x4::TRS(Vector3.zero, Quaternion.EulerAngles(v), Vector3.one);
    setupViewmatrix();
}
