#include "Container.h"

bool Container::begin(vrun mode)
{

}

bool Container::run()
{

}

void Container::add(Device &device)
{
  _index = device.getDeviceCode();
  _instance[_index] = &device;
  _sequencer[_index] = new Sequencer(device);
}

int Container::go1()
{
  return (int) _instance[_index]->getDeviceCode();
}

bool Container::go2()
{
  return _sequencer[_index]->begin(LOW_REFRESH);
}

void Container::go3()
{
  Serial.println("device list:");
  for (int i = 0; i < VDEVICE_COUNT; i++) {
    if(_instance[i]) {
      Serial.println("- " + String(i) + " OK");  
    } 
  }
}


/* device.beep();
  Serial.println("beep ?");
}*/ 

