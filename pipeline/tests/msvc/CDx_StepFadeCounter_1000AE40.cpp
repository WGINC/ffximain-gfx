
class CDx { public: bool StepFadeCounter(float a, float b, int mode); char m_pad[0x174]; float m_fade; };
bool CDx::StepFadeCounter(float a, float b, int mode)
{
    bool result = false;
    switch (mode) {
    case 0: m_fade = 128.0f; break;
    case 1: {
        float d; if ((d = a * b) <= 0.0f) d = 0.01f; m_fade -= d;
        if (m_fade <= 0.0f) { m_fade = 0.0f; result = true; }
        break; }
    }
    return result;
}
