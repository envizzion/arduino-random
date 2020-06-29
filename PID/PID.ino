long sensor[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};

long sensors_average;
int sensors_sum;
int position;


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
 
//IR Sensor monitor
int sensor[6]={analogRead(A0),analogRead(A1),analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5)};
 for(int i=0;i<6;i++){
  Serial.println(sensor[i]);
  if(i==5){
  Serial.println("");
  }
 }
 delay(3000);


sensors_average = 0;
sensors_sum = 0;

for (int i = 0; i < 6; i++)
{
 sensors_average += sensor[i] * i * 1000; //Calculating the weighted mean
 sensors_sum += int(sensor[i]);
 
 }
 
 //Calculating sum of sensor readings
position = int(sensors_average / sensors_sum);
Serial.print(sensors_average);
Serial.print(' ');
Serial.print(sensors_sum);
Serial.print(' ');
Serial.print(position);
Serial.println();
delay(2000);

}
