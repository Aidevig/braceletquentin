#include <aidevig_WAVrecorder.h>

File toSend;
File rec;
String pref="REC";
String suf=".WAV";
unsigned long fileSize;
unsigned long waveChunk;
unsigned int waveType;
unsigned int numChannels;
unsigned long sampleRate;
unsigned long bytesPerSec;
unsigned int blockAlign;
unsigned int bitsPerSample;
unsigned long dataSize;
unsigned long oldTime;
unsigned long newTime;
byte byte1, byte2, byte3, byte4;

void initHeader(){
fileSize = 0L;
waveChunk = 16;
waveType = 1;
numChannels = 1;
sampleRate = 8000;
bytesPerSec = 8000;
blockAlign = 1;
bitsPerSample = 8;
dataSize = 0L;
}

String checkName(){
  int i;
  String nom = pref + i + suf;
  while(SD.exists(nom)){
    i++;
    nom = pref + i + suf;
  }
  return nom;
}

int countRec(){
  int i=0;
  String nom = pref + i + suf;
  while(SD.exists(nom)){
    i++;
    nom = pref + i + suf;
  }
  return i;
}

void sendLast(){
  int count = countRec()-1;
  char data;
  Serial.print(1);
    String nom = pref + count + suf;
    toSend = SD.open(nom);
    if(waitCommand()=='N'){
      Serial.print(toSend.name());
    }
    if(waitCommand()=='O'){
      while (toSend.available()) {
        data = toSend.read(); // on stocke le caractère dans c
        Serial.write(data);
        }
    }
   toSend.close();
}

void sendAudio(){
  int count = countRec() + 1; ///ADD 1 FOR DATALOG FILE
  int i;
  char data;
  String nom;
  Serial.print(count);
  for(i=0;i<count;i++){
    nom = pref + i + suf;
    toSend = SD.open(nom);
    if(waitCommand()=='N'){
      Serial.print(toSend.name());
    }
    if(waitCommand()=='O'){
      while (toSend.available()) {
        data = toSend.read(); // on stocke le caractère dans c
        Serial.write(data);
        }
    }
   toSend.close();
  }
}

void sendLog(){
    char data;
    toSend = SD.open("datalog.txt");
    if(waitCommand()=='N'){
      Serial.print(toSend.name());
    }
    if(waitCommand()=='O'){
      while (toSend.available()) {
        data = toSend.read(); // on stocke le caractère dans c
        Serial.write(data);
        }
    }
    toSend.close();
}


char waitCommand(){
  while(Serial.available()==0);
  return (char)Serial.read();
}

void writeWavHeader() { // write out original WAV header to file
  String Name = checkName();
  recByteSaved = 0;
  bufByteCount = 0;
  Serial.print(Name);
  rec = SD.open(Name, FILE_WRITE);
  delay(300);
  if(rec){
  Serial.print(rec.write("RIFF"));
  byte1 = fileSize & 0xff;
  byte2 = (fileSize >> 8) & 0xff;
  byte3 = (fileSize >> 16) & 0xff;
  byte4 = (fileSize >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.write("WAVE");
  rec.write("fmt ");
  byte1 = waveChunk & 0xff;
  byte2 = (waveChunk >> 8) & 0xff;
  byte3 = (waveChunk >> 16) & 0xff;
  byte4 = (waveChunk >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = waveType & 0xff;
  byte2 = (waveType >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2);
  byte1 = numChannels & 0xff;
  byte2 = (numChannels >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2);
  byte1 = sampleRate & 0xff;
  byte2 = (sampleRate >> 8) & 0xff;
  byte3 = (sampleRate >> 16) & 0xff;
  byte4 = (sampleRate >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = bytesPerSec & 0xff;
  byte2 = (bytesPerSec >> 8) & 0xff;
  byte3 = (bytesPerSec >> 16) & 0xff;
  byte4 = (bytesPerSec >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  byte1 = blockAlign & 0xff;
  byte2 = (blockAlign >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2);
  byte1 = bitsPerSample & 0xff;
  byte2 = (bitsPerSample >> 8) & 0xff;
  rec.write(byte1);  rec.write(byte2);
  rec.write("data");
  byte1 = dataSize & 0xff;
  byte2 = (dataSize >> 8) & 0xff;
  byte3 = (dataSize >> 16) & 0xff;
  byte4 = (dataSize >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  }
  else{
    Serial.println("BUG2");
  }
}


void writeOutHeader() { // update WAV header with final filesize/datasize

  rec.seek(4);
  byte1 = recByteSaved & 0xff;
  byte2 = (recByteSaved >> 8) & 0xff;
  byte3 = (recByteSaved >> 16) & 0xff;
  byte4 = (recByteSaved >> 24) & 0xff;
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.seek(40);
  rec.write(byte1);  rec.write(byte2);  rec.write(byte3);  rec.write(byte4);
  rec.close();

}

void startRec(){
  writeWavHeader();
  tcStartCounter();
}

void stopRec(){
  tcDisable();
  writeOutHeader();
}

void writeWav(){
  if (recByteCount % 1024 == 512 && record == true) {rec.write(buf00,512); recByteSaved+= 512; } // save buf01 to card
  if (recByteCount % 1024 == 0 && record == true) { rec.write(buf01,512); recByteSaved+= 512; } // save buf02 to card
  }
