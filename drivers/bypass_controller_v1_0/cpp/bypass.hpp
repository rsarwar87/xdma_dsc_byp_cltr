#ifndef BYPASS_H
#define BYPASS_H

#include "memory_manager.hpp"
#include "xdma.hpp"

struct dpack{
  uint32_t ctrl;
  uint64_t d_addr;
  uint64_t s_addr;
  uint32_t length;
  uint32_t repeat;
};

class dbypass_device {
 public:
  dbypass_device(MemoryManager *mm, uint32_t id, TYPE typ, pcie_tags *tags) : dbypass(mm->get<mem::bypass>()){
    m_type = typ;
    m_ptags = tags;
    m_id = id;
    m_offset = (typ == C2H) ? reg::bypass::c2h : reg::bypass::h2c; 
  };

  int IssueCmd(dpack data, bool is_cyclic = false)
  {
      dbypass.write_reg(m_offset + reg::bypass::ctrl, data.ctrl);
      dbypass.write_reg(m_offset + reg::bypass::d_addr_h, data.d_addr >> 32);
      dbypass.write_reg(m_offset + reg::bypass::d_addr_l, data.d_addr & 0xFFFFFFFF);
      dbypass.write_reg(m_offset + reg::bypass::s_addr_h, data.s_addr >> 32);
      dbypass.write_reg(m_offset + reg::bypass::s_addr_l, data.s_addr & 0xFFFFFFFF);
      dbypass.write_reg(m_offset + reg::bypass::length, data.length   & 0x0FFFFFFF);
      dbypass.write_reg(m_offset + reg::bypass::repeat, data.repeat   & 0x0FFFFFFF);
      
      uint32_t val = 1;
      val = (is_cyclic) ? val + (0x1 < 1) : val;
      if (m_type == C2H){
        val = val + (0x1 << 3);
        if (m_ptags->chl_c2h[m_id].type == MEMORYMAPPED)
          val = val + (0x1 << 2);
      }
      else{
        val = val + (0x1 << 2);
        if (m_ptags->chl_h2c[m_id].type == MEMORYMAPPED)
          val = val + (0x1 << 3);
      }

      dbypass.write_reg(m_offset + reg::bypass::cmd, val & 0x0F);
      return 0;
  };

  int Reset()
  {
      dbypass.write_reg(m_offset + reg::bypass::ctrl, (0x1 < 32));
      return 0;
  }

 private:
  TYPE m_type;
  uint32_t m_offset;
  uint32_t m_id;
  pcie_tags *m_ptags;
  Memory<mem::bypass> &dbypass;
};

#endif
