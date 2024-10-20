#include "main.h"
#include "RC.h"
#include <cstdio>
#include <cstring>
#include "usart.h"
RC::RC()
{
    for(uint8_t & i : rx_buf_)
    {
        i=0;
    }
    for(uint8_t & i : rx_data_)
    {
        i=0;
    }
    RCswitch_.l=MID;
    RCswitch_.r=MID;
    RCChannel_.l_col=1024;
    RCChannel_.r_col=1024;
    RCChannel_.l_row=1024;
    RCChannel_.r_row=1024;
    MousePos_.x=0;
    MousePos_.y=0;
    MousePos_.z=0;
    MousePre_.L=NON_PRESS_MOUSE;
    MousePre_.R=NON_PRESS_MOUSE;
}
uint8_t* RC::Get_buffer()
{
    return this->rx_buf_;
}
uint8_t* RC::Get_data()
{
    return this->rx_data_;
}
float RC::Get_CH0_r_row()
{
    uint32_t origin_data_CH0= (this->Get_data()[0])|(this->Get_data()[1]<<8)&0x07ff;
    return linearmap(origin_data_CH0,RC_CH_VALUE_MIN,RC_CH_VALUE_MAX,-1,1);
}
float RC::Get_CH1_r_col()
{
    uint32_t origin_data_CH1 = (((this->Get_data()[1])>>3)|((this->Get_data()[2]))<<5)&0x07ff;
    return linearmap(origin_data_CH1,RC_CH_VALUE_MIN,RC_CH_VALUE_MAX,-1,1);
}

float RC::Get_CH2_l_row()
{
    uint32_t origin_data_CH2=(((this->Get_data()[2]>>6)|(this->Get_data()[3]<<2))|(this->Get_data()[4]<<10))&0x07ff;
    return linearmap(origin_data_CH2,RC_CH_VALUE_MIN,RC_CH_VALUE_MAX,-1,1);
}
float RC::Get_CH3_l_col()
{
    uint32_t origin_data_CH3=((this->Get_data()[4]>>1)|(this->Get_data()[5]<<7))&0x07ff;
    return linearmap(origin_data_CH3,RC_CH_VALUE_MIN,RC_CH_VALUE_MAX,-1,1);
}
RC::RCSwitchState_e RC::Get_RCSwitch_l()
{
    uint8_t origin_data_Switch_l=(this->Get_data()[5]>>4)&0x03;
    return static_cast<RCSwitchState_e>(origin_data_Switch_l);
}
RC::RCSwitchState_e RC::Get_RCSwitch_r()
{
    uint8_t origin_data_Switch_r=(this->Get_data()[5]>>6);
    return static_cast<RCSwitchState_e>(origin_data_Switch_r);
}
float RC::Get_MousePos_x()
{
    uint32_t origin_data_MousePos_x = ((this->Get_data()[6])|(this->Get_data()[7]<<8));
    return linearmap(origin_data_MousePos_x,RC_MOUSE_VALUE_MIN,RC_MOUSE_VALUE_MAX,-1,1);
}
float RC::Get_MousePos_y()
{
    uint32_t origin_data_MousePos_y = ((this->Get_data()[8])|(this->Get_data()[9]<<8));
    return linearmap(origin_data_MousePos_y,RC_MOUSE_VALUE_MIN,RC_MOUSE_VALUE_MAX,-1,1);
}
float RC::Get_MousePos_z()
{
    uint32_t origin_data_MousePos_z = ((this->Get_data()[10])|(this->Get_data()[11]<<8));
    return linearmap(origin_data_MousePos_z,RC_MOUSE_VALUE_MIN,RC_MOUSE_VALUE_MAX,-1,1);
}
RC::MousePressState_e RC::Get_MousePre_L()
{
    uint8_t origin_data_MousePre_L = this->Get_data()[12];
    return static_cast<MousePressState_e>(origin_data_MousePre_L);
}
RC::MousePressState_e RC::Get_MousePre_R()
{
    uint8_t origin_data_MousePre_R = this->Get_data()[13];
    return static_cast<MousePressState_e>(origin_data_MousePre_R);
}
RC::KeyPressState_e RC::Get_Keypress_W()
{
    uint8_t origin_data_Keypress_W = this->Get_data()[14]&0x01;
    return static_cast<KeyPressState_e>(origin_data_Keypress_W);
}
RC::KeyPressState_e RC::Get_Keypress_S()
{
    uint8_t origin_data_Keypress_S = (this->Get_data()[14]&0x02)>>1;
    return static_cast<KeyPressState_e>(origin_data_Keypress_S);
}
RC::KeyPressState_e RC::Get_Keypress_A()
{
    uint8_t origin_data_Keypress_A = (this->Get_data()[14]&0x04)>>2;
    return static_cast<KeyPressState_e>(origin_data_Keypress_A);
}
RC::KeyPressState_e RC::Get_Keypress_D()
{
    uint8_t origin_data_Keypress_D = (this->Get_data()[14]&0x08)>>3;
    return static_cast<KeyPressState_e>(origin_data_Keypress_D);
}
RC::KeyPressState_e RC::Get_Keypress_Q()
{
    uint8_t origin_data_Keypress_Q = (this->Get_data()[14]&0x10)>>4;
    return static_cast<KeyPressState_e>(origin_data_Keypress_Q);
}
RC::KeyPressState_e RC::Get_Keypress_E()
{
    uint8_t origin_data_Keypress_E = (this->Get_data()[14]&0x20)>>5;
    return static_cast<KeyPressState_e>(origin_data_Keypress_E);
}
RC::KeyPressState_e RC::Get_Keypress_Shift()
{
    uint8_t origin_data_Keypress_Shift = (this->Get_data()[14]&0x40)>>6;
    return static_cast<KeyPressState_e>(origin_data_Keypress_Shift);
}
RC::KeyPressState_e RC::Get_Keypress_Ctrl()
{
    uint8_t origin_data_Keypress_Ctrl = (this->Get_data()[14]&0x80)>>7;
    return static_cast<KeyPressState_e>(origin_data_Keypress_Ctrl);
}
void RC::Transmit_Data()
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "The CH0_Position is: %.4f\r\n", Get_CH0_r_row());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The CH1_Position is: %.4f\r\n", Get_CH1_r_col());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The CH2_Position is: %.4f\r\n", Get_CH2_l_row());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The CH3_Position is: %.4f\r\n", Get_CH3_l_col());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The CH3_Position is: %.4f\r\n", Get_CH3_l_col());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The left switch is: %s\r\n", get_switch_state_string(Get_RCSwitch_l()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The right switch is: %s\r\n", get_switch_state_string(Get_RCSwitch_r()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The Mouse_y_Position is: %.4f\r\n", Get_MousePos_y());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The Mouse_z_Position is: %.4f\r\n", Get_MousePos_z());
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The mouse left press is: %s\r\n", get_mouse_press_state_string(Get_MousePre_L()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The mouse right press is: %s\r\n", get_mouse_press_state_string(Get_MousePre_R()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key W is: %s\r\n", get_key_press_state_string(Get_Keypress_W()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key S is: %s\r\n", get_key_press_state_string(Get_Keypress_S()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key A is: %s\r\n", get_key_press_state_string(Get_Keypress_A()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key F is: %s\r\n", get_key_press_state_string(Get_Keypress_D()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key Q is: %s\r\n", get_key_press_state_string(Get_Keypress_Q()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key E is: %s\r\n", get_key_press_state_string(Get_Keypress_E()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key Shift is: %s\r\n", get_key_press_state_string(Get_Keypress_Shift()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
    snprintf(buffer, sizeof(buffer), "The key Ctrl is: %s\r\n", get_key_press_state_string(Get_Keypress_Ctrl()));
    HAL_UART_Transmit_DMA(&huart1, reinterpret_cast<uint8_t*>(buffer), strlen(buffer));
}