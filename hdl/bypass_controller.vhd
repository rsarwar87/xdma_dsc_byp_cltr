----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10.12.2018 19:57:18
-- Design Name: 
-- Module Name: bypass_controller - Behavioral
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

entity bypass_controller is
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
       
            dsc_byp_ctl         : out std_logic_vector (15 downto 0);
            dsc_byp_dst_addr    : out std_logic_vector (63 downto 0);
            dsc_byp_src_addr    : out std_logic_vector (63 downto 0);
            dsc_byp_length      : out std_logic_vector (27 downto 0);
            dsc_byp_ready       : in  std_logic;
            dsc_byp_load        : out std_logic
       
       );
end bypass_controller;

architecture Behavioral of bypass_controller is
    TYPE state_machine IS (reset_hold, seek, spin);  -- Define the states
    signal bmachine : state_machine := reset_hold;
    signal smachine : state_machine := reset_hold;
        signal byp_ctl          : std_logic_vector (15 downto 0) := (others => '0');
        signal byp_dst_addr     : std_logic_vector (63 downto 0) := (others => '0');
        signal byp_src_addr     : std_logic_vector (63 downto 0) := (others => '0');
        signal byp_length       : std_logic_vector (27 downto 0) := (others => '0');
        signal byp_repeat       : std_logic_vector (31 downto 0) := (others => '0');
        signal byp_count        : std_logic_vector (30 downto 0) := (others => '0');
        signal byp_busy         : std_logic := '0';
        signal byp_awk          : std_logic := '0';
        signal byp_cyclic       : std_logic := '0';
        
        signal byp_addr_cnt     : std_logic_vector (63 downto 0) := (others => '0');
begin

    process (CLK, RST) 
        variable cnt : integer := 4;
    begin
        if (RST = '0') then
            cnt := 4;
            dsc_byp_load <= '0';
            dsc_byp_ctl <= (others => '0');
            dsc_byp_src_addr <= (others => '0');
            dsc_byp_dst_addr <= (others => '0');
            dsc_byp_length <= (others => '0');
            byp_count <= (others => '0');
            byp_addr_cnt <= (others => '0');
        elsif (rising_edge(CLK)) then
            if (smachine = spin) then
                if (dsc_byp_ready = '1') then
                    if (cnt = 0) then
                        cnt := 4;
                        dsc_byp_ctl <= byp_ctl;
                        dsc_byp_src_addr <= byp_src_addr;
                        dsc_byp_dst_addr <= std_logic_vector(unsigned(byp_dst_addr) + unsigned(byp_addr_cnt));
                        dsc_byp_length <= byp_length;
                        byp_addr_cnt <= std_logic_vector(unsigned(byp_addr_cnt) + unsigned(byp_length));
                        dsc_byp_load <= '1';
                        byp_count <= std_logic_vector(unsigned(byp_count) + 1);
                    else 
                        dsc_byp_load <= '0';
                        cnt := cnt - 1;
                    end if;
                else
                    dsc_byp_ctl <= (others => '0');
                    dsc_byp_src_addr <= (others => '0');
                    dsc_byp_dst_addr <= (others => '0');
                    dsc_byp_length <= (others => '0');
                    byp_addr_cnt <= (others => '0');
                    dsc_byp_load <= '0';
                    cnt := 4;
                end if;
            else
                dsc_byp_ctl <= (others => '0');
                dsc_byp_src_addr <= (others => '0');
                dsc_byp_dst_addr <= (others => '0');
                dsc_byp_length <= (others => '0');
                dsc_byp_load <= '0';
                byp_count <= (others => '0');
                cnt := 4;
            end if;
        end if;
    end process;



    bypass_awk <= byp_awk;
    bypass_busy <= byp_busy;
    bypass_count <= byp_count;

    process (CLK, RST)
        variable pause : integer := 4;
    begin
        if (RST = '0') then
            pause := 4;
            byp_ctl <= (others => '0');
            byp_dst_addr <= (others => '0');
            byp_src_addr <= (others => '0');
            byp_length <= (others => '0');
            byp_repeat <= (others => '0');
            --byp_count <= (others => '0');
            byp_busy <= '0';
            byp_awk <= '0';
            byp_cyclic <= '0';
            smachine <= reset_hold;
        elsif (rising_edge(CLK)) then
            case (smachine) is
                when reset_hold =>
                    byp_ctl <= (others => '0');
                    byp_dst_addr <= (others => '0');
                    byp_src_addr <= (others => '0');
                    byp_length <= (others => '0');
                    byp_repeat <= (others => '0');
                    --byp_count <= (others => '0');
                    byp_busy <= '0';
                    byp_awk <= '0';
                    byp_cyclic <= '0';

                    if (pause = 0) then
                        pause := 4;
                        smachine <= seek;
                    else
                        pause := pause - 1;
                    end if;
                when seek => 
                    if (bypass_start = '1') then
                        smachine <= spin;
                        byp_ctl <= bypass_ctl;
                        byp_dst_addr <= bypass_dst_addr;
                        byp_src_addr <= bypass_src_addr;
                        byp_length <= bypass_length;
                        byp_busy <= '1';
                        byp_awk <= '1';
                        byp_cyclic <= bypass_cyclic;
                        byp_repeat <= bypass_repeat; 
                    end if; 
                when spin =>
                    if (bypass_reset = '1' OR bypass_cyclic = '0' OR byp_count = byp_repeat) then
                        smachine <= reset_hold;
                    end if;
                    byp_awk <= '0';
            end case;

        end if;
    end process;

end Behavioral;
