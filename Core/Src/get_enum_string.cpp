#include "main.h"
#include "RC.h"
#include "usart.h"
//this function returns the state of switch and mouse with string instead of enum
const char* get_switch_state_string(RC::RCSwitchState_e state)
{
    switch (state)
    {
        case RC::UP:
            return "UP";
        case RC::DOWN:
            return "DOWN";
        case RC::MID:
            return "MID";
        default:
            return "UNKNOWN";
    }

}
const char* get_mouse_press_state_string(RC::MousePressState_e state)
{
    switch (state)
    {
        case RC::PRESS_MOUSE:
            return "PRESSED";
        case RC::NON_PRESS_MOUSE:
            return "RELEASED";
        default:
            return "UNKNOWN";
    }
}
const char* get_key_press_state_string(RC::KeyPressState_e state)
{
    switch (state)
    {
        case RC::PRESS_KEY:
            return "PRESSED";
        case RC::N0N_PRESS_KEY:
            return "RELEASED";
         default:
            return "UNKNOWN";
    }
}