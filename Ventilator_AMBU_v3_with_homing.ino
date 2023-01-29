
#define BPM_PIN A2
#define TIDAL_PIN A3
#define LED_GREEN 8
#define LED_RED 7
#define MOTOR_PINA OCR1A
#define MOTOR_PINB OCR1B
#define MOTOR_OPTO_PIN 2
#define START_OPTO_PIN 3
#define GEAR_RATIO 27
#define encoder_pulse 20
#define PRESSURE_SENSING_PIN A0
#define arr_size 20                                                                                                                                                                                                   
#define slope_arr_size 8
#define conversion_factor 0.6082294921875

enum modes{IDLE,HOMING,RUN,LCD,STOP,BT};
enum modes sys_mode=HOMING;
double motor_rot_ref=0,homing_rot_ref=220;
double inhale_bpm_ref=0,exhale_bpm_ref=0;

int pressure=0,direction=-1;
float pressure_array_avg=0,prev_pressure_array_avg=0,slope_array_avg=0,max_pressure=0,min_pressure=1024;
int pressure_array[arr_size]={0};
int index=0,slope_index=0;
float slope,prev_slope;
float slope_array[slope_arr_size]={0};
bool count=1,threshold_crossed=0,cycle_completed = 0,inhale_start = 0,exhale_start = 0,homing_complete=0;
unsigned long temp_time,time_period;
int position_count = 0,addr=0;



void setup() {
  
 Serial.begin(115200);
 pinMode(9, OUTPUT);
 pinMode(10, OUTPUT);
  TCCR1A = _BV(COM1A1)| _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(CS11);
  MOTOR_PINA = 0;
  MOTOR_PINB = 0;
  pinMode(MOTOR_OPTO_PIN, INPUT_PULLUP);
  pinMode(START_OPTO_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MOTOR_OPTO_PIN), motor_opto_intr, RISING );
  attachInterrupt(digitalPinToInterrupt(START_OPTO_PIN), start_opto_intr, RISING );

  lcd_init();
  cli();//stop interrupts

//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 255;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

////set timer1 interrupt at 1Hz
//  TCCR1A = 0;// set entire TCCR1A register to 0
//  TCCR1B = 0;// same for TCCR1B
//  TCNT1  = 0;//initialize counter value to 0
//  // set compare match register for 1hz increments
//  OCR1A = 65535;// = (16*10^6) / (1*1024) - 1 (must be <65536)
//  // turn on CTC mode
//  TCCR1B |= (1 << WGM12);
//  // Set CS10 and CS12 bits for 1024 prescaler
//  TCCR1B |= (1 << CS12) | (1 << CS10);  
//  // enable timer compare interrupt
//  TIMSK1 |= (1 << OCIE1A);
//  
    
sei();//allow interrupts

}

ISR(TIMER0_COMPA_vect){
 // put your main code here, to run repeatedly:
pressure=analogRead(PRESSURE_SENSING_PIN);
pressure_array[index]=pressure;
  pressure_array_avg=0;
  for(int i=0;i<arr_size;i++)
    pressure_array_avg += ((double)pressure_array[i])/arr_size;

  index=(index+1)%arr_size;
  if(index==(arr_size-1))
  {
  prev_slope = slope;
  slope_array_avg=0;
  slope_array[slope_index] = pressure_array_avg - prev_pressure_array_avg;
  slope_index=(slope_index+1)%slope_arr_size;
  for(int i=0;i<slope_arr_size;i++)
  slope_array_avg += (slope_array[i]*5)/slope_arr_size;

  prev_pressure_array_avg = pressure_array_avg;
//
  Serial.print(pressure_array_avg*conversion_factor);
  Serial.print(" ");
  Serial.print(slope_array_avg);
  Serial.print(" ");
  Serial.println(direction*50);
  
  

//  Serial.print(max_pressure*conversion_factor);
//  Serial.print(" ");
//  Serial.println(min_pressure*conversion_factor);
  
  
//  Serial.println(slope_array_avg);
//
if(pressure_array_avg > max_pressure && cycle_completed == 0)
    {
      max_pressure = pressure_array_avg;
    }
if(pressure_array_avg < min_pressure && cycle_completed == 1)
    {
      min_pressure = pressure_array_avg;
    }

  if(slope_array_avg > 45 && count==1)
  {
//    Serial.print("Min Pressure:");
//    Serial.print(min_pressure*conversion_factor);
//    Serial.println(" cm H20");
    time_period = millis() - temp_time;
    temp_time = millis();
//    Serial.print("Breath Duration: "); 
//    Serial.println(time_period);
//    Serial.println("Exhalation Start");
      
    exhale_start = 1;
    inhale_start = 0;

//    Serial.print("Exhale start = ");
//    Serial.println(exhale_start);
        
    min_pressure=1024;
    threshold_crossed = 1;
    count = 0;
    cycle_completed = 0;
  }

  if(slope_array_avg < -50  && count ==0)
  {
//    Serial.print("Max Pressure:");
//    Serial.print(max_pressure*conversion_factor);
//    Serial.println(" cm H20");
//    Serial.println("Inhalation Start");
    inhale_start = 1;
    exhale_start = 0;
//
//     Serial.print("Inhale start = ");
//    Serial.println(inhale_start);            
    
    max_pressure = 0;
    threshold_crossed = 0;
    count = 1;
    cycle_completed = 1;
  }
  }
  
}

int BPM=10,TIDAL_VOL=880,pwm=300,speed_zero_flag=0;
long pulse_count=0,prev_pulse_count=0;
double motor_speed=0,motor_rotation=0;
long systime=0,speed_zero_time=0,BT_timeout=0;
void loop() {

  //BPM=analogRead(BPM_PIN);
  //TIDAL_VOL=analogRead(TIDAL_PIN);
  button_poll();
  if(sys_mode==RUN || sys_mode==STOP)
  {
    motor_update();
    inhale_bpm_ref=300;//(double)((BPM)*15);//(((double)BPM)/1024*300)+100;
    exhale_bpm_ref=500;
    motor_rot_ref=(double)(TIDAL_VOL/4);//((double)TIDAL_VOL)/1024*270;
    
//    Serial.print(BPM);
//    Serial.print(",");
//    Serial.print(TIDAL_VOL);
//    Serial.print(",");
//    Serial.print(motor_speed);
//    Serial.print(",");
//    Serial.print(motor_rotation);
//    Serial.print(",");
//    Serial.println(pulse_count);
    if(millis()-systime>100)
    {
      motor_speed=((double)(pulse_count-prev_pulse_count))/encoder_pulse;
      prev_pulse_count=pulse_count;
      systime=millis();
    }
    if(millis()-speed_zero_time>10)
    {
      if(motor_speed==0)
        speed_zero_flag++;
      else
        speed_zero_flag=0;      
    }
  }
  else if(sys_mode==IDLE)
  {
    if(Serial.available()>0)
    {
      sys_mode=BT;
      BT_timeout=millis();
    }
  }
  else if(sys_mode==BT)
  {
    if(millis()-BT_timeout>10000)
     {
      //sys_mode=IDLE;
      clearLCD();
     }
     if(Serial.available()>0)
     {
        BT_Parse();
        BT_timeout=millis();
     }
  }
  else if(sys_mode == HOMING)
  motor_update();
  
}

void motor_opto_intr() {
  if(direction==1)
    pulse_count++;
  else
    pulse_count--;
  }

void start_opto_intr() {
  
  if(homing_complete == 0);
  {
   // Serial.print("sys_mode = ");
    Serial.println(sys_mode);
  //  Serial.println("In homing mode");
    MOTOR_PINA = 0;
    MOTOR_PINB = 0;
    if(homing_complete==0)
    sys_mode = IDLE;
    pwm=300;
    direction = 1;
    homing_complete=1;
    pulse_count=0;
  }
   if(sys_mode == RUN)
  {
  //  Serial.println("In run mode");
    MOTOR_PINA = 0;
    MOTOR_PINB = 0;
  }
  
}


void motor_update(){
  static long temp=0;
  motor_rotation=(((double)(pulse_count))*360/encoder_pulse/GEAR_RATIO);
  if(sys_mode==RUN)
  {
   // Serial.println(sys_mode);
    //if((motor_rotation>motor_rot_ref && direction==1)||((motor_rotation<10 && direction ==-1)))
    if((direction == 1 && exhale_start == 1) || (direction == -1 && inhale_start == 1)||(motor_rotation>motor_rot_ref && direction==1)||(motor_rotation<10 && direction ==-1))
      pwm=0;
    if(pwm==0 && motor_speed==0)
    {
      if(speed_zero_flag>10 && (slope_array_avg>10||slope_array_avg<-10))
      {
        speed_zero_flag=0;
        direction=-1*direction;
        if(direction==-1 && exhale_start == 1)
        {
          pwm=exhale_bpm_ref;
        }
        else
        {
        if(direction==1 && inhale_start == 1)
        {
          pwm=inhale_bpm_ref;
        }
        }
      }
    }
  }
  else if(sys_mode==STOP){
    if(motor_rotation<10)
    {
      pwm=0;
      sys_mode=IDLE;
    }
    else
    {
      direction =-1;
      pwm=inhale_bpm_ref;
    }
    
  }

  else if(sys_mode==HOMING){
    if(motor_rotation>=homing_rot_ref)
    direction = -1;
   
  }
  
  /*if(millis()-temp>2000)
  {
    temp=millis();
    direction=-1*direction;
    pwm=400;
  }*/
  if(direction==-1)
  {
  
    MOTOR_PINA = 0;
    MOTOR_PINB = pwm;
    
  }
  else
  {
  
    MOTOR_PINA = pwm;
    MOTOR_PINB = 0;
    
  }
}
