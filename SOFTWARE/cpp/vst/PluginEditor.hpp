#pragma once
#include "PluginProcessor.hpp"

namespace wbs::m467 {

class M467PluginEditor {
public:
    explicit M467PluginEditor(M467PluginProcessor& processor);
    ~M467PluginEditor() = default;

private:
    M467PluginProcessor& processor_;
};

} // namespace wbs::m467
