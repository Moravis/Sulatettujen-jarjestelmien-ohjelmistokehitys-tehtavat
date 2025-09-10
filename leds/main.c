//Tekijä Marko Oravisjärvi
// Tavoite pistemäärä: 2-3p
// Koodi vilkuttaa ledejeä punainen -> keltainen -> vihrea
// Pause napista vilkutus loppuu ja uudelleen painettaessa jatkuu
// 2-4 (koodissa 1-3) napit pistävät punaisen, keltaisen ja vihrean päälle ja samasta napista sammuvat. 
//Nappi 5 vilkuttaa keltaista valoa, kunnes nappia painetaan uudestaan.


#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <inttypes.h>

int led_state = 0;          // 0=Red, 1=Yellow, 2=Green, 4=Pause
int saved_led_state = 0;
int red_state = 0;
int green_state = 0;
int blue_state = 0;
int yellow_state = 0;

static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

#define BUTTON_0 DT_ALIAS(sw0)
static const struct gpio_dt_spec button_0 = GPIO_DT_SPEC_GET_OR(BUTTON_0, gpios, {0});
static struct gpio_callback button_0_data;

#define BUTTON_1 DT_ALIAS(sw1)
static const struct gpio_dt_spec button_1 = GPIO_DT_SPEC_GET_OR(BUTTON_1, gpios, {0});
static struct gpio_callback button_1_data;

#define BUTTON_2 DT_ALIAS(sw2)
static const struct gpio_dt_spec button_2 = GPIO_DT_SPEC_GET_OR(BUTTON_2, gpios, {0});
static struct gpio_callback button_2_data;

#define BUTTON_3 DT_ALIAS(sw3)
static const struct gpio_dt_spec button_3 = GPIO_DT_SPEC_GET_OR(BUTTON_3, gpios, {0});
static struct gpio_callback button_3_data;

#define BUTTON_4 DT_ALIAS(sw4)
static const struct gpio_dt_spec button_4 = GPIO_DT_SPEC_GET_OR(BUTTON_4, gpios, {0});
static struct gpio_callback button_4_data;

// Callback for pause button
void button_0_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (led_state != 4) {
        saved_led_state = led_state;
        led_state = 4;  // Pause
        gpio_pin_set_dt(&red, 0);
        gpio_pin_set_dt(&green, 0);
        gpio_pin_set_dt(&blue, 0);
        printk("Button pressed\n");
        printk("Paused\n");
    } else {
        led_state = saved_led_state;  // Resume
        printk("Button pressed\n");
        printk("Pause off\n");
    }
}

void button_1_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    red_state = !red_state;            // vaihda tila
    gpio_pin_set_dt(&red, red_state); 
}

void button_2_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if(red_state == 0 & green_state == 0){
        green_state = 1;
        red_state = 1;
        gpio_pin_set_dt(&red, 1);
        gpio_pin_set_dt(&green, 1);
    }
    else if(red_state == 0 & green_state == 1){
        red_state = 1;
        gpio_pin_set_dt(&red, 1);
    }
    else if(red_state == 1 & green_state == 0){
        green_state = 1;
        gpio_pin_set_dt(&green, 1);
    }
    else{
        red_state = 0;
        green_state = 0;
        gpio_pin_set_dt(&red, 0);
        gpio_pin_set_dt(&green, 0);
    }
}

void button_3_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{

    green_state = !green_state;
    gpio_pin_set_dt(&green, green_state);
}

void button_4_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{

    yellow_state = !yellow_state;
        
}

// LED initialization
int init_leds()
{
    int ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: Red Led configure failed\n");
		return ret;
	}
    gpio_pin_set_dt(&red, 0);

    ret = gpio_pin_configure_dt(&green, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: Green Led configure failed\n");
		return ret;
	}
    gpio_pin_set_dt(&green, 0);

    ret = gpio_pin_configure_dt(&blue, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("Error: Blue Led configure failed\n");
		return ret;
	}
    gpio_pin_set_dt(&blue, 0);

	printk("Led initialized ok\n");

	return 0;
}



int init_button()
{
	int ret;

    //button 0
	if (!gpio_is_ready_dt(&button_0)){
		printk("Error: button 0 is not ready\n");
	}
    ret = gpio_pin_configure_dt(&button_0, GPIO_INPUT | GPIO_PULL_UP);
    if(ret!=0){
		printk("Error: failed to configure pin\n");
	}

	ret = gpio_pin_interrupt_configure_dt(&button_0, GPIO_INT_EDGE_TO_ACTIVE);
	 if(ret!=0){
		printk("Error: failed to configure interrupt on pin\n");
	 }
	gpio_init_callback(&button_0_data, button_0_handler, BIT(button_0.pin));
    gpio_add_callback(button_0.port, &button_0_data);
    printk("Set up button 0 ok\n");

     //button 1
	if (!gpio_is_ready_dt(&button_1)){
		printk("Error: button 0 is not ready\n");
	}
    ret = gpio_pin_configure_dt(&button_1, GPIO_INPUT | GPIO_PULL_UP);
    if(ret!=0){
		printk("Error: failed to configure pin\n");
	}

	ret = gpio_pin_interrupt_configure_dt(&button_1, GPIO_INT_EDGE_TO_ACTIVE);
	 if(ret!=0){
		printk("Error: failed to configure interrupt on pin\n");
	 }
	gpio_init_callback(&button_1_data, button_1_handler, BIT(button_1.pin));
    gpio_add_callback(button_1.port, &button_1_data);
    printk("Set up button 0 ok\n");

    //button 2
	if (!gpio_is_ready_dt(&button_2)){
		printk("Error: button 0 is not ready\n");
	}
    ret = gpio_pin_configure_dt(&button_2, GPIO_INPUT | GPIO_PULL_UP);
    if(ret!=0){
		printk("Error: failed to configure pin\n");
	}

	ret = gpio_pin_interrupt_configure_dt(&button_2, GPIO_INT_EDGE_TO_ACTIVE);
	 if(ret!=0){
		printk("Error: failed to configure interrupt on pin\n");
	 }
	gpio_init_callback(&button_2_data, button_2_handler, BIT(button_2.pin));
    gpio_add_callback(button_2.port, &button_2_data);
    printk("Set up button 0 ok\n");

    //button 3
	if (!gpio_is_ready_dt(&button_3)){
		printk("Error: button 0 is not ready\n");
	}
    ret = gpio_pin_configure_dt(&button_3, GPIO_INPUT | GPIO_PULL_UP);
    if(ret!=0){
		printk("Error: failed to configure pin\n");
	}

	ret = gpio_pin_interrupt_configure_dt(&button_3, GPIO_INT_EDGE_TO_ACTIVE);
	 if(ret!=0){
		printk("Error: failed to configure interrupt on pin\n");
	 }
	gpio_init_callback(&button_3_data, button_3_handler, BIT(button_3.pin));
    gpio_add_callback(button_3.port, &button_3_data);
    printk("Set up button 0 ok\n");

    //button 4
	if (!gpio_is_ready_dt(&button_4)){
		printk("Error: button 0 is not ready\n");
	}
    ret = gpio_pin_configure_dt(&button_4, GPIO_INPUT | GPIO_PULL_UP);
    if(ret!=0){
		printk("Error: failed to configure pin\n");
	}

	ret = gpio_pin_interrupt_configure_dt(&button_4, GPIO_INT_EDGE_TO_ACTIVE);
	 if(ret!=0){
		printk("Error: failed to configure interrupt on pin\n");
	 }
	gpio_init_callback(&button_4_data, button_4_handler, BIT(button_4.pin));
    gpio_add_callback(button_4.port, &button_4_data);
    printk("Set up button 0 ok\n");

	return 0;
}


// Red LED task
void red_led_task(void *, void *, void *)
{
	while (true) {

		if (led_state == 0) {
			// 1. set LED on
			gpio_pin_set_dt(&red, 1);
			printk("Red on\n");

			for (int i = 0; i < 10 && led_state == 0; i++)
            k_sleep(K_MSEC(100));

        	gpio_pin_set_dt(&red, 0);
            k_sleep(K_MSEC(100));

			printk("Red off\n");

			if (led_state == 0){ 
                led_state = 1;
                printk("Led state changed to 1\n");
            }
		}
        k_msleep(100);
	}
}

// Yellow LED task
void yellow_led_task(void *, void *, void*) {

    printk("Yellow bug test\n");

    while(true){
        if (led_state == 1) {
            gpio_pin_set_dt(&red, 1);
            gpio_pin_set_dt(&green, 1);
            printk("Yellow on\n");
            
            for (int i = 0; i < 10 && led_state == 1; i++)
            k_sleep(K_MSEC(100));

            gpio_pin_set_dt(&red, 0);
            gpio_pin_set_dt(&green, 0);
            printk("Yellow off\n");

            for (int i = 0; i < 10 && led_state == 1; i++)
            k_sleep(K_MSEC(100));



            if (led_state == 1){
                led_state = 2;
            } 
        }
        k_msleep(100);
            
    }

}

    

// Green LED task
void green_led_task(void *, void *, void*) {

    while(true){
        if (led_state == 2) {
            gpio_pin_set_dt(&green, 1);
            printk("Green on\n");

            for (int i = 0; i < 10 && led_state == 2; i++)
            k_sleep(K_MSEC(100));

            gpio_pin_set_dt(&green, 0);

            printk("Green off\n");
            gpio_pin_set_dt(&red, 0);
            for (int i = 0; i < 10 && led_state == 2; i++)
            k_sleep(K_MSEC(100));

            

            if (led_state == 2){
                led_state = 0;
            }
        }
        k_msleep(100);
    }
    
}


void continous_yellow_task(void){
    while (true) {
        if (yellow_state == 1) {
            gpio_pin_set_dt(&red, 1);
            gpio_pin_set_dt(&green, 1);
            printk("Yellow on\n");
            k_sleep(K_MSEC(500));

            gpio_pin_set_dt(&red, 0);
            gpio_pin_set_dt(&green, 0);
            printk("Yellow off\n");
            k_sleep(K_MSEC(500));
        } else {
            k_msleep(1000);
        }
    }
}


// Thread initialization
#define STACKSIZE 500
#define PRIORITY 5
K_THREAD_DEFINE(red_thread, STACKSIZE, red_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(yellow_thread, STACKSIZE, yellow_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(green_thread, STACKSIZE, green_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(continous_yellow_thread, STACKSIZE, continous_yellow_task, NULL, NULL, NULL, PRIORITY, 0, 0);

// Main
int main(void)
{
    init_leds();
    init_button();
    while (1) { k_sleep(K_MSEC(100)); }

}

