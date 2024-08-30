#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/hrtimer.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("gpcm00");
MODULE_DESCRIPTION("Power modulator device driver");

static struct gpio_desc *zc = NULL;
static struct gpio_desc *pw = NULL;
static struct hrtimer tmr;

static ktime_t elapsed , earlier, now;

static int powm_probe(struct platform_device *pdev);
static int powm_remov(struct platform_device *pdev);

static struct of_device_id powm_ids[] = {
	{ .compatible = "powermodul,gpcm00",},
	{ /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, powm_ids);

static struct platform_driver powm_driver = {
	.probe = powm_probe,
	.remove = powm_remov,
	.driver = {
		.name = "power_modulator_driver",
		.of_match_table = powm_ids,
	},
};

irqreturn_t gpio_irq_test(int irq_no, void *dev_id)
{
	pr_info("powm - Info: Interrupt triggerred\n");
	return IRQ_HANDLED;
}

static enum hrtimer_restart hrtimer_irq_test(struct hrtimer *timer)
{
	now = hrtimer_cb_get_time(timer);
	elapsed = ktime_us_delta(now, earlier);
	earlier = now;
	printk("Timer interrupt after %ldnsec\n", elapsed);
	hrtimer_forward_now(timer, ktime_set(0,4));
	return HRTIMER_RESTART;
}

static int powm_probe(struct platform_device *pdev) {
	struct device *dev = &pdev->dev;
	int gpio_irq;	
	int ret = -1;

	pr_info("Initializing power modulator module\n");
	
	/* initialize gpio */
	zc = gpiod_get(dev, "zc", GPIOD_IN);
	if(IS_ERR(zc)) {
		pr_err("powm - Error: Could not set up ZC GPIO\n");
		ret = -IS_ERR(zc);
		goto return_error;
	} 
	
	pw = gpiod_get(dev, "pw", GPIOD_OUT_HIGH);
	if(IS_ERR(pw)) {
		pr_err("powm - Error: Could not set ip PW GPIO\n");
		ret = -IS_ERR(pw);
		goto clear_zc;
	}

	gpiod_set_value(pw, 1);
	printk("PW pin is set to %u\n", gpiod_get_value(pw));

	gpio_irq = gpiod_to_irq(zc);
	if(gpio_irq < 0) {
		pr_err("powm - Error: Failed to get an IRQ number\n");
		goto clear_pw;
	}
	
	if(request_irq(gpio_irq, gpio_irq_test, 
				IRQF_TRIGGER_RISING, "powm", NULL) < 0) {
		pr_err("powm - Error: Failed to request IRQ\n");
		goto clear_pw;
	}

	/* initialize timer */
	hrtimer_init(&tmr, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	tmr.function = hrtimer_irq_test;

	hrtimer_start(&tmr, ms_to_ktime(300), HRTIMER_MODE_REL_HARD);
	earlier = hrtimer_cb_get_time(&tmr);

	printk("Initialized hrtimer module at address 0x%X\n", tmr.base);

	return 0;

clear_pw:
	gpiod_put(pw);
clear_zc:
	gpiod_put(zc);
return_error:
	return ret;
}

static int powm_remov(struct platform_device *pdev) {
	printk("Removing power modulator module\n");
	gpiod_put(zc);
	gpiod_put(pw);
	hrtimer_cancel(&tmr);
	return 0;
}

module_platform_driver(powm_driver);
