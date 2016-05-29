all:	hexdriver.vcd viewvectors

rt_functions.o: rtclock/rt_functions.c
	gcc -c rtclock/rt_functions.c

rt_clk.o: rtclock/rt_clk.vhd
	ghdl -a rtclock/rt_clk.vhd

hexdisplay/hexdisplay-qt: hexdisplay/hexdisplay-qt.cpp
	make -C hexdisplay

hexdisplay_c.o:	hexdisplay/hexdisplay.c hexdisplay/hexdisplay-qt
	gcc -c hexdisplay/hexdisplay.c -o hexdisplay_c.o

hexdisplay.o: hexdisplay/hexdisplay.vhd hexdisplay_c.o
	ghdl -a hexdisplay/hexdisplay.vhd

hexdriver.o: hexdriver.vhd
	ghdl -a hexdriver.vhd

testbench.o: testbench.vhd hexdisplay.o rt_functions.o rt_clk.o
	ghdl -a testbench.vhd

counter.o: counter.vhd
	ghdl -a counter.vhd

hexdriver_tb:	testbench.o counter.o hexdisplay.o hexdisplay_c.o hexdriver.o hexdisplay_c.o rt_functions.o
	ghdl -e -Wl,hexdisplay_c.o -Wl,rt_functions.o hexdriver_tb

hexdriver.vcd: hexdriver_tb
	ghdl -r hexdriver_tb --vcd=hexdriver.vcd

viewvectors:	hexdriver.vcd
	gtkwave hexdriver.vcd

clean:
	rm hexdriver.vcd *.o hexdriver_tb work-obj93.cf 
