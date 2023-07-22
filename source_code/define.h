#define fixed_DATA_TYPE

#ifdef fixed_DATA_TYPE
	#define READ_DATA_TYPE sc_lv < 16 >
	#define DATA_TYPE sc_int < 16 >
	#define MUL_DATA_TYPE sc_int < 32 >
#else
	#define DATA_TYPE float
	#define MUL_DATA_TYPE float
#endif
