#include <SPI.h>
#include <SD.h>
#define FULLPIXEL 307200

File buf;


typedef struct _p {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  //unsigned char extra;
} dvxP;

int w           = 640;
int h           = 480;
int split       = 1;
int bmpSplit    = 2;
int totalPixel  = w;


#define PIXEL(r,g,b) ((dvxP){(r),(g),(b)})
dvxP line[640]; //line
byte *lineChar;

void generateLine() {
  byte *l = lineChar;
  //byte b[3] = {127, 255, 10};
  for (int i = 0; i < w; i++) {
    byte b[3] = {random(255), random(255), random(255)};
    memcpy(l, b, sizeof(byte) * 3);
    l+=3;

    
    //sprintf(buf, "%c%c%c", random(60, 127), random(60, 127), random(60, 127));
    //sprintf(buf, "%b%b%b", 60, 127, 60);
    //memcpy(
    //strcat(lineChar, buf);
  }
  //strcat(lineChar, '\n');
}

void sendLine(int lineNum) {
  Serial.write(lineChar, (w * sizeof(byte) * 3)); // Validate lenght here. should by number of byte.
  //Serial.println(lineChar);
}

void sendImageByLine() {
  Serial.println("!");
  for (int y = 0; y < h; y++) {
    //generateLine();
    sendLine(y);
    //Serial.println();
    Serial.send_now();
  }
}


void sendImageData() {
  long t1 = millis();
  int lineNum = 75;
  int byteToSend = sizeof(line) * sizeof(line[0]);
  //Serial.print(byteToSend);
  //Serial.println(" byte sent in serial port.");
  Serial.print("!");
  for (int b = 0; b < 1; b++) {
    for (int y = 0; y < lineNum; y++) {
      //Serial.write(line, byteToSend);
    }
  }

  //Serial.print("      >>>");
  //Serial.print(millis() - t1);
  //Serial.println("ms to send data");
  Serial.println();
  Serial.send_now();

}

void initConnection() {
  char buf[128];
  memset(buf, '+', 127);
  sprintf(buf, "AG-DVX100 Teensy Capture\nw%i\nh%i\nr\n", w, h);
  lineChar = malloc(sizeof(byte) * 3 * w);


  //memcpy(buf, "= AG-DVX100B Teensy capture =", 24);

  /*
    Serial.println();
    Serial.print("w");
    Serial.println(w);
    Serial.print("h");
    Serial.println(h);
  */

  //Serial.println("READY");
  Serial.write(buf, 128);
  Serial.send_now();
}

void setup() {
  pinMode(2, INPUT);



  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    delay(25); // wait for serial port to connect. Needed for native USB port only
  }


  delay(250);
  initConnection();



  /*
    // see if the card is present and can be initialized:
    // Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
    if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
    }
    Serial.println("card initialized.");
  */

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  /*
    File writeTest = SD.open("datalog.txt", FILE_WRITE);

    if (writeTest) {
    writeTest.println("FILE WRITE TEST");
    writeTest.close();
    }

    File readTest = SD.open("datalog.txt");
    // if the file is available, write to it:
    if (readTest) {
    while (readTest.available()) {
      Serial.write(readTest.read());
    }
    readTest.close();
    }
    // if the file isn't open, pop up an error:
    else {
    Serial.println("error opening datalog.txt");
    }
  */
  //generateImageData();
  //sendImageData();
}

bool sendImage = true;
void loop() {

  generateLine();
  sendImageByLine();
  delay(1);
  //digitalWrite(13, HIGH);
  //delay(250);
  //digitalWrite(13, LOW);
  //sendImage = false;
  //delay(1000);
}


void initFlexIO() {
  //FlexIO Shit https://forum.pjrc.com/threads/66201-Teensy-4-1-How-to-start-using-FlexIO
  CCM_CCGR3 |= CCM_CCGR3_FLEXIO2(CCM_CCGR_ON);
  FLEXIO2_CTRL |= 1;

  //init pins
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_10 = 4;    // 10
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_11 = 4;   // 11

  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_00 = 4;   // 0
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_01 = 4;   // 1
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_02 = 4;   // 2
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B0_03 = 4;   // 3

  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_12 = 4;   // 28
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_13 = 4;   // 29

  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_00 = 4;   // 16
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_01 = 4;   // 17
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_02 = 4;   // 18
  IOMUXC_SW_MUX_CTL_PAD_GPIO_B1_03 = 4;   // 19

  FLEXIO2_SHIFTCTL0  = FLEXIO_SHIFTCTL_TIMSEL( 0 )   |     // timer 0
                       // FLEXIO_SHIFTCTL_TIMPOL   |     // on positive edge
                       FLEXIO_SHIFTCTL_PINCFG( 0 )   |     // pin output disabled
                       FLEXIO_SHIFTCTL_PINSEL( 10 )    |     // pin 0
                       // FLEXIO_SHIFTCTL_PINPOL   |     // active high
                       FLEXIO_SHIFTCTL_SMOD( 1 );          // receive mode

}
