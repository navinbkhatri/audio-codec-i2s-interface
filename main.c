/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Audio Test LM49450 PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "cy_retarget_io.h"
#include "wave.h"

#include "tlv320dac3100.h"


/*******************************************************************************
* Macros
********************************************************************************/
/* Master I2C Settings */
#define MI2C_TIMEOUT_MS     10u         /* in ms */
/* Note that we are not able to achieve the desired frequency, so we round up
*  the frequency values to avoid mismatches */
/* Master Clock (MCLK) Settings */
#define MCLK_FREQ_HZ        10    /* in Hz (Ideally 4.096 MHz) */
#define MCLK_DUTY_CYCLE     50       /* in %  */
/* Clock Settings */
#define AUDIO_SYS_CLOCK_HZ  98304000u   /* in Hz (Ideally 98.304 MHz) */
/* PWM MCLK Pin */
#define MCLK_PIN            P9_0
/* Debounce delay for the button */
#define DEBOUNCE_DELAY_MS   10u         /* in ms */
/* HFCLK1 Clock Divider */
#define HFCLK0_CLK_DIVIDER  1u
#define HFCLK1_CLK_DIVIDER  4u
/*******************************************************************************
* Function Prototypes
********************************************************************************/
void i2s_isr_handler(void *arg, cyhal_i2s_event_t event);
void clock_init(void);

/*******************************************************************************
* Global Variables
********************************************************************************/
/* HAL Objects */
cyhal_pwm_t mclk_pwm;

cyhal_i2c_t mi2c;

cyhal_i2s_t i2s;

cyhal_clock_t audio_clock;
cyhal_clock_t pll_clock;
cyhal_clock_t path_clock;
cyhal_clock_t system_clock;

/* HAL Configs */
const cyhal_i2c_cfg_t mi2c_config = {
    .is_slave        = false,
    .address         = 0x7D,
    .frequencyhal_hz = 50000  // can go upto 400kHz
};

const cyhal_i2s_pins_t i2s_pins = {
    .sck  = P9_1,
    .ws   = P9_2,
    .data = P9_3,
};
const cyhal_i2s_config_t i2s_config = {
    .is_tx_slave    = false,    /* TX is Master */
    .is_rx_slave    = false,    /* RX not used */
    .mclk_hz        = 0,        /* External MCLK not used */
    .channel_length = 32,       /* In bits */
    .word_length    = 16,       /* In bits */
    .sample_rate_hz = 16000,    /* 16000 In Hz */
};

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  The main function for the Cortex-M4 CPU does the following:
*   Initialization:
*   - Initializes all the hardware blocks
*   Do forever loop:
*   - Enters Sleep Mode.
*   - Check if the User Button was pressed. If yes, plays the audio track.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{
    cy_rslt_t result;
    //uint16_t i;
    //int16_t wave_data_o[WAVE_SIZE];
    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    /* Init the clocks */
    clock_init();

    /* Initialize the User LED */
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

    /* Initialize the User Button */
    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
    /* Enable the button interrupt to wake-up the CPU */
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, CYHAL_ISR_PRIORITY_DEFAULT, true);

    /* Initialize the Master Clock with a PWM */
    cyhal_pwm_init(&mclk_pwm, MCLK_PIN, NULL);
    cyhal_pwm_set_duty_cycle(&mclk_pwm, MCLK_DUTY_CYCLE, MCLK_FREQ_HZ);
    cyhal_pwm_start(&mclk_pwm);

    /* Wait for the MCLK to clock the audio codec */
    cyhal_system_delay_ms(1);

    while (1)
    {

    }
    /* Initialize the I2S */
    cyhal_i2s_init(&i2s, &i2s_pins, NULL, &i2s_config, &audio_clock);
    cyhal_i2s_register_callback(&i2s, i2s_isr_handler, NULL);
    cyhal_i2s_enable_event(&i2s, CYHAL_I2S_ASYNC_TX_COMPLETE, CYHAL_ISR_PRIORITY_DEFAULT, true);


    /* Initialize the I2C Master */
    cyhal_i2c_init(&mi2c, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
    cyhal_i2c_configure(&mi2c, &mi2c_config);

    /* Configure LM49450 and enable it */
    DAC3100_init(&mi2c);
    DAC3100_speaker_volume(0x18);
    DAC3100_headphone_volume(0x1F);
/*
	for(i=0; i<WAVE_SIZE-1; i+=2)
    {
		wave_data_o[i] = (wave_data[i]+wave_data[i+1])/2 ;

		wave_data_o[i+1] = wave_data_o[i] ;

	}  */
    for (;;)
    {
    	//cyhal_syspm_sleep();

		/* Check if the button was pressed */
		if (cyhal_gpio_read(CYBSP_USER_BTN) == CYBSP_BTN_PRESSED)
		{
			/* Check if I2S is transmitting */
			if (cyhal_i2s_is_write_pending(&i2s))
			{
				/* If already transmitting, don't do anything */
			}
			else
			{
				/* Start the I2S TX */
				cyhal_i2s_start_tx(&i2s);

				/* If not transmitting, initiate a transfer */
				cyhal_i2s_write_async(&i2s, wave_data, WAVE_SIZE);

				/* Turn ON LED to show a transmission */
				cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
			}

			/* Debounce delay */
			cyhal_system_delay_ms(DEBOUNCE_DELAY_MS);
		}
    }
}
/*******************************************************************************
* Function Name: i2s_isr_handler
********************************************************************************
* Summary:
*  I2S ISR handler. Stop the I2S TX and turn OFF the User LED.
*
* Parameters:
*  arg: not used
*  event: event that occurred
*
*******************************************************************************/
void i2s_isr_handler(void *arg, cyhal_i2s_event_t event)
{
	(void) arg;
	(void) event;

	/* Stop the I2S TX */
	cyhal_i2s_stop_tx(&i2s);

	/* Turn off the LED */
	cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
}

/*******************************************************************************
* Function Name: clock_init
********************************************************************************
* Summary:
*  Initialize the clocks in the system.
*
*******************************************************************************/
void clock_init(void)
{
	/* Initialize the Pathmux */
	cyhal_clock_reserve(&path_clock, &CYHAL_CLOCK_PATHMUX[1]);
	cyhal_clock_set_source(&path_clock, &CYHAL_CLOCK_IMO);
	cyhal_clock_set_enabled(&path_clock, true, true);

	/* Initialize PLL */
    cyhal_clock_reserve(&pll_clock, &CYHAL_CLOCK_PLL[0]);
    cyhal_clock_set_source(&pll_clock, &CYHAL_CLOCK_IMO);
    cyhal_clock_set_frequency(&pll_clock, AUDIO_SYS_CLOCK_HZ, NULL);
    cyhal_clock_set_enabled(&pll_clock, true, true);

    /* Initialize system clock (HFCLK0) */
    cyhal_clock_reserve(&system_clock, &CYHAL_CLOCK_HF[0]);
    cyhal_clock_set_source(&system_clock, &pll_clock);
	cyhal_clock_set_divider(&system_clock, HFCLK0_CLK_DIVIDER);
	cyhal_clock_set_enabled(&system_clock, true, true);

	/* Initialize the audio subsystem clock (HFCLK1) */
    cyhal_clock_reserve(&audio_clock, &CYHAL_CLOCK_HF[1]);
    cyhal_clock_set_source(&audio_clock, &pll_clock);
    cyhal_clock_set_divider(&audio_clock, HFCLK1_CLK_DIVIDER);
	cyhal_clock_set_enabled(&audio_clock, true, true);


}

/* [] END OF FILE */
