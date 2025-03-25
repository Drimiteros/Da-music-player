#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class Waveform
{
private:
    //Initialize the waveform
    //VertexArray waveform stores a collection of vertices (each vertex uses a audio sample to set its current y position in the screen). 
    //Linestrip connects the vertices and 770 is the amount of verticies we want to be displayed (the more verticies, the better the resolution)
    VertexArray waveform;
    const Int16* samples = nullptr; //Unsigned 16 bit integer that stores the audio samples
    size_t sample_count = 0; //Unsigned integer that stores the sample amount
    size_t channel_count = 0; //Unsigned integer that stores the audio channel amount.
    float scaleY = 0.01; //Controls the max heihgt of the waveform
    int remain_minutes = 0; //Stores the remain minutes of the audio
    int remain_seconds = 0; //Stores the remain seconds of the audio
    RectangleShape waveformArea; //A rectangle that acts as a backgorund for the waveform

public:
    Waveform();
    void waveform_logic(Music& music, SoundBuffer& soundBuffer);
    void draw(RenderWindow& window);
};

