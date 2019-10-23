#ifndef XDMA_H
#define XDMA_H

#include "memory_manager.hpp"

enum CHTYPE { UNDEFINED = 0, MEMORYMAPPED, STREAMING };
enum TYPE { H2C, C2H };
struct channel_tags {
  int id = 0;
  TYPE direction;
  CHTYPE type;
};
struct pcie_tags {
  channel_tags chl_h2c[4];
  channel_tags chl_c2h[4];
  uint8_t h2c = 0;
  uint8_t c2h = 0;
};

class xdma_device {
 public:
  xdma_device(MemoryManager *mm) : xdma(mm->get<mem::xdma>()){};

  int GetAllChannels() {
    for (int i = 0; i < 4; i++) {
      uint32_t value = xdma.read_reg(reg::xdma::id + reg::xdma::chl_o[i]);
      if (((value >> 20) & 0xFFF) == 0x1fc) {
        tags.chl_c2h[i].id = ((value >> 8) & 0xF);
        tags.chl_c2h[i].direction = C2H;
        tags.chl_c2h[i].type = CHTYPE(((value >> 15) & 0x1) + 1);
        tags.c2h++;
      }
    }
    for (int i = 0; i < 4; i++) {
      uint32_t value =
          xdma.read_reg(reg::xdma::id + reg::xdma::chl_o[i] + reg::xdma::dir_o);
      if (((value >> 20) & 0xFFF) == 0x1fc) {
        tags.chl_h2c[i].id = ((value >> 8) & 0xF);
        tags.chl_h2c[i].direction = C2H;
        tags.chl_h2c[i].type = CHTYPE(((value >> 15) & 0x1) + 1);
        tags.h2c++;
      }
    }
    return 0;
  }

  int PrintChannelInfo() {
    printf("No of C2H channels detected: %d\n", tags.c2h);
    for (int i = 0; i < 4; i++)
      printf("\tChannel: %d\n\t\tID: %d\n\t\tType: %d\n", i, tags.chl_c2h[i].id,
             tags.chl_c2h[i].type);
    printf("No of H2C channels detected: %d\n", tags.h2c);
    for (int i = 0; i < 4; i++)
      printf("\tChannel: %d\n\t\tID: %d\n\t\tType: %d\n", i, tags.chl_h2c[i].id,
             tags.chl_h2c[i].type);
    return 0;
  }

  int ActivateChannel(int32_t channel = -1, bool activate = true) {
    if (channel != -1) {
      xdma.write_reg(
          reg::xdma::ctrl0 + reg::xdma::chl_o[channel] + reg::xdma::dir_o,
          activate & 0x1);
      xdma.write_reg(reg::xdma::ctrl0 + reg::xdma::chl_o[channel],
                     activate & 0x1);
    } else
      for (int i = 0; i < 4; i++) {
        xdma.write_reg(
            reg::xdma::ctrl0 + reg::xdma::chl_o[i] + reg::xdma::dir_o,
            activate & 0x1);
        xdma.write_reg(reg::xdma::ctrl0 + reg::xdma::chl_o[i], activate & 0x1);
      }
    return 0;
  }

  pcie_tags *GetTags() { return &tags; }

 private:
  pcie_tags tags;
  Memory<mem::xdma> &xdma;
};

#endif
