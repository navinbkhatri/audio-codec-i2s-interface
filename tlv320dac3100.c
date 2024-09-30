


#include <stdbool.h>
#include "tlv320dac3100.h"

#define DAC3100_ADDR    0b00110000 //0x30

uint8_t prev_page=0x00;

static cyhal_i2c_t *i2c_ptr;
/*
 DAC Mode: OSR  : CLK at B
	00   : 125  : 250fs
	01   : 128  : 256fs
	10   : 64   : 128fs
	11   : 32   : 128fs
*/


void DAC3100_init(cyhal_i2c_t *i2c_inst)
{
    i2c_ptr = i2c_inst;
	
	dac3100_write(PAGE_SELECT,0);
	dac3100_write(RESET, 0x01);
	
	//Clock settings
	dac3100_write(CLK_REG_1, 0x03);   //pll_clkin=MCLK, codec_CLKIN=PLL_CLK
	dac3100_write(CLK_REG_3, 0x08);   //J =8
	dac3100_write(CLK_REG_4, 0x00);   //0x0000 16 bit, D=0
	dac3100_write(CLK_REG_2, 0x91);   //Power up PLL, P=1, R=1
	dac3100_write(NDAC_CLK_REG_6, 0x88);   //NDAC power up =8
	dac3100_write(MDAC_CLK_REG_7, 0x82);   //MDAC power up =2
	dac3100_write(DAC_OSR_MSB, 0x0080);    // 0x0080 16 bit =128
	dac3100_write(INTERFACE_SET_REG_1, 0x00); //	I2S interface, wordlength 16 bit, BLCK input, WCLK input
	
	//Processing blocks -> PRB_P11
	dac3100_write(DAC_INSTRUCTION_SET, 0x0B); //Processing Block 11 selected
	
	dac3100_write(PAGE_SELECT, 0x08);
	dac3100_write(DAC_RAM_CNTRL, 0x04); // adaptive filtering enabled
	
	//volume control through pin disable
	dac3100_write(PAGE_SELECT, 0x00);
	dac3100_write(VOL_MICDECT_ADC, 0x00); // Volume control through register on
	
	//
	dac3100_write(PAGE_SELECT, 0x01);
	dac3100_write(HPHONE_DRIVERS, 0x04); // Headphone output Power Down
	dac3100_write(HP_OUT_DRIVERS, 0x4E); // Driver-poweron-time= 1.22s Driver ramp up time= 3.9ms
	dac3100_write(DAC_MIXER_ROUTING, 0x44); // DAC_L to left  channel mixer and DAC_R to Right channel mixer
	dac3100_write(HPL_DRIVER, 0x06); // HPL unmute, 0dB, gain not applied
	dac3100_write(HPR_DRIVER, 0x06); // HPR unmute, 0dB, gain not applied
	dac3100_write(SPL_DRIVER, 0x1C); // Output gain 24dB, Class-D driver unmute, gain not applied
	
	dac3100_write(HPHONE_DRIVERS, 0xC4); // Headphone output Power up   ?? double- check
	dac3100_write(CLASS_D_SPK, 0x86);    // Speaker output Power up,
	dac3100_write(LEFT_ANALOG_HPL, 0x92); // Left Channel analog routed to output HPL = 18 (0-127)
	dac3100_write(RIGHT_ANALOG_HPR, 0x92); // Right Channel analog routed to output HPR = 18 (0-127)
	dac3100_write(LEFT_ANALOG_SPL, 0x92); //Left Channel analog routed to output SPL = 18 (0-127)
	
	dac3100_write(PAGE_SELECT, 0x00);
	dac3100_write(DAC_CHN_REG, 0xD4); // Power up Left and Right channel
	dac3100_write(LDAC_VOL, 0xD4);  //DAC Left volume gain = -22dB
	dac3100_write(RDAC_VOL, 0xD4); // DAC Right volume gain = -22dB
	dac3100_write(DAC_MUTE_CTRL_REG, 0x00); // unmute the Left-Right channel
	
	
}

/*******************************************************************************
 * Free the resources used with the Audio codec.
 *******************************************************************************/
void DAC3100_free(void)
{
    i2c_ptr = NULL;
}

void DAC3100_headphone_volume(uint8_t volume)
{
	dac3100_write(LEFT_ANALOG_HPL, volume|0x80); // Left Channel analog routed to output HPL (0-127)
	dac3100_write(RIGHT_ANALOG_HPR, volume|0x80); // Right Channel analog routed to output HPR (0-127)

}

void DAC3100_speaker_volume(uint8_t volume)
{

	dac3100_write(LEFT_ANALOG_SPL, volume|0x80); //Left Channel analog routed to output SPL (0-127)

}

void dac3100_write(unsigned int reg, uint8_t data)
{
	cy_rslt_t rslt;
	uint8_t page;
	uint8_t addr;
	
	page = reg / 128;
	addr = reg % 128;
	
    uint8_t buf[] = {addr, data};
	if(prev_page != page)
	{
		dac3100_change_page(page);
	}
	prev_page= page;
	rslt = cyhal_i2c_master_write(i2c_ptr, DAC3100_ADDR, buf, sizeof(buf), 0, true);
	
	
}

uint8_t dac3100_read(unsigned int reg)
{
	cy_rslt_t rslt;
	uint8_t page;
	uint8_t addr;
    uint8_t data;
	
	page = reg / 128;
	addr = reg % 128;
	
	if(prev_page != page)
	{
		dac3100_change_page(page);
	}
	prev_page= page;
	
    rslt = cyhal_i2c_master_write(i2c_ptr, DAC3100_ADDR, (uint8_t*)(&addr), 1, 0, false);

    rslt = cyhal_i2c_master_read(i2c_ptr, DAC3100_ADDR, &data, 1, 0, true);
	
	return data;
}

void dac3100_change_page(uint8_t page)
{
	cy_rslt_t rslt;
	uint8_t buf[] = {PAGE_SELECT, page};
	rslt = cyhal_i2c_master_write(i2c_ptr, DAC3100_ADDR, buf, sizeof(buf), 0, true);

}





