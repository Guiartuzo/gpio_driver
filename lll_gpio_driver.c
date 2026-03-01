#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/proc_fs.h>

/*************************************************************************************************/

struct raspzero_data {
	struct gpio_desc *led;
};

/*************************************************************************************************/

static ssize_t led_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct raspzero_data *data = dev_get_drvdata(dev);
	return sprintf(buf, "gpio value: %d\n", gpiod_get_value(data->led));
}

/*************************************************************************************************/

static ssize_t led_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct raspzero_data *data = dev_get_drvdata(dev);
	int val;

	if (kstrtoint(buf, 0, &val)) {
		return -EINVAL;
	}

	gpiod_set_value(data->led, val ? 1 : 0);
	return count;
}

/*************************************************************************************************/

static DEVICE_ATTR(led, 0664, led_show, led_store);

/*************************************************************************************************/

static int my_gpio_probe(struct platform_device *pdev)
{
	struct raspzero_data *data;
	int ret;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if(!data) {
		return -ENOMEM;
	}

	data->led = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
	if(IS_ERR(data->led)) {
		dev_err(&pdev->dev, "Failed to get GPIO\n");
		return PTR_ERR(data->led);
	}

	/* Turns GPIO ON */
	gpiod_set_value(data->led, 1); 

	platform_set_drvdata(pdev, data);

	ret = device_create_file(&pdev->dev, &dev_attr_led);
	if (ret) {
		dev_err(&pdev->dev, "Failed to create sysfs attribute\n");
		return ret;
	}

	dev_info(&pdev->dev, "mygpio probed\n");
	return 0;
}

/*************************************************************************************************/

static void my_gpio_remove(struct platform_device *pdev)
{
	device_remove_file(&pdev->dev, &dev_attr_led);
	dev_info(&pdev->dev, "mypgio removed\n");
}

/*************************************************************************************************/

static const struct of_device_id my_gpio_of_match[] = {
	{ .compatible = "gui,mygpio" },
	{ }
};

/*************************************************************************************************/

MODULE_DEVICE_TABLE(of, my_gpio_of_match);

/*************************************************************************************************/

static struct platform_driver my_gpio_driver = {
	.probe = my_gpio_probe,
	.remove = my_gpio_remove,
	.driver = {
		.name = "mygpio",
		.of_match_table = my_gpio_of_match,
	},
};

/*************************************************************************************************/

module_platform_driver(my_gpio_driver);

/*************************************************************************************************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guilherme Martim");
MODULE_DESCRIPTION("Custom GPIO driver");