# AsyncWebserver1.ino
Aszinkron webszerver minta.

htm fájlok tesztelése 
-----------------------------

[SanyiSay HTML](https://github.com/SanyiSay/ESP8266/tree/master/HTML)

Következő két kód a heap méretét adja vissza, ha a html oldal esp-n van tárolva onnan fut (online) akkor mindkét példa működik és megkapja a böngésző (kliens) a heap méretét. Viszont ha a HTML oldal (offline) akkor csak a második a ( /_heap ) oldal meghívására kapjuk meg a heap méretét mert ahhoz van hozzácsatolva a (Access-Control-Allow-Origin:*) fejléc.

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});


    server.on("/_heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/html",String(ESP.getFreeHeap()));
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);

	});


![Sikeres adatfogadás](https://raw.githubusercontent.com/SanyiSay/ESP8266/master/AsyncWebserver/ok.JPG)
