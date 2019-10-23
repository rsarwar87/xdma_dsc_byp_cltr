
#include <array>
#include <cstring>
#include "xdma.hpp"
#include "bypass.hpp"
#include "gpio.hpp"

//#include "memory_manager.hpp"
int main() {
  // ctl.write<reg::led>(value);

  // cout << ctl.read<reg::led>();
  MemoryManager mm;
  mm.open();


  gpio_device gpio(&mm);
  uint32_t data = 0x0;
  gpio.read(PRIMARY, &data);
};
 

