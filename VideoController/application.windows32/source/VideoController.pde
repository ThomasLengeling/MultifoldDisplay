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
Button    av_04;
Button    av_05;
Button    av_06;

//osc
OscP5 oscP5;
NetAddress myRemoteLocation;

//ip values
String strIP = "18.27.113.155";
String strPort = "32000";

String msg = "/av";

PImage img;

void setup() {
  fullScreen();
  //size(1920, 1080);

  img = loadImage("cities.jpeg");

  PFont font = createFont("arial", 16);

  //osc
  oscP5 = new OscP5(this, 32001);
  myRemoteLocation = new NetAddress(strIP, Integer.parseInt(strPort));

  cp5 = new ControlP5(this);

  //controlP5.setAutoDraw(false);
  color activeColor = color(0, 130, 164);
  cp5.setColorActive(activeColor);
  cp5.setColorBackground(color(170));
  cp5.setColorForeground(color(50, 200));
  cp5.setColorCaptionLabel(color(255));
  cp5.setColorValueLabel(color(155));
  // cp5.setColorValueActiveLabel(color(255));


  ///////////////////////////////////////////////////////

  cp5.addTextlabel("OSC Message")
    .setText("OSC Message "+msg)
    .setPosition(1700, 20)
    .setFont(font);

  cp5.addTextfield("ip")
    .setPosition(1700, 45)
    .setSize(150, 20)
    .setFocus(true)
    .setValue(strIP)
    .setColor(color(255, 0, 0))
    .setFont(font)
    ;

  cp5.addTextfield("port")
    .setPosition(1700, 85)
    .setSize(150, 20)
    .setValue(strPort)
    .setAutoClear(false)
    .setFont(font)
    ;

  reset =  cp5.addButton("reset")
    .setPosition(1700, 150)
    .setSize(50, 50)
    .setFont(font)
    ;

  reset =  cp5.addButton("play")
    .setPosition(1770, 150)
    .setSize(50, 50)
    .setFont(font)
    ;

  reset =  cp5.addButton("stop")
    .setPosition(1840, 150)
    .setSize(50, 50)
    .setFont(font)
    ;


  av_00 =  cp5.addButton("idle")
    .setPosition(50, 960)
    .setSize(50, 50)
    .setFont(font)
    ;

  av_00.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(0);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 0");
      }
    }
  }
  );

  av_01 =  cp5.addButton("cai_00")
    .setPosition(180, 575)
    .setSize(75, 75)
    .setFont(font)
    ;

  av_01.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(1);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 1");
      }
    }
  }
  );

  av_02 =  cp5.addButton("gld_00")
    .setPosition(900, 215)
    .setSize(75, 75)
    .setFont(font)
    ;

  av_02.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(2);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 2");
      }
    }
  }
  );

  av_03 =  cp5.addButton("gld_01")
    .setPosition(1065, 360)
    .setSize(75, 75)
    .setFont(font)
    ;

  av_03.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(3);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 3");
      }
    }
  }
  );


  av_04 =  cp5.addButton("phc_00")
    .setPosition(1780, 600)
    .setSize(75, 75)
    .setFont(font)
    ;

  av_04.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(4);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 4");
      }
    }
  }
  );


  av_05 =  cp5.addButton("phc_01")
    .setPosition(1620, 800)
    .setSize(75, 75)
    .setFont(font)
    ;

  av_05.addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_ENTER) {
        OscMessage myMessage = new OscMessage("/av");
        myMessage.add(5);
        oscP5.send(myMessage, myRemoteLocation);
        println("send /av 5");
      }
    }
  }
  );
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
  fill(255, 100);
  rect(0, 0, width, height);

  pushStyle();
  blendMode(BLEND);  
  tint(200, 190);
  image(img, 0, 0, width, height);
  popStyle();
  //  sendValues(msg, sliderCurrX, sliderCurrY, sliderCurrZ);
  //  println("sent values "+msg+"  "+ sliderCurrX+" "+sliderCurrY+" "+sliderCurrZ);
}


void reset() {
  OscMessage myMessage = new OscMessage("/reset");
  myMessage.add(1);
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
  println("send /reset 1");
}

void play() {
  OscMessage myMessage = new OscMessage("/play");
  myMessage.add(1);
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
  println("send /play 1");
}

void stop() {
  OscMessage myMessage = new OscMessage("/stop");
  myMessage.add(1);
  /* send the message */
  oscP5.send(myMessage, myRemoteLocation);
  println("send /stop 1");
}

void keyPressed() {
  if (key == 'p') {
    OscMessage myMessage = new OscMessage("/play");
    myMessage.add(1);
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
    println("send /play 1");
  }

  if (key == 's') {
    OscMessage myMessage = new OscMessage("/stop");
    myMessage.add(1);
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
    println("send /stop 1");
  }

  if (key == 'r') {
    OscMessage myMessage = new OscMessage("/reset");
    myMessage.add(1);
    /* send the message */
    oscP5.send(myMessage, myRemoteLocation);
    println("send /reset 1");
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
