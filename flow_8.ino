const int flowPin = 2;    //This is the input pin on the Arduino
double flowRate = 0;    //This is the value we intend to calculate. 
volatile int count; //This integer needs to be set as volatile to ensure it updates correctly during the interrupt process.  
unsigned long tbefore = 0;
double t = 0;
int line = 0;

void setup() {
  
  pinMode(flowPin, INPUT_PULLUP);           //Sets the pin as an input
  attachInterrupt(0, Flow, CHANGE);  //Configures interrupt 0 (pin 2 on the Arduino Uno) to run the function "Flow"  
  Serial.begin(115200);                //Start Serial
  //Serial.println("CLEARDATA");        //Communication with PLX DAQ Excel
  //Serial.println("LABEL,Time,Tempo,FlowRate"); //Col names in Excel
  
}
void loop()
{
  if (millis() - tbefore > 200
  )
   {
    
    line++;
    noInterrupts(); //Disable the interrupts on the Arduino
    t = millis();
    int my_count = count ;      // Reset the counter so we start counting from 0 again
    count = 0; // update count
    interrupts();   //Enables interrupts on the Arduino
  
    // handle sampling time in one place and consistently:
     unsigned long now = millis() ;   // sample time once only per loop, otherwise you mis-account it.
     unsigned long duration = now - tbefore ;
     tbefore = now;  // setup tbefore for next time, 
  
     flowRate = ((1000 / duration) * my_count) / 5.5;  // use the local variable only here.
     //flowRate = (flowRate / 60) * 1000 ; // ml/s
     
    //Serial.print("DATA,TIME,");  //Printing in the Excel file
    //Serial.print(t);
    //Serial.print(",");
    Serial.println(flowRate);

   }
   
  if (line > 50)
  {
    line = 0;
    //Serial.println("ROW,SET,2"); //Jump to row 2 in the PLX DAQ excel file
  }
  
}
 
void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}
