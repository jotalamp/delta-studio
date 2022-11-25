#include "../include/yds_sdl_input_system.h"

#include "../include/yds_sdl_input_device.h"
#include "../include/yds_key_maps.h"

#include <SDL2/SDL.h>

ysSdlInputSystem::ysSdlInputSystem() : ysInputSystem(Platform::Sdl) {
    /* void */
}

ysSdlInputSystem::~ysSdlInputSystem() {
    /* void */
}

void ysSdlInputSystem::ProcessEvent(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            auto &keyboardEvent = event.key;
            ysKeyboard *keyboard = GetKeyboard();

            ysKey::Code index = keyboard->GetKeyMap(keyboardEvent.keysym.scancode);
            const ysKey *key = keyboard->GetKey(index);

            ysKey::Variation newConf = key->m_configuration;
            ysKey::State newState = event.type == SDL_KEYDOWN
                ? ysKey::State::DownTransition
                : ysKey::State::UpTransition;

            keyboard->SetKeyState(index, newState, newConf);
            break;
        }

        case SDL_MOUSEMOTION: {
            auto &motionEvent = event.motion;
            ysMouse *mouse = GetMouse();

            mouse->UpdatePosition(motionEvent.x, motionEvent.y, false);
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            auto &buttonEvent = event.button;
            ysMouse *mouse = GetMouse();

            ysMouse::Button button;
            switch (buttonEvent.button) {
                case SDL_BUTTON_LEFT:   button = ysMouse::Button::Left;   break;
                case SDL_BUTTON_RIGHT:  button = ysMouse::Button::Right;  break;
                case SDL_BUTTON_MIDDLE: button = ysMouse::Button::Middle; break;
                case SDL_BUTTON_X1:     button = ysMouse::Button::Aux_01;  break;
                case SDL_BUTTON_X2:     button = ysMouse::Button::Aux_02;  break;
                default: return; // We don't know how to handle this button so bail
            }

            auto newState = event.type == SDL_MOUSEBUTTONDOWN
                ? ysMouse::ButtonState::DownTransition
                : ysMouse::ButtonState::UpTransition;

            mouse->UpdateButton(button, newState);
            break;
        }

        case SDL_MOUSEWHEEL: {
            auto &wheelEvent = event.wheel;
            ysMouse *mouse = GetMouse();

            // Match the Windows impl by scaling by WHEEL_DELTA
            const int WHEEL_DELTA = 120;
            mouse->UpdateWheel(wheelEvent.y * WHEEL_DELTA);
            break;
        }
        case SDL_CONTROLLERAXISMOTION: {
            ysJoystick *joystick = GetJoystick();
            auto &controllerEvent = event.caxis;
            switch(controllerEvent.axis) {
                case SDL_CONTROLLER_AXIS_RIGHTX         : joystick->UpdatePositionRX(controllerEvent.value, false); break;
                case SDL_CONTROLLER_AXIS_RIGHTY         : joystick->UpdatePositionRY(controllerEvent.value, false); break;
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT   : joystick->UpdatePosition(controllerEvent.value,   joystick->GetY(),   joystick->GetZ(),       joystick->GetRX(),      joystick->GetRY(),      false); break;
                case SDL_CONTROLLER_AXIS_LEFTX          : joystick->UpdatePosition(joystick->GetX(),        joystick->GetY(),   controllerEvent.value,  joystick->GetRX(),      joystick->GetRY(),      false); break;
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT    : joystick->UpdatePosition(joystick->GetX(),    controllerEvent.value,  joystick->GetZ(),       joystick->GetRX(),      joystick->GetRY(),      false); break;
            }
            break;
        }

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        {
            auto &joystickEvent = event.cbutton;
            ysJoystick *joystick = GetJoystick();
            
            ysJoystick::Button button;
            switch(joystickEvent.button) {
                case SDL_CONTROLLER_BUTTON_A:               button = ysJoystick::Button::Button_A; break;
                case SDL_CONTROLLER_BUTTON_B:               button = ysJoystick::Button::Button_B; break;
                case SDL_CONTROLLER_BUTTON_X:               button = ysJoystick::Button::Button_X; break;
                case SDL_CONTROLLER_BUTTON_Y:               button = ysJoystick::Button::Button_Y; break;
                case SDL_CONTROLLER_BUTTON_BACK:            button = ysJoystick::Button::Button_Back; break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:       button = ysJoystick::Button::Button_Left; break;
                case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:    button = ysJoystick::Button::Button_L1; break;
                case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:   button = ysJoystick::Button::Button_R1; break;
                case SDL_CONTROLLER_BUTTON_START:           button = ysJoystick::Button::Button_Start; break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:       button = ysJoystick::Button::Button_Down; break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:         button = ysJoystick::Button::Button_Up; break;
            }

            if(joystickEvent.state == SDL_PRESSED) {
                joystick->UpdateButton(button, ysJoystick::ButtonState::DownTransition);
            }

            if(joystickEvent.state == SDL_RELEASED){
                joystick->UpdateButton(button, ysJoystick::ButtonState::UpTransition);
            }
            break;
        }
        default:
            // Ignore it
            break;
    }
}

ysError ysSdlInputSystem::CheckDeviceStatus(ysInputDevice *device) {
    YDS_ERROR_DECLARE("CheckDeviceStatus");

    return YDS_ERROR_RETURN(ysError::None);
}

ysError ysSdlInputSystem::CheckAllDevices() {
    YDS_ERROR_DECLARE("CheckAllDevices");

    return YDS_ERROR_RETURN(ysError::None);
}

ysError ysSdlInputSystem::CreateDevices() {
    YDS_ERROR_DECLARE("CreateDevices");

    return YDS_ERROR_RETURN(ysError::None);
}

ysInputDevice *ysSdlInputSystem::CreateDevice(ysInputDevice::InputDeviceType type, int id) {
    ysSdlInputDevice *newDevice = m_inputDeviceArray.NewGeneric<ysSdlInputDevice>();

    const char *baseName;
    switch (type) {
    case ysInputDevice::InputDeviceType::CUSTOM:
        baseName = "HID";
        break;
    case ysInputDevice::InputDeviceType::KEYBOARD:
        baseName = "KEYBOARD";
        break;
    case ysInputDevice::InputDeviceType::MOUSE:
        baseName = "MOUSE";
        break;
    case ysInputDevice::InputDeviceType::JOYSTICK:
        baseName = "JOYSTICK";
        break;
    default:
        baseName = "";
    }

    char deviceName[128];
    snprintf(deviceName, sizeof(deviceName), "%s%0.3d", baseName, id);

    newDevice->SetName(deviceName);
    newDevice->SetType(type);
    newDevice->SetDeviceID(id);
    newDevice->SetGeneric(true);

    if (type == ysInputDevice::InputDeviceType::KEYBOARD) {
        // Load Keymap
        ysKeyboard *keyboard = newDevice->GetAsKeyboard();
        keyboard->RegisterKeyMap(ysKeyMaps::GetSdlKeyMap());
    }

    RegisterDevice(newDevice);

    return newDevice;
}

ysKeyboard *ysSdlInputSystem::GetKeyboard() {
    // Lazy, assume there's only 1 keyboard
    auto *keyboard = GetInputDevice(0, ysInputDevice::InputDeviceType::KEYBOARD);
    return keyboard->GetAsKeyboard();
}

ysMouse *ysSdlInputSystem::GetMouse() {
    // Lazy, assume there's only 1 mouse
    auto *device = GetInputDevice(1, ysInputDevice::InputDeviceType::MOUSE);
    return device->GetAsMouse();
}

ysJoystick *ysSdlInputSystem::GetJoystick() {
    // Lazy, assume there's only 1 joystick
    auto *device = GetInputDevice(1, ysInputDevice::InputDeviceType::JOYSTICK);
    return device->GetAsJoystick();
}
