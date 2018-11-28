#include <SPI.h>
#include <WiFi101.h>


const int PIN1 = 12;    //Pin 12 for led
const int PIN2 = 11;    //Pin 11 for led
const int PIN3 = 10;    //Pin 10 for led
const int BUTTON1 = 15; //pin 15(A1) for button 1
const int BUTTON2 = 16; //pin 16(A2) for button 2
int Speed = 1000;       //Game ball speed
int buttonRead1 = 0;    //read from button 1
int buttonRead2 = 0;    //read from button 2
char ssid[] = "NETGEAR73";   //my network SSID name
char pass[] = "exoticboat603";     //my network password (I trust you guys:))
int keyIndex = 0;             //networks key Index
int player1 = 0;      //player 1 score
int player2 = 0;      //player 2 score
int singlePlayer = 0; //single player score
int mode = 0;             //game mode being played

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup()
{
  WiFi.setPins(8,7,4,2); //set pins for feather M0 board for network
  
  Serial.begin(9600); //serial terminal

  
  pinMode(BUTTON1, INPUT);   //set button 1 as input
  pinMode(BUTTON2, INPUT);   //set button 2 as input


  while ( status != WL_CONNECTED) {
    Serial.print("Connecting to network: ");
    Serial.println(ssid);                   // network name
    // Connect to network
    status = WiFi.begin(ssid, pass);
    // delay 10 seconds:
    delay(10000);
  }
  server.begin();                           // connect to web server 

  //provide user address to web page
  IPAddress ip = WiFi.localIP();
  Serial.print("To see the the webpage go to this address http://");
  Serial.println(ip);
  
}

void loop(){
   WiFiClient client = server.available();   // get clients
  if (client) {                             // is someone is on the page
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n') {                    // if the byte is a newline character

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            // the content of the HTTP response follows the header:
            client.print("<head><title>Welcome to the great LEG Tennis Game</title></head>");
            client.print("<center><h1 style=color:DodgerBlue;>Welcome to the Great LEG Tennis Game</h1></center>");
            client.print("<center>Click <a href=\"/H\">here</a> to play two player mode</center><br>");
            client.print("<center>Click <a href=\"/L\">here</a> to play single player mode</center><br>");
            client.print("<center style= color:red;>Player 1 Score: </center>");
            client.print("<center>");
            client.print(player1);  //display player 1 score
            client.print("</center>");
            client.print("<br><center style = color:green;>Player 2 Score: </center>");
            client.print("<center>");
            client.print(player2);  //display player 2 score
            client.print("</center>");
            client.print("<br><center style = color:pink;>Single player Score: </center>");
            client.print("<center>");
            client.print(singlePlayer); //display sign player mode score
            client.print("</center>");            
            client.println();

            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        //if two player mode is chosen
        if (currentLine.endsWith("GET /H") && mode==0) {
          mode =1;
          buttonRead1 = HIGH;
        }
        //if one player mode is chosen
        if(currentLine.endsWith("GET /L") &&mode ==0){
          mode =2;
          buttonRead1 =HIGH;
          singlePlayer=0;
          }
      }
    }
    // close connection
    client.stop();
  }


  //check to see if game should be played
  if (mode !=0){
    //was buttons one pressed
  if (buttonRead1 == HIGH){
    buttonRead1 = 0;
    moveLeft();   //move ball left

    }
    //was buttons 2 pressed or is the game in one player mode
  else if (buttonRead2== HIGH||mode == 2){
    buttonRead2 = 0;    
    moveRight();    //move balls left
   Speed = Speed/1.3;
    }else{    //if no button are pressed in time
      mode = 0;   //end game
      flash();     //flash leds
      Speed = 1000;   //reset the ball speed
    }
  }
}


void flash(){
  int flashCount = 0;   //count of how many times the leds will flash
  Speed = 5;            //speed decreased to 5
  //loop to flash leds
  while (flashCount <4){
  flashCount+=1;
  //keep lights on for a certain amount of time
  for (int i =0; i<10; i++){
    moveLeft();
    }
  //turn all leds off  
  pinMode(PIN1, OUTPUT);    //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);    //row3
  digitalWrite(PIN3, LOW);
  delay(200);
  }
  
}

void moveLeft(){
  //turn on LED 6
  pinMode(PIN1, OUTPUT);    //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, INPUT);     //row 2
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);    //row3
  digitalWrite(PIN3, HIGH);
 
  delay(Speed); //delay by game speed

    
  //turn on LED 5
  pinMode(PIN1, OUTPUT);
  digitalWrite(PIN1, HIGH);
  pinMode(PIN2, INPUT);
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed

 //turn on LED 4
  pinMode(PIN1, INPUT);     //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);    //row 2
  digitalWrite(PIN2, HIGH);  
  pinMode(PIN3, OUTPUT);    //row 3
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed

   //turn on LED 3
  pinMode(PIN1, INPUT);      //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, LOW);  
  pinMode(PIN3, OUTPUT);     //row 3
  digitalWrite(PIN3, HIGH);
  
  delay(Speed);   //delay by game speed

  //turn on LED 2
  pinMode(PIN1, OUTPUT);     //row 1
  digitalWrite(PIN1, HIGH);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, LOW);   
  pinMode(PIN3, INPUT);      //row 3
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed

    //turn on LED 1
  pinMode(PIN1, OUTPUT);     //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, HIGH);  
  pinMode(PIN3, INPUT);      //row 3
  digitalWrite(PIN3, LOW);

  //read button if the game is in player 2 mode
  if (mode ==1){
    //check button input in a loop
  for (int i=0; i<Speed; i++){
    buttonRead2 = digitalRead(BUTTON2);
    if (buttonRead2 == HIGH){
      break;
      }
    delay(1);
    //if button is never pressed, increase player two score
    if(i ==(Speed -1)){
      player1=1+player1;
      }
    }
  }
}

void moveRight(){

    //turn on LED 1
  pinMode(PIN1, OUTPUT);     //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, HIGH);  
  pinMode(PIN3, INPUT);      //row 3
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed
  
  //turn on LED 2
  pinMode(PIN1, OUTPUT);     //row 1
  digitalWrite(PIN1, HIGH);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, LOW);   
  pinMode(PIN3, INPUT);      //row 3
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed
  
  //turn on LED 3
  pinMode(PIN1, INPUT);      //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);     //row 2
  digitalWrite(PIN2, LOW);  
  pinMode(PIN3, OUTPUT);     //row 3
  digitalWrite(PIN3, HIGH);
  
  delay(Speed);   //delay by game speed
  
  //turn on LED 4
  pinMode(PIN1, INPUT);     //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);    //row 2
  digitalWrite(PIN2, HIGH);  
  pinMode(PIN3, OUTPUT);    //row 3
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed
  
  //turn on LED 5
  pinMode(PIN1, OUTPUT);
  digitalWrite(PIN1, HIGH);
  pinMode(PIN2, INPUT);
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);
  digitalWrite(PIN3, LOW);
  
  delay(Speed);   //delay by game speed

  //turn on LED 6
  pinMode(PIN1, OUTPUT);    //row 1
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, INPUT);     //row 2
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);    //row3
  digitalWrite(PIN3, HIGH);

  //read button inputs in a loop for player 1
  for (int i=0; i<Speed; i++){
    buttonRead1 = digitalRead(BUTTON1);
    if (buttonRead1 == HIGH){
      singlePlayer+=1;    //increment score for single player game
      break;
      }

    delay(1);
    //if button is not pressed and in two player mode then increase player 2 score
    if(i ==(Speed -1)&&(mode==1)){
      player2=1+player2;
      }else if(i ==(Speed -1)&&(mode==2)){  //if button not pressed for on player mode then end game
        mode =1;
        }
  }
  

}
