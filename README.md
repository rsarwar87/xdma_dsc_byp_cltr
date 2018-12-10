# Descriptor bypass controller for Xilinx XDMA IP for PCIe

The descriptor fetch engine can be bypassed on a per channel basis through Vivado IDE
parameters. A channel with descriptor bypass enabled accepts descriptor from its 
respective c2h_dsc_byp or h2c_dsc_byp bus. Before the channel accepts descriptors,
the Control register Run bit must be set. All descriptors written to the channel are
processed, barring writing of new descriptors when the channel buffer is full.

This IP controls the c2h/h2c_byp ports. The IP is controlled through a slave port, which
may be connected to PS/Microblace or controlled from the host throught the PCIe to AXI-Lite 
Master Interface. 

It has 8 words to control the IP:

    signal slv_reg0	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg1	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg2	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg3	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg4	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg5	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg6	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal slv_reg7	:std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);

    bypass_ctl <= slv_reg0(15 downto 0); -- table 2-4 PG195
    bypass_dst_addr <= slv_reg1 & slv_reg2; -- destination address
    bypass_src_addr <= slv_reg3 & slv_reg4; -- source address
    bypass_length <= slv_reg5(27 downto 0); -- length of frame
    bypass_reset <= slv_reg7(31); -- stop transfer
    bypass_cyclic <= slv_reg7(1); -- enable cyclic transfer
    bypass_start <= slv_reg7(0); -- start transfer
    bypass_repeat <= slv_reg6; -- repeat


TODO:
    - create test bench
    - test on device
