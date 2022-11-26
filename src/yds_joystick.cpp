#include "../include/yds_joystick.h"
#include <stdio.h>

ysJoystick::ysJoystick() {
    Reset();
     m_x = 0;
    m_y = 0;
    m_z = 0;
    m_rx = 0;
    m_ry = 0;
    m_wheel = 0;

    m_osPosition_x = 0;
    m_osPosition_y = 0;
}

ysJoystick::~ysJoystick() {
    /* void */
}

void ysJoystick::Reset() {
    for (int i = 0; i < (int)Button::Count; ++i) m_buttonStates[i] = ButtonState::Up;

    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_wheel = 0;

    m_osPosition_x = 0;
    m_osPosition_y = 0;
}

void ysJoystick::UpdatePosition(int x, int y, int z, int rx, int ry, bool delta) {
    if (delta) {
        m_x += x;
        m_y += y;
        m_z += z;
        m_rx += rx;
        m_ry += ry;
    }
    else {
        m_x = x;
        m_y = y;
        m_z = z;
        m_rx = rx;
        m_ry = ry;
    }
}

void ysJoystick::UpdatePositionRX(int rx, bool delta) {
    if (delta) {
        m_rx += rx;
    }
    else {
        m_rx = rx;
    }
}

void ysJoystick::UpdatePositionRY(int ry, bool delta) {
    if (delta) {
        m_ry += ry;
    }
    else {
        m_ry = ry;
    }
}

void ysJoystick::UpdateButton(Button button, ButtonState state) {
    m_buttonStates[(int)button] = state;
}

void ysJoystick::SetOsPosition(int x, int y, int z) {
    m_osPosition_x = x;
    m_osPosition_y = y;
    m_osPosition_z = z;
}

bool ysJoystick::IsDown(Button button) const {
    return
        m_buttonStates[(int)button] == ButtonState::DownTransition ||
        m_buttonStates[(int)button] == ButtonState::Down;
}

bool ysJoystick::ProcessJoystickButton(Button button, ButtonState state) {
    ButtonState &currentState = m_buttonStates[(int)button];

    if (currentState != state) return false;

    if (currentState == ButtonState::UpTransition) currentState = ButtonState::Up;
    if (currentState == ButtonState::DownTransition) currentState = ButtonState::Down;

    return true;
}
