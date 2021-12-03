### Step 1: Install required software

- `gcc-arm-none-eabi`: To compile our code for the STM32 on the Linux computer, we will need a cross-compiler.
- `openocd`: To debug and flash the STM32
- `telnet`: To communicate with the STM32 when it's in debug mode.

```
sudo apt-get install -y gcc-arm-none-eabi
sudo apt-get install -y openocd
sudo apt-get install -y telnet
```

To help with setting up the openOCD connection with the STM32, we'll also need a package called `stlink-tools`. (STM has a its own serial debugging interface called "stlink")

```
sudo apt -y install stlink-tools
sudo systemctl restart udev
```

Now plug in your STM32 and you should be able to connect using the following command:

```
openocd -f board/stm32f4discovery.cfg
```

In a seperate terminal window, we can now connect to the STM32 board in debug mode:

```
telnet localhost 4444
```