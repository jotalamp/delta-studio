#ifndef YDS_JOYSTICK_AGGREGATOR_H
#define YDS_JOYSTICK_AGGREGATOR_H

#include "yds_joystick.h"

#include "yds_expanding_array.h"

class ysJoystickAggregator : public ysJoystick {
public:
    ysJoystickAggregator();
    virtual ~ysJoystickAggregator();

    void RegisterJoystick(ysJoystick *joystick);
    void DeleteJoystick(ysJoystick *joystick);
    int GetJoystickCount() const { return m_joystick.GetNumObjects(); }
    ysJoystick *GetJoystick(int index) { return m_joystick[index]; }

    virtual int GetOsPositionX() const;
    virtual int GetOsPositionY() const;
    virtual int GetOsPositionZ() const;

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetZ() const;
    virtual int GetRX() const;
    virtual int GetRY() const;
    virtual int GetWheel() const;
    virtual bool IsDown(Button button) const;

    virtual bool ProcessJoystickButton(Button button, ButtonState state);

protected:
    ysExpandingArray<ysJoystick *, 4> m_joystick;
};

#endif /* YDS_JOYSTICK_AGGREGATOR_H */
