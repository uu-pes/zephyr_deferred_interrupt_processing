# Zephyr - Deferred Interrupt Processing using Semaphores
This is an example illustrating deferred interrupt processing using semaphores 
for the course **1DT106 Programming Embedded Systems** at Uppsala University.

## Introduction

Interrupt service routines (ISRs) execute asynchronously in response to hardware/software interrupts.
It is important that ISRs execute quickly, allowing the kernel to promptly respond to other interrupts.
Hence, ISRs commonly offload most processing to tasks (this processing is deferred, hence the name *deferred interrupt processing*).
These tasks are notified by the ISR to perform interrupt-related processing using synchronization mechanisms such as semaphores or queues.

In this example, an interrupt is registered on pressing a button.
The corresponding ISR toggles an LED, toggling which is deferred to a task using semaphores.
The example also illustrates how buttons can be added to the device tree and configured in Zephyr.
Notice that to change the LED/button we only need to tweak the device tree, while our application stays unmodified!
The same case would be if we wanted to run our application on a different board!

## Building 

We assume you have `west` and Zephyr installed, which can be done following the [Zephyr getting started tutorial](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).
To build the application you can issue the following command on a terminal: 

```bash 
west build 
```

In case you get an error saying that `build` is not one of the available commands, you might miss some environment variables. You can always load those into the current session in the following way: 

```bash 
source ZEPHYR_PROJECT_DIR/zephyr/zephyr-env.sh 
``` 

where `ZEPHYR_PROJECT_DIR` is the directory of where you installed the Zephyr operating system.
Once successful, building will create a `build` folder where the application binary is in `build/zephyr/zephyr.uf2`.

## Loading and running

Loading the application onto the device can be done by connecting to the device via USB with Bootsel pressed, and then running:

```bash
picotool load build/zephyr/zephyr.uf2 
```
