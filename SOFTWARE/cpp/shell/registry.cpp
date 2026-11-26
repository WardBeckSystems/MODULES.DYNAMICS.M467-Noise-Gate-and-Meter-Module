// Copyright (c) 2026 Ward-Beck Systems (Ward-Beck.com - Ward-Beck.ca). All rights reserved.
// M467: its part of 2030.env/vst/clap_wrapper/ClapModuleRegistry.cpp (suite index 11).

// --- descriptor ---
static const clap_plugin_descriptor_t s_desc_m467 = {
    CLAP_VERSION,
    "ca.wbsps.m467_noise_gate",
    "Ward-Beck M467 Noise Gate",
    "Ward-Beck Systems Preservation Society",
    "https://wbsps.ca",
    "",
    "https://wbsps.ca/contact",
    "1.0.0",
    "Differential FET noise gate with Fourier cosine attack, sidechain filter, and dual vertical edgewise meters",
    kGateFeatures
};

// --- parameters ---
std::vector<ParameterInfo> buildM467Params() {
    std::vector<ParameterInfo> p;
    p.push_back({
        0, "Threshold", "Dynamics", -50.0, 10.0, -30.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setThresholdDb(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        1, "Attenuation", "Dynamics", 10.0, 60.0, 40.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setAttenuationDb(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f dB", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        2, "Release", "Dynamics", 50.0, 5000.0, 500.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setReleaseSec(static_cast<float>(val * 0.001)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f ms", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        3, "Attack", "Dynamics", 0.1, 20.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setAttackMs(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.1f ms", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        4, "Key HPF", "Sidechain", 20.0, 1000.0, 80.0,
        CLAP_PARAM_IS_AUTOMATABLE,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setKeyHpfFreq(static_cast<float>(val)); },
        nullptr,
        [](double v) { char buf[32]; std::snprintf(buf, sizeof(buf), "%.0f Hz", v); return std::string(buf); },
        nullptr
    });
    p.push_back({
        5, "Gate In", "Master", 0.0, 1.0, 1.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setGateIn(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "ACTIVE" : "BYPASS"; },
        nullptr
    });
    p.push_back({
        6, "Fast Attack", "Dynamics", 0.0, 1.0, 0.0,
        CLAP_PARAM_IS_AUTOMATABLE | CLAP_PARAM_IS_STEPPED,
        [](engine::SharedPluginFrame* f, double val) { static_cast<m467::M467PluginProcessor*>(f)->setFastAttack(val >= 0.5); },
        nullptr,
        [](double v) { return (v >= 0.5) ? "0.2ms FAST" : "NORM"; },
        nullptr
    });
    return p;
}

// --- instance ---
        case 11:
            inst = new ClapPluginInstance(
                &s_desc_m467, host,
                []() { return std::make_unique<m467::M467PluginProcessor>(); },
                buildM467Params(),
                11
            );
            break;

