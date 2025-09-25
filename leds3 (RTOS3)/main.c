#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <inttypes.h>
#include <zephyr/drivers/uart.h>


/*int led_state = 0;          // 0=Red, 1=Yellow, 2=Green, 4=Pause
int saved_led_state = 0;
int red_state = 0;
int green_state = 0;
int blue_state = 0;
int yellow_state = 0;*/

static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

bool debug_state = true;

K_FIFO_DEFINE(debug_fifo);

K_FIFO_DEFINE(data_fifo);

struct data_t {
	void *fifo_reserved;
	char seq[10];
	int len;
	uint64_t time;
};

int init_uart(void){
	if (!device_is_ready(uart_dev)){
		return 1;
	}
	return 0;
}

K_SEM_DEFINE(red_sem, 0, 1);
K_SEM_DEFINE(yellow_sem, 0, 1);
K_SEM_DEFINE(green_sem, 0, 1);
K_SEM_DEFINE(release_sem, 0, 1);


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


void button_add_char(char c){
	struct data_t *item = k_malloc(sizeof(struct data_t));
	if(item){
		item->seq[0] = c;
		item->len = 1;
		k_fifo_put(&data_fifo, item);
		printk("Lisättiin sekvenssiin: %c\n", c);
	}
}

// Callback for pause button
void button_0_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Olen vielä olemassa\n");
    /*if (led_state != 4) {
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
    }*/
}

void button_1_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	button_add_char('R');
}

void button_2_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	button_add_char('Y');
}

void button_3_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{

	button_add_char('G');
}

void button_4_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{

	//yellow_state = !yellow_state;
        
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

char buffer[10];
int index = 0;
void uart_task(void *, void *, void *)
{
	unsigned char ch;

	while(true){
		if(uart_poll_in(uart_dev, &ch) == 0) {
			if(ch == 'D'){
				debug_state = !debug_state;
				printk("Debug is %s\n", debug_state ? "ON" : "OFF");
			}
			else if(ch != '\r'){
				buffer[index++] = ch;
			} else {
				struct data_t *item = k_malloc(sizeof(struct data_t));
				if(item){
					memcpy(item->seq, buffer, 10);
					item->len = 10;
					k_fifo_put(&data_fifo, item);
				}
			}
						
			if(index == 10){

				index = 0;
			}
		}
		k_msleep(20);
	}
}
void dispatcher_task(void *, void *, void *)
{
	while(true){
	
		timing_start();
		timing_t start_time = timing_counter_get();
		
		struct data_t *rec_item = k_fifo_get(&data_fifo, K_FOREVER);

		for(int i = 0; i < rec_item->len; i++){
			char c = rec_item->seq[i];

			if(c >= 'a' && c <= 'z'){
				c = c - 'a' + 'A';
			}

			switch(c){
				case 'R' :
					k_sem_give(&red_sem);
					break;
				case 'Y' :
					k_sem_give(&yellow_sem);
					break;
				case 'G' :
					k_sem_give(&green_sem);
					break;
				default : 
					printk("Was given wrong char, give a new one\n");
					break;
			}
			k_sem_take(&release_sem, K_FOREVER);
		}
		timing_stop();
		timing_t end_time = timing_counter_get();
		uint64_t diff = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
			
		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf) {
		buf->time = diff;
		k_fifo_put(&debug_fifo, buf);		
		}
		k_free(rec_item);
	}
}

// Red LED task
void red_led_task(void *, void *, void *)
{
	while(true){
	
		timing_start();
		timing_t start_time = timing_counter_get();
		
		k_sem_take(&red_sem, K_FOREVER);
		gpio_pin_set_dt(&red, 1);
        k_sleep(K_MSEC(1000));
        gpio_pin_set_dt(&red, 0);
		
		timing_stop();
		timing_t end_time = timing_counter_get();
		uint64_t diff = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
		
		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf) {
			buf->time = diff;
			k_fifo_put(&debug_fifo, buf);		
		}
		k_sem_give(&release_sem);
	}
}

// Yellow LED task
void yellow_led_task(void *, void *, void*) {

	while(true){
	
		timing_start();
		timing_t start_time = timing_counter_get();
		
   		k_sem_take(&yellow_sem, K_FOREVER);
        gpio_pin_set_dt(&red, 1);
        gpio_pin_set_dt(&green, 1);
        k_sleep(K_MSEC(1000));
        gpio_pin_set_dt(&red, 0);
        gpio_pin_set_dt(&green, 0);
		
		timing_stop();
		timing_t end_time = timing_counter_get();
		uint64_t diff = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
		
		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf) {
			buf->time = diff;
			k_fifo_put(&debug_fifo, buf);		
		}
		
		k_sem_give(&release_sem);
	}

}

    

// Green LED task
void green_led_task(void *, void *, void*) {

	while(true){
	
		timing_start();
		timing_t start_time = timing_counter_get();
		
		k_sem_take(&green_sem, K_FOREVER);
        gpio_pin_set_dt(&green, 1);
        k_sleep(K_MSEC(1000));
        gpio_pin_set_dt(&green, 0);
        gpio_pin_set_dt(&red, 0);
		k_sleep(K_MSEC(1000));
		

		
		timing_stop();
		timing_t end_time = timing_counter_get();
		uint64_t diff = timing_cycles_to_ns(timing_cycles_get(&start_time, &end_time));
		
		struct data_t *buf = k_malloc(sizeof(struct data_t));
		if (buf) {
			buf->time = diff;
			k_fifo_put(&debug_fifo, buf);		
		}
		k_sem_give(&release_sem);
	}
}

void debug_task(void *, void *, void *) {
	struct data_t *received;
	while(true) {
		received = k_fifo_get(&debug_fifo, K_FOREVER);
		if(debug_state){
			printk("Debug received: %lld\n", received->time);
		}
		k_free(received);
		k_yield();
	}
}


// Thread initialization
#define STACKSIZE 500
#define PRIORITY 5
K_THREAD_DEFINE(red_thread, STACKSIZE, red_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(yellow_thread, STACKSIZE, yellow_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(green_thread, STACKSIZE, green_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(dispatcher_thread, STACKSIZE, dispatcher_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(debug_thread, STACKSIZE, debug_task, NULL, NULL, NULL, PRIORITY, 0, 0);

// Main
int main(void)
{
	int ret = init_uart();
	if (ret != 0) {
		printk("UART init failed\n");
		return ret;
	}

	k_msleep(100);

    init_leds();
    init_button();
    while (1) { k_sleep(K_MSEC(100)); }
	return 0;

}