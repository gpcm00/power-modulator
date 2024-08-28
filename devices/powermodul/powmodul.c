#include<linux/module.h>
#include<linux/init.h>
#include<linux/mod_devicetable.h>
#include<linux/platform_device.h>
#include<linux/property.h>
#include<linux/of_device.h>
#include<linux/gpio/consumer.h>

static struct gpio_desc *zc = NULL;
static struct gpio_desc *pw = NULL;

static struct proc_dir_entry *pownmod = NULL;

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
		.name = "power_modulator_driver"
		.of_match_table = powm_ids,
	},
};

irqreturn_t placeholder(int irq_no, void *dev_id)
{
	pr_info("powm - Info: Interrupt triggerred\n");
	return IRQ_HANDLED;
}

static int powm_probe(struct platform_device *pdev) {
	struct device *dev = &pdev->dev;
	int gpio_irq;	
	int ret = -1

	printk("Initializing power modulator module\n");

	zc = gpiod_get(dev, "zc", GPIOD_IN);
	if(IS_ERR(zc)) {
		pr_error("powm - Error: Could not set up ZC GPIO\n");
		ret = -IS_ERR(zc);
		goto return_error;
	} 
	
	pw = gpiod_get(dev, "pw", GPIOD_OUT_HIGH);
	if(IS_ERR(pw)) {
		pr_error("powm - Error: Could not set ip PW GPIO\n");
		ret = -IS_ERR(pw);
		goto clear_zc;
	}

	gpio_irq = gpiod_to_irq(zc);
	if(gpio_irq < 0) {
		pr_error("powm - Error: Failed to get an IRQ number\n");
		goto clear_pw;
	}
	
	if(request_irq(gpio_irq, placeholder, 
					IRQF_TRIGGER_RISING, "powm", NULL) < 0) {
		pr_error("powm - Error: Failed to request IRQ\n");
		goto clear_pw;
	}

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
}

module_platform_driver(powm);
