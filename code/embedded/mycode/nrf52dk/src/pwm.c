/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM.
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(sensor0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define NANOSECPERIOD 25000U

#define BIG_PERIOD 2000000U

int main(void)
{

	if (!pwm_is_ready_dt(&pwm_led0)) {
		return 0;
	}

	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	pwm_set_dt(&pwm_led0, NANOSECPERIOD, NANOSECPERIOD / 4U);
	/*
	// Restore PWM pulse width to its original value
	pwm_set_dt(&pwm_led0, NANOSECPERIOD, NANOSECPERIOD / 4U);

	// Delay for 9ms
	k_sleep(K_USEC(9000));

	// Turn off PWM by setting pulse width to zero
	pwm_set_pulse_dt(&pwm_led0, 0);

	// Delay for 4.5ms
	k_sleep(K_USEC(4500));
	*/


	while (1) {


        gpio_pin_set_dt(&led, 1);
        k_usleep(1);  // 562.2 µs burst
        //k_msleep(1);
        gpio_pin_set_dt(&led, 0);
        //k_msleep(1);
        k_usleep(1); // 1.687 ms low period
        //k_sleep(K_SECONDS(1));
		// Restore PWM pulse width to its original value
		// pwm_set_pulse_dt(&pwm_led0, NANOSECPERIOD / 2);

		// // Delay for 562 microseconds
		// k_sleep(K_USEC(560));

		// // Turn off PWM by setting pulse width to zero
		// pwm_set_pulse_dt(&pwm_led0, 1000);

		// // Delay for 1680 microseconds
		// k_sleep(K_USEC(560));
			
	}

    return 0;
}
