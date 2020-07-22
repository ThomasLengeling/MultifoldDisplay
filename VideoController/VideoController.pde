import oscP5.*;
import netP5.*;
import controlP5.*;

//gui
ControlP5 cp5;

Button    reset;
Button    play;
Button    stop;


Button    av_00;
Button    av_01;
Button    av_02;
Button    av_03;

//osc
OscP5 oscP5;
NetAddress myRemoteLocation;

//ip values
String strIP = "127.0.0.1";
String strPort = "32000";

String msg = "/dirxyz";



void setup() {
  fullScreen();
  //size(1920, 1080);

  PFont font = createFont("arial", 20);

  //osc
  oscP5 = new OscP5(this, 12000);
  myRemoteLocation = new NetAddress(strIP, Integer.parseInt(strPort));

  cp5 = new ControlP5(this);
  
    //controlP5.setAutoDraw(false);
    color activeColor = color(0,130,164);
  cp5.setColorActive(activeColor);
  cp5.setColorBackground(color(170));
  cp5.setColorForeground(color(50));
  cp5.setColorCaptionLabel(color(50));
  cp5.setColorValueLabel(color(255));


  ///////////////////////////////////////////////////////

  cp5.addTextlabel("OSC Message")
    .setText("OSC Message "+msg)
    .setPosition(1500, 50)
    .setColorValue(0xffffffff);

  cp5.addTextfield("ip")
    .setPosition(1500, 80)
    .setSize(150, 20)
    .setFocus(true)
    .setValue(strIP)
    .setColor(color(255, 0, 0))
    .setFont(font)
    ;

  cp5.addTextfield("port")
    .setPosition(1500, 130)
    .setSize(150, 20)
    .setValue(strPort)
    .setAutoClear(false)
    .setFont(font)
    ;
    
  reset =  cp5.addButton("reset")
    .setPosition(1500, 250)
    .setSize(300, 100)
    .setFont(font)
    ;
    
      reset =  cp5.addButton("play")
    .setPosition(1500, 400)
    .setSize(300, 100)
    .setFont(font)
    ;
    
    reset =  cp5.addButton("stop")
    .setPosition(1500, 550)
    .setSize(300, 100)
    .setFont(font)
    ;
    
   av_00 =  cp5.addButton("Videos_00")
    .setPosition(500, 50)
    .setSize(500, 150)
    .setFont(font)
    ;
    
   av_01 =  cp5.addButton("Videos_01")
    .setPosition(500, 300)
    .setSize(500, 150)
    .setFont(font)
    ;
    
   av_02 =  cp5.addButton("Videos_02")
    .setPosition(500, 550)
    .setSize(500, 150)
    .setFont(font)
    ;
    
   av_03 =  cp5.addButton("Videos_03")
    .setPosition(500, 800)
    .setSize(500, 150)
    .setFont(font)
    ;
}


void ip(String strValue) {
  strIP = strValue;
  myRemoteLocation = new NetAddress(strIP, Integer.parseInt(strPort));
  println(strIP+ " "+strPort);
}

void port(String strValue) {
  strPort = strValue;
  myRemoteLocation = new NetAddress(strIP, Integer.parseInt(strPort));
  println(strIP+ " "+strPort);
}


void draw() {
  background(220);

 //  sendValues(msg, sliderCurrX, sliderCurrY, sliderCurrZ);
  //  println("sent values "+msg+"  "+ sliderCurrX+" "+sliderCurrY+" "+sliderCurrZ);
 
}

void keyPressed() {
  if (key == 'a') {
    OscMessage myMessage = new OscMessage("/play");
    myMessage.add(1);
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
    println("send /play 1");
  }
}

void sendValues(String msg, float x, float y, float z) {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage(msg);

  myMessage.add(x);
  myMessage.add(y);
  myMessage.add(z);

  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
}
