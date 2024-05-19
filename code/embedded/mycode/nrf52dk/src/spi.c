#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h> 

#define SPI_OP  SPI_OP_MODE_MASTER |SPI_MODE_CPOL | SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_LINES_SINGLE


void main(void)
{

	uint8_t my_buffer[4] = {0};
	struct spi_buf my_spi_buffer[1];
	my_spi_buffer[0].buf = my_buffer;
	my_spi_buffer[0].len = 4;
	const struct spi_buf_set rx_buff = { my_spi_buffer, 1 };

	const struct spi_dt_spec mcp3201_dev =
					SPI_DT_SPEC_GET(DT_NODELABEL(mcp3201), SPI_OP, 0);
	int ret = spi_read_dt(&mcp3201_dev, &rx_buff);
	if (ret) { 
		//LOG_INF("spi_read status: %d", ret); 
	}


}

