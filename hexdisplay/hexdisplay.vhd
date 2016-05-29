--
--	Display Hex digits with VHDL
--

package hexdisplay is
	function init_hex_display (dummy: integer) return integer;
	attribute foreign of init_hex_display : function is "VHPIDIRECT init";
	
	function shutdown_hex_display (dummy: integer) return integer;
	attribute foreign of shutdown_hex_display : function is "VHPIDIRECT shutdown";

	function display_hex_digits(display: integer; segment: integer; s : string) return integer;
	attribute foreign of display_hex_digits :
		function is "VHPIDIRECT display_hex_digits";

	function hex_display_ui_exited(dummy: integer) return integer;
	attribute foreign of hex_display_ui_exited :
		function is "VHPIDIRECT hex_display_ui_exited";

end hexdisplay;

package body hexdisplay is

	function display_hex_digits(display: integer; segment: integer; s : string) return integer is
 	begin
    		assert false report "VHPI" severity failure;
	end display_hex_digits;

	function init_hex_display (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end init_hex_display;

	function shutdown_hex_display (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end shutdown_hex_display;

	function hex_display_ui_exited (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end hex_display_ui_exited;



end hexdisplay;	         
