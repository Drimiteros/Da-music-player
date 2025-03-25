#include "Waveform.h"

Waveform::Waveform() {

    waveform.setPrimitiveType(LineStrip);
    waveform.resize(770);
}

void Waveform::waveform_logic(Music& music) {

    /*if (soundBuffer.getSampleCount() > 0 && sound.getStatus() != Sound::Stopped) {
        samples = soundBuffer.getSamples();
        sample_count = soundBuffer.getSampleCount();
        channel_count = soundBuffer.getChannelCount();
        size_t startSample = sound.getPlayingOffset().asSeconds() * soundBuffer.getSampleRate() * channel_count;
        for (size_t i = 0; i < 770; ++i) {
            size_t sampleIndex = startSample + (i * 5);
            if (sampleIndex < sample_count) {
                float sampleValue = 0;
                for (size_t channel = 0; channel < channel_count; ++channel) {
                    if (sampleIndex + channel < sample_count)
                        sampleValue += samples[sampleIndex + channel];
                }
                sampleValue /= channel_count;
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2) + (sampleValue * scaleY));
                float intensity = abs(sampleValue) / 32768.0f;
                waveform[i].color = Color(150 + 190 * intensity, 100 + 195 * intensity, 55);
            }
            else {
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2));
                waveform[i].color = Color(50, 50, 250);
            }
        }
    }*/
}

void Waveform::draw(RenderWindow& window) {

    window.draw(waveform);
}