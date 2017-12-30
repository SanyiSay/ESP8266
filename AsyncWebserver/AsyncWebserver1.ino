
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

AsyncWebServer server(80); 


#define ssid "*****"
#define password "********"

// reboot kapcsoló HTTP OTA után
bool shouldReboot =false;

void setup() {
	// serial port on
	Serial.begin(115200);
	Serial.println("\nSerial Debug ON");

	// Wifi SAT ON
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		WiFi.disconnect(false);
		delay(500);
		WiFi.begin(ssid, password);
	}

	Serial.print("\n--- \n\n");
	Serial.print("\nServer IP -> ");
	Serial.print(WiFi.localIP());
	Serial.print("\n--- \n\n");

	// fájlrendszer ON
	SPIFFS.begin();


	//***************************************************
	// FS Editor ON
	//***************************************************
	server.addHandler(new SPIFFSEditor());

	
	//***************************************************
	// heap kiírása
	// Access-Control-Allow-Origin", "*"    nélkül
	//***************************************************
	server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});

	
	
	//***************************************************
	// heap kiírása
	//***************************************************
	server.on("/_heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/html",String(ESP.getFreeHeap()));
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);

	});

	
	
	//***************************************************
	// Fájlrendszerből is keressen html oldalakat
	// kezdőoldal beállítása
	//***************************************************
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");


	
	//***************************************************
	// Simple Firmware Update Form
	//***************************************************
	server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update' ><input type='submit' value='Update' ></form>");
	});

	server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
		shouldReboot = !Update.hasError();
		AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot?"OK":"FAIL");
		response->addHeader("Connection", "close");
		request->send(response);
	}, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
		if(!index) {
			//Serial.printf("Update Start: %s\n", filename.c_str());
			Update.runAsync(true);
			if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)) {
				//Update.printError(Serial);
			}
		}
		if(!Update.hasError()) {
			if(Update.write(data, len) != len) {
				//Update.printError(Serial);
			}
		}
		if(final) {
			if(Update.end(true)) {
				shouldReboot=true;
				//Serial.printf("Update Success: %uB\n", index+len);
			} else {
				//Update.printError(Serial);
			}
		}
	});

	//***************************************************
	// hiányzó oldal alálat lekezelése
	//***************************************************
	server.onNotFound([](AsyncWebServerRequest *request) {
		Serial.printf("NOT_FOUND: ");
		if(request->method() == HTTP_GET)
		Serial.printf("GET");
		else if(request->method() == HTTP_POST)
		Serial.printf("POST");
		else if(request->method() == HTTP_DELETE)
		Serial.printf("DELETE");
		else if(request->method() == HTTP_PUT)
		Serial.printf("PUT");
		else if(request->method() == HTTP_PATCH)
		Serial.printf("PATCH");
		else if(request->method() == HTTP_HEAD)
		Serial.printf("HEAD");
		else if(request->method() == HTTP_OPTIONS)
		Serial.printf("OPTIONS");
		else
		Serial.printf("UNKNOWN");
		Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

		if(request->contentLength()) {
			Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
			Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
		}

		int headers = request->headers();
		int i;
		for(i=0;i<headers;i++) {
			AsyncWebHeader* h = request->getHeader(i);
			Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
		}

		int params = request->params();
		for(i=0;i<params;i++) {
			AsyncWebParameter* p = request->getParam(i);
			if(p->isFile()) {
				Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
			} else if(p->isPost()) {
				Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
			} else {
				Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}
		request->send(404);
	});

	//***************************************************
	// server ON
	//***************************************************
	server.begin();
}

void loop() {
	//***************************************************
	// reboot kapcsoló ellenőrzése
	//***************************************************
	if (shouldReboot == true) {
		delay(100);
		ESP.restart();
	}
}
