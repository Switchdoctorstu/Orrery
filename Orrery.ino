// Stuart's Orrery controller
// Plots the planets on a series of rings constructed of WS2812 LEDs
/*
Unix time (also known as POSIX time or UNIX Epoch time]) 
is a system for describing a point in time, 
defined as the number of seconds that have elapsed since 00:00:00, Thursday, 1 January 1970, 
minus the number of leap seconds that have taken place since then.





*/





// #include <DS1307RTC.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Time.h>
#include <TimeLib.h>
#include <math.h>
// #define PI          3.14159265358979323846
#define RADEG       (180.0/PI)
#define DEGRAD      (PI/180.0)
#define sind(x)     sin((x)*DEGRAD)
#define cosd(x)     cos((x)*DEGRAD)
#define tand(x)     tan((x)*DEGRAD)
#define asind(x)    (RADEG*asin(x))
#define acosd(x)    (RADEG*acos(x))
#define atand(x)    (RADEG*atan(x))
#define atan2d(y,x) (RADEG*atan2((y),(x)))

struct Planet {
	double period;
	double orbitStart;
	int startPixel;
	int endPixel;
	int p;   			// current pixel
	int x;
	int y;
	int z;
	double RA;
	double Decl;
	double r;
	double degrees;
};

typedef struct  { 
  uint8_t Second; 
  uint8_t Minute; 
  uint8_t Hour; 
  uint8_t Wday;   // day of week, sunday is day 1
  uint8_t Day;
  uint8_t Month; 
  uint8_t Year;   // offset from 1970
  double epoch_count;  // count of days since/to epoch
} 	time_elements;

double nowTime;
double targetEpoch;  // target day expressed as +/- from 1/1/1970
double displayTime;
time_elements target_date; // storage for gui target date
Planet solarSystem[9]; // define the solar system

#include <FastLED.h>
#define LED_PIN     6
// Information about the LED strip itself
#define NUM_LEDS    367
#define CHIPSET     WS2811
#define COLOR_ORDER GRB
// setup display buffer
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  128

boolean targetSet = false;

void setup(){
	setupTime();
	delay( 2000 ); // power-up safety delay
	// It's important to set the color correction for your LED strip here,
	// so that colors can be more accurately rendered through the 'temperature' profiles
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
	FastLED.setBrightness( BRIGHTNESS );
	introduction(); // exercise the screen
	setupData(); // load the planets
}

// setup our time and date
void setupTime(){
	nowTime=0;
	displayTime=0;
	target_date.Year=1967
	target_date.Month=5;
	target_date.Day=16;
}

// splash something on the display to show we're alive
void introduction() {
	// draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);
  FastLED.show();
  delay(500);
  FASTLED.clear();
}

// MAIN LOOOP
void loop() {
	double d;
	time_t t = 0;
	if (targetSet) {
		t = getTargetDate(); //get the date that we've been asked for
	}
	else {
		t = getClock(); // get clock now
	}
	setPlanets(t); // project the planets in their places
	displayPlanets(); // project planets to display

}

double get_epoch(time_elements te){
	double seconds=0;
	 
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= te.Year*(SECS_PER_DAY * 365);
  for (i = 0; i < te.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < te.Month; i++) {
    if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (te.Day-1) * SECS_PER_DAY;
  seconds+= te.Hour * SECS_PER_HOUR;
  seconds+= te.Minute * SECS_PER_MIN;
  seconds+= te.Second;
	return seconds;
}

time_t getTargetDate() {
	//get the date that we've been asked for
	time_t t = 0;
	
	return t;
}

time_t getClock() {
	time_t t = 0; 
	t =now();
	
	return t;
}



void setPlanets(time_t t) { 
// project the planets in their places
// sets solarsystem[].p (pixel) to projected value
	/*
	// get epoch seconds from time
	double d = 367 * year(t);
	d = d - (7 * (year(t) + ((month(t) + 9) / 12))) / 4;
	d=d+(275 * month(t)) / 9;
	d = d + day(t) - 730530;
	*/
	
	
	for (int p = 0; p < 9; p++) {   // loop though the planets
	
	// delta = target_date - epoch_date
	
	// remainder = delta mod orbit_period
	
	// move = remainder / orbit_period
	
	// movepels = move x (end_pixel - start_pixel)
	
	// pel = startPixel + movepels
	
	
	}
}


void displayPlanets() {
	// project planets to display

	FASTLED.clear();
	for (int p = 0; p < 9; p++) {   // loop though the planets
		leds[solarSystem[p].p]=0xFF007F;
	}
	FastLED.show();
}


int days_in_month(int month, int year) {
        boolean leap = is_leap_year(year);
        /*               J   F   M   A   M   J   J   A   S   O   N   D */
        int days[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                           {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
        if (month < 0 || month > 11 || year < 1753)
                return -1;
		if (leap){
			return days[1][month];
			} else { 
			return days[0][month];
			}
}

boolean is_leap_year(int year) {
        if (year % 400 == 0) {
                return true;
        } else if (year % 100 == 0) {
                return false;
        } else if (year % 4 == 0) {
                return true;
        } else {
                return false;
        }
}

// Setup the main database for the program
void setupData() {
/*
orbitStart[0] = 361;	orbitStop[0]= 367;
orbitStart[1]=0;		orbitStop[1]=7;
orbitStart[2]=8;		orbitStop[2]=19;
orbitStart[3]=20;		orbitStop[3]=35;
orbitStart[4]=36;		orbitStop[4]=59;
orbitStart[5]=119;		orbitStop[5]=60;
orbitStart[6]=120;		orbitStop[6]=172;
orbitStart[7]=173;		orbitStop[7]=249;
orbitStart[8]=250;		orbitStop[8]=360;
*/
	// setup solar system
	solarSystem[0].period = 0;  // setup Sun
	solarSystem[0].orbitStart = 0;  // 
	solarSystem[0].startPixel = 361;  // setup 
	solarSystem[0].endPixel = 367;  // setup 

	solarSystem[1].period = 0.24084204;  // setup Mercury
	solarSystem[1].orbitStart = 0;  // 
	solarSystem[1].startPixel = 0;  // setup 
	solarSystem[1].endPixel = 7;  // setup 

	solarSystem[2].period = 0.61517237;  // setup Venus
	solarSystem[2].orbitStart = 0;  // 
	solarSystem[2].startPixel = 8;  // setup 
	solarSystem[2].endPixel = 19;  // setup 

	solarSystem[3].period = 1.0000007;  // setup Earth
	solarSystem[3].orbitStart = 0;  // 
	solarSystem[3].startPixel = 20;  // setup 
	solarSystem[3].endPixel = 35;  // setup 

	solarSystem[4].period = 1.8808524;  // setup Mars
	solarSystem[4].orbitStart = 0;  // 
	solarSystem[4].startPixel = 36;  // setup 
	solarSystem[4].endPixel = 59;  // setup 

	solarSystem[5].period = 11.86631421;  // setup Jupiter
	solarSystem[5].orbitStart = 0;  // 
	solarSystem[5].startPixel = 119;  // setup 
	solarSystem[5].endPixel = 60;  // setup 

	solarSystem[6].period = 29.47305083;  // setup Saturn
	solarSystem[6].orbitStart = 0;  // 
	solarSystem[6].startPixel = 120;  // setup 
	solarSystem[6].endPixel = 172;  // setup 

	solarSystem[7].period = 84.05122725;  // setup Uranus Year
	solarSystem[7].orbitStart = 0;  // 
	solarSystem[7].startPixel = 173;  // setup 
	solarSystem[7].endPixel = 249;  // setup 

	solarSystem[8].period = 164.88839750;  // setup Neptune Year
	solarSystem[8].orbitStart = 0;  // 
	solarSystem[8].startPixel = 250;  // setup 
	solarSystem[8].endPixel = 360;  // setup 
}

double rev(double x)
{
	return  x - floor(x / 360.0)*360.0;
}

double cbrt(double x)
{
	if (x > 0.0) {
		double y = log(x);
		y = exp(y);
			y= y/ 3.0;
		return y;
	}
	else if (x < 0.0)
		return -cbrt(-x);
	else /* x == 0.0 */
		return 0.0;
}
