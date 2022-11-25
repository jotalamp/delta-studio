#ifndef YDS_JOYSTICK_H
#define YDS_JOYSTICK_H

class ysJoystick {
public:
    enum class Button {
        Button_A,
        Button_B,
        Button_X,
        Button_Y,
        Button_R1,
        Button_L1,
        Button_Left,
        Button_Right,
        Button_Up,
        Button_Down,
        Button_Back,
        Button_Start,

        Count
    };

    enum class ButtonState {
        Up,
        Down,

        UpTransition,
        DownTransition
    };

public:
    ysJoystick();
    virtual ~ysJoystick();

    void Reset();
    void UpdatePosition(int x, int y, int z, int rx, int ry, bool delta = true);
    void UpdatePositionRX(int rx, bool delta = true);
    void UpdatePositionRY(int ry, bool delta = true);
    void UpdateButton(Button button, ButtonState state);
    void SetOsPosition(int x, int y, int z);

    virtual int GetOsPositionX() const { return m_osPosition_x; }
    virtual int GetOsPositionY() const { return m_osPosition_y; }
    virtual int GetOsPositionZ() const { return m_osPosition_z; }

    virtual int GetX() const { return m_x; }
    virtual int GetY() const { return m_y; }
    virtual int GetZ() const { return m_z; }
    virtual int GetRX() const { return m_rx; }
    virtual int GetRY() const { return m_ry; }
    virtual int GetWheel() const { return m_wheel; }
    virtual bool IsDown(Button button) const;

    virtual bool ProcessJoystickButton(Button button, ButtonState state);

protected:
    int m_x;
    int m_y;
    int m_z;
    int m_rx;
    int m_ry;
    int m_wheel;

    int m_osPosition_x;
    int m_osPosition_y;
    int m_osPosition_z;

protected:
    ButtonState m_buttonStates[(int)Button::Count];
};

#endif /* YDS_JOYSTICK_H */
