#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{

    m_types.append(ImageType("Fake C64 png Palette 1", LImage::Type::QImageBitmap, LColorList::Type::C64));
    m_types.append(ImageType("Fake C64 png Palette 2", LImage::Type::QImageBitmap, LColorList::Type::C64_ORG));
    m_types.append(ImageType("C64 MultiColor bitmap", LImage::Type::MultiColorBitmap,LColorList::Type::C64));
    m_types.append(ImageType("C64 Hires Bitmap", LImage::Type::HiresBitmap,LColorList::Type::C64));
    m_types.append(ImageType("C64 Multicolor Charmap", LImage::Type::CharMapMulticolor,LColorList::Type::C64));
    m_types.append(ImageType("C64 Regular Charmap", LImage::Type::CharmapRegular,LColorList::Type::C64));
    m_types.append(ImageType("C64 Fullscreen Characters", LImage::Type::FullScreenChar,LColorList::Type::C64));
    m_types.append(ImageType("C64 Level Editor", LImage::Type::LevelEditor,LColorList::Type::C64));
    m_types.append(ImageType("C64 Sprite Editor", LImage::Type::Sprites,LColorList::Type::C64));

    /*    m_types.append(ImageType("CGA Palette 1 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA1_LOW));
    m_types.append(ImageType("CGA Palette 1 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("CGA Palette 2 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA2_LOW));
    m_types.append(ImageType("CGA Palette 2 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA2_HIGH));
    m_types.append(ImageType("TIFF", LImage::Type::Tiff,LColorList::Type::TIFF));
*/

    New(5,CharmapGlobalData());
}

ImageWorker::~ImageWorker()
{
    for (ImageEdit* ie: m_images)
        delete ie;
    m_images.clear();
}

ImageType *ImageWorker::findType(LImage::Type imageType, LColorList::Type colType)
{
    for (int i=0;i<m_types.count();i++) {
        ImageType* it=&m_types[i];
        if (it->colorType==colType && it->type == imageType)
            return it;
    }
    return nullptr;//&m_types[0];
}

void ImageWorker::UpdateListView(QListView *lst)
{
    QStandardItemModel *model = new QStandardItemModel( m_images.count(), 1, nullptr);
    for (int i=0;i<m_images.count();i++) {
        QStandardItem *item = new QStandardItem(m_images[i]->m_name);
        model->setItem(i,0, item);
    }

    lst->setModel(model);

}

QStringList ImageWorker::getImageTypes()
{
    QStringList l;
    for (ImageType it : m_types)
        l<< it.name;
    return l;
}

void ImageWorker::New(LImage *image, QString name)
{
    ImageType* imageType = findType(image->m_type, image->m_colorList.m_type);

    if (imageType == nullptr) {
        qDebug() << "WTF NULLPTR imagetype ";
        return;
    }

    m_currentImage = new ImageEdit(image, imageType, name);
    //m_currentImage = new ImageEdit(imageType, "New Image");
    m_images.append(m_currentImage);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
}

void ImageWorker::New(int image, CharmapGlobalData gd = CharmapGlobalData())
{
    m_currentImage = new ImageEdit(&m_types[image], "New Image");
    if (m_types[image].type==LImage::Type::LevelEditor)
        dynamic_cast<ImageLevelEditor*>(m_currentImage->m_image)->Initialize(gd);

    m_images.append(m_currentImage);
    m_currentImage->m_fileName="";

    Data::data.redrawFileList = true;
}

void ImageWorker::SetImage(int cur)
{
    if (cur>=0 && cur<m_images.count())
        m_currentImage = m_images[cur];

}



