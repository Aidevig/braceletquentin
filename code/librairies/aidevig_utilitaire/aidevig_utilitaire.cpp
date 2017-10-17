#include <aidevig_utilitaire.h>


///*****************TIMER INTERRUPTS**********************************************************
///*******VARIABLES GLOBALES ENREGISTREMENT AUDIO SUR INTERRUPTIONS TIMER
unsigned long recByteCount;
unsigned long recByteSaved;
byte buf00[512]; // buffer array 1
byte buf01[512]; // buffer array 2
unsigned int bufByteCount;
byte bufWrite;

void TC4_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{
  // Check for overflow (OVF) interrupt
  if (TC4->COUNT8.INTFLAG.bit.OVF && TC4->COUNT8.INTENSET.bit.OVF)
  {
    // Put your timer overflow (OVF) code here:
    recByteCount++; // increment sample counter
    bufByteCount++;
    if (bufByteCount == 512 && bufWrite == 0) {
      bufByteCount = 0;
      bufWrite = 1;
    } else if (bufByteCount == 512 & bufWrite == 1) {
      bufByteCount = 0;
      bufWrite = 0;
    }
    if (bufWrite == 0) { buf00[bufByteCount] = anaRead(); }
    if (bufWrite == 1) { buf01[bufByteCount] = anaRead();}
    //*************
    REG_TC4_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC0 && TC4->COUNT8.INTENSET.bit.MC0)
  {
    // Put your counter compare 0 (CC0) code here:
    // ...
    //buf00[0]=analogRead(A3);

    Serial.println("!!!!!!!!!!!!!IT!!!!!!!!!!");

    REG_TC4_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC4->COUNT8.INTFLAG.bit.MC1 && TC4->COUNT8.INTENSET.bit.MC1)
  {
    // Put your counter compare 1 (CC1) code here:
    // ...
    REG_TC4_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag
  }
}

void TC5_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{
  // Check for overflow (OVF) interrupt
  if (TC5->COUNT16.INTFLAG.bit.OVF && TC5->COUNT16.INTENSET.bit.OVF)
  {
    // Put your timer overflow (OVF) code here:
    // ...
    REG_TC5_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC5->COUNT16.INTFLAG.bit.MC0 && TC5->COUNT16.INTENSET.bit.MC0)
  {
    timer_flg=true;
    REG_TC5_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC5->COUNT16.INTFLAG.bit.MC1 && TC5->COUNT16.INTENSET.bit.MC1)
  {
    // Put your counter compare 1 (CC1) code here:
    // ...
    REG_TC5_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag
  }
}
///***********************************FONCTIONS DE GESTION DE L'ACCELEROMETRE********************************************
///********VARIABLES GLOBALES CAPTEUR CARDIAQUE
ADXL345 adxl;
int x,y,z;

void adxlInit(){
  pinMode(ACCELEROMETRE_IT,INPUT);
  adxl.powerOn();
  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?

  //look of activity movement on this axes - 1 == on; 0 == off
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);

  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);

  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);

  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625μs per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment

  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

  //setting all interupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );

  //register interupt actions - 1 == on; 0 == off
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}

void adxlInterrupts(){

    //getInterruptSource clears all triggered actions after returning value
    //so do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();

    // freefall
    if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
      Serial.println("freefall");
      //add code here to do when freefall is sensed
    }

    //inactivity
    if(adxl.triggered(interrupts, ADXL345_INACTIVITY)){
      Serial.println("inactivity");
       //add code here to do when inactivity is sensed
    }

    //activity
    if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
      Serial.println("activity");
       //add code here to do when activity is sensed
    }

    //double tap
    if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
      Serial.println("double tap");
       //add code here to do when a 2X tap is sensed
    }

    //tap
    if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
      Serial.println("tap");
       //add code here to do when a tap is sensed
    }
}

///******************FONCTION DE GESTIONS DU CAPTEUR CARDIAQUE**************************
///*****VARIABLES GLOBAL CAPTEUR CARIDAQUE
BH1790GLC bh1790glc;
uint8_t  s_cnt_freq;
uint16_t ret16;
bool timer_flg;
uint8_t  bpm;
uint8_t  wearing;

void cardioInit(){
  ret16 = ERROR_NONE;
  timer_flg = false;
  Wire.begin();
  Wire.setClock(400000L);
  tc5StartCounter();
  s_cnt_freq = 0;

  ret16 = hr_bh1790_Init();
  if (ret16 == ERROR_NONE) {
    if(DEBUG) Serial.println(F("BPM, wearing"));
    ret16 = hr_bh1790_StartMeasure();
    if (ret16 != ERROR_NONE) {
      if(DEBUG){
        Serial.println(F("Error: hr_bh1790_StartMeasure function"));
        Serial.print(F("ret16 = "));
        Serial.println(ret16, HEX);
      }
      else blinkFast(LED_GREEN);
    }
  }
  else {
    if(DEBUG){
      Serial.println(F("Error: hr_bh1790_Init function"));
      Serial.print(F("ret16 = "));
      Serial.println(ret16, HEX);
    }
  }
}


int8_t bh1790_Write(uint8_t adr, uint8_t *data, uint8_t size)
{
  byte   rc  = 0;
  int8_t ret = 0;

  rc = bh1790glc.write(adr, data, size);
  if (rc == 0) {
    ret = BH1790_RC_OK;
  } else {
    ret = BH1790_RC_I2C_ERR;
  }

  return (ret);
}

int8_t bh1790_Read(uint8_t adr, uint8_t *data, uint8_t size)
{
  byte   rc  = 0;
  int8_t ret = 0;

  rc = bh1790glc.read(adr, data, size);
  if (rc == 0) {
    ret = BH1790_RC_OK;
  } else {
    ret = BH1790_RC_I2C_ERR;
  }

  return (ret);
}

void resetDataCardio(){
    bpm     = 0U;
    wearing = 0U;
    ret16   = ERROR_NONE;
}

///*********************************FONCTIONS DE GESTION DU BLUETOOTH**********************************

//TODO: RECUPERATION DE LA DATE LORS DE L'APPAIRAGE

void startBluetooth(){
  ble.begin();
  ble.println("AT+GAPDEVNAME=GO-SAFE");
  ble.waitForOK();
  ble.setMode(BLUEFRUIT_MODE_DATA);
}

void sendAlert(){
  ///*********** Fait fonctionner le "protocole" d'alerte bluetooth. A changer lorsqu'une appli sera en place
  char heartrate[3];
  heartrate[0]='0';
  heartrate[1]='1';
  heartrate[2]='2';
  ///***********
    ble.write('S');
    ble.write(heartrate,3);
}



///*****************************FONCTIONS DE GESTION DE LA CARTE SD*******************************
void sdStart(){
  if(!SD.begin(SD_PIN)){
    if(DEBUG){
      Serial.println("PB SD");
      while(1);
      }
    else blinkFreeze(LED_RED);
  }
  else{
    if(DEBUG) Serial.print("SD OK");
  }
}

///Enregistre les valeurs du BPM et X,Y et Z de l'accéléromètre ainsi que l'heure au format BPM,X,Y,Z DD/MM/AAAA hh:mm:ss
void saveLog(){
    File myFile = SD.open("datalog.txt",FILE_WRITE);
    if(!myFile) {
            if(DEBUG) Serial.println("PB FILE");
    }
    else{
      myFile.print(bpm);
      myFile.print(",");
      myFile.print(x);
      myFile.print(",");
      myFile.print(y);
      myFile.print(",");
      myFile.print(z);
      myFile.print(" ");
      printTimestamp(myFile);
      myFile.close();
    }
}


///Affiche les valeurs du BPM et X,Y et Z de l'accéléromètre au format BPM//X,Y,Z
void displayLog(){
        //Serial.print("count:");Serial.println(heartcount);
        Serial.print(bpm);
        Serial.print("//");
        // Output x,y,z values - Commented out
        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.print(",");
        Serial.print(z);
        displayTimestamp();
}


void wait5sec(){
  int i = 0;
    while(!Serial);
    for(i=0;i<5;i++){
      Serial.println(i);
      delay(1000);
    }
}



void pinConfig(){
  pinMode(LED_RED,OUTPUT);
  digitalWrite(LED_RED,LOW);
  pinMode(LED_GREEN,OUTPUT);
  digitalWrite(LED_GREEN,LOW);
  pinMode(BTN_1,INPUT_PULLUP);
  pinMode(BTN_2,INPUT_PULLUP);
}

///Fait clignoter une LED de manière infinie
///Permet de freezer le programme en informant l'utilisateur
void blinkFreeze(int pin){
    while(1){
        digitalWrite(pin,HIGH);
        delay(80);
        digitalWrite(pin,LOW);
        delay(80);
    }
}

void blinkFast(int pin){
    int i;
    for(i=0;i<25;i++){
        digitalWrite(pin,HIGH);
        delay(100);
        digitalWrite(pin,LOW);
        delay(100);
    }
}




///*************************************GESTION DES APPUIS BOUTON********************************************
///*****VARIABLES GLOBAL BOUTON******
int debouncetime=150;
int debouncetimer;
int timerbtnpressed;
int timelongpress=750;
int timeshortpress=1000;
bool record=false;
bool shortime=false;
int shorttimer;
bool btnactive=false;
int btncount=0;


///Gère un bouton passé en paramètre. Peut capter un appui long, ou compter différent appuis courts et supprime également l'effet de rebond
int managebtn(int pin){
  if(!digitalRead(pin)){
    if(!btnactive){
      if(!shortime){
        shortime=true;
        shorttimer=millis();
      }
      btnactive=true;;
      timerbtnpressed=millis();
      debouncetimer=timerbtnpressed;
      digitalWrite(LED_RED,HIGH);
    }
    if(btnactive && ((millis()-timerbtnpressed)>timelongpress)&& !record){
      shortime=false;
      record=true;
      return START_RECORD;
    }
  }
  else{
    if(btnactive && millis()-debouncetimer>debouncetime){
      digitalWrite(LED_RED,LOW);
      btnactive=false;
      if(millis()-shorttimer<timeshortpress){
        btncount++;
      }
      if(record){
        record=false;
        return STOP_RECORD;
      }
    }
  }

  if(shortime && millis()-shorttimer>timeshortpress){
    shortime=false;
    int temp= btncount;
    btncount=0;
    return temp;
  }
  return 0;
}


