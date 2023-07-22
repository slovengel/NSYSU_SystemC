#include "systemc.h"
#include "define.h"
#include <iostream>

using namespace std;

// vvvvv put your code here vvvvv

SC_MODULE( Conv ) {
  sc_in_clk clk;
	sc_in < bool > rst;
	
  sc_out < bool > input_rd;
  sc_out < sc_uint<16> > input_addr;
  sc_in < DATA_TYPE > input_data;
  
  sc_out < bool > weight_rd;
  sc_out < sc_uint<16> > weight_addr;
  sc_in < DATA_TYPE > weight_data;
  
  sc_out < bool > output_valid;
  sc_out < sc_uint<16> > output_addr;
  sc_out < DATA_TYPE > output_data;
 
  sc_out < bool > next;
  
  int inputSize;
  int inputChannel;
  int kernalSize;
  int filter;
  
  DATA_TYPE ***input, ****kernal, *bias;
  MUL_DATA_TYPE sum;
  int inputPos, inputIndex, kernalPos, kernalIndex, inputSkip, weightSkip, count, error;
  bool start, isRead, isConv, isNext;
  
  sc_event callRestNext;
  
  void run();
  void resetNext();
  
  SC_HAS_PROCESS( Conv );

  Conv( sc_module_name name, int size, int channel_in, int channel_out, int input_data_addr, int weight_addr) : sc_module(name), inputSize(size), inputChannel(channel_in), filter(channel_out), inputSkip(input_data_addr), weightSkip(weight_addr){
  
	  kernalSize = 5;
    error=(!input_data_addr? 0:1);
     
	  input = new DATA_TYPE** [inputChannel];
    for(int i = 0; i < inputChannel; i++){
      input[i] = new DATA_TYPE* [inputSize];
      for(int j = 0 ; j < inputSize; j++){
        input[i][j] = new DATA_TYPE [inputSize];
      }
    }
    
    kernal = new DATA_TYPE*** [filter];
    for(int i = 0; i < filter; i++){
      kernal[i] = new DATA_TYPE** [inputChannel];
      for(int j = 0; j < inputChannel; j++){
        kernal[i][j] = new DATA_TYPE* [kernalSize];
        for(int k = 0 ; k < kernalSize; k++){
          kernal[i][j][k] = new DATA_TYPE [kernalSize];
        }
      }
    }
    
    bias = new DATA_TYPE [filter];
   
    SC_METHOD( run );
    sensitive << rst.pos() << clk.pos();
    dont_initialize();
    
    SC_THREAD( resetNext );
    sensitive << callRestNext;
    dont_initialize();
  }
};

SC_MODULE( MaxPooling ) {
  sc_in_clk clk;
	sc_in < bool > rst;
	
  sc_out < bool > input_rd;
  sc_out < sc_uint<16> > input_addr;
  sc_in < DATA_TYPE > input_data;
  
  sc_out < bool > output_valid;
  sc_out < sc_uint<16> > output_addr;
  sc_out < DATA_TYPE > output_data;
 
  sc_out < bool > next;
  
  int inputSize;
  int inputChannel;
  int kernalSize;
  
  DATA_TYPE ***input, max;
  int inputPos, inputIndex, inputSkip, count;
  bool start, isRead, isMaxP, isNext;
  
  sc_event callRestNext;
  
  void run();
  void resetNext();
  
  SC_HAS_PROCESS( MaxPooling );
  
  MaxPooling( sc_module_name name, int size, int channel_in, int input_data_addr) : sc_module( name ), inputSize( size ), inputChannel(channel_in), inputSkip(input_data_addr){
	  kernalSize = 2;
	  
    input = new DATA_TYPE** [inputChannel];
    for(int i = 0; i < inputChannel; i++){
      input[i] = new DATA_TYPE* [inputSize];
      for(int j = 0 ; j < inputSize; j++){
        input[i][j] = new DATA_TYPE [inputSize];
      }
    }
     
    SC_METHOD( run );
    sensitive << rst.pos() << clk.pos();
    dont_initialize();
    
    SC_THREAD( resetNext );
    sensitive << callRestNext;
    dont_initialize();
  } 
};

SC_MODULE( Dense ) {
  sc_in_clk clk;
	sc_in < bool > rst;
	
  sc_out < bool > input_rd;
  sc_out < sc_uint<16> > input_addr;
  sc_in < DATA_TYPE > input_data;
  
  sc_out < bool > weight_rd;
  sc_out < sc_uint<16> > weight_addr;
  sc_in < DATA_TYPE > weight_data;
  
  sc_out < bool > output_valid;
  sc_out < sc_uint<16> > output_addr;
  sc_out < DATA_TYPE > output_data;
 
  sc_out < bool > next;
  
  int inputSize;
  int outputSize;
  
  DATA_TYPE *input, **weight, *bias;
  MUL_DATA_TYPE sum;
  int inputPos, weightPos, inputSkip, weightSkip, count;
  bool start, isRead, isDes, isNext, flat, print;
  
  sc_event callRestNext;
  
  void run();
  void resetNext();
  
  SC_HAS_PROCESS( Dense );

  Dense( sc_module_name name, int neuron_in, int neuron_out, int input_data_addr, int weight_addr) : sc_module( name ), inputSize( neuron_in ), outputSize( neuron_out ), inputSkip(input_data_addr), weightSkip(weight_addr)
  {
    flat=inputSize==256;
    
    print=outputSize==10;
    
    input = new DATA_TYPE [inputSize];
    
    weight = new DATA_TYPE* [outputSize];
    for(int i = 0; i < outputSize; i++){
      weight[i] = new DATA_TYPE [inputSize];
    }
    
    bias = new DATA_TYPE [outputSize];
  
    SC_METHOD( run );
    sensitive << rst.pos() << clk.pos();
    dont_initialize();
    
    SC_THREAD( resetNext );
    sensitive << callRestNext;
    dont_initialize();
  } 

};

// ^^^^^ put your code here ^^^^^
