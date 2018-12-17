#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>


class Camera
{
public:

    QVector3D m_camera = QVector3D(0,0,-10);
    QVector3D m_target = QVector3D(0,0,0);
    QVector3D m_up = QVector3D(0,1,0.001);
    QMatrix4x4 m_matrix;
    QMatrix4x4 m_viewMatrix, m_viewMatrixInv, m_projection,  m_rotMatrix, m_invVP;
    float m_fov = 70;
    float m_aspect = 1;


    Camera();
    void TranslateXY(QVector3D& delta);

    void ZoomXY(float delta);

    void RotateUp(float r);


    QMatrix4x4 GetRotationMatrix();

    void RotateVertical(float angle);
    void RotateHorisontal(float angle);


    void setupViewmatrix();

    QVector3D coord2ray(float x, float y, float width, float height);


    void setRotation(QVector3D& v);

};

#endif // CAMERA_H
