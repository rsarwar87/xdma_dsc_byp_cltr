----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11.12.2018 12:09:21
-- Design Name: 
-- Module Name: tbench - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity tbench is
--  Port ( );
end tbench;

architecture Behavioral of tbench is

  component bypass_controller is
    Port ( CLK : in STD_LOGIC;
           RST : in STD_LOGIC;
        
            bypass_ctl          : in std_logic_vector (15 downto 0) := (others => '0');
            bypass_dst_addr     : in std_logic_vector (63 downto 0) := (others => '0');
            bypass_src_addr     : in std_logic_vector (63 downto 0) := (others => '0');
            bypass_length       : in std_logic_vector (27 downto 0) := (others => '0');
            bypass_start        : in std_logic := '0';
            bypass_reset        : in std_logic := '0';
            bypass_cyclic       : in std_logic := '0';
            bypass_repeat       : in std_logic_vector (31 downto 0) := (others => '0');
            bypass_count        : out std_logic_vector (30 downto 0) := (others => '0');
            bypass_busy         : out std_logic := '0';
            bypass_awk          : out std_logic := '0';
            bypass_src_mm       : in std_logic := '0';
            bypass_dst_mm       : in std_logic := '0';
      
            dsc_byp_ctl         : out std_logic_vector (15 downto 0);
            dsc_byp_dst_addr    : out std_logic_vector (63 downto 0);
            dsc_byp_src_addr    : out std_logic_vector (63 downto 0);
            dsc_byp_length      : out std_logic_vector (27 downto 0);
            dsc_byp_ready       : in  std_logic;
            dsc_byp_load        : out std_logic
       
       );
    end component bypass_controller;


        signal bypass_ctl          : std_logic_vector (15 downto 0) := (others => '0');
        signal bypass_dst_addr     : std_logic_vector (63 downto 0) := (others => '0');
        signal bypass_src_addr     : std_logic_vector (63 downto 0) := (others => '0');
        signal bypass_length       : std_logic_vector (27 downto 0) := (others => '0');
        signal bypass_start        : std_logic := '0';
        signal bypass_reset        : std_logic := '0';
        signal bypass_cyclic       : std_logic := '0';
        signal bypass_repeat       : std_logic_vector (31 downto 0) := (others => '0');
        signal bypass_count        : std_logic_vector (30 downto 0) := (others => '0');
        signal bypass_busy         : std_logic := '0';
        signal bypass_awk          : std_logic := '0';
        signal bypass_src_mm       : std_logic := '0';
        signal bypass_dst_mm       : std_logic := '0';
        
        signal dsc_byp_ctl            : std_logic_vector (15 downto 0) := (others => '0');
        signal dsc_byp_dst_addr       : std_logic_vector (63 downto 0) := (others => '0');
        signal dsc_byp_src_addr       : std_logic_vector (63 downto 0) := (others => '0');
        signal dsc_byp_length         : std_logic_vector (27 downto 0) := (others => '0');
        signal dsc_byp_ready          : std_logic := '0';
        signal dsc_byp_load, RST, CLK : std_logic := '0';
        
        constant CLK_PERIOD : time := 4 ns;
begin


byps_ctlr : bypass_controller 
    Port map ( 
        CLK => CLK,
        RST => RST, 
        
        bypass_ctl        => bypass_ctl, 
        bypass_dst_addr   => bypass_dst_addr, 
        bypass_src_addr   => bypass_src_addr, 
        bypass_length     => bypass_length, 
        bypass_start      => bypass_start, 
        bypass_reset      => bypass_reset, 
        bypass_cyclic     => bypass_cyclic,
        bypass_repeat     => bypass_repeat,
        bypass_count      => bypass_count, 
        bypass_busy       => bypass_busy, 
        bypass_awk        => bypass_awk,
        bypass_dst_mm     => bypass_dst_mm,
        bypass_src_mm     => bypass_src_mm,
      

        dsc_byp_ctl       => dsc_byp_ctl,
        dsc_byp_dst_addr  => dsc_byp_dst_addr, 
        dsc_byp_src_addr  => dsc_byp_src_addr,   
        dsc_byp_length    => dsc_byp_length, 
        dsc_byp_ready     => dsc_byp_ready,
        dsc_byp_load      => dsc_byp_load
                  
       );

   -- Clock process definitions( clock with 50% duty cycle is generated here.
   Clk_process :process
   begin
        Clk <= '0';
        --adcin <= std_logic_vector(unsigned(adcin) + 1);
        wait for CLK_PERIOD/2;  --for half of clock period clk stays at '0'.
        Clk <= '1';
        wait for CLK_PERIOD/2;  --for next half of clock period clk stays at '1'.
   end process;


  -- Stimulus process, Apply inputs here.
  stim_proc: process
   begin        
            RST <='0';                    --then apply reset for 2 clock cycles.
        wait for CLK_PERIOD*10; --wait for 10 clock cycles.
            RST <='1';                    --then apply reset for 2 clock cycles.
            dsc_byp_ready <= '1';
        wait for CLK_PERIOD*10;
            RST <='1';                    --then pull down reset for 20 clock cycles.
            bypass_ctl <= x"0003";
            bypass_dst_addr   <= (others => '0'); 
            bypass_src_addr   <= (others => '0'); 
            bypass_dst_mm     <= '0';
            bypass_src_mm     <= '1'; 
            bypass_length     <= x"0000200";
            bypass_start      <= '1'; 
            bypass_reset      <= '0'; 
            bypass_cyclic     <= '0';
            bypass_repeat     <= x"00000020";
        wait for CLK_PERIOD;
            bypass_start      <= '0'; 
            bypass_ctl <= x"000f";
        wait for CLK_PERIOD*10;
            bypass_reset      <= '1';
        wait for CLK_PERIOD*1;
            bypass_reset      <= '0'; 
        wait for CLK_PERIOD*10;
            bypass_start      <= '1';
        wait for CLK_PERIOD*10;
            bypass_start      <= '0';
            dsc_byp_ready      <= '1';
        wait for CLK_PERIOD*10;
            dsc_byp_ready      <= '1';
        wait for CLK_PERIOD*100;
            dsc_byp_ready      <= '0';
        wait for CLK_PERIOD*100;
            dsc_byp_ready      <= '1';
        wait for CLK_PERIOD*10000;
            --GATE_SIZE.short <= "010111";               --pull down reset and let the counter run.
            --GATE_SIZE.short_d <= "01000011";
            --reset <='1';                    --then apply reset for 2 clock cycles.
        wait for CLK_PERIOD*10; --wait for 10 clock cycles.
            RST <='0';                    --then apply reset for 2 clock cycles.
        
        wait;
  end process;

end Behavioral;
