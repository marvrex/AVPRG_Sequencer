#ifndef SOUNDOBJECT_H
#define SOUNDOBJECT_H

#include "midioutput.h"


class SoundObject
{
public:
    SoundObject(int xPos, int yPos, int colorId, int shapeId, int channel);
    ~SoundObject();

signals:

public slots:
    int xPos;
    int yPos;
    int colorId;
    int shapeId;
    int channel;


    void addSoundObject(int xPos, int yPos, int colorId, int shapeId, int channel);
    void play(int xPos, int yPos, int colorId, int shapeId, int channel);


private:
    drumstick::rt::MIDIOutput midiOutput;

};

#endif // SOUNDOBJECT_H
