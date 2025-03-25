#include "Waveform.h"

Waveform::Waveform() {

    waveform.setPrimitiveType(LineStrip);
    waveform.resize(995);

    waveformArea.setSize(Vector2f(995, 142));
    waveformArea.setPosition(0, 370);
    waveformArea.setFillColor(Color(30, 30, 30));
}

void Waveform::waveform_logic(Music& music, SoundBuffer& soundBuffer) {

    if (soundBuffer.getSampleCount() > 0 && music.getStatus() != Sound::Stopped) {
        samples = soundBuffer.getSamples();
        sample_count = soundBuffer.getSampleCount();
        channel_count = soundBuffer.getChannelCount();
        size_t startSample = music.getPlayingOffset().asSeconds() * soundBuffer.getSampleRate() * channel_count;
        for (size_t i = 0; i < 995; ++i) {
            size_t sampleIndex = startSample + (i * 5);
            if (sampleIndex < sample_count) {
                float sampleValue = 0;
                for (size_t channel = 0; channel < channel_count; ++channel) {
                    if (sampleIndex + channel < sample_count)
                        sampleValue += samples[sampleIndex + channel];
                }
                sampleValue /= channel_count;
                float intensity = abs(sampleValue) / 92768.0f;
                waveformArea.setPosition(0, 370 + (intensity * 212.1));
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2) + (sampleValue * scaleY));
                waveform[i].color = Color(51, 54 * (1.0 + intensity), 64 * (1.0 + intensity)); //31, 34, 44
            }
            else {
                waveform[i].position = Vector2f(waveformArea.getPosition().x + i, waveformArea.getPosition().y + (waveformArea.getSize().y / 2));
                waveform[i].color = Color(50, 50, 250);
            }
        }
    }
}

void Waveform::draw(RenderWindow& window) {

    window.draw(waveform);
}