#include "VComScreen.h"

VComScreen screen;

int len = 10;
String menu[] = {
  "Menu>LCD>Off",           //0
  "Menu>LED>Noop",          //1
  "Menu>LED>Off",           //2
  "Menu>LED>On",            //3
  "Menu>LED>Fade",          //4
  "Menu>LED>Blink",         //5
  "Menu>LCD>light",         //6
  "Menu>Nothing1",          //7  
  "Menu>Nothing2",          //8
  "Menu>Nothing3"           //9
};

String listWords()
{
  String out = "";

  for (int i=0; i<len; i++) 
  { 
    String words = menu[i];
    int index;
 
    do {
      index = words.indexOf(">");
      if (index != -1) {
        out = addResult(out, words.substring(0, index));
        words = words.substring(index+1, words.length());
      } else {
        out = addResult(out, words);
      }
    }
    while(index != -1);
  }

  return out;
}

String matchWord(String command)
{
  String out = "";

  for (int i=0; i<len; i++) 
  { 
    String words = menu[i];
    if (words.indexOf(command) != -1) {
      out = addResult(out, words);
    }
  }

  return out;
}

String addResult(String out, String word) 
{
  if (out.indexOf(word) == -1) {
    if (out != "") {
      out += "\n";
    }
    out += word;
  }

  return out;
}

int countResults(String out)
{
  int length = 1;
  for (int i = 0; i<out.length(); i++) {
    if (String(out.charAt(i)) == "\n") {
      length++;
    }
  }

  return length;
}

void execResults(String out, std::function<void(String, bool)> callback, int select)
{
  int index;
  int length = 0;
  do {
    index = out.indexOf("\n");
    if (index != -1) {
      callback(out.substring(0, index), length == select);
      out = out.substring(index+1, out.length());
    } else {
      callback(out, length == select);
    }
    length++;
  } 
  while(index != -1);
}

void doIt(String line, bool selected)
{
  if (selected) {
    Serial.println("<" + line + ">");
  } else {
    Serial.println(line);
  }
}


// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  /*screen.begin();
  screen.title("Menu", 0, 0);
  screen.text("Vous trouverez ci-dessous le menu", 0, 20);
  screen.menu();*/


  Serial.println(matchWord("Off"));  
  Serial.println(countResults(matchWord("Off")));
  execResults(listWords(), doIt, 1);
  
/*    
  int i;
  String focus = "Off";

  for (i=0; i<len; i++) 
  { 
    String words = menu[i];

    int index = 0;
    int level = 0;
    bool find = false;

    while(index != -1) 
    {
      String word;
      index = words.indexOf(">");
      if (index == -1) {
        word = words;
      } else {
        word = words.substring(0, index);
        words = words.substring(index+1, words.length());
      }
      if (find == true) {
        Serial.println(String(i) + ":" + String(level) + " " + word);
        break;
      }

      if (word == focus) {
        find = true;
      }
      
      level++;
    }
  }
*/

}

// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------
void loop()
{
   
}