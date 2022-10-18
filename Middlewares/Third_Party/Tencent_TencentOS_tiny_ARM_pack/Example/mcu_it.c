#include "tos_k.h"

/*
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
*/
void SysTick_Handler(void)
{
  if (tos_knl_is_running())
  {
    tos_knl_irq_enter();
    tos_tick_handler();
    tos_knl_irq_leave();
  }
}
