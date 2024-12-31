#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define STACK_SIZE 500
#define PRIO_BUTTON_TASK 5

#define LED0_NODE DT_ALIAS(led0)
#define BTN_NODE DT_ALIAS(btn)

/* Creating mechanisms for synchronization */
struct k_sem sem;

/* Creating LED and button structures*/
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(BTN_NODE, gpios);

/* Callback structure */
static struct gpio_callback btn_cb_data;

/* On ISR give semaphore */
void button_isr() {
    k_sem_give(&sem);
}

void init() {
    gpio_pin_configure_dt(&led,  GPIO_OUTPUT_ACTIVE);
    gpio_pin_set_dt(&led, GPIO_ACTIVE_HIGH);
    
    gpio_pin_configure_dt(&btn, GPIO_INPUT);
    /* Sets interrupt on edge to active level */
    gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
    /* Initializes callback struct with ISR and the pins on which ISR should trigger  */
    gpio_init_callback(&btn_cb_data, button_isr, BIT(btn.pin));
    /* Adds it to the device */
    gpio_add_callback_dt(&btn, &btn_cb_data);

    /* Initializes semaphore with count set to 0 and limit to 1 */
    k_sem_init(&sem, 0, 1);
}

/* Toggles LED on button press*/
void button_task() {
    init();
    for (;;) {
        /* Wait until semaphore is given by ISR (meaning button is pressed), 
        take semaphore and toggle LED*/
        k_sem_take(&sem, K_FOREVER);
        gpio_pin_toggle_dt(&led);
    }
}

K_THREAD_DEFINE(button_tid, STACK_SIZE, button_task, NULL, NULL, NULL, 
    PRIO_BUTTON_TASK, 0, 0);
