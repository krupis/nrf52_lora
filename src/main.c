/*
 * Copyright (c) 2019 Manivannan Sadhasivam
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>

// usb stuff
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>

// Lora stuff
#include "lora.h"
K_THREAD_STACK_DEFINE(my_stack_area, LORA_THREAD_STACKSIZE);
struct k_thread my_thread_data;

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

int main(void)
{
	
	//tx
	if (usb_enable(NULL)) {
		return 0;
	}

	if(Initialize_LoRa(true) == 1){
		LOG_INF("Lora TX sucesfully initialized \n ");
		k_tid_t my_tid = k_thread_create(&my_thread_data, my_stack_area,
                                 K_THREAD_STACK_SIZEOF(my_stack_area),
                                 Setup_LoRa_send_thread,
                                 NULL, NULL, NULL,
                                 LORA_THREAD_PRIORITY, 0, K_NO_WAIT);
	}

	//rx
	// if(Initialize_LoRa(false) == 1){
	// 	Setup_LoRa_receive();
	// }


}
