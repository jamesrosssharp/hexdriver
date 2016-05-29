--
--	Counter
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter is
	generic (WIDTH : natural);
	port ( clk : in std_logic;
	       rst : in std_logic;
	       output : out std_logic_vector (WIDTH-1 downto 0)
	     );
end counter;

architecture rtl of counter is
	signal count : unsigned (WIDTH-1 downto 0); 
begin	
	
cnt0: process (clk, rst)
begin
	if (rst = '1') then
		count <= (others => '0');
	elsif rising_edge(clk) then
		count <= count + 1;
	end if;
end process;

output <= std_logic_vector(count);

end rtl;
