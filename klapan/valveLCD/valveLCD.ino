//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
int REPEAT_DELAY = 300;
File myFile;


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
int minIntDelay = 50;
String Menu[3] = {"1.Set HP level", "2.Set LP level", "3.Set sycle fr"};


bool valueChanged = false;
long press_timer = 0;


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

int LREPEAT_DELAY;

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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(2)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");



  //если память пустая меняем значения на 0 -- должно отработатьдолжно отработать при первой прошивке контроллера
  if (EEPROM.read(9) > 180) {
    for (int i = 0; i < 15; i++) {
      EEPROM.write(i, 0);
    }
  }



  read_params();

  lcd.begin(16, 2);              // start the library
  lcd.setCursor(4, 0);
  lcd.print("LAR TECH");
  delay(1000);
  lcd.clear();

}



void WriteFile(String f_name, String s_write) {
  myFile = SD.open(f_name, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to ");
    Serial.print(f_name);
    Serial.print(" ... ");
    myFile.println(s_write);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}


void ReadFile(String f_name) {
  // re-open the file for reading:
  myFile = SD.open(f_name);
  if (myFile) {

    Serial.print(f_name);
    Serial.print(": ");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}




void menu_get_int(String s, int* v, int minv, int maxv)
{
  LREPEAT_DELAY = 300;
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
//     Serial.println( read_LCD_buttons());
      if (read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 && read_LCD_buttons() == 5 ) LREPEAT_DELAY = 300;
      if (lcd_key == btnDOWN) {
        press_timer = millis() + LREPEAT_DELAY;
        LREPEAT_DELAY-=10;
        if (LREPEAT_DELAY < minIntDelay) {
          LREPEAT_DELAY = minIntDelay;
        }
        (*v)--;
        if (*v < minv) *v = minv;
        valueChanged = true;
      }
      if (lcd_key == btnUP) {
        press_timer = millis() + LREPEAT_DELAY;
        LREPEAT_DELAY-=10;
        if (LREPEAT_DELAY < minIntDelay) {
          LREPEAT_DELAY = minIntDelay;
        }
        (*v)++;
        if (*v > maxv) *v = maxv;
        valueChanged = true;
      }

      if (lcd_key == btnLEFT) {
        lcd.clear();
        save_params();
        return;
      }
    }
  }
}

int menu_select(String Header, int Header_pos, String *paragraph, int p, int start) {

  //    lcd.clear();

  lcd.clear();
  lcd.setCursor(Header_pos, 0);
  lcd.print(Header);
  int menu_num = start;
  int last_key = btnLEFT;
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

    if (lcd_key == btnLEFT && last_key != btnLEFT) {
      lcd.clear();
      delay(wait_del);
      return -1;
    }

    if (lcd_key == btnRIGHT) {
      lcd.clear();
      delay(wait_del);
      return menu_num;
    }
    last_key = lcd_key;
  }
}

void save_params() {
  EEPROM_uint_write(0, HPL_val);
  EEPROM_uint_write(2, LPL_val);
  EEPROM_uint_write(4, fC);
}


void read_params() {
  HPL_val = EEPROM_uint_read (0);
  LPL_val = EEPROM_uint_read (2);
  fC = EEPROM_uint_read (4);
}



unsigned int EEPROM_uint_read(int addr) {
  byte raw[2];
  for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr + i);
  unsigned int &num = (unsigned int&)raw;
  return num;
}
// запись
void EEPROM_uint_write(int addr, unsigned int num) {
  byte raw[2];
  (unsigned int&)raw = num;
  for (byte i = 0; i < 2; i++) EEPROM.write(addr + i, raw[i]);
}



void loop()
{

  MainScreen();
  lcd_key = read_LCD_buttons();  // read the buttons

  if (lcd_key == btnRIGHT) {
    delay(wait_del);

    int choise = 0;
    while (1) {
      choise = menu_select("MENU", 6, Menu, 3, choise);
      if (choise == -1) {
        break;
        delay(200);
      }
      if (choise == 0)  menu_get_int("HP level -", &HPL_val, HPL_min_val, HPL_max_val);
      if (choise == 1)  menu_get_int("LP level -", &LPL_val, LPL_min_val, LPL_max_val);
      if (choise == 2)  menu_get_int("fC -", &fC, fC_min_val, fC_max_val);

    }

  }


}




