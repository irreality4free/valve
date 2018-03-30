//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#define REPEAT_DELAY 300


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int wait_del = 200;
String Menu[3] = {"1.Set HP level", "2.Set LP level", "3.Set sycle fr"};


int HPL_val = 100;
int HPL_min_val = 0;
int HPL_max_val = 100;
int LPL_val = 100;
int LPL_min_val = 0;
int LPL_max_val = 100;
int fC = 1000;
int fC_min_val = 0;
int fC_max_val = 1000;
int nC = 1;

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(A0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */


  return btnNONE;  // when all others fail, return this...
}


void MainScreen() {
  lcd.setCursor(0, 0);
  lcd.print("HPL-");
  lcd.setCursor(4, 0);
  lcd.print(HPL_val);
  lcd.setCursor(7, 0);
  lcd.print(",fC-");
  lcd.setCursor(11, 0);
  lcd.print(fC);

  lcd.setCursor(0, 1);
  lcd.print("LPL-");
  lcd.setCursor(4, 1);
  lcd.print(LPL_val);
  lcd.setCursor(7, 1);
  lcd.print(",nC-");
  lcd.setCursor(11, 1);
  lcd.print(nC);
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(4, 0);
  lcd.print("LAR TECH");
  delay(1000);
  lcd.clear();
}
bool valueChanged = false;
long press_timer = 0;



void menu_get_int(String s, int* v, int minv, int maxv)
{
  valueChanged = true;
  int l = s.length();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s);
  String erase;
  for (int i = 0; i < 16 - l; i++) erase += " ";
  while (1) {
    if (valueChanged) {
      valueChanged = false;
      lcd.setCursor(l, 0);
      lcd.print(erase);
      lcd.setCursor(l, 0);
      lcd.print(*v);
    }
    if (press_timer < millis())
    {
      lcd_key = read_LCD_buttons();
      if (lcd_key == btnDOWN) {
        press_timer = millis() + REPEAT_DELAY;
        (*v)--;
        if (*v < minv) *v = minv;
        valueChanged = true;
      }
      if (lcd_key == btnUP) {
        press_timer = millis() + REPEAT_DELAY;
        (*v)++;
        if (*v > maxv) *v = maxv;
        valueChanged = true;
      }

      if (lcd_key == btnLEFT) {
        lcd.clear();
        return;
      }
    }
  }
}

int menu_select(String Header, int Header_pos, String *paragraph, int p) {

  //    lcd.clear();

  lcd.clear();
  lcd.setCursor(Header_pos, 0);
  lcd.print(Header);
  int menu_num = 0;
  while (1) {
    for (int i = 0; i < p; i++) {
      if (menu_num == i) {
        lcd.setCursor(Header_pos, 0);
        lcd.print(Header);
        lcd.setCursor(0, 1);
        lcd.print(paragraph[i]);
      }
    }

    lcd_key = read_LCD_buttons();

    if (lcd_key == btnUP) {
      menu_num--;
      delay(wait_del);
      if (menu_num < 0) {
        menu_num = p - 1;
      }
    }

    if (lcd_key == btnDOWN) {
      menu_num++;
      delay(wait_del);
      if (menu_num > p - 1) {
        menu_num = 0;
      }
    }

    if (lcd_key == btnLEFT) {
      lcd.clear();
      delay(wait_del);
      return -1;
    }

    if (lcd_key == btnRIGHT) {
      lcd.clear();
      delay(wait_del);
      return menu_num;
    }

  }
}




void loop()
{

  MainScreen();
  lcd_key = read_LCD_buttons();  // read the buttons

  if (lcd_key == btnRIGHT) {
    delay(wait_del);
    int choise = menu_select("MENU", 6, Menu, 3);
    Serial.print("choise - ");
    Serial.println(choise);

    if (choise == 0)  menu_get_int("HP level -", &HPL_val, HPL_min_val, HPL_max_val);
    if (choise == 1)  menu_get_int("LP level -", &LPL_val, LPL_min_val, LPL_max_val);
    if (choise == 2)  menu_get_int("fC -", &fC, fC_min_val, fC_max_val);

  }


}























//void MenuSel(){
//  lcd_key = read_LCD_buttons();  // read the buttons
//
//  if (lcd_key == btnRIGHT) {
//    delay(wait_del);
//    lcd.clear();
//
//    int menu_num = 1;
//
//
//
//    while (1) {
//      if (menu_num == 1) {
//        lcd.setCursor(6, 0);
//        lcd.print("MENU");
//        lcd.setCursor(0, 1);
//        lcd.print("1.Set HP level");
//      }
//
//      if (menu_num == 2) {
//        lcd.setCursor(6, 0);
//        lcd.print("MENU");
//        lcd.setCursor(0, 1);
//        lcd.print("2.Set LP level");
//      }
//
//      if (menu_num == 3) {
//        lcd.setCursor(6, 0);
//        lcd.print("MENU");
//        lcd.setCursor(0, 1);
//        lcd.print("3.Set sycle fr");
//      }
//      lcd_key = read_LCD_buttons();
//
//      if (lcd_key == btnUP) {
//        menu_num--;
//        delay(wait_del);
//        if (menu_num < 1) {
//          menu_num = 3;
//        }
//      }
//
//      if (lcd_key == btnDOWN) {
//        menu_num++;
//        delay(wait_del);
//        if (menu_num > 3) {
//          menu_num = 1;
//        }
//      }
//
//      if (lcd_key == btnLEFT) {
//        lcd.clear();
//        delay(wait_del);
//        break;
//      }
//      if (lcd_key == btnRIGHT) {
//        if (menu_num == 1)  menu_get_int("HP level -", &HPL_val, HPL_min_val, HPL_max_val);
//        if (menu_num == 2)  menu_get_int("LP level -", &LPL_val, LPL_min_val, LPL_max_val);
//        if (menu_num == 3)  menu_get_int("fC -", &fC, fC_min_val, fC_max_val);
//
//
//      }
//    }
//  }
//
//}
