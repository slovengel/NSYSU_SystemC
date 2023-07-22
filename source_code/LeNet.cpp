#include "LeNet.h"
// vvvvv put your code here vvvvv
void Conv::run(){
  if(rst){
    //cout << "---" << sc_core::sc_get_current_process_b()->get_parent_object()->basename() << endl;
    start=true, isRead=true, isConv=false, isNext=false;
    inputPos=0, inputIndex=0, kernalPos=0, kernalIndex=0, count=0;
  }else if(start){
    //cout << "start..." << endl;
    output_valid.write(false);
    input_rd.write(false);
    weight_rd.write(false);
    if(isRead){
      //cout << "read..." << endl;
      if(count<inputChannel*inputSize*inputSize){
        input_rd.write(true);
        input_addr.write(inputSkip+count); 
        //cout << "ask input " << inputSkip+count << endl; 
      }else if(count-inputChannel*inputSize*inputSize<filter*inputChannel*kernalSize*kernalSize+filter){
        weight_rd.write(true);
        weight_addr.write(weightSkip+count-inputChannel*inputSize*inputSize);
        //cout << "ask weight " << weightSkip+count-inputChannel*inputSize*inputSize << endl; 
      }
      if(count>=1+error && count<inputChannel*inputSize*inputSize+1+error){
        input[inputIndex][inputPos/inputSize][inputPos%inputSize]=input_data;
        //cout << "input[" << inputIndex << "][" << inputPos/inputSize << "][" << inputPos%inputSize << "]=" << input_data << endl; 
        inputPos++;
        if(inputPos==inputSize*inputSize){
          inputPos=0;
          inputIndex++;
          if(inputIndex==inputChannel){
            inputIndex=0;
          }
        }
      }else if(count>=inputChannel*inputSize*inputSize+2 && count<inputChannel*inputSize*inputSize+2+filter*inputChannel*kernalSize*kernalSize+filter){
        if(inputIndex==inputChannel){
          bias[kernalIndex]=weight_data;
          //cout << "bias[" << kernalIndex << "]=" << weight_data << endl; 
        }else{
          kernal[kernalIndex][inputIndex][kernalPos/kernalSize][kernalPos%kernalSize]=weight_data;
          //cout << "kernal[" << kernalIndex << "][" << inputIndex << "][" << kernalPos/kernalSize << "][" << kernalPos%kernalSize << "]=" << weight_data << endl; 
        }
        kernalPos++;
        if(kernalPos==kernalSize*kernalSize || inputIndex==inputChannel){
          kernalPos=0;
          inputIndex++;
          if(inputIndex==inputChannel+1){
            inputIndex=0;
            kernalIndex++;
            if(kernalIndex==filter){
              kernalIndex=0;
              isRead=false;
              isConv=true;
            }
          }
        }
      }
      count++;
      if(isRead==false){
        count=0;
      }
    }
    if(isConv){
      //cout << "conv..." << endl;
      sum=0;
      for(int l=0; l<inputChannel; l++){
        for(int m=0; m<kernalSize; m++){
          for(int n=0; n<kernalSize; n++){
            sum+=input[l][inputPos/(inputSize-kernalSize+1)+m][inputPos%(inputSize-kernalSize+1)+n]*kernal[kernalIndex][l][m][n];
            //cout << "sum+=input[" << l << "][" << inputPos/(inputSize-kernalSize+1)+m << "][" << inputPos%(inputSize-kernalSize+1)+n << "]=" << input[l][inputPos/(inputSize-kernalSize+1)+m][inputPos%(inputSize-kernalSize+1)+n] << "*kernal[" << kernalIndex << "][" << l << "][" << m << "][" << n << "]=" << kernal[kernalIndex][l][m][n] << endl;
            //cout << "sum=" << sum << endl;
          }
        }
      }
      sum+=bias[kernalIndex];
      //cout << "sum+=bias[" << kernalIndex << "]=" << bias[kernalIndex] << endl;
      //cout << "sum=" << sum << endl;
      output_valid.write(false);
      output_addr.write(count);
      if(sum<0){
        sum=0;
      }
      #ifdef fixed_DATA_TYPE
      if(sum.range(31, 16)){
        sum[15]=0;
        sum.range(14, 0)=32767;
      }
      #endif
      output_data.write((DATA_TYPE)sum);
      count++;
      inputPos++;
      if(inputPos==(inputSize-kernalSize+1)*(inputSize-kernalSize+1)){
        inputPos=0;
        kernalIndex++;
        if(kernalIndex==filter){
          kernalIndex=0;
          isConv=false;
          isNext=true;
        }
      }
    }
    if(isNext){
      for(int i = 0; i < inputChannel; i++){
        for(int j = 0 ; j < inputSize; j++){
          delete [] input[i][j];
        }
        delete [] input[i];
      }
      delete [] input;
      
      for(int i = 0; i < filter; i++){
        for(int j = 0; j < inputChannel; j++){
          for(int k = 0 ; k < kernalSize; k++){
            delete [] kernal[i][j][k];
          }
          delete [] kernal[i][j];
        }
        delete [] kernal[i];
      }
      delete [] kernal;
      
      delete [] bias;
      
      callRestNext.notify();
      isNext=false;
      start=false;
    }
  }
}

void Conv::resetNext(){
  next.write(true);
 	wait( 3, SC_NS );
 	next.write(false);  
}

void MaxPooling::run(){
  if(rst){
    //cout << "---" << sc_core::sc_get_current_process_b()->get_parent_object()->basename() << endl;
    start=true, isRead=true, isMaxP=false, isNext=false;
    inputPos=0, inputIndex=0, count=0;
  }else if(start){
    output_valid.write(false);
    input_rd.write(false);
    if(isRead){
      //cout << "read..." << endl;
      if(count<inputChannel*inputSize*inputSize){
        input_rd.write(true);
        input_addr.write(inputSkip+count); 
        //cout << "ask input " << inputSkip+count << endl; 
      }
      if(count>=1 && count<inputChannel*inputSize*inputSize+1){
        input[inputIndex][inputPos/inputSize][inputPos%inputSize]=input_data;
        //cout << "input[" << inputIndex << "][" << inputPos/inputSize << "][" << inputPos%inputSize << "]=" << input_data << endl; 
        inputPos++;
        if(inputPos==inputSize*inputSize){
          inputPos=0;
          inputIndex++;
          if(inputIndex==inputChannel){
            inputIndex=0;
            isRead=false;
            isMaxP=true;
          }
        }
      }
      count++;
      if(isRead==false){
        count=0;
      }
    }
    if(isMaxP){
      //cout << "maxp..." << endl;
      max=0;
      //cout << "inputPos=" << inputPos;
      for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
          if(input[inputIndex][inputPos/((inputSize-kernalSize)/2+1)*2+i][inputPos%((inputSize-kernalSize)/2+1)*2+j]>max){
            max=input[inputIndex][inputPos/((inputSize-kernalSize)/2+1)*2+i][inputPos%((inputSize-kernalSize)/2+1)*2+j];
          }
          //cout << " input[" << inputIndex << "][" << inputPos/((inputSize-kernalSize)/2+1)*2+i << "][" << inputPos%((inputSize-kernalSize)/2+1)*2+j << "]=" << input[inputIndex][inputPos/((inputSize-kernalSize)/2+1)*2+i][inputPos%((inputSize-kernalSize)/2+1)*2+j];
        }
      }
      //cout << " max=" << max << endl;
      output_valid.write(false);
      output_addr.write(count);
      output_data.write(max);
      count++;
      inputPos++;
      if(inputPos == ((inputSize-kernalSize)/2+1) * ((inputSize-kernalSize)/2+1) ){
        inputPos=0;
        inputIndex++;
        if(inputIndex==inputChannel){
          inputIndex=0;
          isMaxP=false;
          isNext=true;
        }
      }
    }
    if(isNext){
      for(int i = 0; i < inputChannel; i++){
        for(int j = 0 ; j < inputSize; j++){
          delete [] input[i][j];
        }
        delete [] input[i];
      }
      delete [] input;
      callRestNext.notify();
      isNext=false;
      start=false;
    }
  }
}

void MaxPooling::resetNext(){
  next.write(true);
 	wait( 3, SC_NS );
 	next.write(false);  
}

void Dense::run(){
  if(rst){
    //cout << "---" << sc_core::sc_get_current_process_b()->get_parent_object()->basename() << endl;
    start=true, isRead=true, isDes=false, isNext=false;
    inputPos=0, weightPos=0, count=0;
  }else if(start){
    //cout << "start..." << endl;
    output_valid.write(false);
    input_rd.write(false);
    weight_rd.write(false);
    if(isRead){
      //cout << "read..." << endl;
      if(count<inputSize){
        input_rd.write(true);
        input_addr.write(inputSkip+count); 
        //cout << "ask input " << inputSkip+count << endl; 
      }else if(count-inputSize<outputSize*inputSize+outputSize){
        weight_rd.write(true);
        weight_addr.write(weightSkip+count-inputSize);
        //cout << "ask weight " << weightSkip+count-inputSize << endl; 
      }
      if(count>=1 && count<inputSize+1){
        if(flat){
          input[16*(inputPos%16)+inputPos/16]=input_data;
          //cout << "input[" << 16*(inputPos%16)+inputPos/16 << "]=" << input_data << endl; 
        }else{
          input[inputPos]=input_data;
          //cout << "input[" << inputPos << "]=" << input_data << endl; 
        }
        inputPos++;
        if(inputPos==inputSize){
          inputPos=0;
        }
      }else if(count>=inputSize+2 && count<inputSize+2+outputSize*inputSize+outputSize){
        if(inputPos==inputSize){
          bias[weightPos]=weight_data;
          //cout << "bias[" << weightPos << "]=" << weight_data << endl; 
        }else{
          weight[weightPos][inputPos]=weight_data;
          //cout << "weight[" << weightPos << "][" << inputPos << "]=" << weight_data << endl; 
        }
        inputPos++;
        if(inputPos==inputSize+1){
          inputPos=0;
          weightPos++;
          if(weightPos==outputSize){
            weightPos=0;
            isRead=false;
            isDes=true;
          }
        }
      }
      count++;
      if(isRead==false){
        count=0;
      }
    }
    if(isDes){
      //cout << "dense..." << endl;
      sum=0;
      for(int i=0; i<inputSize; i++){
        sum+=input[i]*weight[weightPos][i];
        //cout << "sum+=input[" << i << "]*weight[" << weightPos << "][" << i << "]" << endl;
      }
      sum+=bias[weightPos];
      //cout << "sum+=bias[" << weightPos << "]" << endl;
      //cout << "sum=" << sum << endl;
      if(print){
        output_valid.write(true);
      }else{
        output_valid.write(false);
      }
      output_addr.write(count);
      if(sum<0){
        sum=0;
      }
      #ifdef fixed_DATA_TYPE
      if(sum.range(31, 16)){
        sum[15]=0;
        sum.range(14, 0)=32767;
      }
      #endif
      output_data.write((DATA_TYPE)sum);
      count++;
      weightPos++;
      if(weightPos==outputSize){
        weightPos=0;
        isDes=false;
        isNext=true;
      }
    }
    if(isNext){
      delete [] input;
      
      for(int i = 0; i < outputSize; i++){
        delete [] weight[i];
      }
      delete [] weight;
      
      delete [] bias;
      callRestNext.notify();
      isNext=false;
      start=false;
    }
  }
}

void Dense::resetNext(){
  next.write(true);
 	wait( 3, SC_NS );
 	next.write(false);  
}
// ^^^^^ put your code here ^^^^^
