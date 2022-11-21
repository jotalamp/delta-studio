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
                //case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: mouse->UpdateWheel(controllerEvent.value/32800.0f - mouse->GetWheel()); break;
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT: joystick->UpdatePosition(controllerEvent.value, joystick->GetY(), joystick->GetZ(), false); break;
                case SDL_CONTROLLER_AXIS_LEFTX       : joystick->UpdatePosition(joystick->GetX(), joystick->GetY(), controllerEvent.value, false); break;
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT : joystick->UpdatePosition(joystick->GetX(), controllerEvent.value, joystick->GetZ(), false); break;
                //case SDL_CONTROLLER_AXIS_TRIGGERLEFT : joystick->UpdatePositionZ(controllerEvent.value, false); break;
                //case SDL_CONTROLLER_AXIS_LEFTX       : joystick->UpdatePositionZ(controllerEvent.value, false); break;
                //case SDL_CONTROLLER_AXIS_LEFTX: mouse->UpdatePosition(controllerEvent.value/32000.0f, mouse->GetY()); break;
                //case SDL_CONTROLLER_AXIS_LEFTY: mouse->UpdatePosition(mouse->GetX(), controllerEvent.value); break;
            }
            break;
        }
        /*
        case SDL_JOYAXISMOTION: {
            ysMouse *mouse = GetMouse();
            auto &joystickEvent = event.jaxis;
            switch(joystickEvent.axis) {
                case 5: mouse->UpdateWheel(0.2f * joystickEvent.value - mouse->GetWheel()); break;
                case 2: mouse->UpdatePosition(joystickEvent.value/32000.0f, mouse->GetY()); break;
                case 1: mouse->UpdatePosition(mouse->GetY(), joystickEvent.value); break;
            }
            break;
        }*/
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        {
            auto &joystickEvent = event.cbutton;
            //ysMouse *mouse = GetMouse();
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
                //printf("Button %i pressed \n", joystickEvent.button);
                joystick->UpdateButton(button, ysJoystick::ButtonState::DownTransition);
            }

            if(joystickEvent.state == SDL_RELEASED){
                //printf("Button %i released\n", joystickEvent.button);
                joystick->UpdateButton(button, ysJoystick::ButtonState::UpTransition);
            }
            break;
        }
        /*
        case SDL_JOYBUTTONUP:
        case SDL_JOYBUTTONDOWN:
        {
            auto &joystickEvent = event.jbutton;
            //auto &joystickEvent = event.cbutton;
            ysKeyboard *keyboard = GetKeyboard();
            ysMouse *mouse = GetMouse();
            
            ysMouse::Button button;
            switch(joystickEvent.button) {
                case  0: button = ysMouse::Button::Aux_00; break;
                case  1: button = ysMouse::Button::Aux_01; break;
                case  2: button = ysMouse::Button::Aux_02; break;
                case  3: button = ysMouse::Button::Aux_03; break;
                case  4: button = ysMouse::Button::Aux_04; break;
                case  5: button = ysMouse::Button::Aux_05; break;
                case  6: button = ysMouse::Button::Aux_06; break;
                case  7: button = ysMouse::Button::Aux_07; break;
                case  8: button = ysMouse::Button::Aux_08; break;
                case  9: button = ysMouse::Button::Aux_09; break;
                case 10: button = ysMouse::Button::Aux_10; break;
            }

            if(joystickEvent.state == SDL_PRESSED) {
                printf("Button %i pressed \n", joystickEvent.button);
                mouse->UpdateButton(button, ysMouse::ButtonState::DownTransition);
            }

            if(joystickEvent.state == SDL_RELEASED){
                printf("Button %i released\n", joystickEvent.button);
                mouse->UpdateButton(button, ysMouse::ButtonState::UpTransition);
            }
            break;
        }*/
        

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
