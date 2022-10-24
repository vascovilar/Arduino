#include "VComScreen.h"

void VComScreen::begin(String* menu, int size) 
{
  _menu = menu;
  _size = size;
  
  _tft.init();
  _tft.setRotation(0);

  next(0);
}

void VComScreen::next(int i) 
{  
  String page = _matchMenu(_word);

  _position += i;
  if (_position < 0) {
    _position = _countData(page)-1;
  }
  if (_position > (_countData(page)-1)) {
    _position = 0;
  }
  clear();
  _iterateData(page, _position);
}

void VComScreen::click(std::function<bool(String)> callback)
{
  String line = _getData(_matchMenu(_word), _position);
  
  if (line == SCREEN_BACK_WORD) {
    _word = "";
    _position = 0;
    next(0);
  } else if (!callback(line)) {
    _word = line;
    _position = 0;
    next(0);
  }
}

String VComScreen::_enumMenu()
{
  String out = "";

  for (int i=0; i<_size; i++) 
  { 
    String words = _menu[i];
    int index = 0;
 
    while(index != -1) {
      index = words.indexOf(" > ");
      if (index != -1) {
        out = _addData(out, words.substring(0, index));
        words = words.substring(index+3, words.length());
      } else {
        out = _addData(out, words);
      }
    }
  }

  return out;
}

String VComScreen::_matchMenu(String command)
{
  if (command == "") {
    return _enumMenu();
  }
  
  String out = SCREEN_BACK_WORD;
  
  for (int i=0; i<_size; i++) 
  { 
    String words = _menu[i];
    if (words.indexOf(command) != -1) {
      out = _addData(out, words);
    }
  }

  return out;
}

String VComScreen::_addData(String data, String command) 
{
  if (data.indexOf(command) == -1) {
    if (data != "") {
      data += "\n";
    }
    data += command;
  }

  return data;
}

int VComScreen::_countData(String data)
{
  int length = 1;
  
  for (int i = 0; i<data.length(); i++) {
    if (String(data.charAt(i)) == "\n") {
      length++;
    }
  }

  return length;
}

String VComScreen::_getData(String data, int position)
{
  int rank = 0;
  int index = 0;

  while(index != -1) {
    index = data.indexOf("\n");
    if (rank == position) {
      if (index != -1) {
        return data.substring(0, index);
      } else {
        return data;
      }      
    }
    data = data.substring(index+1, data.length());
    rank++;
  }
}

void VComScreen::_iterateData(String data, int position)
{
  int rank = 0;
  int index = 0;

  while(index != -1) {
    index = data.indexOf("\n");
    if (index != -1) {
      menuBox(data.substring(0, index), rank, rank == position);
    } else {
      menuBox(data, rank, rank == position);
    }
    data = data.substring(index+1, data.length());
    rank++;
  }
}
