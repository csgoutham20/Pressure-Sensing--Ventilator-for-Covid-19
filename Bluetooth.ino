void BT_Parse()
{
  if(Serial.available()>=5)
  {
    if(Serial.read()=='A')
    {
      switch(Serial.read())
      {
        case 'B': TIDAL_VOL=(Serial.read()*256)+Serial.read();if(TIDAL_VOL<100)TIDAL_VOL=100;if(TIDAL_VOL>1000)TIDAL_VOL=1000;break;
        case 'C': BPM=Serial.read();if(BPM<10)BPM=10;if(BPM>40)BPM=40;break;
        case 'D': sys_mode=RUN;break;
      }
      drawBTMenu();
    }
    while(Serial.available())
      Serial.read();
  }
}
