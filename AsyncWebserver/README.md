# AsyncWebserver1.ino
Aszinkron webszerver minta.



[TOC]



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


Ha chrome ban használjuk a Ctrl+Shift+i  billentyű kombinációt akkor a megnyíló fejlesztői ablakban láthatjuk hol volt a hiba.

sikeres adatfogadáskor ezt látjuk
---------------------------------
Response Headers résznél látjuk az esp felől érkező adatokat és szépen ott van az (Access-Control-Allow-Origin:*) és a válasz is megérkezett az oldalra. 

![Sikeres adatfogadás](https://github.com/SanyiSay/ESP8266/blob/master/DOC/origin_ok.JPG?raw=true)

sikertelen adatfogadáskor ezt látjuk
--------------------------------- 
Hibás fejlécnél viszont hiányzik és szép pirossal figyelmeztet is minket a problémára, és természetesen így az adat sem jött meg az esp-től.

![Sikeres adatfogadás](https://github.com/SanyiSay/ESP8266/blob/master/DOC/origin_hiba.JPG?raw=true)
