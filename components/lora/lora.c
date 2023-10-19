

#include "lora.h"


#define DEFAULT_RADIO_NODE DT_ALIAS(lora0)
BUILD_ASSERT(DT_NODE_HAS_STATUS(DEFAULT_RADIO_NODE, okay),
	     "No default LoRa radio specified in DT");

#define MAX_DATA_LEN 10

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lora_send);

char data[MAX_DATA_LEN] = {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

const struct device *const lora_dev = DEVICE_DT_GET(DEFAULT_RADIO_NODE);


// mode 0 rx
// mode 1 tx
bool Initialize_LoRa(bool mode){

	struct lora_modem_config config;
	int ret;

	if (!device_is_ready(lora_dev)) {
		printk("Device is not ready %s \n",lora_dev->name);
        return 0;
	}

	printk("device is ready \n");
	k_sleep(K_MSEC(200));

	config.frequency = 433000000;
	config.bandwidth = BW_125_KHZ;
	config.datarate = SF_10;
	config.preamble_len = 8;
	config.coding_rate = CR_4_5;
	config.iq_inverted = false;
	config.public_network = false;
	config.tx_power = 4;
	config.tx = mode;


	ret = lora_config(lora_dev, &config);
	if (ret < 0) {
		printk("LoRa config failed\n");
        return 0;
	}
    return 1;
}



void Setup_LoRa_send_thread(void *param)
{
    int ret;
	while (1) {
		ret = lora_send(lora_dev, data, MAX_DATA_LEN);
		if (ret < 0) {
			printk("LoRa send failed, try again in 5 seconds\n");
		}
		else{
			printk("Data sent! \n");
		}
		k_sleep(K_MSEC(5000));
	}
}

void print_thread(void *param)
{
	while (1) {
		printk("hello from thread \n");
		k_sleep(K_MSEC(5000));
	}
}





void lora_receive_cb(const struct device *dev, uint8_t *data, uint16_t size,
		     int16_t rssi, int8_t snr)
{
	static int cnt;

	ARG_UNUSED(dev);
	ARG_UNUSED(size);

	LOG_INF("Received data: %s (RSSI:%ddBm, SNR:%ddBm)",
		data, rssi, snr);

	/* Stop receiving after 10 packets */
	if (++cnt == 10) {
		LOG_INF("Stopping packet receptions");
		lora_recv_async(dev, NULL);
	}
}

void Setup_LoRa_receive()
{
	LOG_INF("Asynchronous reception");
	lora_recv_async(lora_dev, lora_receive_cb);
	k_sleep(K_FOREVER);
}