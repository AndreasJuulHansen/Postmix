/**************************************************************************

PINOUT

DISPLAY            ESP32
GND      -->       GND
VCC      -->       5V
SCL      -->       GPIO18 (Clock)
SDA      -->       GPIO23
RES      -->       EN (Screen reset - custom)
DC       -->       GPIO2 (Register select)
CS       -->       GPIO5 (Slave select)
BLK      -->       5V

SD KORT            ESP32
GND      -->       GND
VCC      -->       5V
MISO     -->       330 Ohm resistor --> GPIO19
MOSI     -->       GPIO23
SCK      -->       GPIO18 (Clock)
CS       -->       GPIO15 (Slave select)

*************************************************************************/

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

// List with all the flavours and their filenames on the SD card
std::string typeList[] = {
    "/cocacola.bmp",
    "/fantafruittwist.bmp",
    "/fantaorange.bmp",
    "/faxe.bmp",
    "/pepsi.bmp",
    "/pepsimax.bmp",
    "/schweppes.bmp",
    "/schweppeslemon.bmp",
    "/sprite.bmp",
    // "/fanta.bmp",
    // "/fantaexotic.bmp",
    "/fantagrape.bmp",
    "/fantagreenapple.bmp",
    "/fantalemon.bmp",
    "/fantapineapple.bmp",
    "/fantastrawberry.bmp",
    "/layout.bmp",
    "/penis.bmp",
    "/penis2.bmp"};

std::string myArray[3][3] = {
    {"display0", "display1", "display2"},
    {"display3", "display4", "display5"},
    {"display6", "display7", "display8"},
};

// std::string getName(int index) {
//     return names[index];
// }

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.

// SD Card pin
#define SD_CS 15 // SD card chip select

SdFat SD;                        // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys

// Pins for the displays - the ones they share
#define TFT_RST -1 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 2

// Initialises the display and calls it displayX
// All the possible display and their custom pin
Adafruit_ST7735 display0 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display1 = Adafruit_ST7735(21, TFT_DC, TFT_RST); // Virtual display
Adafruit_ST7735 display2 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display3 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display4 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display5 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display6 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display7 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display
Adafruit_ST7735 display8 = Adafruit_ST7735(5, TFT_DC, TFT_RST);  // Virtual display

// Creates an array to call them, so it can be variable in functions. The array has da default setup of 3x3
Adafruit_ST7735 *displays[3][3] = {
    {&display0, &display1, &display2},
    {&display3, &display4, &display5},
    {&display6, &display7, &display8}};

void displaySetup()
{

    // Initializes all the displays for the wors and culoms
    for (int row = 0; row <= 0; row++)
    {
        for (int col = 0; col <= 1; col++)
        {
            // Prints what display we are initialising
            Serial.print("Initialising display [");
            Serial.print(row);
            Serial.print(",");
            Serial.print(col);
            Serial.println("]");

            // The actual setup
            displays[row][col]->initR(INITR_MINI160x80_PLUGIN); // Init ST7735S mini display
            displays[row][col]->setRotation(1);                 // Sets rotation/orienation of the display
            displays[row][col]->fillScreen(ST77XX_BLACK);       // Confirmation that
            // displays[row][col]->fillScreen(ST77XX_BLUE);
            // delay(3000);
        }
    }

    Serial.println(F("Display Initialized"));
}

void sdCardSetup()
{
    // SD KORT SETUP
    Serial.print(F("Initializing filesystem..."));

    // We start the SD card
    if (!SD.begin(SD_CS, SD_SCK_MHZ(10)))
    { // Breakouts require 10 MHz limit due to longer wires
        Serial.println(F("SD begin() failed"));

        displays[0][0]->setCursor(0, 0);
        displays[0][0]->setTextColor(ST77XX_RED);
        displays[0][0]->setTextSize(2);
        displays[0][0]->println("SD Card error!");

        for (;;)
            ; // Fatal error, do not continue
    }
    Serial.println(F("Filesystem OK!"));
}

int currentPicture[3][3] = {
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1}};

void loadType(int row, int col, int type)
{
    ImageReturnCode stat; // Status from image-reading functions
    // displays[row][col]->fillScreen(ST77XX_BLACK); // Makes screen black

    if (currentPicture[col][row] != type)
    {
        // print new value
        // Serial.print("Changing value: ");
        // Serial.print(currentPicture[col][row]);
        // Serial.print(" to ");
        // Serial.println(type);
        // Serial.printf("Changing picture value: %d to %d \n", currentPicture[col][row], type);

        // update previous value
        currentPicture[col][row] = type;

        if (type == -1)
        {
            displays[row][col]->fillScreen(ST77XX_BLACK); // Confirmation that
        }
        else
        {
            // Serial.print("Reading picture ");
            // Serial.println(typeList[type].c_str());
            stat = reader.drawBMP(typeList[type].c_str(), *displays[row][col], 0, 0); // prints picture
            // reader.printStatus(stat);                                                 // How'd we do?
        }
    }
}

/* const int MAX_IMAGES = 10; // Maximum number of images
int row[MAX_IMAGES];
int col[MAX_IMAGES];
int type[MAX_IMAGES];

void loadType(int index)
{
    ImageReturnCode stat; // Status from image-reading functions

    if (row[index] != ::row || col[index] != ::col || type[index] != ::type) // Check if there are changes
    {
        Serial.print("Reading picture ");
        Serial.println(typeList[type[index]].c_str());
        stat = reader.drawBMP(typeList[type[index]].c_str(), *displays[row[index]][col[index]], 0, 0); // prints picture
        reader.printStatus(stat);                                                                         // How'd we do?

        ::row = row[index]; // Update global variables
        ::col = col[index];
        ::type = type[index];
    }
} */

int currentPercentage[3][3] = {
    {-1, -1, -1},
    {-1, -1, -1},
    {-1, -1, -1}};

void drawLiquid(int row, int col, float percentage)
{
    // if (percentage == 1)
    // {
    //     // Loads status bar
    //     displays[row][col]->fillRect(0, 64, displays[row][col]->width(), 16, ST7735_WHITE);     // Hvid baggrund på progress bar
    //     displays[row][col]->fillRect(1, 65, (displays[row][col]->width() - 2), 14, ST7735_RED); // Rød bar
    // }
    // int numLedsToLight = map(waterFlow, 0, 1000, 100, 0);
    // displays[row][col]->fillRect(displays[row][col]->width() * numLedsToLight / 100, 65, displays[row][col]->width() - displays[row][col]->width() * numLedsToLight / 100, 14, ST7735_WHITE);
    // displays[row][col]->fillRect(displays[row][col]->width() * percentage / 100, 65, displays[row][col]->width() - displays[row][col]->width() * percentage / 100, 14, ST7735_RED);

    if (currentPercentage[col][row] != percentage)
    {
        // print new value
        // Serial.print("Changing value: ");
        // Serial.print(currentPicture[col][row]);
        // Serial.print(" to ");
        // Serial.println(type);
        // Serial.printf("Changing percentage value: %d to %d \n", currentPercentage[col][row], percentage);

        // update previous value
        currentPercentage[col][row] = percentage;
        displays[row][col]->drawRect(0, 64, displays[row][col]->width(), 16, ST7735_WHITE);                                                                                                       // White box around the status bar
        displays[row][col]->fillRect(1, 65, displays[row][col]->width() - (displays[row][col]->width() - 1 - displays[row][col]->width() * percentage / 100), 14, ST7735_RED);                    // Red part of status bar
        displays[row][col]->fillRect(1 + displays[row][col]->width() * percentage / 100, 65, displays[row][col]->width() - displays[row][col]->width() * percentage / 100 - 2, 14, ST7735_WHITE); // White part of status bar
    }
}

/* void loop()
{
    // displays[0][1]->fillScreen(ST77XX_BLACK);
    Serial.print("Reading picture ");
    Serial.println(typeList[10].c_str());
    reader.drawBMP(typeList[10].c_str(), display1, 0, 0);



    for (int x = 0; x < 18; x++)
    {
        loadType(0,0,x);
        loadType(0,1,x + 1);
        // ImageReturnCode stat; // Status from image-reading functions
        // // displays[0][0]->fillScreen(ST77XX_BLACK);
        // Serial.print("Reading picture ");
        // Serial.println(typeList[x].c_str());
        // stat = reader.drawBMP(typeList[x].c_str(), display0, 0, 0);
        // reader.printStatus(stat); // How'd we do?

        // displays[0][0]->fillRect(0, 64, displays[0][0]->width(), 16, ST7735_WHITE);     // Hvid baggrund på progress bar
        // displays[0][0]->fillRect(1, 65, (displays[0][0]->width() - 2), 14, ST7735_RED); // Rød bar

        for (float y = 100; y >= 0; y -= 0.25)
        {
            drawLiquid(0, 0, y);
            drawLiquid(0, 1, y);
            delay(20);
        }
    }
}
 */