#ifndef __DRIVERS_MEMORY_HPP__
#define __DRIVERS_MEMORY_HPP__

#include <array>
#include <cstdint>
#include <tuple>
#include <string>

extern "C" {
#include <sys/mman.h>  // PROT_READ, PROT_WRITE
}

namespace mem {
constexpr size_t dmem = 0;
constexpr uintptr_t dmem_addr = 0x34bc00000;
constexpr uint32_t dmem_range = 100*1024*1024;
constexpr uint32_t dmem_nblocks = 1;
constexpr uint32_t dmem_mmap = 0;
constexpr size_t gpio = 1;
constexpr uintptr_t gpio_addr = 0x00000;
constexpr uint32_t gpio_range = 4 * 1024;
constexpr uint32_t gpio_nblocks = 1;
constexpr uint32_t gpio_mmap = 1;
constexpr size_t bypass = 2;
constexpr uintptr_t bypass_addr = 0x10000;
constexpr uint32_t bypass_range = 512 * 1024;
constexpr uint32_t bypass_nblocks = 1;
constexpr uint32_t bypass_mmap = 1;
constexpr size_t xdma = 3;
constexpr uintptr_t xdma_addr = 0x0;
constexpr uint32_t xdma_range = 64 * 1024;
constexpr uint32_t xdma_nblocks = 1;
constexpr uint32_t xdma_mmap = 2;

constexpr size_t count = 4;

constexpr std::array<std::tuple<uintptr_t, uint32_t, uint32_t, uint32_t, uint32_t>, count>
    memory_array = {{std::make_tuple(dmem_addr, dmem_range,
                                     PROT_READ | PROT_WRITE, dmem_nblocks, dmem_mmap),
                     std::make_tuple(gpio_addr, gpio_range,
                                     PROT_READ | PROT_WRITE, gpio_nblocks, gpio_mmap),
                     std::make_tuple(bypass_addr, bypass_range,
                                     PROT_READ | PROT_WRITE, bypass_nblocks, bypass_mmap),
                     std::make_tuple(xdma_addr, xdma_range,
                                     PROT_READ | PROT_WRITE, xdma_nblocks, xdma_mmap)
    }};

}  // namespace mem

namespace reg {
// -- Control offsets
/*constexpr uint32_t led = 0;
static_assert(led < mem::control_range, "Invalid control register offset led");
constexpr uint32_t trig = 4;
static_assert(trig < mem::control_range, "Invalid control register offset
trig");

// -- Status offsets
constexpr uint32_t reg1 = 0;
static_assert(reg1 < mem::status_range, "Invalid status register offset reg1");
*/
namespace bypass {
constexpr uint32_t c2h  = 0x0;
constexpr uint32_t h2c  = 0x20000;

constexpr uint32_t ctrl  = 0x0;
constexpr uint32_t d_addr_h = 0x04;
constexpr uint32_t d_addr_l = 0x08;
constexpr uint32_t s_addr_h = 0x0c;
constexpr uint32_t s_addr_l = 0x10;
constexpr uint32_t length   = 0x14;
constexpr uint32_t repeat   = 0x18;
constexpr uint32_t cmd      = 0x1c;
}  // namespace spi

namespace xdma {
constexpr uint32_t id  = 0x0;
constexpr uint32_t ctrl0 = 0x4;
constexpr uint32_t chl_o[4] = {0x0, 0x100, 0x200, 0x300};
constexpr uint32_t dir_o = 0x1000;
}  // namespace spi

namespace dmem {
constexpr uint32_t read  = 10*1024*1024;
constexpr uint32_t write = 30*1024*1024;
}  // namespace spi

namespace gpio {
constexpr uint32_t DATA0 = 0x0;
constexpr uint32_t TSTATE0 = 0x4;
constexpr uint32_t DATA1 = 0x8;
constexpr uint32_t TSTATE1 = 0xC;
}  // namespace gpio
}  // namespace reg

#endif  // __DRIVERS_MEMORY_HPP__
