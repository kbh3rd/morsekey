
/* vim: set ts=2 sw=2 number:
 
 */

#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

Adafruit_LiquidCrystal lcd(0);

char Version[] = "$Revision: 1.15 $    $Locker:  $ \n" ;

unsigned short keyPin = 2 ;
unsigned short bt1Pin = 9 ;

unsigned short ledPin = 10; /* 13 */
unsigned short buzzerPin = 8 ;

unsigned short digPins[] = { 
  5, 3, 4, 6 } 
;
unsigned short digPinValue[] = { 
  1, 2, 4, 8 } 
;

byte keyState = LOW ;
byte prvState = LOW ;
byte bt1State = LOW ;
byte bt1PrvState = LOW ;
unsigned short Hz = 700 ;

byte wheelIndex = 0 ;
float wpmList[] = { 
  16.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 20.0, 22.0, 24,0
} ; // default value in 0
unsigned short ditLengths [10] ;  // differentiated in setup()
char *wpmStr[] = {
	"16 wpm", "8 wpm", "10 wpm", "12 wpm", "14 wpm", "16 wpm", "18 wpm", "20 wpm", "22 wpm", "24 wpm"
} ;

unsigned int prvDit = 0 ;
unsigned int dit = 60 ;
unsigned int dah = dit * 3 ;

unsigned int ditMin ;
unsigned int ditMax ;
unsigned int dahLen ;
unsigned int dahMax ;

int node = 0 ;

//- ------------------------------------------------------------------------
// https://en.wikipedia.org/wiki/Dichotomic_search

struct mnode  
{
  char *mchar ;
  byte  dah ;
  byte  dit ;
};

mnode mtree[] =
{
  { 
    "^", 1, 2   }
  ,	// 0
  { 
    "T", 3, 4   }
  ,	// 1
  { 
    "E", 5, 6   }
  ,	// 2
  { 
    "M", 7, 8   }
  ,	// 3
  { 
    "N", 9, 10   }
  ,	// 4
  { 
    "A", 11, 12   }
  ,	// 5
  { 
    "I", 13, 14   }
  ,	// 6
  { 
    "O", 27, 30   }
  ,	// 7
  { 
    "G", 15, 16   }
  ,	// 8
  { 
    "K", 17, 18   }
  ,	// 9
  { 
    "D", 19, 20   }
  ,	// 10
  { 
    "W", 21, 22   }
  ,	// 11
  { 
    "R", 37, 23   }
  ,	// 12
  { 
    "U", 40, 24   }
  ,	// 13
  { 
    "S", 25, 26   }
  ,	// 14
  { 
    "Q", 70, 70   }
  ,	// 15
  { 
    "Z", 46, 32   }
  ,	// 16
  { 
    "Y", 70, 33   }
  ,	// 17
  { 
    "C", 49, 70   }
  ,	// 18
  { 
    "X", 70, 34   }
  ,	// 19
  { 
    "B", 70, 35   }
  ,	// 20
  { 
    "J", 36, 70   }
  ,	// 21
  { 
    "P", 54, 70   }
  ,	// 22
  { 
    "L", 70, 39   }
  ,	// 23
  { 
    "F", 70, 70   }
  ,	// 24
  { 
    "V", 42, 60   }
  ,	// 25
  { 
    "H", 43, 44   }
  ,	// 26
  { 
    "^", 28, 29   }
  ,	// 27
  { 
    "0", 70, 70   }
  ,	// 28
  { 
    "9", 70, 70   }
  ,	// 29
  { 
    "^", 70, 31   }
  ,	// 30
  { 
    "8", 70, 45   }
  ,	// 31
  { 
    "7", 70, 70   }
  ,	// 32
  { 
    "(", 48, 70   }
  ,	// 33
  { 
    "/", 70, 70   }
  ,	// 34
  { 
    "6", 52, 70   }
  ,	// 35
  { 
    "1", 70, 53   }
  ,	// 36
  { 
    "^", 70, 38   }
  ,	// 37
  { 
    "+", 56, 70   }
  ,	// 38
  { 
    "&", 70, 70   }
  ,	// 39
  { 
    "^", 41, 57   }
  ,	// 40
  { 
    "2", 70, 70   }
  ,	// 41
  { 
    "3", 66, 70   }
  ,	// 42
  { 
    "4", 70, 70   }
  ,	// 43
  { 
    "5", 70, 63   }
  ,	// 44
  { 
    ":", 70, 70   }
  ,	// 45
  { 
    "^", 47, 70   }
  ,	// 46
  { 
    ",", 70, 70   }
  ,	// 47
  { 
    ")", 70, 70   }
  ,	// 48
  { 
    "^", 50, 51   }
  ,	// 49
  { 
    "!", 70, 70   }
  ,	// 50
  { 
    ";", 70, 70   }
  ,	// 51
  { 
    "-", 70, 70   }
  ,	// 52
  { 
    "\'", 70, 70   }
  ,	// 53
  { 
    "^", 70, 55   }
  ,	// 54
  { 
    "@", 70, 70   }
  ,	// 55
  { 
    ".", 70, 70   }
  ,	// 56
  { 
    "^", 58, 59   }
  ,	// 57
  { 
    "_", 70, 70   }
  ,	// 58
  { 
    "?", 70, 70   }
  ,	// 59
  { 
    "^", 70, 61   }
  ,	// 60
  { 
    "^", 62, 70   }
  ,	// 61
  { 
    "$", 70, 70   }
  ,	// 62
  { 
    "^", 70, 64   }
  ,	// 63
  { 
    "^", 70, 65   }
  ,	// 64
  { 
    "<error>\n", 70, 70   }
  ,	// 65
  { 
    "^", 70, 67   }
  ,	// 66
  { 
    "^", 70, 68   }
  ,	// 67
  { 
    "^", 70, 69   }
  ,	// 68
  { 
    "<!SOS!>", 70, 70   }
  ,	// 69
  { 
    "^", 70, 70   }
  ,	// 70

} 
;

//- ------------------------------------------------------------------------

static char myline[17] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0' } ;
static byte cursor = 0;
static byte needScroll = 0 ;

void scrollup () {
	lcd.clear () ;
  lcd.setCursor(0,0) ;
  lcd.print(myline) ;
  lcd.setCursor(0,1) ;
  for (cursor = 0 ; cursor < 16 ; cursor++)
    myline[cursor] = ' ' ;
  cursor = 0 ;
}

void printchar (char c) {
  if (c == '\n') {
    scrollup() ;
    return ;
  } 
  else {
    if (cursor >= 16)
      scrollup () ;
    myline[cursor++] = c ;
    lcd.print(c) ;
  }
}

void printstring (char *s) {
  for (byte ix = 0 ; s[ix] != '\0' ; ix++)
    printchar (s[ix]) ;
}

//- ------------------------------------------------------------------------

void setup ()
{
  pinMode (keyPin, INPUT) ;
  pinMode (bt1Pin, INPUT) ;

  for (byte ix = 0 ; ix < 4 ; ix++) {
    pinMode (digPins[ix], INPUT) ;
  }

  pinMode (buzzerPin, OUTPUT) ;
  pinMode (ledPin, OUTPUT) ;

  delay (dah) ;

  for (byte ix = 0 ; ix < 10 ; ix++) {
    ditLengths[ix] = round ( (60.0/ (50.0 * wpmList[ix]) ) * 1000.0) ;
  }

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);

  Serial.begin(9600);
  Serial.print(Version);
  //Serial.print(readWheel()) ;
  //Serial.print("\n") ;
  printstring (Version) ;
}

//- ------------------------------------------------------------------------

void sendBit (unsigned int len)
{
  tone (buzzerPin, Hz) ;
  digitalWrite (ledPin, HIGH) ;
  delay (len) ;
  noTone (buzzerPin) ;
  digitalWrite (ledPin, LOW) ;
  delay(dit) ;
}
//- ------------------------------------------------------------------------

byte readWheel ()
{
  byte wix = 0 ;
  for (byte ix = 0 ; ix < 4 ; ix++) {
    if (digitalRead(digPins[ix]) == HIGH)	{
      wix += digPinValue[ix] ;
    }
  }
  if (wix != wheelIndex) {
    return (wix) ;
  }
  return (wheelIndex) ;
}

//- ------------------------------------------------------------------------

unsigned int ditLen ()
{
  if (readWheel() != wheelIndex) {
    delay (250); // some internal bounce in the wheel, so delay and look again
    wheelIndex = readWheel();
  }
  return (ditLengths[wheelIndex]) ;
}


//- ------------------------------------------------------------------------

void CQ ()
{

	//- check speed wheel when the CQ button is pressed
	dit = ditLen() ;
	if (prvDit != dit) {
		prvDit = dit ;
		dah = dit + dit + dit ;

		ditMax = round(float(dit)*1.75) ;
		ditMin = 5 ;
		dahLen = dit * 6 ;
		dahMax = dit * 9 ;

		Serial.print ("dit ") ; 
		Serial.print (dit) ;
		Serial.print ("\nditMax ") ; 
		Serial.print (ditMax) ; 
		Serial.print (" ratio ") ; 
		Serial.print ((float)ditMax / (float)dit) ;
		Serial.print ("\ndahLen ") ; 
		Serial.print (dahLen) ; 
		Serial.print (" ratio ") ; 
		Serial.print (dahLen / dit) ;
		Serial.print ("\ndahMax ") ; 
		Serial.print (dahMax) ; 
		Serial.print (" ratio ") ; 
		Serial.print (dahMax / dit) ;
		Serial.print ("\n") ;

	}

	printchar('\n') ;
	printstring (wpmStr[wheelIndex]) ;
	printchar('\n') ;

  //Serial.print("\nC") ;
  printstring ("C") ;
  sendBit (dah) ;
  sendBit (dit) ;
  sendBit (dah) ;
  sendBit (dit) ;

  delay (dah) ;
  //Q
  //Serial.print("Q\n") ;
  printstring("Q ") ;
  sendBit (dah) ;
  sendBit (dah) ;
  sendBit (dit) ;
  sendBit (dah) ;

}

//- ------------------------------------------------------------------------

void morseDecode () {

  if (node != 0) {
    //Serial.print (mtree[node].mchar) ;
    printstring (mtree[node].mchar) ;
  }
  node = 0 ;
}

//- ------------------------------------------------------------------------

byte timeTransition (byte wasPressed, unsigned int len) {

  //float ditFrac = (float)len / (float)dit ;

//  if (!wasPressed)
//    Serial.print("        ") ; 
//  Serial.print(len) ;
//  Serial.print("\n") ;

  if ( len  <= ditMax) {
    // Dit len
    if (wasPressed) {
      if (len < ditMin) {
        // ignore key bounce
        return (0) ;
      }
      //code += "." ;
      node = mtree[node].dit ;
      //Serial.print ("*") ;
    }
    return (1) ;

  } 
  else  {
    // Dah len
    if (wasPressed) {
      //code += "-" ;
      node = mtree[node].dah ;
      //Serial.print ("-") ;
    } 
    else if (len < dahLen) {
      morseDecode () ;
    } 
    else if (len < dahMax) {
      //Serial.print (" ") ;
      printchar (' ') ;
    } 
    else {
      morseDecode () ;
      //Serial.print ("\n") ;
      //!scrollup () ;
			needScroll = 1 ;
    }
    return (2) ;
  }
}

//- ------------------------------------------------------------------------
//- ------------------------------------------------------------------------

void loop ()
{


  byte starting = 1 ;
  unsigned int duration = 0 ;
  unsigned int now ;
  unsigned int startms ;
  unsigned int releasedAt = 0 ;

  digitalWrite (ledPin, LOW) ;

	CQ () ;

  while (1)
  {

		if (needScroll) {
			scrollup () ;
			needScroll = 0 ;
		}
		now = millis () ;

    keyState = digitalRead(keyPin);

    if (keyState != prvState) {

      if (keyState == HIGH) {
        tone (buzzerPin, Hz) ;
        digitalWrite(ledPin, HIGH) ;

        if (!starting) {
          // not start of first press
          duration = now - startms ; // duration of just-completed gap.
          timeTransition (0, duration) ;

        }

      }

      else {
        // End of a press, start of a gap
        noTone (buzzerPin) ;
        digitalWrite(ledPin, LOW) ;

        duration = now - startms ;
        timeTransition (1, duration) ;
        releasedAt = now ;

      }

      prvState = keyState ;
      starting = 0 ;
      startms = now;

			//Serial.print("\n? ") ; Serial.print(millis() - now) ; Serial.print("\n");


    } 
    else { // no transition

      //- see if we need to emit a word
      if (keyState == LOW && releasedAt > 0 &&  ((now - releasedAt) > dah*3) && node != 0) {
        morseDecode() ;
        releasedAt = now ;
				startms = now ;
      }

      //- check CQ button
      bt1State = digitalRead(bt1Pin) ;
      if ( (bt1State != bt1PrvState) && (bt1State == LOW) ) {
        CQ () ;

      }
      bt1PrvState = bt1State ;
    }
  }
}
