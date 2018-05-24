#ifndef LIMAGEEFFECTS_H
#define LIMAGEEFFECTS_H


#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/multicolorimage.h"
#include <QString>
class LImageEffect {
  public:
    QString m_name;
    QMap<QString, float> m_params;

    LImageEffect(QString name) {
        m_name=name;
    }

};


class LImageEffects
{
public:
    LImageEffects();
    LImage* m_image;
    QVector<LImageEffect*> m_effects;

    QStringList getStringList();


    void Init(LImage* image) { m_image=image;}

    void InitEffects();

    void RenderCircles(LImageEffect* le);

    void Render(QString effect);

};

#endif // LIMAGEEFFECTS_H
