#include "rtc.h"
#include "lpc177x_8x_rtc.h"
#include "Globalvalue/GlobalValue.h"
//RTC_TIME_Type RTC_TIME_DISP;
/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RTC interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RTC_IRQHandler(void)
{
//	uint32_t secval;

	/* This is increment counter interrupt*/
	if (RTC_GetIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE))
	{
        RTC_GetFullTime (LPC_RTC, &RTC_TIME_DISP);
        Rtc_intflag=1;

//		/* Send debug information */
//		_DBG ("Second: "); _DBD(secval);
//		_DBG_("");

		// Clear pending interrupt
		RTC_ClearIntPending(LPC_RTC, RTC_INT_COUNTER_INCREASE);
	}
}
void RTC_INT(void)
{
    RTC_Init(LPC_RTC);

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);

	//Set current time = 0
//	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
//    RTC_CalibConfig(LPC_RTC, 5, RTC_CALIB_DIR_FORWARD);
//	RTC_CalibCounterCmd(LPC_RTC, ENABLE);

	/* Set the CIIR for second counter interrupt*/
	RTC_CntIncrIntConfig (LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);

    /* Enable RTC interrupt */
    NVIC_EnableIRQ(RTC_IRQn);


}
