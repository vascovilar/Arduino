#include "VComWifi.h"
#include "VComWebApi.h"
#include "VGearSdCard.h"
#include "VSensorCam.h"

VComWifi net;
VComWebApi api;
VGearSdCard card;
VSensorCam cam;

// Chemin des fichiers
String getPath(int id)
{
  return "/" + String(id) + ".jpg";
}
// id de fichier
int getId(String path)
{
  String sub = path.substring(path.lastIndexOf("/") + 1, path.lastIndexOf("."));
  return sub.toInt();
}

// Affiche le contenu de la carte SD
String handleHome() 
{
  Page list = card.list("/");

  String output = "\
<style>\
  #roll{background:#222222;border-radius:20px;padding:5px 20px 5px 20px;margin-bottom:10px;overflow:hidden;height:155px;}\
  #photo{width:100%;}\
</style>\
<script>\
  function shoot() {\
    call('/capture');\
  }\
  function del(id) {\
    call('/' + id + '/delete');\
  }\
  function show(id) {\
    call('/' + id + '/show', 'photo');\
  }\
</script>\
<h1><span class='material-icons'>perm_media</span> ESP32-CAM</h1>\
<div class='action'>\
  <div class='material-icons'>add_a_photo</div>\
  <div class='button' onclick='shoot();reload(this);'>Prendre une photo</div>\
</div>\
<h2>Dernières photos</h2>\
<p>Vous trouverez ici les dernières photos disponibles sur la carte SD (total " + String(list.total) + " photos)</p>\
<div id='roll'>";

  for (int i = 0; i < list.length; i++) {
    if (list.data[i] != "") {
      output += "\
      <div class='tile' onclick='show(" + String(getId(list.data[i])) + ")'>\
        <img src='" + list.data[i] + "' width='160' height='120'><br>\
        " + list.data[i] + "\
      </div>";
    }
  }
  
  output += "\
</div>\
<div id='photo'></div>";

  return output;
}

// Affiche l'image
String handleShow(int id) 
{
  return "\
<img src='" + getPath(id) + "' width='100%' style='border-radius:20px;'>\
<div class='action'>\
  <div class='button' onclick='del(" + String(id) + ");reload(this);'>Supprimer</div>\
</div>\
<p>Prise le " + card.date(getPath(id)) + "</p>";
}

// Affiche le binaire JPG
File handleBinary(int id) 
{
  return card.open(getPath(id));
}

// Prise photo et création du fichier jpeg
void handleCapture(int id) 
{
  camera_fb_t * fb = cam.open(false);
  card.save(getPath(id), fb->buf, fb->len);
  cam.close(fb);
}

// Suppression d'un fichier
void handleDelete(int id) 
{
  card.remove(getPath(id));
}


void setup(void) 
{
  Serial.begin(115200);

  // Cam
  cam.begin();

  // SD card memory
  card.begin();

  // wifi network
  net.begin();

  // web server
  api.onPage("/", []() { return handleHome(); });
  api.onXhr("/{}/show", [](int id) { return handleShow(id); });
  api.onCommand("/{}/delete", [](int id) { handleDelete(id); });
  api.onJpg("/{}.jpg", [](int id) { return handleBinary(id); });
  api.onCommand("/capture", []() { handleCapture(net.getTimeStamp()); });
  api.begin();
}
 
void loop(void) {
  api.update(10);
}
