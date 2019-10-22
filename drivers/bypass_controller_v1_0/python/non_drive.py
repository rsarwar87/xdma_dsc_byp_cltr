#!/usr/bin/python3
import sys
import devmem
import numpy as np
import matplotlib.pyplot as plt
import time


class PCIeDriver:
  def __init__(self, driver_id = 0):
    self.dId = driver_id
    self.fcontrol = "/dev/xdma%d_control" % driver_id
    self.fuser = "/dev/xdma%d_user" % driver_id
    self.mmControl = devmem.DevMem(0x0, 16*1024, self.fcontrol, 1)
    self.mmUser = devmem.DevMem(0x0, 3*64*1024, self.fuser, 1)

    self.offset_type = 0x1000
    self.offset_ch = 0x100
    self.offset_id = 0x00
    self.offset_ctrl = 0x04
    self.offset_status = 0x40
    self.nChannelsIn = 0
    self.nChannelsOut = 0

    self.en_chtype = ["unidentified", "memory mapped", "streaming"]

    self.h2c = 0 
    self.c2h = 0 
    self.h2c_type = np.zeros(4) 
    self.h2c_id = np.zeros(4) 
    self.c2h_type = np.zeros(4) 
    self.c2h_id = np.zeros(4) 

  def ListAllChannels(self):
    for x in range(4):
      identity = self.mmControl.read(0x0 + x * (self.offset_ch), 1).data[0]
      if ((identity >> 20) == 0x1fc):
        self.c2h_id[x] = ((identity >> 8) & 0xF) 
        self.c2h_type[x] = ((identity >> 15) & 0x1) + 1
        self.c2h = self.c2h + 1 
        
      identity = self.mmControl.read(x * (self.offset_ch) + self.offset_type, 1).data[0]
      if ((identity >> 20) == 0x1fc):
        self.h2c_id[x] = ((identity >> 8) & 0xF) 
        self.h2c_type[x] = ((identity >> 15) & 0x1) + 1
        self.h2c = self.h2c + 1 
  
  def PrintAllChannels(self):
    print ("No of C2H channels detected: %d" %self.c2h)
    for x in range(self.c2h):
      print ("\tChannel: %d\n\t\tID: %d\n\t\tType: %s\n" %(x, self.c2h_id[x], self.en_chtype[np.int32(self.c2h_type[x])]))
    print ("No of H2C channels detected: %d" %self.h2c)
    for x in range(self.h2c):
      print ("\tChannel: %d\n\t\tID: %d\n\t\tType: %s\n" %(x, self.h2c_id[x], 
        self.en_chtype[np.int32(self.h2c_type[x])]))

  def BypassDescriptorReset(self, addr):
    self.mmUser.write(addr + 7*0x4, 0x1)
  
  def BypassDescriptorRepeat(self, addr):
    self.mmUser.write(addr + 6*0x4, 1 << 32)
  
  def BypassDescriptorIssueCmd(self, addr, ctrl_flag, d_addr, s_addr, length, 
      is_cyclic, repeat = 1, is_c2h = 0, ch_id = 0):
    self.mmUser.write(addr       , [ctrl_flag          ])
    self.mmUser.write(addr + 0x04, [d_addr >> 32       ])
    self.mmUser.write(addr + 0x08, [d_addr & 0xFFFFFFFF])
    self.mmUser.write(addr + 0x0C, [s_addr >> 32       ])
    self.mmUser.write(addr + 0x10, [s_addr & 0xFFFFFFFF])
    self.mmUser.write(addr + 0x14, [length & 0x0FFFFFFF])
    self.mmUser.write(addr + 0x18, [repeat & 0x0FFFFFFF])
    val = ((is_cyclic & 0x1) << 1)
    if (is_c2h == 0):
      print ("Issueing command for H2C interface")
      val = val + (0x1 << 2) 
      if self.h2c_type[ch_id] == 1:
        val = val + (0x1 << 3)
    else:
      print ("Issueing command for C2H interface")
      val = val + (0x1 << 3) 
      if self.c2h_type[ch_id] == 1:
        val = val + (0x1 << 2)
    self.mmUser.write(addr + 0x1C, [0xD & 0xFFFFFFFF]) #[(0x1 + val) & 0xF])
    
    print ("FLAGs          ", hex(addr)       , hex(ctrl_flag)          )
    print ("dest addr high ", hex(addr + 0x04), hex(d_addr >> 32)       )
    print ("dest addr low  ", hex(addr + 0x08), hex(d_addr & 0xFFFFFFFF))
    print ("src addr high  ", hex(addr + 0x0C), hex(s_addr >> 32)       )
    print ("src addr low   ", hex(addr + 0x10), hex(s_addr & 0xFFFFFFFF))
    print ("length         ", hex(addr + 0x14), hex(length & 0x0FFFFFFF))
    print ("cmd            ", hex(addr + 0x1C), hex((0x1 + val) & 0xF)  )
  
  def WriteUser(self, addr, off, val):
    self.mmUser.write(addr + off, val)
    return val
  
  def ReadUser(self, addr, off, n_samples):
    return self.mmUser.read(addr + off, n_samples)
  
  def ToggleSGDMA(self, val):
    for x in range(4):
      if (self.c2h_type[x] == 2) :
        identity = self.mmControl.write(0x4 + x * (self.offset_ch), val)
      if (self.h2c_type[x] == 2) :
        identity = self.mmControl.write(0x4 + x * (self.offset_ch) 
            + self.offset_type, val)
      

def main():
  driv = PCIeDriver()
  driv.ListAllChannels()
  driv.PrintAllChannels()
  driv.ToggleSGDMA([0x1])

  mem = devmem.DevMem(0x34bc00000, 100*1024*1024, "/dev/mem", 0)
  ptr_read  = 10*1024*1024
  ptr_write = 30*1024*1024
  sz_buffer = 10*1024*1024
  mem.write(ptr_read, (np.multiply(np.sin(range(sz_buffer)) + 1, 1000).astype(int).tolist()) )
  mem.write(ptr_write, np.zeros(sz_buffer).astype(int).tolist())

  print((driv.ReadUser(0x00, 0, 1)).data ) 
  print(" Writing gpio@0x08: {0}" %(driv.WriteUser(0x00, 0x8, [0xF45F45F])) ) 
  #mem.write(ptr_read, np.sin(np.arange(sz_buffer)))
  #mem.write(ptr_write, np.zeros(sz_buffer))

  driv.BypassDescriptorIssueCmd(0x10000, 0x10, 0x34bc00000 + ptr_write, 0, sz_buffer, 0, 1, 1)
  driv.BypassDescriptorIssueCmd(0x30000, 0x10, 0, 0x34bc00000 + ptr_read, sz_buffer, 0, 1, 0)

  time.sleep(5)
  plt.plot(mem.read(ptr_read , 1024).data)
  plt.plot(mem.read(ptr_write, 1024).data)
  plt.show()

if __name__ ==  '__main__':
    sys.exit(main())

