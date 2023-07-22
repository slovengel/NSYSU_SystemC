#include "RAM.h"

void RAM::read_data() {
	if ( wr.read() ){
		data_out = mem[addr.read()];
    //cout << "RAM " << mem[addr.read()] << " = mem[" << addr.read() << "]" << endl;
  }
}

void RAM::write_data() {
	if ( !wr.read() ){
		mem[addr.read()] = data_in;
    //cout << "RAM mem[" << addr.read() << "] = " << data_in << endl;
  }
}



