#pragma once
#include "../core/SharedPluginFrame.hpp"
#include "../core/BallisticsCore.hpp"
#include "../core/StateVariableCore.hpp"

namespace wbs::m467 {

class M467PluginProcessor : public engine::SharedPluginFrame {
public:
    M467PluginProcessor();
    ~M467PluginProcessor() override = default;

    void prepareModule(double hostRate, double oversampledRate, size_t maxBlockSize, size_t numChannels) override;
    void releaseModule() override;
    void updateParametersFromHost() noexcept override;
    void processModuleSlice(engine::AudioBlock& slice) noexcept override;
    void updateMeters(const engine::AudioBlock& audioBlock) noexcept override;

    void setThresholdDb(float threshDb) noexcept;
    void setAttenuationDb(float attenDb) noexcept;
    void setReleaseSec(float releaseSec) noexcept;
    void setAttackMs(float attackMs) noexcept;
    void setKeyHpfFreq(float hpfHz) noexcept;
    void setGateIn(bool gateIn) noexcept;
    void setFastAttack(bool fast) noexcept;

    [[nodiscard]] float getAttenuationMeterNorm() const noexcept { return meterAttenNorm_; }
    [[nodiscard]] float getOutputVuMeterNorm() const noexcept { return meterOutputVuNorm_; }
    [[nodiscard]] bool isGateOpen() const noexcept { return gateOpenState_; }

private:
    float thresholdDb_{ -30.0f };
    float attenuationDb_{ 40.0f };
    float releaseSec_{ 0.5f };
    float attackMs_{ 1.0f };
    float keyHpfFreq_{ 80.0f };
    bool gateIn_{ true };
    bool fastAttack_{ false };

    // DSP State
    float envelope_[engine::kMaxChannels]{ 0.0f };
    float currentGain_[engine::kMaxChannels]{ 1.0f };
    float alphaAttack_{ 0.1f };
    float alphaRelease_{ 0.999f };
    float minGainLinear_{ 0.01f };

    engine::StateVariableFilterCore sidechainFilter_[engine::kMaxChannels];
    engine::VuBallisticsFilter outputVu_[engine::kMaxChannels];

    bool gateOpenState_{ true };
    float meterAttenNorm_{ 1.0f };    // 1.0 = 0 dB (top), 0.0 = -50 dB (bottom)
    float meterOutputVuNorm_{ 0.0f };  // 0.0 = -20 VU (bottom), 1.0 = +3 VU (top)
};

} // namespace wbs::m467
