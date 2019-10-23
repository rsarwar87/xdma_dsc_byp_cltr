#ifndef GPIO_H
#define GPIO_H

#include "memory_manager.hpp"

enum GpioChannel { PRIMARY, SECONDARY };

class gpio_device {
 public:
  gpio_device(MemoryManager *mm) : gpio(mm->get<mem::gpio>()){};

  int read(GpioChannel channel, uint32_t *data) {
    if (channel == PRIMARY)
      *data = gpio.read<reg::gpio::DATA0>();
    else
      *data = gpio.read<reg::gpio::DATA1>();
    return 0;
  }

  int write(GpioChannel channel, uint32_t *data) {
    if (channel == PRIMARY)
      gpio.write<reg::gpio::DATA0>(*data);
    else
      gpio.write<reg::gpio::DATA1>(*data);
    return 0;
  }

 private:
  Memory<mem::gpio> &gpio;
};

#endif
