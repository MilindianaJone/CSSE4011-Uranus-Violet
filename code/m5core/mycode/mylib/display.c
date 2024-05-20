#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(sample, LOG_LEVEL_INF);
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>

int main(void)
{
	size_t x;
	size_t y;
	size_t rect_w;
	size_t rect_h;
	size_t h_step;
	size_t scale;
	uint8_t bg_color;
	uint8_t *buf;
	int32_t grey_scale_sleep;
	const struct device *display_dev;
	struct display_capabilities capabilities;
	struct display_buffer_descriptor buf_desc;
	size_t buf_size = 0;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device %s not found. Aborting sample.",
			display_dev->name);
		return 0;
	}

	LOG_INF("Display sample for %s", display_dev->name);
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
		LOG_ERR("Could not allocate memory. Aborting sample.");
		return 0;
	}

	buf_desc.buf_size = buf_size;
	buf_desc.pitch = capabilities.x_resolution;
	buf_desc.width = capabilities.x_resolution;
	buf_desc.height = h_step;

	display_blanking_off(display_dev);
	printf("%d\n", buf_size);

	while (1) {
		for (int y = 0; y < capabilities.y_resolution; y += h_step) {
			for (int x = 0; x < buf_size; x++) {
				buf[x] = bg_color;
			}
			display_write(display_dev, 0, y, &buf_desc, buf);
		}
		k_msleep(grey_scale_sleep);
	}
	return 0;
}
