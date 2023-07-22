#include "ROM.h"

void ROM::read_data() {	
	if ( ird.read() ){
		data_out = mem[addr.read()];
    //cout << "ROM " << mem[addr.read()] << " = mem[" << addr.read() << "]" << endl;
  }
}



