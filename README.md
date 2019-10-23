# Descriptor bypass controller for Xilinx XDMA IP for PCIe

The descriptor fetch engine can be bypassed on a per channel basis through Vivado IDE
parameters. A channel with descriptor bypass enabled accepts descriptor from its 
respective c2h_dsc_byp or h2c_dsc_byp bus. Before the channel accepts descriptors,
the Control register Run bit must be set. All descriptors written to the channel are
processed, barring writing of new descriptors when the channel buffer is full.

This IP controls the c2h/h2c_byp ports. The IP is controlled through a slave port, which
may be connected to PS/Microblace or controlled from the host throught the PCIe to AXI-Lite 
Master Interface. 

It works with both streaming and memory mapped mode. For C2H transfer, allocated a continuous memory for DMA using the mem="XX" boot parameter at known physical address, and set that address as the starting destination address on the FPGA. For H2C, you probably still need to ensure a single memory block, and set that address as the starting source address.

The IP provisions:

1. cyclic transfer

2. max payload size of 250 MB

3. a 32-bit repeat counter to allocate the number of times the transfer is to be repeated. Hence the Maximum transfer size per request is 250*4294967296 MB. The IP breaks down the request into smaller packets accodring to the Max payload/packet limits

It has 8 words to control the IP:

| Name                     | Offset | Bit         | Description                                                                               |
|--------------------------|--------|-------------|-------------------------------------------------------------------------------------------|
| XDMA Control Bit         | 0x00   | 31 downto 0 | Refer to Table 33/34 in PG195.                                                            |
| Destination_address_high | 0x04   | 31 downto 0 | MSB of the 64-bit address in the destination; if it is a stream, this register is ignored |
| Destination_address_high | 0x08   | 31 downto 0 | LSB of the 64-bit address in the destination; if it is a stream, this register is ignored |
| Source_address_high      | 0x0c   | 31 downto 0 | MSB of the 64-bit address in the source; if it is a stream, this register is ignored      |
| Source_address_low       | 0x10   | 31 downto 0 | LSB of the 64-bit address in the source; if it is a stream, this register is ignored      |
| Length                   | 0x14   | 31 downto 0 | Length of each transaction                                                                |
| N_Repeat                 | 0x18   | 31 downto 0 | Number of times the transaction is to be repeated. Must be one or more.                   |
| Control Bit              | 0x1c   | 31          | Stop/Reset bypass IP, the last transaction issued will be completed by the xdma (Not self clearing)           |
| Control Bit              | 0x1c   | 30 downto 4 | Reserved                                                                                  |
| Control Bit              | 0x1c   | 3           | Set to indicate that the destination is memory mapped, clear if destination is streaming  |
| Control Bit              | 0x1c   | 2           | Set to indicate that the source is memory mapped, clear if source is streaming            |
| Control Bit              | 0x1c   | 1           | Set to enable cyclic operation, i.e. transactions will loop infinitely                    |
| Control Bit              | 0x1c   | 0           | Start issuing commands to the XDMA. it is self clearing                                   |

## Reference
 
How to reserve memory block on linux machine: 

 -- append GRUB_CMDLINE_LINUX="mem=7000m nopat" to /etc/default/grub
 
 -- exec sudo update-grub

How to access reserved memory:
https://elinux.org/Memory_Management#Reserving_.28and_accessing.29_the_top_of_memory_on_startup

## TODO:

    - add cpp driver

## Driver
1. Python driver to control IP over PCIe to AXI-lite master interface is provided in drivers/bypass_controller_v1_0/python.

2. Cpp-14 driver to control IP over PCIe to AXI-lite master interface is provided in drivers/bypass_controller_v1_0/python.

3. C driver to control IP over PCIe to AXI-lite master interface is provided in drivers/bypass_controller_v1_0/python.
