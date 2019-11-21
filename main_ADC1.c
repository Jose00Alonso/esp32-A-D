/*
 * main.c
 *
 *  Created on: 14 oct. 2019
 *      Author: jose
 */


#include "ESP32.h"
#include <driver/adc.h>


#define pulsador 	0
#define led1 		2
#define size 64

int16_t promedio0[size];
int16_t promedio6[size];
int32_t pro0;
int32_t pro6;

bool estado_led1=1;



void main()
{
	gpio_set_direction(pulsador,GPIO_MODE_INPUT);
	gpio_set_pull_mode(pulsador,GPIO_PULLUP_ONLY);

	gpio_set_direction(led1,GPIO_MODE_OUTPUT );

	adc1_config_width(ADC_WIDTH_BIT_12);
	adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
	adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);

    printf("Convertidor Analogico digital\n\r");

    int8_t i=0;

    while(1)
    {
    	if(gpio_get_level(pulsador))
    	{

			int16_t val6 = adc1_get_raw(ADC1_CHANNEL_6); //GPIO 34
			int16_t val0 = adc1_get_raw(ADC1_CHANNEL_0); //GPIO 36

			promedio0[i++]=val0;
			promedio6[i++]=val6;

			for(uint8_t j=0;j<size;j++)
			{
				pro0 = pro0 + promedio0[j];
				pro6 = pro6 + promedio6[j];
			}
			//pro0 = pro0/size;
			pro6 = pro6/size;

			if(i==size)i=0;

			estado_led1 = !estado_led1;

			gpio_set_level(led1,estado_led1);

			printf("AD 0 (data cruda): %d\n\r",val0);
			printf("AD 6 (data cruda): %d\n\r",val6);


			printf("AD 0 (promedio): %2.2f\n\r",(float)(pro0)/size);
			pro0=0;
			printf("AD 6 (promedio): %d\n\r",pro6);
			pro6=0;

			printf("_____________________________________________________\n\r");
    	}
    	else
    	{
    		printf("BOOT presionado \n\r");
    	}

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    }

}
