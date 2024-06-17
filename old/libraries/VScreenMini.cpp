#include "VScreenMini.h"

void VScreenMini::begin(int addr)
{
  if (addr == 0x3C | addr == 0x3D) {
    _display = Adafruit_SSD1306(MINI_SCREEN_WIDTH, MINI_SCREEN_HEIGHT, &Wire);
    _display.begin(SSD1306_SWITCHCAPVCC, addr); 
    _display.fillRect(0, 0, MINI_SCREEN_WIDTH, MINI_SCREEN_HEIGHT, WHITE);
    _display.display();
    delay(10);
    _display.clearDisplay();
    _display.display();
  }
}

void VScreenMini::display()
{
  _display.display();
}

void VScreenMini::clear()
{
  _display.clearDisplay();
}

void VScreenMini::pixelAt(int x, int y)
{
  _display.drawPixel(x, y, WHITE);
}

void VScreenMini::charAt(int x, int y, int code)
{
  _text(x + 5 * (3 - String(code).length()), y, String(code));
  _char(x + 20, y, code);
}

void VScreenMini::printAt(int x, int y, String text)
{
  _text(x, y, text);
}

void VScreenMini::numericBox(int x, int y, String key, float value, int unit)
{
  _display.fillRect(x, y, 27, 9, WHITE);
  _inv(x + 1, y + 1, key);
  
  String unity = String(int(value));
  int subval = int((value - int(value)) * 100);
  String decimal = String(subval);
  if (subval < 10) {
    decimal = "0" + decimal;
  }
  _title(x + 30, y, unity); 
  _title(x + 50, y, ".");
  _title(x + 60, y, decimal);
  _char(122, y, unit);
}

void VScreenMini::graphBox(int x, int y, int height, float maxValue, float minValue, float moyValue, float* buffer)
{
  // show grid
  _grid(x, y);
  _grid(x, y + height / 2);
  _grid(x, y + height - 1);
  for (int i = y; i <=  y + height; i++) {
    pixelAt(x, i);  
  }
  
  // show average
  for (int i = x + 16; i < MINI_SCREEN_WIDTH; i += 2) {
    pixelAt(i, ((1 - ((moyValue - minValue) / (maxValue - minValue))) * height) + y);  
  }

  // show min and max values
  printAt(x + 4, y, String(ceil(maxValue), 0));
  printAt(x + 4, y + height - 7, String(floor(minValue), 0));

  // show trend
  if (buffer[0] > moyValue) {
    _char(x + 6, y + height / 2 - 4, 24); 
  } else if (buffer[0] < moyValue) {
    _char(x + 6, y + height / 2 - 4, 25); 
  }

  // draw graph
  for(int i = 0; i < (MINI_SCREEN_WIDTH - x) ; i++) {
    if (buffer[i] != 0) {
      int xo = MINI_SCREEN_WIDTH - i - 1;
      int yo = ((1 - ((buffer[i] - minValue) / (maxValue - minValue))) * height) + y;
      pixelAt(xo, yo);
    }
  }
}

void VScreenMini::iconBox(int x, int y, const unsigned char* data)
{
  _display.drawBitmap(x, y, data, 32, 32, WHITE);
}

void VScreenMini::rectangleBox(int x, int y, int w, int h, bool fill)
{
  if (fill) {
    _display.fillRect(x, y, w, h, WHITE);
  } else {
    _display.drawRect(x, y, w, h, WHITE);
  }
  
}

void VScreenMini::_grid(int x, int y)
{
  pixelAt(x + 1, y);
  pixelAt(x + 2, y);
  for (int i = MINI_SCREEN_WIDTH - 1; i > x; i -= 30) {
    pixelAt(i, y);  
  }
}

void VScreenMini::_title(int x, int y, String text)
{
  _display.setTextColor(WHITE); 
  _display.setTextSize(2); 
  _display.setCursor(x, y);
  _display.print(text);
}

void VScreenMini::_text(int x, int y, String text)
{
  _display.setTextColor(WHITE); 
  _display.setTextSize(1);
  _display.setCursor(x, y);
  _display.print(text);
}

void VScreenMini::_inv(int x, int y, String text)
{
  _display.setTextColor(BLACK, WHITE); 
  _display.setTextSize(1);
  _display.setCursor(x, y);
  _display.print(text);
}

void VScreenMini::_char(int x, int y, int code)
{
  _display.setTextColor(WHITE); 
  _display.setTextSize(1);
  _display.setCursor(x, y);
  _display.write(code); 
}
