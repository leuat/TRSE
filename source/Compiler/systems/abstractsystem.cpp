#include "abstractsystem.h"

QMap<SystemLabel::Type, QColor> AbstractSystem::m_labelColors;

AbstractSystem::AbstractSystem(AbstractSystem *a) {
}

void AbstractSystem::InitLabelColors() {
    m_labelColors[SystemLabel::IO] = QColor(255,128,50);
    m_labelColors[SystemLabel::SID] = QColor(255,255,128);
    m_labelColors[SystemLabel::VIC] = QColor(64,128,255);
    m_labelColors[SystemLabel::FREE] = QColor(128,192,128);
    m_labelColors[SystemLabel::BASIC] = QColor(64,64,255);
    m_labelColors[SystemLabel::STACK] = QColor(255,128,64);
    m_labelColors[SystemLabel::COLOUR] = QColor(255,64,255);
    m_labelColors[SystemLabel::KERNAL] = QColor(64,64,64);
    m_labelColors[SystemLabel::SCREEN] = QColor(64,255,255);
    m_labelColors[SystemLabel::ZEROPAGE] = QColor(64,255,128);


}
