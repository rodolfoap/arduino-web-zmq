// Arduino will reset if serial is opened. Heres how to disble autoreset from bash:
// stty -F /dev/ttyACM1 -hupcl
// See http://playground.arduino.cc/Main/DisablingAutoResetOnSerialConnection

// From Bash, send data like this:
// printf '100\n' >/dev/ttyACM1
// Or use the provided scripts:
// ./initialize_serial.bash 
// ./send_data.bash "LT 200"
// ./send_data.bash "HT 2000"

// On a different bash session, we can do 
// tail -f /dev/ttyACM1
// But before, we require to initialize the port correctly
// stty -F /dev/ttyACM1 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
// See https://playground.arduino.cc/Interfacing/LinuxTTY

#include <LiquidCrystal.h>
#define WSPACE "                "
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int led=13;
String inString="";
int state=1;
int count=10;
int shieldKey=1024;
int HGTIME=2000;
int LWTIME=1000;

void setup() { pinMode(led, OUTPUT); lcd.begin(16, 2); Serial.begin(115200); while (!Serial) { ; } }

void lcdwipe(int line) { lcd.setCursor(0, line); lcd.print(WSPACE); }
int lcdprint(int line, String input) { lcdwipe(line); lcd.setCursor(0, line); lcd.print(input); }

void parseCommand(String command) {
  String mnemonic=command.substring(0,2); String mnevalue=command.substring(3);
  if(mnemonic=="LT") { LWTIME=mnevalue.toInt(); } 
  if(mnemonic=="HT") { HGTIME=mnevalue.toInt(); } 
  lcdprint(1, command);
}
int getkey(unsigned int input) { if(input <  30) {return 1;} if(input < 150) {return 2;} if(input < 360) {return 3;} if(input < 535) {return 4;} if(input < 760) {return 5;} return 0; }

void checkButtons() { shieldKey=getkey(analogRead(0)); }
void checkSerial() { while (Serial.available() > 0) { int inChar = Serial.read(); if (inChar == '\n')  { parseCommand(inString); inString = ""; } else { inString += (char)inChar; } } }
void executeActions() {
  if(state==0) {
    if(count-- < 0) { 
      digitalWrite(led, HIGH); lcdprint(0, String(HGTIME)); state=1; count=HGTIME; 
      if(shieldKey>0) { Serial.print("SK:"); Serial.println(shieldKey); } 
    }
  } else { 
    if(count-- < 0) { digitalWrite(led, LOW ); lcdprint(0, String(LWTIME)); state=0; count=LWTIME; } 
  } 
}

// We'll emulate a threaded process by pooling, therefore calls should be non-blocking.
void loop() {
  checkButtons();
  checkSerial();
  executeActions();
  delay(1);
}
