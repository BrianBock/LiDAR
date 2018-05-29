import processing.serial.*; // add the serial library
import java.util.Date;
Serial myPort; // the serial port to monitor
PrintWriter output;
final float resolution=.5; // half step resolution
final float steps=200/resolution; //# of steps per rotation
final float resolutionConst=360/steps; // degrees/step

void setup() {
  output = createWriter("data "+month()+"-"+day()+" "+hour()+"-"+minute()+"-"+second()+".csv"); //Create CSV with the current time as it's filename
  size(1000, 512); // set the window size, size() also sets width and height variables
  // first argument = width, second argument = height
  printArray(Serial.list()); // list all available serial ports
  myPort = new Serial(this, Serial.list()[3], 115200); // define input port
  myPort.clear(); // clear the port of any initial junk
  fill(0, 0, 255); // pick the fill color (r,g,b)
  noStroke();
}

int timecoord=0;


void draw () {
  if (myPort.available () > 0) { // make sure port is open
    String inString = myPort.readStringUntil('\n'); // read input string
    if (inString != null) { // ignore null strings
      inString = trim(inString); // trim off any whitespace
      String[] xyRaw = splitTokens(inString, ","); // extract x & y into an array
      // proceed only if correct # of values extracted from the string:
      if (xyRaw.length == 3) {
        int azi = int(xyRaw[0]); 
        int alti = int(xyRaw[1]);
        int distance = int(xyRaw[2]);

        int alticorrected=alti-67;
        float phi=90+(alticorrected*resolutionConst);
        float theta=azi*resolutionConst;

        float x=distance*sin(radians(phi))*cos(radians(theta));
        float y=distance*sin(radians(phi))*cos(radians(phi));
        float z=distance*cos(radians(phi));

        //Output data to CSV
        output.print(x);
        output.print(",");
        output.print(y);
        output.print(",");
        output.println(z);
      }
    }
  }
}



void keyPressed() //triggers with any keystroke
{ 
  output.flush(); 
  output.close(); 
  exit();
}