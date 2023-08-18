#include "my_library.h"
float userInput;
float timePerRev; //time Per Revolution
float value_of_ocr1a; //value of OCR1A Register stored in this variable
int intValue;
void calculation ()
{
  timePerRev = 1/((userInput/60)*200);
  value_of_ocr1a = ((timePerRev*16000000)/256);
  intValue = int(value_of_ocr1a);

}
