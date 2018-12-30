#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Time.h>
#include <TimeLib.h>

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
EthernetServer server(80);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 5, 180 };
byte a[8] = {	B00100,	B00100,	B00010,	B00001,	B00001,	B10001,	B10001,	B01110 };
byte C[8] = {	B11110,	B10001,	B10010,	B11100,	B10000,	B11110,	B10001,	B10001 };
byte r[8] = {	B10000,	B10000,	B10000,	B10000,	B11110,	B10001,	B10001,	B10001 };
byte T[8] = {   B01010,	B10101,	B10101,	B10101,	B10101,	B10101,	B10101,	B01001 };
byte u[8] = {	B11011,	B00101,	B00001,	B00001,	B00001,	B10001,	B10001,	B01110 };
byte l[8] = {	B01110,	B10001, B10001, B10000, B10000, B01000, B10100, B01111 };
byte i[8] = {	B01110,	B10001,	B10001,	B10001,	B10001,	B10001,	B10001,	B10001 };

const int buttonPin0 = A0;
const int buttonPin1 = A1;
const int buttonPin2 = A2;
const int ledGreen = A5;
const int ledRed = A4;
const int led = 7;
int period, UpdatePeriod;
unsigned long timeNowSwtBtn, timeNowIncrementBtn, timeNowDecrementBtn, timeGlobal, prevTime, timer, defaultTime, prev;
bool pumpIsOn, swtBtnOncePressed, incrementBtnOncePressed, decrementBtnOncePressed;
 
void setup() {
	Serial.begin(9600);
	Ethernet.begin(mac, ip);
	server.begin();
	lcd.begin(16, 2);
	lcd.clear();
	lcd.print("Welcome...");
	delay(2000);
	lcd.clear();
	// lcd.print("IP:192.168.10.180");
	lcd.print("Follow   http://");
	lcd.setCursor(0, 1);
	lcd.print(ip[0]);
	lcd.print(".");
	lcd.print(ip[1]);
	lcd.print(".");
	lcd.print(ip[2]);
	lcd.print(".");
	lcd.print(ip[3]);
	delay(3000);
	lcd.clear();
	lcd.print("Made by B.Kodua");
  	lcd.setCursor(2, 2);
	lcd.print("03.07.2017");
	delay(4000);
	lcd.clear();

	lcd.createChar (1, C);
	lcd.createChar (2, a);
	lcd.createChar (3, r);
	lcd.createChar (4, T);
	lcd.createChar (5, u);
	lcd.createChar (6, l);
	lcd.createChar (7, i);

	swtBtnOncePressed = false;
	incrementBtnOncePressed = false;
	decrementBtnOncePressed = false;
	pumpIsOn = false;

	pinMode(buttonPin0, INPUT);
	pinMode(buttonPin1, INPUT);
	pinMode(buttonPin2, INPUT);
	 
	pinMode(led, OUTPUT);
	pinMode(ledGreen, OUTPUT);
	pinMode(ledRed, OUTPUT);
	
	defaultTime = 600;
	timer = defaultTime - 1; // milliseconds, 10 minutes

	period = 50;
	UpdatePeriod = 100;
	prevTime = 0;
	prev = now();

	timeGlobal = -1000;
	timeNowSwtBtn = -1000;
	timeNowIncrementBtn = -1000;
	timeNowDecrementBtn = -1000;
}

void updateScreen(String _timer){
	lcd.setCursor(0, 0);
  	if(pumpIsOn == true){
  		/*printing georgian word*/
  		lcd.print(char(1));
		lcd.print(char(2));
		lcd.print(char(3));
		lcd.print(char(4));
		lcd.print(char(5));
		lcd.print(char(6));
		lcd.print(char(7));
		lcd.print(char(2));
	}else{
		lcd.print("Paused: ");
	}

	lcd.print(" " + _timer);

  	lcd.setCursor(2, 1);

  	if(pumpIsOn == true){
	  	lcd.print(" Status: ON");
  	}else{
	  	lcd.print("Status: OFF");
  	}
}


void buttonStates(){
	if(digitalRead(buttonPin2) == HIGH){
		timeNowSwtBtn = millis();
	}else if(digitalRead(buttonPin0) == HIGH){
		timeNowIncrementBtn = millis();
	}else if(digitalRead(buttonPin1) == HIGH){
		timeNowDecrementBtn = millis();
	}
	pressStates();
}

void pressStates(){
	if(millis() - timeNowSwtBtn < period){
		if(swtBtnOncePressed == false){
			swtBtnOncePressed = true;
			pumpIsOn = !pumpIsOn;
		}
	}else{
		swtBtnOncePressed = false;
	}
	if(millis() - timeNowIncrementBtn < period && timer<3600){
		if(incrementBtnOncePressed == false){
			incrementBtnOncePressed = true;
			timer+=60;//1 minute
		}
	}else{
		incrementBtnOncePressed = false;
	}
	if(millis() - timeNowDecrementBtn < period && timer > 60){
		if(decrementBtnOncePressed == false){
			decrementBtnOncePressed = true;
			timer-=60; //1 min
		}
	}else{
		decrementBtnOncePressed = false;
	}
}

void ledIndicator(){
	if(pumpIsOn == true){
		digitalWrite(led, 1);
		digitalWrite(ledGreen,1);
		digitalWrite(ledRed,0);
	}else{
		digitalWrite(led, 0);
		digitalWrite(ledGreen,0);
		digitalWrite(ledRed,1);
	}
}

void myServer(String _timer){
		EthernetClient client = server.available();
		if(client){
			String data = "", keyVal = "";
			String myArray[2];
			int myIndex = 0;
			while(client.connected()){
					int dataCount = client.available();
					if (dataCount > 0){
						for (int i = 0; i < dataCount; i++){
							char ch = client.read();
							/*
							read line by line
							*/
							if(ch == '\n'){
								data = "";
							}else{
								data += String(ch);
							}
						}
					}
					for (int i=0;i<data.length();i++) {
						if(String(data[i]) == "&"){ //value=ase&value2=ise
							Serial.println(keyVal);
							myArray[myIndex]=keyVal;
							myIndex++;
							keyVal="";
						}else{
							keyVal+=String(data[i]);
						}
					}
					String v="",k="";
					String kk[2],vv[2];
					for (int p = 0; p < 2; p++) {
						if(myArray[0].length()>0 && myArray[1].length()>0){
							for (int o = 0; o < myArray[p].length(); o++) {
								if(String(myArray[p][o])=="="){
									v=myArray[p].substring(k.length()+1,myArray[p].length());
									vv[p]=v;
									kk[p]=k;
									k="";
									break;
								}else{
									k+=myArray[p][o];
								}
							}
						}
					}
			if(kk[1]=="onoff"){
				if(pumpIsOn == true){
					pumpIsOn = false;
				}else{
					if(kk[0]=="value"){
						if(String(vv[0]).length()>0){
							//min 1 minute max 1 hour
							if(vv[0].toInt()>0 && vv[0].toInt()<60){
								pumpIsOn = true;
								timer = vv[0].toInt()*60*100;
							}
						}
					}
				}
			}


			client.println("HTTP/1.1 200 OK");
			client.println("Content-Type: text/html; charset=utf-8");
			client.println();
			client.println("<html>");
			client.println("<head>");
			client.println("<title>წყლის ტუმბოს ავტომატიზაცია</title>");
			client.println("</head>");
			client.println("<body>");
			client.println("<style>*{margin:10px auto;text-align:center;}.r{color:rgba(255,0,0,0.6);}.g{color:rgba(0,255,0,0.6);}</style>");
			client.println("<body>");
			client.println("<form method='post'>");
			client.println("<h1>წყლის ტუმბოს ავტომატიზაციის სისტემა</h1>");
			if(pumpIsOn == true){
				client.print("<h2>სტატუსი <b class='g'>ჩართულია");
			}else{
				client.print("<h2>სტატუსი <b class='r'>გამორთულია");
			}
			client.println("</b></h2>");
			client.print("<h2>გაითიშება: <b>");
			client.print(_timer);
			client.print("</h2><br>");
			client.println("<input type='submit'value='განახლება &#x21bb'>");
			client.println("<br>");
			client.print("<input type='number' name='value' min='1' max='60' value='");
			client.print(defaultTime/60);
			client.print("'>");
			client.println("<br>");
			client.print("<button type=\"submit\" name=\"onoff\" value=\"data\">");
			if(pumpIsOn == true){
				client.print("გამორთვა {0}");
			}else{
				client.print("ჩართვა {1}");
			}
			client.println("</button>");
			client.println("<input type='hidden' name='hdn'>");
			client.println("</form>");
			client.println("</body>");
			client.println("</html>");
			client.stop();
		}
	}
}

void loop() {
	int seconds = timer;
	int ss = seconds % 60;
	int mm = seconds / 60;	

	String currTimer = String(mm<10?(String("0")+String(mm)):String(mm))+String(":")+String(ss<10?(String("0")+String(ss)):String(ss));
	
	if(pumpIsOn == true){
		if(now() - prev >= 1){
			timer -= 1;
			prev = now();
			myServer(currTimer);
		}
	}


	buttonStates();
	ledIndicator();

	if( timer <= 0){
		pumpIsOn = false;
		timer = defaultTime;
		digitalWrite(led, 0);
	}

	if(millis() - timeGlobal > UpdatePeriod){
		//update screen
		updateScreen(currTimer);
		timeGlobal = millis();
	}

	prevTime = millis();
}
