#include "camera.h"

Camera::Camera()
{

}

void Camera::TranslateXY(QVector3D &delta) {
    QVector3D right = QVector3D::crossProduct((m_camera-m_target), m_up).normalized();
    QVector3D d =  -delta.y()*m_up.normalized() + right*delta.x();
    m_camera = m_camera + d;
    m_target = m_target + d;
}

void Camera::ZoomXY(float delta) {
    m_camera = m_camera - (m_camera-m_target).normalized()*delta;
}

void Camera::RotateUp(float r) {
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

    qDebug() << M;


    //    M = m_viewMatrix.inverted();
    /*    M(3, 0) = 0;
        M(3, 1) = 0;
        M(3, 2) = 0;
    */
    return M;

}

void Camera::RotateVertical(float angle) {
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
    m_projection.perspective(m_fov,m_aspect,1,1000.0);
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

QVector3D Camera::coord2ray(float x, float y, float width, float height) {
    double xx = x / (width  * 0.5) - 1.0;
    double yy = y / (height * 0.5) - 1.0;

    QVector4D screenPos = QVector4D(xx, -yy, 1.0, 1.0);
    QVector4D worldPos = m_invVP * screenPos;
    return worldPos.toVector3D().normalized();
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
