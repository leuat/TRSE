#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
#include <QString>
#include <QImage>
#include <QVector>
#include <math.h>

class Texture {
public:
    QString m_name="";
    QVector<QImage*> m_images;

    QImage* m_image = nullptr;

    void Load(QString f) {
        m_image = new QImage();
        m_image->load(f);
        GenerateMipMaps();
    }
    QImage* get(int lvl) {
        if (lvl>=m_images.count()) lvl = m_images.count()-1;
        if (lvl<0) lvl = 0;
        return m_images[lvl];
    }

    void GenerateMipMaps();

};


class Material
{
public:
    Material();

    QVector3D m_color =QVector3D(1,0.5,0.5);
    float m_shininess_strength =1;
    float m_shininess=0;
    float m_reflectivity=0.5;
    float m_perlinness = 0.3;
    float m_uvScale = 0.03;
    float m_perlinScale = 5;
    QVector3D m_checkerBoard = QVector3D(0,0,0);
    Texture m_texture;
    bool m_hasTexture = false;

    Material(QVector3D col, float shin, float ref, float perlin, float ps, QString texture) {
        m_color = col;
        m_shininess = shin;
        m_reflectivity = ref;
        m_perlinness = perlin;
        m_perlinScale = ps;

        if (texture!="") {
            m_hasTexture = true;
            m_texture.Load("textures/"+texture);

        }
    }

};

#endif // MATERIAL_H
