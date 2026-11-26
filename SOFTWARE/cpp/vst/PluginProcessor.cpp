#include "PluginProcessor.hpp"
#include <cmath>
#include <algorithm>

namespace wbs::m467 {

M467PluginProcessor::M467PluginProcessor()
    : engine::SharedPluginFrame("Ward-Beck M467 Noise Gate & Meter", "M467") {
    defaultOversampling_ = engine::OversamplingRate::None;
}

void M467PluginProcessor::prepareModule(double hostRate, double /*oversampledRate*/, size_t maxBlockSize, size_t numChannels) {
    for (size_t ch = 0; ch < numChannels; ++ch) {
        envelope_[ch] = 0.0f;
        currentGain_[ch] = 1.0f;
        sidechainFilter_[ch].prepare(hostRate);
        sidechainFilter_[ch].update(static_cast<double>(keyHpfFreq_), 0.707, 0.0, engine::FilterType::HighPass18dB);
        outputVu_[ch].prepare(hostRate);
    }
    updateParametersFromHost();
}

void M467PluginProcessor::releaseModule() {
    for (size_t ch = 0; ch < numChannels_; ++ch) {
        envelope_[ch] = 0.0f;
        currentGain_[ch] = 1.0f;
        sidechainFilter_[ch].reset();
        outputVu_[ch].reset();
    }
}

void M467PluginProcessor::updateParametersFromHost() noexcept {
    const float effAttackMs = fastAttack_ ? 0.2f : attackMs_;
    alphaAttack_ = 1.0f - std::exp(-1.0f / (std::max(0.0001f, effAttackMs * 0.001f) * static_cast<float>(sampleRate_)));
    alphaRelease_ = std::exp(-1.0f / (std::max(0.01f, releaseSec_) * static_cast<float>(sampleRate_)));

    // Minimum gain during gate closed state: -attenuationDb
    minGainLinear_ = std::pow(10.0f, -std::abs(attenuationDb_) / 20.0f);

    for (size_t ch = 0; ch < numChannels_; ++ch) {
        sidechainFilter_[ch].update(static_cast<double>(keyHpfFreq_), 0.707, 0.0, engine::FilterType::HighPass18dB);
    }
}

void M467PluginProcessor::processModuleSlice(engine::AudioBlock& slice) noexcept {
    if (!gateIn_) return;

    const float threshLinear = std::pow(10.0f, thresholdDb_ / 20.0f);
    float avgGain = 0.0f;

    for (size_t ch = 0; ch < slice.numChannels; ++ch) {
        float* channelData = slice.getChannelPointer(ch);

        for (size_t i = 0; i < slice.numSamples; ++i) {
            const float inSamp = channelData[i];

            // Sidechain filtering
            const float scSamp = sidechainFilter_[ch].processSample(inSamp);
            const float scLevel = std::abs(scSamp);

            // Envelope detection
            if (scLevel > envelope_[ch]) {
                envelope_[ch] += alphaAttack_ * (scLevel - envelope_[ch]);
            } else {
                envelope_[ch] *= alphaRelease_;
            }

            // Target gain: S-curve smooth transition
            // When level > threshold -> targetGain = 1.0; when level << threshold -> targetGain = minGainLinear_
            const float diffDb = (envelope_[ch] > 1e-6f) ? (20.0f * std::log10(envelope_[ch]) - thresholdDb_) : -100.0f;
            const float sigmoid = 1.0f / (1.0f + std::exp(-diffDb * 0.75f));
            const float targetGain = minGainLinear_ + (1.0f - minGainLinear_) * sigmoid;

            // Smooth slew-shaping to avoid audio clicks (1.0 ms anti-click slew)
            const float slewAlpha = 1.0f - std::exp(-1.0f / (0.001f * static_cast<float>(sampleRate_)));
            currentGain_[ch] += slewAlpha * (targetGain - currentGain_[ch]);

            // Apply differential gain
            channelData[i] = inSamp * currentGain_[ch];
        }

        avgGain += currentGain_[ch];
    }

    avgGain /= static_cast<float>(slice.numChannels);
    gateOpenState_ = (avgGain > (minGainLinear_ + 0.15f * (1.0f - minGainLinear_)));

    // Meter attenuation: 1.0 = 0 dB (top), 0.0 = -50 dB (bottom)
    const float curGainDb = (avgGain > 1e-5f) ? (20.0f * std::log10(avgGain)) : -50.0f;
    meterAttenNorm_ = std::clamp((curGainDb + 50.0f) / 50.0f, 0.0f, 1.0f);
}

void M467PluginProcessor::updateMeters(const engine::AudioBlock& audioBlock) noexcept {
    float peakOut = 0.0f;
    for (size_t ch = 0; ch < audioBlock.numChannels; ++ch) {
        const float* d = audioBlock.getChannelPointer(ch);
        for (size_t i = 0; i < audioBlock.numSamples; ++i) {
            peakOut = std::max(peakOut, std::abs(d[i]));
        }
    }
    const float vu = outputVu_[0].processSample(peakOut);
    meterOutputVuNorm_ = std::clamp((vu - 0.1f) / 0.9f, 0.0f, 1.0f);
}

void M467PluginProcessor::setThresholdDb(float threshDb) noexcept {
    thresholdDb_ = threshDb;
}

void M467PluginProcessor::setAttenuationDb(float attenDb) noexcept {
    attenuationDb_ = attenDb;
    updateParametersFromHost();
}

void M467PluginProcessor::setReleaseSec(float releaseSec) noexcept {
    releaseSec_ = releaseSec;
    updateParametersFromHost();
}

void M467PluginProcessor::setAttackMs(float attackMs) noexcept {
    attackMs_ = attackMs;
    updateParametersFromHost();
}

void M467PluginProcessor::setKeyHpfFreq(float hpfHz) noexcept {
    keyHpfFreq_ = hpfHz;
    updateParametersFromHost();
}

void M467PluginProcessor::setGateIn(bool gateIn) noexcept {
    gateIn_ = gateIn;
}

void M467PluginProcessor::setFastAttack(bool fast) noexcept {
    fastAttack_ = fast;
    updateParametersFromHost();
}

} // namespace wbs::m467
