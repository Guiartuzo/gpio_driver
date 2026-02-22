This repo is a simple gpio driver made for raspberry pi zero to control gpios through a kernel module from user space. The intent behind this is just for studying the development of kernel modules and how to use them in a practical way.

### procfs

procfs is a process file system generated dynamically by the kernel that gives access to kernel parameters, process and hardware information, system memory, CPU stats, etc.

Some tools that use procfs are: __ps__, __top__, __htop__, __free__, __uptime__, __lscpu__.

This driver will make use of procfs for controlling gpios from user space.

### Details

When executed on rasp zero 2W, the command:
`cat /proc/iomem | grep gpio`

will give the output:
`00000000-00000000 : 3f200000.gpio gpio@7e200000`

This gives us the physical address to the gpio peripheral, but we don't want use this hardcoded address...instead we should use the <pre>```gpiod_get()```</pre> which is part of the Linux GPIO descriptor API. By using this function we can guarantee portability between rasp models and future proof as well (in case the address changes in the future), by using the <pre>```ioremap(0x3f20000, size);```<\pre> we are ignoring concurrency and power management which can cause conflicts with other drivers and GPIO users.
