#include "U8glib.h"
#define encoder_pinA 7
#define encoder_pinB 8
#define encoder_button 5
#define buzzer 4

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

U8GLIB_ST7920_128X64_1X u8g(6, 12, 11);  // SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
uint8_t uiKeyCode = KEY_NONE;
#define u8g_logo_width 50
#define u8g_logo_height 54
static unsigned char u8g_logo_bits[] U8G_PROGMEM =  {
   0x00, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x01,
   0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0xe0, 0xff,
   0xff, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00,
   0xfc, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x01,
   0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x80, 0xff, 0xff, 0xff,
   0xff, 0x07, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xc0, 0xff,
   0xff, 0xff, 0xff, 0x0f, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x70, 0xe0, 0xff, 0xff, 0x1f,
   0x38, 0x00, 0x70, 0x80, 0xff, 0xff, 0x07, 0x38, 0x00, 0x78, 0x00, 0xff,
   0xff, 0x03, 0x78, 0x00, 0x78, 0x00, 0xfe, 0xff, 0x01, 0x78, 0x00, 0x78,
   0x00, 0xf8, 0x7f, 0x00, 0x78, 0x00, 0x78, 0x00, 0xf0, 0x3f, 0x00, 0x78,
   0x00, 0x78, 0x00, 0xe0, 0x1f, 0x00, 0x78, 0x00, 0x7c, 0x00, 0x80, 0x07,
   0x00, 0x78, 0x00, 0x7c, 0x00, 0x00, 0x03, 0x00, 0xf8, 0x00, 0x7c, 0xe0,
   0xff, 0xff, 0x1f, 0xf8, 0x00, 0x7c, 0x20, 0x00, 0x00, 0x18, 0xf8, 0x00,
   0x7c, 0x20, 0x00, 0x00, 0x10, 0xf8, 0x00, 0x7c, 0x20, 0x00, 0x00, 0x18,
   0xf8, 0x00, 0x7c, 0xe0, 0x3f, 0xf8, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x3f,
   0xf9, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0xbf, 0xfc, 0x1f, 0xf8, 0x00, 0x7c,
   0xe0, 0x7f, 0xfa, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x3f, 0xf9, 0x1f, 0xf8,
   0x00, 0x7c, 0xe0, 0xbf, 0xfc, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x7f, 0xfa,
   0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x3f, 0xf9, 0x1f, 0xf8, 0x00, 0x7c, 0xe0,
   0x3f, 0xfc, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x7f, 0xfa, 0x1f, 0xf8, 0x00,
   0x7c, 0xe0, 0x7f, 0xf8, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x3f, 0xfc, 0x1f,
   0xf8, 0x00, 0x7c, 0xe0, 0xff, 0xfc, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x7f,
   0xf8, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x3f, 0xf9, 0x1f, 0xf8, 0x00, 0x7c,
   0xe0, 0xff, 0xfc, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x7f, 0xfe, 0x1f, 0xf8,
   0x00, 0x7c, 0xe0, 0x3f, 0xfd, 0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0xbf, 0xfc,
   0x1f, 0xf8, 0x00, 0x7c, 0xe0, 0x7f, 0xfe, 0x1f, 0xf8, 0x00, 0x7c, 0xe0,
   0x7f, 0xfd, 0x1f, 0xf8, 0x00, 0x7e, 0xe0, 0xff, 0xfe, 0x1f, 0xf8, 0x01,
   0x7f, 0xe0, 0xff, 0xff, 0x1f, 0xf8, 0x03, 0x7f, 0xe0, 0xff, 0xfe, 0x1f,
   0xf8, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x03 };



int menu_level=0,sub_menu_level=0;
const char *main_menu_strings[2] = { "Constant Volume", "Constant Pressure"};

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;

void lcd_init()
{
  
  pinMode(encoder_pinA, INPUT_PULLUP);
  pinMode(encoder_pinB, INPUT_PULLUP);
  pinMode(encoder_button, INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);

  digitalWrite(buzzer,LOW);
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  u8g.firstPage();
        do  {
            u8g.drawXBMP( 0, 0, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
         
        } while( u8g.nextPage() );
  delay(3000);
  menu_redraw_required = 1;     // force initial redraw
  
}
void clearLCD(){
    u8g.firstPage(); 
    do {
    } while( u8g.nextPage() );
}
void drawMainMenu(void) {
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  /*u8g.setFont(u8g_font_cursor);
  u8g.drawStr( 20, 10, "\x91");
  u8g.drawStr( 20, 30, "\x91");
  u8g.drawStr( 20, 60, "\x91");*/
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < 2; i++ ) {
    d = (w-u8g.getStrWidth(main_menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, (i+1)*2*h, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(0, (i+1)*2*h, "\xbb");
    u8g.drawStr(d, (i+1)*2*h, main_menu_strings[i]);
  }

  u8g.setFont(u8g_font_8x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  u8g.drawStr(0, 0, "Main Menu");
}

void drawVolMenu(void) {
  uint8_t i, h;
  String vol_menu_strings[4] = { "Tidal: ", "BPM  : ","Start", "Back"};
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  /*u8g.setFont(u8g_font_cursor);
  u8g.drawStr( 20, 10, "\x91");
  u8g.drawStr( 20, 30, "\x91");
  u8g.drawStr( 20, 60, "\x91");*/
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  vol_menu_strings[0]+=String(TIDAL_VOL);
  vol_menu_strings[1]+=String(BPM);
  for( i = 0; i < 4; i++ ) {
    d = (w-u8g.getStrWidth(vol_menu_strings[i].c_str()))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, (i+2)*h, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(0, (i+2)*h, "\xbb");
    u8g.drawStr(d, (i+2)*h, vol_menu_strings[i].c_str());
  }

  u8g.setFont(u8g_font_8x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  u8g.drawStr(0, 0, "Constant Volume");
}
void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  int menu_count=0;
  switch(menu_level){
    case 0: menu_count=2;break;
    case 1: menu_count=4;break;
  }
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if(sub_menu_level!=0)
      {
        switch(menu_level){
          case 1:
                  switch(menu_current){
                    case 0: TIDAL_VOL+=50;if(TIDAL_VOL>1000)TIDAL_VOL=1000;break;
                    case 1: BPM++;if(BPM>40)BPM=40;break;
                  }
                  break;
        }
      }
      else
      {
        menu_current++;
        if ( menu_current >= menu_count )
          menu_current = 0;
      }
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
    if(sub_menu_level!=0)
      {
        switch(menu_level){
          case 1:
                  switch(menu_current){
                    case 0: TIDAL_VOL-=50;if(TIDAL_VOL<100)TIDAL_VOL=100;break;
                    case 1: BPM--;if(BPM<10)BPM=10;break;
                  }
                  break;
        }
      }
      else
      {
        if ( menu_current == 0 )
          menu_current = menu_count;
        menu_current--;
      }
      menu_redraw_required = 1;
      break;
  }
}

int valRotary,lastValRotary;
long int lcd_timeout=0;
void button_poll()
{
  static int encoder_pinA_prev=0;
  static int pot_encoder_pulse=0;
  if(digitalRead(encoder_button)==LOW)
  {
    while(digitalRead(encoder_button)==LOW);
    if(sys_mode==LCD)
    {
      switch(menu_current){
        case 0: if(menu_level==0)
                  menu_level=1;
                else
                  sub_menu_level=!sub_menu_level;
                break;
        case 1: if(menu_level==0)
                  menu_level=2;
                else
                  sub_menu_level=!sub_menu_level;
                break;
        case 2: sys_mode=RUN;break;
        case 3:menu_level=0;
               break;
      }
      menu_redraw_required = 1;
    }
    else if(sys_mode==RUN)
      sys_mode=STOP;
    else if(sys_mode==IDLE)
      sys_mode=LCD;
    lcd_timeout=millis();
  }
  if(sys_mode==LCD)
  {
    if(millis()-lcd_timeout>10000)
     {
      sys_mode=IDLE;
      clearLCD();
     }
     if(digitalRead(encoder_pinA)!=encoder_pinA_prev)
     {
        encoder_pinA_prev=digitalRead(encoder_pinA);
        lcd_timeout=millis();
        if(digitalRead(encoder_pinA)==digitalRead(encoder_pinB))
          pot_encoder_pulse++;
        else
          pot_encoder_pulse--;
        valRotary=pot_encoder_pulse/2;
     }
     if(valRotary>(lastValRotary+1))
     {
        uiKeyCode=KEY_NEXT;
        lastValRotary=valRotary;
     }
     else if(valRotary<(lastValRotary-1))  {
  
        uiKeyCode=KEY_PREV;
        lastValRotary=valRotary;
     }
     else 
      uiKeyCode=KEY_NONE;
     if (  menu_redraw_required != 0 ) {
        u8g.firstPage();
        do  {
          switch(menu_level){
            case 0:drawMainMenu();break;
            case 1:drawVolMenu();break;
          }
          
        } while( u8g.nextPage() );
        menu_redraw_required = 0;
      }
    
      updateMenu();                            // update menu bar
  }
}
void drawBTMenu(void) {
  u8g.firstPage();
  do  {
         
    uint8_t i, h;
    String vol_menu_strings[2] = { "Tidal: ", "BPM  : "};
    u8g_uint_t w, d;
  
    if(sys_mode==RUN)
    {
      u8g.setFont(u8g_font_9x18);
      u8g.setFontRefHeightText();
      u8g.setFontPosTop();
      h = u8g.getFontAscent()-u8g.getFontDescent();
      w = u8g.getWidth();
      d = (w-u8g.getStrWidth("System Running"))/2;
        u8g.setDefaultForegroundColor();
      u8g.drawStr(d, 0, "System Running");
    }
    else
    {
      u8g.setFont(u8g_font_6x13);
      u8g.setFontRefHeightText();
      u8g.setFontPosTop();
      
      h = u8g.getFontAscent()-u8g.getFontDescent();
      w = u8g.getWidth();
      vol_menu_strings[0]+=String(TIDAL_VOL);
      vol_menu_strings[1]+=String(BPM);
      for( i = 0; i < 2; i++ ) {
        d = (w-u8g.getStrWidth(vol_menu_strings[i].c_str()))/2;
        u8g.setDefaultForegroundColor();
        if ( i == menu_current ) {
          u8g.drawBox(0, (i+1)*2*h, w, h);
          u8g.setDefaultBackgroundColor();
        }
        u8g.drawStr(0, (i+1)*2*h, "\xbb");
        u8g.drawStr(d, (i+1)*2*h, vol_menu_strings[i].c_str());
      }
    
      u8g.setFont(u8g_font_8x13);
      u8g.setFontRefHeightText();
      u8g.setFontPosTop();
      h = u8g.getFontAscent()-u8g.getFontDescent();
      w = u8g.getWidth();
      u8g.drawStr(0, 0, "Bluetooth Mode");
    }
 } while( u8g.nextPage() );
}
