//By: Vincent Ching-Roa
//Version: 1.2
//Description:
//  Pulse sensor code to be used with Arduino Uno
//  Shows the pulse signal (blue) in the serial plotter (Ctrl+Shift+L) or serial monitor (Ctrl+Shift+M)
//  Also displays heartrate (red) and last valid (above threshold) peak signal (green)
//  Works well with a clean signal ie. without a lot of movement. 
//  Make sure to use the finger strap or wrist strap

int PulseSensorPurplePin = 0;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int SignalMem[] = {-1,-1,-1}; //Signal memory for peak determination
int Threshold = 540;            // Determine which Signal to "count as a beat", and which to ignore.
int DeltaSignal1; // difference in signal[1] - signal[0] in temporary memory
int DeltaSignal2; // difference in signal[2] - signal[1] in temporary memory
int LastSig; //signal from previous reading
int PeakSig; //peak signal
unsigned long time; //tracks time 
unsigned long PeakTime[] = {0,0}; //times of peaks memory 
unsigned long Heartrate; //heart rate in bpm
unsigned long DeltaTime = 0; //difference between 2 peak times 
unsigned long Conversion = 60000; //conversion factor from ms to min 
boolean SignalMemFilled = false; //if SignalMem is filled with valid data 
boolean PeakTimeFilled = false; //if PeakTime is filled with valid data 


// The Setup Function:
void setup() {
  pinMode(LED13, OUTPUT);        // pin that will blink to your heartbeat!
  Serial.begin(9600);         // Set's up Serial Communication at certain speed.
}

// The Main Loop Function
void loop() {
  
  LastSig = Signal;
 
  Signal = analogRead(PulseSensorPurplePin);

  //Signal Memory Recording
  if(!SignalMemFilled){
    for(int i = 0; i<3; i++){
      if(SignalMem[i]==-1){
        SignalMem[i] = Signal;
        break;
      } else if (SignalMem[2]!=-1){
        SignalMemFilled = true;
        break;
      }
    }
  } else {
      SignalMem[0] = SignalMem[1];
      SignalMem[1] = SignalMem[2];
      SignalMem[2] = Signal;
      DeltaSignal1 = SignalMem[1]-SignalMem[0];
      DeltaSignal2 = SignalMem[2]-SignalMem[1];
  }

  //Signal Analysis
  if(DeltaSignal1>=0 && DeltaSignal2<=0){
      if(!PeakTimeFilled){
        for(int i = 0; i < 2; i++){
          if(PeakTime[i]==0){
            PeakTime[i] = millis();
            break;
          } else if(PeakTime[1]!=0){
            PeakTimeFilled = true;
            break;
          }
        }
      } else{
          if(LastSig>Threshold){
            PeakTime[0] = PeakTime[1];
            PeakTime[1] = millis();
            PeakSig = LastSig;
            DeltaTime = PeakTime[1]-PeakTime[0];
            if(Conversionon/DeltaTime<120&&Conversion/DeltaTime>30){ //Check bounds for valid heartrate [30 120] bpm
              Heartrate = Conversion/DeltaTime;
            }
          }
      }
  }
  
  Serial.print(Signal); //blue 
  Serial.print("  ");
  Serial.print(Heartrate); //red
  Serial.print("  ");
  Serial.println(PeakSig); //green

  delay(10);


}


