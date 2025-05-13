#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define GPIO_OUTPUT_IO_0    1
#define GPIO_OUTPUT_IO_1    23
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_IO_0) | (1ULL<<GPIO_OUTPUT_IO_1))

void blink1(void *parameter)
{
    while(1)
    {
        gpio_set_level(GPIO_OUTPUT_IO_0, 1);
		vTaskDelay(800 / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_IO_0, 0);
		vTaskDelay(800 / portTICK_PERIOD_MS);
    }
}

void blink2(void *parameter)
{
    while(1)
    {
        gpio_set_level(GPIO_OUTPUT_IO_1, 1);
		vTaskDelay(300 / portTICK_PERIOD_MS);
		gpio_set_level(GPIO_OUTPUT_IO_1, 0);
		vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    esp_task_wdt_deinit();

    //zero-initialize the config structure.
	gpio_config_t io_conf = {};
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);
	
    xTaskCreate(
        blink1,      // Function name of the task
        "Blink 1",   // Name of the task (e.g. for debugging)
        2048,        // Stack size (bytes)
        NULL,        // Parameter to pass
        1,           // Task priority
        NULL         // Task handle
    );


    
    xTaskCreate(
        blink2,     // Function name of the task
        "Blink 2",  // Name of the task (e.g. for debugging)
        2048,       // Stack size (bytes)
        NULL,       // Parameter to pass
        1,          // Task priority
        NULL        // Task handle
    );

}
