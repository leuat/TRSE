#include "dialogeffects.h"
#include "ui_dialogeffects.h"

LuaScript* m_script = nullptr;


DialogEffects::DialogEffects(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);
    m_file = file;

}

DialogEffects::~DialogEffects()
{

    Abort();

    delete ui;
}

void DialogEffects::Abort()
{
    if (m_effect!=nullptr) {
        m_effect->m_abort = true;
        m_effect->quit();
        m_effect->wait();
        m_effect->terminate();
        m_effect->wait();
        m_effect->deleteLater();
//        delete m_effect;
    }

}
static RayTracer m_rt;




void DialogEffects::Create()
{
    if (m_effect!=nullptr) {
        Abort();
        delete m_effect;
        return;
    }
    if (m_error!="") {
        ui->txtOutput->setText(m_error);

        return;
    }

    if (ui->comboBox->currentText()=="Twister")
        m_effect = new DemoEffectTwister(ui->glParams);


    if (ui->comboBox->currentText()=="Raytracer") {
        m_effect = new DemoEffectRaytracer(ui->glParams);
        if (m_script!=nullptr) {

            dynamic_cast<DemoEffectRaytracer*>(m_effect)->m_rt = &m_rt;
        }
    }
    m_effect->Initialize();
    connect(m_effect,SIGNAL(SignalImageUpdate()),this,SLOT(UpdateImage()));
//    m_effect->FillToGUI();
    m_effect->start();
}


static int LuaSin(lua_State *L) {
    lua_pushnumber(L, sin(lua_tonumber(L, 1)));
    return 1;
}

static int LuaCos(lua_State *L) {
    lua_pushnumber(L, cos(lua_tonumber(L, 1)));
    return 1;
}



static int AddObject(lua_State *L)
{
//    qDebug();// << text;
    int n = lua_gettop(L);
  //  if (n!=5) {
//       return 0;
   // }
    QString object = QString::fromStdString(lua_tostring(L,1)).toLower();
    Material mat = Material(QVector3D(100,1,1),50,0.5, 0,0,"");
    QString name = lua_tostring(L,2);
    QString material = lua_tostring(L,3);
    if (object=="torus") {


        m_rt.m_objects.append(
                    new RayObjectTorus(
                        QVector3D(lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6)) ,
                        QVector3D(lua_tonumber(L,7),lua_tonumber(L,8),0),
                        QVector3D(0,1,0),
                        mat));

    }

    if (object=="box") {
        m_rt.m_objects.append(
                    new RayObjectBox(
                        QVector3D(lua_tonumber(L,4),lua_tonumber(L,5),lua_tonumber(L,6)) ,
                        QVector3D(0,1,0),
                        QVector3D(lua_tonumber(L,7),lua_tonumber(L,8),lua_tonumber(L,9)),
                        mat));

    }


        m_rt.m_objects.last()->m_name = name;
        m_rt.m_objects.last()->m_material.m_color =
            m_script->getVec(material+".color");

        m_rt.m_objects.last()->m_material.m_reflectivity =
                m_script->get<float>(material+".reflection");

        m_rt.m_objects.last()->m_material.m_shininess=
                m_script->get<float>(material+".shininess");


}


static int SetRotation(lua_State *L)
{
    int n = lua_gettop(L);
    QString name = lua_tostring(L,1);

    for (AbstractRayObject* aro : m_rt.m_objects) {
        if (aro->m_name == name) {
            aro->SetRotation(QVector3D(lua_tonumber(L,2),lua_tonumber(L,3),lua_tonumber(L,4)));
        }
    }
}


void DialogEffects::LoadScript(QString file)
{
    m_script = new LuaScript(file);

    if (m_script->L==nullptr) {
        m_error = m_script->m_error;
        delete m_script;
        m_script = nullptr;

        return;
    }

    m_rt.m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
    m_rt.m_objects.clear();



    m_rt.m_camera.m_camera = QVector3D(0,0,-5);
    float a = 0.05;
    m_rt.m_globals.m_ambient = QVector3D(a,a,a);


    lua_register(m_script->L, "AddObject", AddObject);
    lua_register(m_script->L, "SetRotation", SetRotation);
    lua_register(m_script->L, "sin", LuaSin);
    lua_register(m_script->L, "cos", LuaCos);

    /* run the script */
    int ret = luaL_dostring(m_script->L, "Init()");

    UpdateGlobals();
}

void DialogEffects::Init()
{
    if (m_file!="")
        LoadScript(m_file);

    Create();



}

void DialogEffects::UpdateGlobals()
{
    if (m_script==nullptr)
        return;
    m_rt.m_camera.m_camera = m_script->getVec("globals.camera");
    m_rt.m_globals.m_lights[0]->m_direction = m_script->getVec("globals.light0.direction");
    m_rt.m_globals.m_lights[0]->m_color = m_script->getVec("globals.light0.color");
    m_rt.m_globals.m_skyScale = m_script->get<float>("globals.sky");
    m_rt.m_globals.m_shadowScale = m_script->get<float>("globals.shadow_scale");

    m_rt.m_globals.m_width = m_script->get<float>("output.resolution.width");
    m_rt.m_globals.m_height = m_script->get<float>("output.resolution.height");

    m_rt.m_globals.m_c64Output = m_script->get<float>("output.c64_output");
    m_rt.m_globals.m_dither = m_script->get<float>("output.dither");

}

void DialogEffects::UpdateImage()
{
    if (m_effect==nullptr)
        return;

    if (m_error!="") {
        ui->txtOutput->setText(m_error);
        return;
    }

    QPixmap p = QPixmap::fromImage(m_effect->m_img);
//    p = p.scaled(m_effect->m_img.width(), m_effect->m_img.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    ui->lblImage->setPixmap(p);

    m_avg = (int)(0.2*m_effect->m_elapsedTime +0.8*m_avg) ;
    ui->lblTime->setText(QString::number(m_avg));

    ui->txtOutput->setText(m_effect->m_infoText);




    if (m_script!=nullptr) {
    /* run the script */
        int ret = luaL_dostring(m_script->L, "Update()");
  //      qDebug() << QString::number(ret);
        UpdateGlobals();
//        qDebug() << m_rt.m_camera.m_camera;
        if (m_script->m_error!="") m_error = m_script->m_error;

    }


}

void DialogEffects::on_pushButton_clicked()
{
    Abort();
    close();
}

void DialogEffects::on_btnToggleAnim_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->ToggleAnim();
//    m_effect->m_toggleAnim=!m_effect->m_toggleAnim;
 //   m_effect->m_time = 0;
}

void DialogEffects::on_btnToggleC64_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->m_toggleC64=!m_effect->m_toggleC64;

}

void DialogEffects::on_btnSave_clicked()
{
    if (m_effect==nullptr)
        return;
    bool c64=m_effect->m_toggleC64;
    bool anim=m_effect->m_toggleAnim;
    m_effect->m_toggleC64 = true;
    m_effect->m_toggleAnim = false;

//    m_effect->Save("twister.bin");

    m_effect->m_toggleC64 = c64;
    m_effect->m_toggleAnim = anim;

}

void DialogEffects::on_comboBox_activated(const QString &arg1)
{
    Create();
}
