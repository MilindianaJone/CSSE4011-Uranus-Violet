#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys_clock.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1
#define SENSOR_NODE DT_ALIAS(sensor0)

static struct gpio_callback sensor_cb_data;
static const struct gpio_dt_spec sensor = GPIO_DT_SPEC_GET(SENSOR_NODE, gpios);

uint8_t continued = 0;
uint32_t time_start = 0;
uint32_t time_elapsed = 0;

void sensor_triggered(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int val = gpio_pin_get_dt(&sensor);
	time_elapsed = ((k_cycle_get_32() - time_start))/240;
	time_start = k_cycle_get_32();

	if (val == 0) {
		if (time_elapsed > 500 && time_elapsed < 700) {
			printk("0");
		} else if (time_elapsed > 1500 && time_elapsed < 1700) {
			printk("1");
		}
	}
}

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&sensor)) {
		printk("Error: Sensor GPIO is not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&sensor, GPIO_INPUT);
	if (ret < 0) {
		printk("Error: Sensor GPIO could not be configured\n");
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&sensor, GPIO_INT_EDGE_BOTH);
	if (ret != 0) {
		printk("Error activating GPIO interrupt");
		return 0;
	}

	gpio_init_callback(&sensor_cb_data, sensor_triggered, BIT(sensor.pin));
	gpio_add_callback(sensor.port, &sensor_cb_data);

	int i = 0;
	while (1) {
		k_sleep(K_MSEC(1));
	}

	return 0;
}

