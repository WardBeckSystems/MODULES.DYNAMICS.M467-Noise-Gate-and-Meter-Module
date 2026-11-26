// M467: its block of 2030.env/vst/core/CircuitDiagramDefinitions.hpp.

        case 11: // M467 Noise Gate & Meter Module
            d.modelCode = "M467";
            d.circuitTitle = "Ward-Beck M467 Noise Gate & Dynamic Expander";
            d.theoreticalExplanation =
                "Solid-state console noise gate featuring differential phase-cancellation FET attenuation, "
                "frequency-selective sidechain keying with continuous HPF, anti-click Fourier cosine attack shaper, "
                "and authentic dual vertical edgewise meters displaying gate attenuation and audio level.";
            d.nodes = {
                { "KEY_HPF", "Sidechain HPF", "20-1000Hz Cutoff", { 15, 20, 95, 48 }, true, Palette::CircuitBorder },
                { "DET",     "FET Slew Det",  "Envelope S-Curve", { 125, 20, 95, 48 }, true, Palette::TextGold },
                { "GATE_VCA","VCA Atten",     "-50dB FET Bridge", { 235, 20, 95, 48 }, true, Palette::CircuitHighlight },
                { "METER_ATT","Atten Meter",  "Edgewise 0..-50dB",{ 345, 12, 95, 36 }, true, Palette::LedAmber },
                { "METER_VU", "Output VU",    "Edgewise Level",   { 345, 52, 95, 36 }, true, Palette::LedGreen },
                { "OUT_AMP", "Line Driver",   "Discrete Driver",  { 455, 20, 90, 48 }, true, Palette::CopperWire }
            };
            d.wires = {
                { "KEY_HPF", "DET", "Filtered Key" },
                { "DET", "GATE_VCA", "Control Voltage" },
                { "GATE_VCA", "METER_ATT", "Gain Reduction" },
                { "GATE_VCA", "OUT_AMP", "Gated Audio" },
                { "OUT_AMP", "METER_VU", "Output Level" }
            };
            break;
