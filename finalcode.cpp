#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 5, 180 };
// byte ip[] = { 10,10,10,10 };
EthernetServer server(80);
const int  buttonPin0 = A0;
const int  buttonPin1 = A1;
const int  buttonPin2 = A2;
const int  ledGreen = A5;
const int  ledRed = A4;
const int led = 7;
int val = (10 * 60) - 1; // seconds
int defaultTime = 10 * 60; // seconds
String sec,mint;

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

bool pumpIsOn = false;

byte a[8] =   {	B00100,	B00100,	B00010,	B00001,	B00001,	B10001,	B10001,	B01110 };
byte C[8] =   {	B11110,	B10001,	B10010,	B11100,	B10000,	B11110,	B10001,	B10001 };
byte r[8] =   {	B10000,	B10000,	B10000,	B10000,	B11110,	B10001,	B10001,	B10001 };
byte T[8] =   { B01010,	B10101,	B10101,	B10101,	B10101,	B10101,	B10101,	B01001 };
byte u[8] =   {	B11011,	B00101,	B00001,	B00001,	B00001,	B10001,	B10001,	B01110 };
byte l[8] =   {	B01110,	B10001, B10001, B10000, B10000, B01000, B10100, B01111 };
byte i[8] =   {	B01110,	B10001,	B10001,	B10001,	B10001,	B10001,	B10001,	B10001 };

void setup() {
	Serial.begin(9600);
	Ethernet.begin(mac, ip);
	server.begin();

	pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

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
	pinMode(led, OUTPUT);
	pinMode(ledGreen, OUTPUT);
	pinMode(ledRed, OUTPUT);
	pumpIsOn = false;
	gaitisheba();
}


void gaitisheba(){
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

	int minutes = val / 60;
	int seconds = val % 60;
	if(seconds < 10){
		sec = '0' + String(seconds);
	}else{
		sec = String(seconds);
	}

	if(minutes < 10){
		mint = '0' + String(minutes);
	}else{
		mint = String(minutes);
	}

	lcd.print(" " + mint + ":" + sec);

  	lcd.setCursor(2, 1);

  	if(pumpIsOn == true){
	  	lcd.print(" Status: ON");
  	}else{
	  	lcd.print("Status: OFF");
  	}

}

void myServer(){
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
								val = vv[0].toInt()*60;
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
			if(String(val/60).length()==1){
				client.print("0"+String(val/60));
			}else{
				client.print(String(val/60));
			}
			client.print(":");
			if(String(val%60).length()==1){
				client.print("0"+String(val%60));
			}else{
				client.print(String(val%60));
			}
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

	ledIndicator();
	buttonStates();
}

void buttonStates(){
	int buttonState0 = digitalRead(buttonPin0);
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);
	if (buttonState0 == HIGH && val/60 < 60) {
		if(pumpIsOn == false) delay(300);
		val += 60;
  }

  if (buttonState1 == HIGH && val/60 > 0) {
		if(pumpIsOn == false) delay(300);
		val -= 60;
  }

  if (buttonState2 == HIGH) {
		if(pumpIsOn == true){
			pumpIsOn = false;
			delay(1000);
		}else{
			pumpIsOn = true;
			delay(300);
		}
  }
}

void myTimer(){
	myServer();
	if(val == 0){
		pumpIsOn = false;
		digitalWrite(led, 0);
		val = defaultTime;
		gaitisheba();
	}else{
		if(pumpIsOn == true){
			val--;
		  lcd.clear();
			gaitisheba();
			delay(982); // es dro yvelaze zustia
			digitalWrite(led, 1);
			myTimer();
		}else{
			digitalWrite(led, 0);
			gaitisheba();
		}
	}
}

void ledIndicator(){
	if(pumpIsOn == true){
		digitalWrite(ledGreen,1);
		digitalWrite(ledRed,0);
	}else{
		digitalWrite(ledGreen,0);
		digitalWrite(ledRed,1);
	}
}
void loop() {
  myTimer();
}
