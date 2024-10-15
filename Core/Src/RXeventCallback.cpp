#include "main.h"
#include "RC.h"
#include "usart.h"
extern RC rc;
extern uint16_t data_index;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        HAL_DMA_Abort(huart->hdmarx);
        for(uint16_t i = 0; i < Size; i++)
        {
            if(data_index<RC_RX_DATA_SIZE)
            {
                rc.Get_data()[data_index++]=rc.Get_buffer()[i];
            }
            else
            {
                data_index-=18;
                rc.Get_data()[data_index++]=rc.Get_buffer()[i];
            }
        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1,rc.Get_buffer(), sizeof(rc.Get_buffer()));
    }
}
