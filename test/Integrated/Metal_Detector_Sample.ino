float metal_detected;
int monitoring;
int monitorPin = 1;
int var = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
monitoring= analogRead(monitorPin);
if(monitoring < 250)
{
 Serial.println("metal detected");
}
/*Serial.println("monitoring value = " + monitoring);
Serial.println("testing....");*/
Serial.println(var);
var = var + 5; 
delay(1000);


}
