/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "midioutput.h"
#include "soundobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_note_1_clicked(bool checked);
    void on_note_2_clicked(bool checked);
    void on_note_3_clicked(bool checked);
    void on_note_4_clicked(bool checked);
    void on_note_5_clicked(bool checked);
    void on_note_6_clicked(bool checked);
    void on_note_7_clicked(bool checked);
    void on_note_8_clicked(bool checked);
    void on_note_9_clicked(bool checked);
    void on_note_10_clicked(bool checked);
    void on_note_11_clicked(bool checked);
    void on_note_12_clicked(bool checked);
    void on_volume_valueChanged(int value);

    void on_pitchbend_valueChanged(int value);

    void on_midichannel_valueChanged(int arg1);

    void on_comboBox_activated(const QString &arg1);

    void on_program_valueChanged(int arg1);

    void createSoundObject(SoundObject o);



private:
    Ui::MainWindow *ui;
    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;

    int listLength;


};

#endif // MAINWINDOW_H
*/
