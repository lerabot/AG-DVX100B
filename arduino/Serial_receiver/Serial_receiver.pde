import processing.serial.*;

Serial teensy;
color bg;
int rFrames = 0;
void setup() {
  size(640, 480);
  bg = color(0, 0, 0);
  //printArray(Serial.list());
  teensy = new Serial(this, "/dev/ttyACM0", 921600);
  // teensy.buffer(bSize);
}

void draw() {
  int t1 = 0;
  int t2 = 0;
  background(bg);
  
  if (teensy.available() > 0) {
    t1 = millis();
    byte[] s = teensy.readBytesUntil(10);
    if (s != null) { 
      //if (s.equals("!FRAME") == true ) {
      if (s[0] == '!') {
        rFrames++;
        println("Received bytes: " + s.length);
        for(int i = 0; i < s.length; i++) {
          //stroke(data[i]);
          //line(i, 0, i, 480);
        } 
        //println("PROCESSING: received frame!");
        //bg = color(s.charAt(1), s.charAt(2), s.charAt(3));
      }
      else {
        //print(s);
      }
      t2 = millis();
    }
  }
  
  fill(255,0,0);
  text((rFrames / 60) + " " + (millis() / 1000), 25,25);
  text(rFrames + " " + (millis() / 1000), 25,45);
  //text(rFrames + " " + frameRate, 25,25);
}








void serialEvent(Serial port) {
}
