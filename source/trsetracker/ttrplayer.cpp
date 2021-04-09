#include "ttrplayer.h"

bool TTRPlayer::m_initialized = false;

TTRPlayer::TTRPlayer()
{
    QString instruments = Util::GetSystemPrefix()+QDir::separator()+"themes"+QDir::separator()+"instruments.txt";
//    qDebug() << instruments << QFile::exists(instruments);
    m_instruments.Load(instruments);
/*    {
        ColumnData &col = test_tracker_column.steps;
        col.resize(16);
        col[0].note = 40;
        col[1].note_off = true;
        col[2].note = 47;
        col[5].note_off = true;
        col[6].note = 43;
        col[8].note = 40;
        col[9].note = 41;
        col[10].note_off = true;
        col[11].note = 47;
        col[12].note = 43;
    }
*/

}

TTRPlayer::~TTRPlayer()
{
    if (m_initialized)
    for (auto& voc : mynthesizer.voices) {
        mynth::voice_shutdown(voc);
        m_initialized = false;
    }

}

void TTRPlayer::Initialize(TTRFile *ttr) {
    m_ttr = ttr;
    m_curRow = 0;
    m_curOrder = 0;
    if (!m_initialized) {
        msleep(10);
        audio_init();
        m_initialized = true;
    }
    for (auto& voc : mynthesizer.voices) {
        mynth::voice_init(&audio_engine, voc);
        ma_sound_start(&voc.sound);
    }
    m_silentChannels.resize(m_ttr->m_noChannels);
    m_silentChannels.fill(false);
}


void TTRPlayer::run()
{
    while (m_isPlaying){
        msleep(100);
        if (!m_isPlaying)
            return;

        Play();
    }
}




void TTRPlayer::Play() {

    //    PlayNote(60,12);
    //qDebug() << m_curOrder << m_curRow;
    for (int i=0;i<m_ttr->m_noChannels;i++) {
        int patt = m_ttr->m_orders[m_curOrder][i];
        QByteArray& p = m_ttr->m_patterns[patt];
        if (!m_silentChannels[i])
            PlayNote(i,p.mid(m_curRow*m_ttr->m_noBytesPerLine,m_ttr->m_noBytesPerLine));
    }
    m_curRow++;
    if (m_curRow==m_ttr->m_patternLength) {
        m_curRow = 0;
        m_curOrder+=1;
        if (m_curOrder ==m_ttr->m_orders.count())
            m_curOrder = 0;
        emit emitUpdateOrder(m_curOrder);
    }
    emit emitUpdateRow(m_curRow);
}

void TTRPlayer::PlayNote(int channel,QByteArray data)
{
//    PlayNote(channel,0,0);
    int instr = (data[2] & 0xf0) >> 4;

    int trsI = m_ttr->getTRSEInstrument(instr);
    if (trsI>=m_instruments.m_instruments.count()) {
        qDebug() << "ERROR : Trying to play instrument "<<trsI<< " which is out of bounds!";
        return;
    }
    if ((data[0]&0x80)==0x80) {
        StartSound(channel);
        PlayNote(channel,data[0]&0x7F,256,m_instruments.m_instruments[trsI]);
    }
}

void TTRPlayer::PlayNote(int channel, int midi_note, int velocity, QSharedPointer<TRSEInstrument> ins)
{
    int n = channel;
    mynth::SynthCommand cmd;										// New command
    jam_context.adsr.time[0] = ins->a;
    jam_context.adsr.time[1] = ins->d;
    jam_context.adsr.time[2] = ins->s;
    jam_context.adsr.time[3] = ins->r;

    jam_context.adsr.level[0] = ins->l0;
    jam_context.adsr.level[1] = ins->l1;
    jam_context.adsr.level[2] = ins->l2;

    jam_context.wavemixer[0] = ins->w0;
    jam_context.wavemixer[1] = ins->w1;
    jam_context.wavemixer[2] = ins->w2;
    jam_context.wavemixer[3] = ins->w3;
    midi_note +=12*ins->oct;
    cmd.active = true;										// Tells the synth this is a new command
    cmd.gate_set = true;
    cmd.gate = velocity > 0;								// Gate: Sound is on
    cmd.freq = mynth::TET12(midi_note);							// 12-TET = translate midi note to frequency
    cmd.velocity = velocity / 128.f;						// MIDI velocity, 0-128. Not used
    // adsr
    cmd.adsr_set = true;									// Let synth know we're supplying a new ADSR
    cmd.adsr = jam_context.adsr;							// Copy ADSR
    // wave mixer
    cmd.wavemixer_set = true;
    memcpy(cmd.wavemixer,									// Copy oscillator mixer
           jam_context.wavemixer, sizeof(mynth::Wavemixer));
    // vibrato
//    mynth::set_adsr(cmd, jam_context.adsr);
    cmd.vibrato_set = true;
    cmd.vibrato = jam_context.vibrato;						// Copy vibrato
    // internal jam_context
    jam_context.mono_last_frequency = cmd.freq;
    jam_context.mono_last_note = midi_note;
    // Submit command to synth
//    mynth::note_off(mynthesizer.voices[jam_context.mono_channel+n], cmd);
    mynth::voice_command(mynthesizer.voices[jam_context.mono_channel+n], cmd);
//    mynth::note_on(mynthesizer.voices[jam_context.mono_channel+n], cmd,cmd.freq);


}

void TTRPlayer::Stop()
{
    m_curRow = 0;

    emit emitUpdateRow(-1);
    if (!m_isPlaying)
        return;
    StopSound();
    m_isPlaying = false;
}

void TTRPlayer::StartPlaying()
{
    Stop();
    msleep(10);
    m_curRow = 0;
    m_isPlaying = true;
    StartSound();



    start();
}

void TRSEInstruments::Load(QString fname)
{
    m_instruments.clear();
    m_fileName = fname;
    QStringList lst = Util::loadTextFile(fname).split("\n");
    for (QString s : lst) {
        s = s.trimmed().simplified();
        if (s=="" || s.startsWith("#"))
            continue;
        QStringList in = s.split(" ");
        if (in.count()!=14) {
            qDebug() << "incorrect instrument at : " <<s;
            continue;
        }
        QSharedPointer<TRSEInstrument> ins = QSharedPointer<TRSEInstrument>(new TRSEInstrument);
        ins->name = in[0];
        ins->a = in[1].toFloat();
        ins->d = in[2].toFloat();
        ins->s = in[3].toFloat();
        ins->r = in[4].toFloat();
        ins->l0 = in[5].toFloat();
        ins->l1 = in[6].toFloat();
        ins->l2 = in[7].toFloat();
        ins->w0 = in[8].toFloat();
        ins->w1 = in[9].toFloat();
        ins->w2 = in[10].toFloat();
        ins->w3 = in[11].toFloat();
        ins->oct = in[12].toFloat();
        ins->vibrato = in[13].toFloat();
        m_instruments.append(ins);
    }
}

void TRSEInstruments::Save()
{
    QString s;
    for (auto& t : m_instruments) {
        s+=t->name;
        s+=" " +QString::number(t->a);
        s+=" " +QString::number(t->d);
        s+=" " +QString::number(t->s);
        s+=" " +QString::number(t->r);
        s+=" " +QString::number(t->l0);
        s+=" " +QString::number(t->l1);
        s+=" " +QString::number(t->l2);
        s+=" " +QString::number(t->w0);
        s+=" " +QString::number(t->w1);
        s+=" " +QString::number(t->w2);
        s+=" " +QString::number(t->w3);
        s+=" " +QString::number(t->oct);
        s+=" " +QString::number(t->vibrato);
        s+="\n";
    }

    Util::SaveTextFile(m_fileName,s);
}

void TRSEInstruments::New() {
    auto i = QSharedPointer<TRSEInstrument>(new TRSEInstrument);
    i->name = "New instrument";
    i->w0 = 0.8;
    i->a = 0.01;
    i->l0 = 0.3;
    i->l0 = 0.3;
    i->l1 = 0.3;
    i->l2 = 1;
    m_instruments.append(i);
}
