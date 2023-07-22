#include "systemc.h"
#include "define.h"
#include <iostream>

using namespace std;

SC_MODULE( RAM ) {	
	sc_in_clk clk;
	sc_in < bool > wr;
	sc_in < sc_uint<16> > addr;
	sc_out < DATA_TYPE > data_out;
	sc_in < DATA_TYPE > data_in;
	
	DATA_TYPE mem[65536];
	
	void read_data();
	void write_data();
	
	SC_CTOR( RAM )
	{
		for ( int i = 0 ; i < 65536 ; i++ )
			mem[i] = 0;
			
		SC_METHOD( read_data );
		sensitive << clk.neg();
		
		SC_METHOD( write_data );
		sensitive << clk.pos();
	}
};


