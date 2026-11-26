// M467: its block of 2030.env/vst/core/ModuleGuiLayouts.hpp (176 x 770 = the faceplate).

        case 11: { // M467 Noise Gate & Meter Module (176 x 770, Dual Vertical Edgewise Meters)
            m.title = "M467 Noise Gate";
            m.subtitle = "FET Slew-Shaped Noise Gate & Dual Vertical Edgewise Meters";
            m.modelCode = "M467";

            // Dual Vertical Edgewise Meters (Left: Attenuation dB dropping from 0 to -50; Right: Output VU Level)
            m.hasEdgewiseMeters = true;
            m.edgewiseMeter1Rect = { 32.0f, 65.0f, 46.0f, 130.0f };
            m.edgewiseMeter2Rect = { 98.0f, 65.0f, 46.0f, 130.0f };
            m.edgewiseMeter1Label = "ATTEN dB";
            m.edgewiseMeter2Label = "LEVEL VU";

            // Knobs
            m.knobs.push_back({ 0, "THRESHOLD", "dB", 88.0f, 245.0f, 22.0f, -50.0, 10.0, -30.0, false, KnobStyle::WbsElmaWing, Palette::ElmaRed });
            m.knobs.push_back({ 1, "ATTEN", "dB", 88.0f, 320.0f, 22.0f, 10.0, 60.0, 40.0, false, KnobStyle::WbsElmaPointer, Palette::ElmaSlate });
            m.knobs.push_back({ 2, "RELEASE", "ms", 88.0f, 395.0f, 20.0f, 50.0, 5000.0, 500.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaAmber });
            m.knobs.push_back({ 3, "ATTACK", "ms", 88.0f, 470.0f, 20.0f, 0.1, 20.0, 1.0, false, KnobStyle::WbsElmaRegular, Palette::ElmaGreen });
            m.knobs.push_back({ 4, "KEY HPF", "Hz", 88.0f, 545.0f, 22.0f, 20.0, 1000.0, 80.0, false, KnobStyle::WbsElmaWing, Palette::ElmaBlue });

            // Buttons
            m.buttons.push_back({ 5, "GATE IN", { 43.0f, 615.0f, 90.0f, 26.0f }, true, ButtonStyle::LatchingPush, Palette::LedGreen });
            m.buttons.push_back({ 6, "FAST ATK", { 43.0f, 655.0f, 90.0f, 24.0f }, false, ButtonStyle::LatchingPush, Palette::LedAmber });

            m.themeToggleRect = { 25.0f, 726.0f, 126.0f, 22.0f };
            m.bottomScrewRect = { 74.0f, 742.0f, 28.0f, 28.0f };
            break;
        }
