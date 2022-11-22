#include "../include/yds_joystick_aggregator.h"

ysJoystickAggregator::ysJoystickAggregator() {
    /* void */
}

ysJoystickAggregator::~ysJoystickAggregator() {
    /* void */
}

void ysJoystickAggregator::RegisterJoystick(ysJoystick *joystick) {
    m_joystick.New() = joystick;
}

void ysJoystickAggregator::DeleteJoystick(ysJoystick *joystick) {
    m_joystick.Delete(m_joystick.Find(joystick));
}

int ysJoystickAggregator::GetOsPositionX() const {
    if (m_joystick.GetNumObjects() == 0) return 0;

    return m_joystick[0]->GetOsPositionX();
}

int ysJoystickAggregator::GetOsPositionY() const {
    if (m_joystick.GetNumObjects() == 0) return 0;

    return m_joystick[0]->GetOsPositionY();
}

int ysJoystickAggregator::GetOsPositionZ() const {
    if (m_joystick.GetNumObjects() == 0) return 0;

    return m_joystick[0]->GetOsPositionZ();
}

int ysJoystickAggregator::GetX() const {
    int aggregateX = 0;
    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        aggregateX += m_joystick[i]->GetX();
    }

    return aggregateX;
}

int ysJoystickAggregator::GetY() const {
    int aggregateY = 0;
    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        aggregateY += m_joystick[i]->GetY();
    }

    return aggregateY;
}

int ysJoystickAggregator::GetZ() const {
    int aggregateZ = 0;
    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        aggregateZ += m_joystick[i]->GetZ();
    }

    return aggregateZ;
}

int ysJoystickAggregator::GetWheel() const {
    int aggregateWheel = 0;
    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        aggregateWheel += m_joystick[i]->GetWheel();
    }

    return aggregateWheel;
}

bool ysJoystickAggregator::IsDown(Button button) const {
    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        if (m_joystick[i]->IsDown(button)) return true;
    }

    return false;
}

bool ysJoystickAggregator::ProcessJoystickButton(Button button, ButtonState state) {
    bool result = false;

    const int joystickCount = m_joystick.GetNumObjects();
    for (int i = 0; i < joystickCount; ++i) {
        result = result || m_joystick[i]->ProcessJoystickButton(button, state);
    }

    return result;
}
