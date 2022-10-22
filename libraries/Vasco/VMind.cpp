#include "VMind.h"

void VMind::analyse(int field, int status, float* values)
{
  VBuffer buffer;

  mind_graph_data info = _buildGraph(field, status, buffer.stat(values, 0, BUFFER_ARRAY_LENGTH));
  info = _addAnalysis(info, buffer.stat(values, 0, int(BUFFER_ARRAY_LENGTH / MIND_SENSITIVITY)));

  _comfort[field] = info;
}

mind_graph_data VMind::info(int field)
{
  return _comfort[field];
}

mind_graph_data VMind::_buildGraph(int field, int status, buffer_data_stat stat)
{
  mind_graph_data info;

  info.value = stat.value;
  info.maximum = stat.maximum;
  info.minimum = stat.minimum;
  info.average = stat.average;      
  info.delta = stat.delta;
  info.tolerance = _tolerance(field);
  info.top = stat.average + _tolerance(field) / 2;
  info.bottom = stat.average - _tolerance(field) / 2;  
  info.status = status;    
  info.color = _color(find(field, status));

  if (info.bottom < 0) {
    info.bottom = 0;
  }

  if (info.top < stat.maximum) {
    info.top = stat.maximum;
  } 
  
  if (info.bottom > stat.minimum) {
    info.bottom = stat.minimum;
  }

  return info;
}

mind_graph_data VMind::_addAnalysis(mind_graph_data info, buffer_data_stat slice)
{
  info.comment += "x" + String(info.delta / info.tolerance, 1) + "(" + String(int(info.tolerance)) + ") "; 
  info.background = "#grey";
  
  if (slice.average < info.average) { info.comment += "down "; };
  if (slice.average > info.average) { info.comment += "up "; };
  if (slice.delta < info.delta) { info.comment += "stab "; };
  if (slice.delta > info.delta) { info.comment += "ampl "; };

  if (abs(slice.value - info.average) > info.tolerance / 2) { 
    info.comment += "pulse "; 
    info.background = "#red";

    /*
    if (slice.average > info.average) {
      // TODO VGearSound::beep
      ledcWriteTone(0, 2000);
      delay(10);
      ledcWriteTone(0, 0);      
    }
    */
  }

  if (abs(slice.value - info.average) > info.tolerance) {
    info.comment += "hot ";
  }

  return info;
}
