/**
 * Simple program to drive LED0 on the nrf5340 evaluation board using the nrfx driver for lower level access
 * Jonas Forssell
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/drivers/pwm.h>
#include <nrfx_pwm.h>
#include <zephyr/kernel.h>


// Use Zephyr macro to get access to the LED0 on the board
#define OUTPUT_PIN DT_GPIO_PIN(DT_ALIAS(led0), gpios)

// Use nrfx to create a PWM instance which we will connect to the LED0 later
static nrfx_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);

// Declare variables holding PWM sequence values. In this example only one channel is used 
nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};
nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};


// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle)
{
    
    // Check if value is outside of range. If so, set to 100%
    if(duty_cycle >= 100)
    {
        seq_values->channel_0 = 100;
    }
    else
    {
        seq_values->channel_0 = duty_cycle;
    }
    
    nrfx_pwm_simple_playback(&m_pwm0, &seq, 1, NRFX_PWM_FLAG_LOOP);
}

static void pwm_init(void)
{
    nrfx_pwm_config_t const config0 =
    {
        .output_pins =
        {
            OUTPUT_PIN,                        // channel 0 now connected to LED0
            NRFX_PWM_PIN_NOT_USED,             // channel 1
            NRFX_PWM_PIN_NOT_USED,             // channel 2
            NRFX_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    nrfx_pwm_init(&m_pwm0, &config0, NULL, NULL);
    
}


int main(void)
{

    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
        ;
    
    pwm_init();

    for (;;)
    {
        for(int i = 0; i <= 100; i++)
        {
            k_msleep(10);
            pwm_update_duty_cycle(i);
        }
    }
}
