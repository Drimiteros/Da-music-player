#include "waveform_class.h"

waveform_class::waveform_class() {
    waveform.setPrimitiveType(LineStrip);
    waveform.resize(770);

    waveformArea.setSize(Vector2f(770, 142));
    waveformArea.setPosition(220, 588);
    waveformArea.setFillColor(Color(30, 30, 30));
}

void waveform_class::waveform_logic(SoundBuffer& soundBuffer, Sound& sound) {
    if (soundBuffer.getSampleCount() > 0 && sound.getStatus() != Sound::Stopped) {
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
    }
}

void waveform_class::draw(RenderWindow& window) {
    window.draw(waveformArea);
    window.draw(waveform);
}