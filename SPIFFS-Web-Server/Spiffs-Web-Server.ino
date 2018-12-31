
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

const char *ssid = "Hall-Wifi";
const char *password = "crispytrail748";

ESP8266WebServer server(80);


void setup() {

Serial.begin(115200); 

if (!setupWifi()){
  Serial.println("Wifi Setup Failed.");
}

if (!SPIFFS.begin()){
  Serial.println("SPIFFS Startup Failed.");
}

  server.onNotFound([]() {
    if (!fileRead(server.uri())) {
      server.send(404, "text/plain", "File not found!");
    }
  });

  server.begin();

  Serial.println("Server Started");

}

void loop(){
server.handleClient();
}

bool setupWifi(){

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (MDNS.begin("esp8266")) { //Setup MDNS Responder
    Serial.println("MDNS responder started");
  }
  else{
    return false;
  }

return true;
}


String getFileType(String filename){

  if(filename.endsWith(".htm")) return "text/html";

  else if(filename.endsWith(".html")) return "text/html";

  else if(filename.endsWith(".css")) return "text/css";

  else if(filename.endsWith(".js")) return "application/javascript";

  else if(filename.endsWith(".ico")) return "image/x-icon";

  else if(filename.endsWith(".xml")) return "text/xml";

  else if(filename.endsWith(".jpg")) return "image/jpeg";

  else if(filename.endsWith(".png")) return "image/png";

  else if(filename.endsWith(".gif")) return "image/gif";

  else if(filename.endsWith(".pdf")) return "application/x-pdf";

  else if(filename.endsWith(".zip")) return "application/x-zip";

  else if(filename.endsWith(".gz")) return "application/x-gzip";

  return "text/plain";


}


bool fileRead(String filePath){
if(filePath.endsWith("/")) filePath += "index.html";
String fileType = getFileType(filePath);

String fileGZ = filePath + ".gz";

  if(SPIFFS.exists(filePath) || SPIFFS.exists(fileGZ)){ //Checks if it exists or a compressed version exists
    Serial.print("File Found:");
    Serial.println(filePath);   // If the file doesn't exist, return false

    if(SPIFFS.exists(fileGZ)){
      filePath = fileGZ; //If a compressed version exists set filepath to that.
    }
    File getFile = SPIFFS.open(filePath, "r"); // Open it in the file system
    size_t sendFile = server.streamFile(getFile, fileType); //Send it back to the client
    getFile.close();//Close file
    return true;
  }
  else{
      Serial.print("File not Found:");
      Serial.println(filePath);   // If the file doesn't exist, return false
      return false;

  }

}
