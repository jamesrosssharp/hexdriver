--
--	Testbench for hex display drivers
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.hexdisplay.all;
use work.rt_utils.all;

entity hexdriver_tb is
end hexdriver_tb;

architecture rtl of hexdriver_tb is

	constant BITS : natural := 4;

	component counter 
		generic (WIDTH : natural);
		port ( 	clk : in std_logic;
	      		rst : in std_logic;
	       		output : out std_logic_vector (WIDTH-1 downto 0)
	     	);
 	end component;
	
	component hexdriver
		port (
			input : in std_logic_vector(3 downto 0);
			output : out std_logic_vector(7 downto 0)
		);
	end component;

	signal tb_clk : std_logic;
	signal tb_rst : std_logic := '0';
	signal tb_out : std_logic_vector (BITS-1 downto 0);
	signal tb_stop: std_logic := '0';

	signal hex_display_out : std_logic_vector(7 downto 0);
begin

clk0:	entity work.rt_clk
	generic map(ms => 100)
	port map (clk => tb_clk, stop => tb_stop);

ctr0:	counter generic map (WIDTH => BITS) 
		port map (clk => tb_clk, rst => tb_rst, output => tb_out);

hd0: 	hexdriver port map (input => tb_out, output => hex_display_out);

process
variable dummy : integer;
begin
	dummy := init_hex_display(0);
	tb_rst <= '1';
	wait for 5 ns;
	tb_rst <= '0';
	wait for 5 ns;
	wait;
end process;

process(tb_clk,tb_rst) is
   variable dummy : integer;
begin
    if rising_edge(tb_clk) then

      for i in 0 to 7 loop	
      	dummy := display_hex_digits(0,i,std_logic'image(hex_display_out(i)));
      end loop;

      if hex_display_ui_exited(0) = 1 then
        tb_stop <= '1';
		dummy := shutdown_hex_display(0);
	  end if;
    end if;
end process;

end rtl;
