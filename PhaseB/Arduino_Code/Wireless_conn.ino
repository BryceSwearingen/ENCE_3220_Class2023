String data ;

#define LED 7  

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100); // adjust to get all data read if needed 
}

void loop(){
if (Serial.available() > 0){
 data = Serial.readString();
 Serial.print(data);


 if ( data == "$STR\n"){
      Serial.print("LEDon");
      digitalWrite(LED, HIGH);
    }

if ( data == "$STP\n"){
      Serial.print("LEDoff");
      digitalWrite(LED, LOW);
    }
 
  }
}