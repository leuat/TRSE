
SystemSchip::SystemSchip(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemChip8(settings, proj) {
    m_system = SCHIP8;
}