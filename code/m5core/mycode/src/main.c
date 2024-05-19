#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys_clock.h>
#include <zephyr/drivers/display.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1
#define SENSOR_NODE DT_ALIAS(sensor0)

static struct gpio_callback sensor_cb_data;
static const struct gpio_dt_spec sensor = GPIO_DT_SPEC_GET(SENSOR_NODE, gpios);

uint8_t continued = 0;
uint32_t time_start = 0;
uint32_t time_elapsed = 0;
int signal_start = -1;
uint32_t pixel_data = 0;
uint8_t pixel_row[640];
int rowIndex = 0;
int columnIndex = 0;
size_t buf_size = 0;
uint8_t *buf;
const struct device *display_dev;
struct display_capabilities capabilities;
struct display_buffer_descriptor buf_desc;

void draw_column(void) {
	for (int x = 0; x < 640; x++) {
		buf[x] = pixel_row[x];
		// printf("%02x\n", pixel_row[x]);
	}
	display_write(display_dev, 0, columnIndex, &buf_desc, buf);
	columnIndex += 1;
	if (columnIndex == 240) {
		columnIndex = 0;
	}
}

void sensor_triggered(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int val = gpio_pin_get_dt(&sensor);
	time_elapsed = ((k_cycle_get_32() - time_start))/240;
	time_start = k_cycle_get_32();

	if (val == 0 && time_elapsed > 4350 && time_elapsed < 4550) {
		signal_start = 0;
		return;
	}
	if (signal_start != -1) {
		if (val == 0) {
			if (time_elapsed > 400 && time_elapsed < 700) {
				pixel_data &= ~(1 << (31 - signal_start));
			} else if (time_elapsed > 1500 && time_elapsed < 1900) {
				pixel_data |= 1 << (31 - signal_start);
			}
			signal_start++;
		}
	}
	if (signal_start == 32) {
		printf("%08x\n", pixel_data);
		if ((pixel_data == 0x12345678 && rowIndex > 320) || rowIndex >= 640) {
			pixel_data = 0;
			columnIndex += 1;
			for (int i = rowIndex; i < 640; i++) {
				pixel_row[i] = 0x00;
			}
			draw_column();
			rowIndex = 0;
			return; 
		} else if (pixel_data == 0x12345678) {
			rowIndex = 0;
			return; 
		}
		signal_start = -1;
		pixel_row[rowIndex] = (pixel_data >> 0) & 0xFF;
		pixel_row[rowIndex + 1] = (pixel_data >> 0) & 0xFF;
		pixel_row[rowIndex + 2] = (pixel_data >> 0) & 0xFF;
		pixel_row[rowIndex + 3] = (pixel_data >> 0) & 0xFF;
		pixel_row[rowIndex + 4] = (pixel_data >> 8) & 0xFF;
		pixel_row[rowIndex + 5] = (pixel_data >> 8) & 0xFF;
		pixel_row[rowIndex + 6] = (pixel_data >> 8) & 0xFF;
		pixel_row[rowIndex + 7] = (pixel_data >> 8) & 0xFF;
		pixel_row[rowIndex + 8] = (pixel_data >> 16) & 0xFF;
		pixel_row[rowIndex + 9] = (pixel_data >> 16) & 0xFF;
		pixel_row[rowIndex + 10] = (pixel_data >> 16) & 0xFF;
		pixel_row[rowIndex + 11] = (pixel_data >> 16) & 0xFF;
		pixel_row[rowIndex + 12] = (pixel_data >> 24) & 0xFF;
		pixel_row[rowIndex + 13] = (pixel_data >> 24) & 0xFF;
		pixel_row[rowIndex + 14] = (pixel_data >> 24) & 0xFF;
		pixel_row[rowIndex + 15] = (pixel_data >> 24) & 0xFF;

		if (pixel_data != 0x12345678) {
			rowIndex += 16;
		}
		pixel_data = 0;
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

	size_t x;
	size_t y;
	size_t rect_w;
	size_t rect_h;
	size_t h_step;
	size_t scale;
	uint8_t bg_color;
	int32_t grey_scale_sleep;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		return 0;
	}

	display_get_capabilities(display_dev, &capabilities);

	rect_w = 2;
	rect_h = 1;
	h_step = rect_h;
	scale = (capabilities.x_resolution / 8) / rect_h;
	rect_w *= scale;
	rect_h *= scale;
	grey_scale_sleep = 100;
	buf_size = rect_w * rect_h;

	bg_color = 0x18u;
	buf_size *= 2;
	buf = k_malloc(buf_size);

	if (buf == NULL) {
		return 0;
	}

	buf_desc.buf_size = buf_size;
	buf_desc.pitch = capabilities.x_resolution;
	buf_desc.width = capabilities.x_resolution;
	buf_desc.height = h_step;

	display_blanking_off(display_dev);

	while (1) {
		k_msleep(grey_scale_sleep);
	}

	return 0;
}

