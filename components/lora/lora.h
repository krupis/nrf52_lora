#ifndef LORA_H
#define LORA_H

#include <zephyr/drivers/lora.h>
#include <zephyr/kernel.h>

#define LORA_THREAD_STACKSIZE       1024
#define LORA_THREAD_PRIORITY        5 



void Setup_LoRa_send_thread(void *param);
bool Initialize_LoRa(bool mode);

void Setup_LoRa_receive();

#endif