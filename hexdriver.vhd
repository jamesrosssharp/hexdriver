--
--	Hex driver
--

library ieee;
use ieee.std_logic_1164.all;

entity hexdriver is
	port (
		input : in std_logic_vector(3 downto 0);
		output : out std_logic_vector(7 downto 0)
	);
end hexdriver;

--
--	Common cathode 7 Segment Display Driver
--        -          a-0   
--	 	 | |      f-5   b-1
--        -          g-6  
--       | |      e-4   c-2
--	  	  -  .       d-3   dp-7
--

architecture rtl of hexdriver is
begin
output <= "11000000" when input = "0000" else -- 0
	  "11111001" when input = "0001" else -- 1
	  "10100100" when input = "0010" else -- 2
	  "10110000" when input = "0011" else -- 3
	  "10011001" when input = "0100" else -- 4
	  "10010010" when input = "0101" else -- 5
	  "10000010" when input = "0110" else -- 6
	  "11111000" when input = "0111" else -- 7
	  "10000000" when input = "1000" else -- 8
	  "10010000" when input = "1001" else -- 9
	  "10001000" when input = "1010" else -- A
	  "10000011" when input = "1011" else -- b
	  "11000110" when input = "1100" else -- C
	  "10100001" when input = "1101" else -- d
	  "10000100" when input = "1110" else -- e
	  "10001110" when input = "1111" else -- f
	  "00000000";

end rtl;

