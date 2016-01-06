#ifndef SOUNDOBJECT_H
#define SOUNDOBJECT_H

#include "midioutput.h"

class Node;

class SoundObject
{
public:
    SoundObject();
    ~SoundObject();

signals:

public slots:

    void addNode(int xPos, int yPos, int colorId, int shapeId, int channel);
    void deleteNode(int channel);
    void play();

    Node *start;
    Node *actual;


private:
    drumstick::rt::MIDIOutput midiOutput;

};

#endif // SOUNDOBJECT_H
