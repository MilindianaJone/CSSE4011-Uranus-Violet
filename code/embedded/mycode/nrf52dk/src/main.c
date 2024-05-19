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

#define PERIOD 562 // Period in microseconds

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

void main(void)
{
    const struct device *pwm_dev;

    printk("NEC Signal Generation\n");

    pwm_dev = device_get_binding(pwm_led0.dev->name);
    if (!pwm_dev) {
        printk("Error: PWM device not found\n");
        return;
    }

    while (1) {
        // Send logical '1' (562.5μs high followed by 562.5μs low)
        //pwm_pin_set_usec(pwm_dev, pwm_led0.channel, PERIOD, PERIOD / 4, 0);
        pwm_set_cycles(pwm_dev, pwm_led0.channel, PERIOD, PERIOD / 4, 0);
        k_sleep(K_USEC(562));

        // Send logical '0' (562.5μs high followed by 1687.5μs low)
        //pwm_pin_set_usec(pwm_dev, pwm_led0.channel, PERIOD, 0, 0);
        pwm_set_cycles(pwm_dev, pwm_led0.channel, PERIOD, 0, 0);
        k_sleep(K_USEC(1687));
    }
}