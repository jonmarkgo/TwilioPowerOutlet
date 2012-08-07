#include <WiFlyHQ.h>
#include <SoftwareSerial.h>
#include <PusherClient.h>

const char mySSID[] = "my$ssid"; //use $ to represent spaces in your ssid or passphrase
const char myPassword[] = "my$pass$phrase";
const char pusherKey[] = "abc123";

PusherClient client;

SoftwareSerial wifiSerial(2,3); //we will use these pins for debug output

int poweroutlet = 12; //pinout for the powerswitch

int led = 13; //status indicator led

WiFly wifly;

void setup() {
  //initialize our digital pinouts
  pinMode(poweroutlet, OUTPUT);
  pinMode(led, OUTPUT);

  //turn off the poweroutlet and led starting out
  digitalWrite(poweroutlet, LOW);
  digitalWrite(led, LOW);
  wifiSerial.begin(57600);

  Serial.begin(9600);
  if (!wifly.begin(&Serial, &wifiSerial)) { //start up the serial connection to our wifi, tell the WiFlyHQ lib to use our softwareserial for debug
    wifiSerial.println(F("Failed to start wifly"));
    wifly.terminal(); //if the wifly fails to start, give us access to a direct serial terminal to the RN-XV
  }

  if (!wifly.isAssociated()) { //check to see if we are already associated with the network before connecting
    wifiSerial.println(F("Joining network"));
    if (wifly.join(mySSID, myPassword, true)) { //using the true flag at the end of wifly.join indicates that we are using WPA 
      wifly.save();
      wifiSerial.println(F("Joined wifi network"));
    } 
    else {
      wifiSerial.println(F("Failed to join wifi network"));
      wifly.terminal();
    }
  } 
  else { //if we are already associated with the network
    wifiSerial.println(F("Already joined network"));
  }
  client.setClient(wifly); //initialize the pusher client with our 
  if(client.connect(pusherKey)) { //connect to our pusher account
    client.bind("powersms", powerSwitch); //bind the powersms event to the powerSwitch callback function
    client.subscribe("robot_channel"); //subscribe to our pusher channel
  }
  else { //if we fail to connect, just loop
    while(1) {
    }
  }
}

void loop() {
  if (client.connected()) {
    digitalWrite(led,HIGH);
    client.monitor();
  }

}

void powerSwitch(String data) {
  String cmd = client.parseMessageMember("command", data);
  wifiSerial.print("Command: ");
  wifiSerial.println(cmd);

  if (cmd == "on") {
    digitalWrite(poweroutlet,HIGH);
  }
  else {
    digitalWrite(poweroutlet,LOW);
  }

}