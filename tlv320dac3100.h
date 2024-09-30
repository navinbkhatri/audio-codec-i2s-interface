
#ifndef TLV320DAC3100_H
#define TLV320DAC3100_H

#include <stdint.h>
#include "cyhal_i2c.h"


/* ****************** Page 0 Registers **************************************/
/* Page select register */
#define	PAGE_SELECT			    0
/* Software reset register */
#define	RESET				    1
/* OT FLAG register */
#define OT_FLAG				    3
/* Clock clock Gen muxing, Multiplexers*/
#define	CLK_REG_1			    4
/* PLL P and R-VAL register*/
#define	CLK_REG_2			    5
/* PLL J-VAL register*/
#define	CLK_REG_3			    6
/* PLL D-VAL MSB register */
#define	CLK_REG_4			    7
/* PLL D-VAL LSB register */
#define	CLK_REG_5			    8
/* DAC NDAC_VAL register*/
#define	NDAC_CLK_REG_6		        11
/* DAC MDAC_VAL register */
#define	MDAC_CLK_REG_7		        12
/*DAC OSR setting register1,MSB value*/
#define DAC_OSR_MSB			        13
/*DAC OSR setting register 2,LSB value*/
#define DAC_OSR_LSB			        14
/*Clock setting register 8, PLL*/
#define	NADC_CLK_REG_8		        18
/*Clock setting register 9, PLL*/
#define	MADC_CLK_REG_9		        19
/*ADC Oversampling (AOSR) Register*/
#define ADC_OSR_REG			20
/*Clock setting register 9, Multiplexers*/
#define CLK_MUX_REG_9		        25
/*Clock setting register 10, CLOCKOUT M divider value*/
#define CLK_REG_10			        26
/*Audio Interface Setting Register 1*/
#define INTERFACE_SET_REG_1	        27
/*Audio Interface Setting Register 2*/
#define AIS_REG_2			        28
/*Audio Interface Setting Register 3*/
#define AIS_REG_3			        29
/*Clock setting register 11,BCLK N Divider*/
#define CLK_REG_11			        30
/*Audio Interface Setting Register 4,Secondary Audio Interface*/
#define AIS_REG_4			        31
/*Audio Interface Setting Register 5*/
#define AIS_REG_5			        32
/*Audio Interface Setting Register 6*/
#define AIS_REG_6			        33
/* I2C Bus Condition */
#define I2C_FLAG				    34
/* DAC Flag Registers */
#define DAC_FLAG_1				    37
#define DAC_FLAG_2				    38
/* Interrupt flag (overflow) */
#define OVERFLOW_FLAG				39
/* Interrupt flags (DAC */
#define INTR_FLAG_1				    44
#define INTR_FLAG_2				    46
/* INT1 interrupt control */
#define INT1_CTRL				    48
/* INT2 interrupt control */
#define INT2_CTRL				    49
/* GPIO1 control */
#define GPIO1_CTRL				    51

 /**/
/*DOUT Function Control*/
#define DOUT_CTRL				    53
/*DIN Function Control*/
#define DIN_CTL					    54
/*DAC Instruction Set Register*/
#define DAC_INSTRUCTION_SET			60
/*DAC channel setup register*/
#define DAC_CHN_REG				    63
/*DAC Mute and volume control register*/
#define DAC_MUTE_CTRL_REG			64
/*Left DAC channel digital volume control*/
#define LDAC_VOL				    65
/*Right DAC channel digital volume control*/
#define RDAC_VOL				    66
/* Headset detection */
#define HS_DETECT				    67
/* DRC Control Registers */
#define DRC_CTRL_1				    68
#define DRC_CTRL_2				    69
#define DRC_CTRL_3				    70
/* Beep Generator */
#define BEEP_GEN_L				    71
#define BEEP_GEN_R				    72
/* Beep Length */
#define BEEP_LEN_MSB				73
#define BEEP_LEN_MID				74
#define BEEP_LEN_LSB				75
/* Beep Functions */
#define BEEP_SINX_MSB				76
#define BEEP_SINX_LSB				77
#define BEEP_COSX_MSB				78
#define BEEP_COSX_LSB				79

/*Channel AGC Control Register 1*/
#define CHN_AGC_1			        86
/*Channel AGC Control Register 2*/
#define CHN_AGC_2			        87
/*Channel AGC Control Register 3 */
#define CHN_AGC_3			        88
/*Channel AGC Control Register 4 */
#define CHN_AGC_4			        89
/*Channel AGC Control Register 5 */
#define CHN_AGC_5			        90
/*Channel AGC Control Register 6 */
#define CHN_AGC_6			        91
/*Channel AGC Control Register 7 */
#define CHN_AGC_7			        92
/* VOL/MICDET-Pin SAR ADC Volume Control */
#define VOL_MICDECT_ADC			    116
/* VOL/MICDET-Pin Gain*/
#define VOL_MICDECT_GAIN		    117

/******************** Page 1 Registers **************************************/
#define PAGE_1				        128
/* Headphone drivers */
#define HPHONE_DRIVERS			    (PAGE_1 + 31)
/* Class-D Speakear Amplifier */
#define CLASS_D_SPK			        (PAGE_1 + 32)
/* HP Output Drivers POP Removal Settings */
#define HP_OUT_DRIVERS			    (PAGE_1 + 33)
/* Output Driver PGA Ramp-Down Period Control */
#define PGA_RAMP			        (PAGE_1 + 34)
/* DAC_L and DAC_R Output Mixer Routing */
#define DAC_MIXER_ROUTING   		(PAGE_1 + 35)
/*Left Analog Vol to HPL */
#define LEFT_ANALOG_HPL			    (PAGE_1 + 36)
/* Right Analog Vol to HPR */
#define RIGHT_ANALOG_HPR		    (PAGE_1 + 37)
/* Left Analog Vol to SPL */
#define LEFT_ANALOG_SPL			    (PAGE_1 + 38)
/* Right Analog Vol to SPR */
#define RIGHT_ANALOG_SPR		    (PAGE_1 + 39)
/* HPL Driver */
#define HPL_DRIVER			        (PAGE_1 + 40)
/* HPR Driver */
#define HPR_DRIVER			        (PAGE_1 + 41)
/* SPL Driver */
#define SPL_DRIVER			        (PAGE_1 + 42)
/* SPR Driver */
#define SPR_DRIVER			        (PAGE_1 + 43)
/* HP Driver Control */
#define HP_DRIVER_CONTROL		    (PAGE_1 + 44)
/*MICBIAS Configuration Register*/
#define MICBIAS_CTRL			    (PAGE_1 + 46) 	// (PAGE_1 + 51)
/* MIC PGA*/
#define MICPGA_VOL_CTRL			    (PAGE_1 + 47)
/* Delta-Sigma Mono ADC Channel Fine-Gain Input Selection for P-Terminal */
#define MICPGA_PIN_CFG			    (PAGE_1 + 48)
/* ADC Input Selection for M-Terminal */
#define MICPGA_MIN_CFG			    (PAGE_1 + 49)
/* Input CM Settings */
#define MICPGA_CM			        (PAGE_1 + 50)
/*MICBIAS Configuration*/
#define MICBIAS				        (PAGE_1 + 46)  // (PAGE_1 + 51)

/****************************************************************************/
/*  Page 3 Registers 				              	  	    */
/****************************************************************************/
#define PAGE_3				        (128 * 3)

/* Timer Clock MCLK Divider */
#define TIMER_MCLK_DIV			    (PAGE_3 + 16)

#define BUFFER_PAGE_DAC			    (128 * 8)

#define DAC_RAM_CNTRL				(BUFFER_PAGE_DAC + 1)


void dac3100_write(unsigned int reg, uint8_t data);
void dac3100_change_page(uint8_t page);
uint8_t dac3100_read(unsigned int reg);
void DAC3100_free(void);

void DAC3100_init(cyhal_i2c_t *i2c_inst);
void DAC3100_speaker_volume(uint8_t volume);
void DAC3100_headphone_volume(uint8_t volume);
#endif
