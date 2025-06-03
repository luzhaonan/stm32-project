

struct gpioled_dev{
	dev_t devid;    //设备号
	struct cdev cdev; //cdev
	struct class *class; //
	struct device *device;
	int major;
	int minjor;
	
	struct device_node  *nd; //设备节点，使用设别树是要用
	int led_gpio; //led 使用的GPIO编号、
}；

struct gpioled_dev gpioled;


static int led_open(struct inode *inode,struct file *filp)
{
	filp->private = &gpioled;
	return 0;
}

static int led_release(struct inode *inode ,struct *filp)
{
	return 0;
}

static ssize_t led_read(struct file *filp,char __user *buf,size_t cnt,loff_t *offt)
{
	return 0;
}

static ssize_t led_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
{
	int retvalue;
	char databuf[1];
	
	struct gpioled_dev *gpioled = filp->private;
	
	retvalue = copy_from_user(databuf,buf,cnt);
	if(retvalue != 0)
	{
		printk("kernel write fail!\r\n");
		return -1;
	}
	
	if(databuf[0] == 0)
	{
		gpio_set_value(gpioled->led_gpio,0);
	}
	else
	{
		gpio_set_value(gpioled->led_gpio,1);
	}
	
	return 0;
}

static struct file_operations gpioled_fops={
	.open = led_open;
	.write = led_write;
	.read = led_read;
	.release = led_release;
};

static int __init led_init(void)
{
	int retvalue;
	
	retvalue = gpioled.nd = of_find_node_by_path("/gpioled");  //gpioled是设备树中创建的节点
	if(retvalue == NULL)
	{
		
	}
	else{
		
	}
	
	//通过LED的设备树节点找gpio的pinctrl节点，获得gpio编号，可以通过这个编号来控制输出
	gpioled.led_gpio = of_get_named_gpio(gpioled.nd,"led-gpio",0);
	if(gpioled.led_gpio < 0)
	{
		
	}
	
	if(gpioled.major)
	{
		gpioled.devid = MADEV(gpioled.major,0);
		
		register_chrdev_region(gpioled.devid,GPIOLED_CNT,GPIOLED_NAME);
	}
	else{
			alloc_chrdev_region(gpioled.devid,0,GPIOLED_CNT,GPIOLED_NAME);
			gpioled.major=MAJOR(gpioled.devid);
			gpioled.minjor=MINJOR(gpioled.devid);
	}
	
	gpioled.dev.owner = THIS_MODULE;
	
	cdev_init(&gpioled.cdev,&gpioled_fops);
	
	cdev_add(&gpioled.cdev,gpioled.devid,GPIOLED_CNT);
	
	gpioled.class = class_create(THIS_MODULE,GPIOLED_NAME);
	if(IS_ERR(gpioled.class))
	{
		
	}
	gpioled.device = device_create(gpioled.class,NULL,gpioled.devid,NULL,GPIOLED_NAME);
	if(IS_ERR(gpioled.device))
	{
		
	}
	return 0;
}

static int __exit led_exit(void)
{
	cdev_del(&gpioled.cedv);
	unregister_chrdev_region(gpio.devid,GPIOLED_CNT);
	
	device_destory(gpioled.class,gpioled.devid);
	class_destory(gpioled.class,gpioled.devid);
	
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("luzhaonan");
