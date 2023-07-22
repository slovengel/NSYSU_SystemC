#include "Monitor.h"

void Monitor:: monitor() {
	if ( rst ) {
		
	}
	else if ( data_valid ) {
		#ifdef fixed_DATA_TYPE
			cout << x << ": " << double(data_in.read())/1024 << "!" << endl;
		#else
			cout << x << ": " << data_in.read() << "!" << endl;
		#endif
		
		x++;
		if ( x == 10 ){
			sc_stop();
    }
	}
}
