int R1= 1002; 
int EC_Read = A0;
int ECPower = A1;
int Temp_pin = A5;
float Temp_C; 
float Temp_F; 
float Temp1_Value = 0;
float Temp_Coef = 0.019; 
float Calibration_PPM =1080 ;
float K=1.89; 
float PPM_Con=0.5; 
float CalibrationEC= (Calibration_PPM*2)/1000;
float Temperature;
float EC;
float EC_at_25;
int ppm;
float A_to_D= 0;
float Vin= 5;
float Vdrop= 0;
float R_Water;
float Value=0;
float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;
float ph_act;
int EC_Isolator = 8; // 3906 PNP 
int EC_GND_Wire = 9; // 2N2222 NPN 
 
void setup() {
 Serial.begin(9600);
pinMode(EC_Read,INPUT);
pinMode(ECPower,OUTPUT);
pinMode(EC_Isolator, OUTPUT);
pinMode(EC_GND_Wire, OUTPUT);
pinMode(3,OUTPUT);
pinMode(4,OUTPUT);
pinMode(5,OUTPUT);
pinMode(6,OUTPUT);
digitalWrite(EC_Isolator, HIGH);
digitalWrite(EC_GND_Wire, LOW); 
// căn chỉnh hệ số k theo chất liệu đầu đo
 Calibrate (); // sau khi căn chỉnh đặt '//' ở đầu dòng
//
}

void loop() {
digitalWrite(EC_Isolator, HIGH);
digitalWrite(EC_GND_Wire, LOW); 
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
//
GETpH();
Serial.println("\npH Val: ");
Serial.print(ph_act);
//
if ((6 <= ph_act)&&(ph_act <= 7.5)) // có thể đặt biến thay đổi theo biến trở
{   
} 
while (ph_act < 6) 
{
GETpH();
delay(1000);
digitalWrite(3, LOW);
delay(2000);
digitalWrite(3,HIGH);
}
while (ph_act > 7.5) 
{
GETpH();
delay(1000);
digitalWrite(4, LOW);
delay(2000);
digitalWrite(4, HIGH);
}
digitalWrite(EC_Isolator,LOW); 
digitalWrite(EC_GND_Wire, HIGH);
//
GETEC();
Serial.print(" \nEC: ");
Serial.print(EC_at_25);
Serial.print(" milliSiemens(mS/cm) ");
Serial.print(ppm);
Serial.print(" ppm ");
Serial.print(Temperature);
Serial.print(" *C ");
//
if ((2 <= EC_at_25)&&(EC_at_25 <= 2,5))
{
}
while(EC_at_25 < 2 )
{
GETEC();
delay(1000);
digitalWrite(5,LOW);
delay(2000);
digitalWrite(5, HIGH);
}
while(EC_at_25 > 2,5 )
{
GETEC();
// cài đặt van mở nước vào bình chứa 
}
delay(4000);
}
void GETpH()
{
 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A2);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6; 
  ph_act = -5.70 * volt + calibration_value;
 delay(1000);
 }
 void GETEC()
{
int val;
double Temp;
val=analogRead(Temp_pin);
Temp = log(((10240000/val) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp);
Temp_C = Temp - 273.15; // Kelvin to Celsius
Temp_F = (Temp_C * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit
Temp1_Value = Temp_C;
Temperature = Temp_C;
digitalWrite(ECPower,HIGH);
A_to_D= analogRead(EC_Read);
A_to_D= analogRead(EC_Read);
digitalWrite(ECPower,LOW);
Vdrop= (Vin*A_to_D) / 1024.0;
R_Water = (Vdrop*R1) / (Vin-Vdrop);
EC = 1000/ (R_Water*K);
EC_at_25 = EC / (1+ Temp_Coef*(Temperature-25.0));
ppm=(EC_at_25)*(PPM_Con*1000);
delay(1000);
}
void Calibrate ()
{
Serial.println("Calibration routine started");
float Temperature_end=0;
float Temperature_begin=0;
int val;
double Temp;
val=analogRead(Temp_pin);
Temp = log(((10240000/val) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp);
Temp_C = Temp - 273.15; // Kelvin to Celsius

Temp_F = (Temp_C * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit
Temp1_Value = Temp_C;
Temperature_begin=Temp_C;
Value = 0;
int i=1;
while(i<=10){
digitalWrite(ECPower,HIGH);
A_to_D= analogRead(EC_Read);
A_to_D= analogRead(EC_Read);
digitalWrite(ECPower,LOW);
Value=Value+A_to_D;
i++;
delay(6000);
};
A_to_D=(Value/10);
val=analogRead(Temp_pin);
Temp = log(((10240000/val) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp);
Temp_C = Temp - 273.15; // Kelvin to Celsius
Temp_F = (Temp_C * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit
Temp1_Value = Temp_C;
Temperature_end=Temp_C;
EC =CalibrationEC*(1+(Temp_Coef*(Temperature_end-25.0)));
Vdrop= (((Vin)*(A_to_D))/1024.0);
R_Water=(Vdrop*R1)/(Vin-Vdrop);
float K_cal= 1000/(R_Water*EC);
Serial.print("Thay thế K = ");
Serial.println(K_cal);
Serial.print("Temperature difference start to end were = ");
Temp_C=Temperature_end-Temperature_begin;
Serial.print(Temp_C);
Serial.println("*C");
Serial.println("Temperature difference start to end must be smaller than 0.15*C");
Serial.println("");
Calibrate ();
}
