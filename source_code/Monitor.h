#include "systemc.h"
#include "define.h"
#include <iostream>

using namespace std;

SC_MODULE( Monitor ) {	
	sc_in_clk clk;
	sc_in < bool > rst;
	sc_in < DATA_TYPE > data_in;
	sc_in < bool > data_valid;
	
	void monitor();
	
	int x, cycle;
	
	SC_CTOR( Monitor )
	{
		x = 0;
		cycle = 0;
		
		SC_METHOD( monitor );
		sensitive << clk.pos();
	}
};

