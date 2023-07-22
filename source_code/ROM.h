#include "systemc.h"
#include "define.h"
#include <iostream>
#include <fstream>

using namespace std;

SC_MODULE( ROM ) {
	sc_in_clk clk;
	sc_in < bool > ird;
	sc_in < sc_uint<16> > addr;
	sc_out < DATA_TYPE > data_out;
	
	DATA_TYPE mem[45210];
	void read_data();
	
	SC_CTOR( ROM )
	{
		string t_b, t_c;
		DATA_TYPE t_d;
		ifstream win;
		
		cout << "Loading weights and input data...\n";
		// vvvvv change the path of input file here vvvvv
		win.open( "/home/systemc_28/final_project/LeNet_All_b.txt" );
		for ( int i = 0 ; i < 45210 ; i++ ) {
			win >> t_b >> t_c >> t_d;
			#ifdef fixed_DATA_TYPE
				READ_DATA_TYPE temp;
				temp = t_b.c_str();
				mem[i] = temp.to_int();
			#else
				mem[i] = t_d;
			#endif
		}
		win.close();
		cout << "done!\n";
		
		SC_METHOD( read_data );
		sensitive << clk.pos();
	}
};


