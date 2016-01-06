#include "mainwindow.h"
#include "soundobject.h"
#include "midiinput.h"
#include "midioutput.h"
#include <stdlib.h>
#include <unistd.h>


SoundObject::SoundObject(int xPos, int yPos, int colorId, int shapeId, int channel){
    this->xPos = xPos;
    this->yPos = yPos;
    this->colorId = colorId;
    this->shapeId = shapeId;
    this->channel = channel;

    QStringList connections = midiOutput.connections(true);

    //play(this->xPos, this->yPos, this->colorId, this->shapeId,this->channel);

}

SoundObject::~SoundObject(){
}

void SoundObject::play(int xPos, int yPos, int colorId, int shapeId, int channel){

    midiOutput.sendProgram(channel, shapeId);
    midiOutput.sendNoteOn(channel, yPos+60, colorId);

    for(int i = 0; i<xPos;i++){
        usleep(100000);
    }


    //midiOutput.sendNoteOff(channel, yPos+60, colorId);

}

