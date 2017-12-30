# AsyncWebserver1.ino
Aszinkron webszerver minta.

[ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

[Offline HTML oldalak használata](https://github.com/SanyiSay/ESP8266/tree/master/AsyncWebserver#htm-f%C3%A1jlok-tesztel%C3%A9se)

[ESP AsyncFSBrowser ](https://github.com/SanyiSay/ESP8266/blob/master/AsyncWebserver/README.md#esp-asyncfsbrowser)  (egy könnyed html alapú fájlkezelő)

[ESP fájlrendszeréből is használja a webszerverünk a feltöltött oldalakat) ](https://github.com/SanyiSay/ESP8266/blob/master/AsyncWebserver/README.md#esp-asyncfsbrowser)




### Offline htm fájlok tesztelése 

[SanyiSay HTML](https://github.com/SanyiSay/ESP8266/tree/master/HTML)

Következő két kódrészlet a heap méretét adja vissza, ha a html oldal esp-n van tárolva onnan fut (online) akkor mindkét példa működik és megkapja a böngésző (kliens) a heap méretét. Viszont ha a HTML oldal (offline) akkor csak a második a ( /_heap ) oldal meghívására kapjuk meg a heap méretét mert ahhoz van hozzácsatolva a (Access-Control-Allow-Origin:*) fejléc.

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", String(ESP.getFreeHeap()));
	});


    server.on("/_heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/html",String(ESP.getFreeHeap()));
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);

	});


Ha chrome ban használjuk a Ctrl+Shift+i  billentyű kombinációt akkor a megnyíló fejlesztői ablakban láthatjuk hol volt a hiba.

### sikeres adatfogadáskor ezt látjuk

Response Headers résznél látjuk az esp felől érkező adatokat és szépen ott van az (Access-Control-Allow-Origin:*) és a válasz is megérkezett az oldalra. 

![Sikeres adatfogadás](https://github.com/SanyiSay/ESP8266/blob/master/DOC/origin_ok.JPG?raw=true)

### sikertelen adatfogadáskor ezt látjuk

Hibás fejlécnél viszont hiányzik és szép pirossal figyelmeztet is minket a problémára, és természetesen így az adat sem jött meg az esp-től.

![Sikeres adatfogadás](https://github.com/SanyiSay/ESP8266/blob/master/DOC/origin_hiba.JPG?raw=true)

----------
----------

# ESP AsyncFSBrowser

**Meghívása:**
 /edit  esp oldalai között az edit oldalon találjuk.
 
Sokat nem lehet róla írni az oldal magáért beszél. 
Fájl fel letöltés az esp SPIFFS fájlrendszerébe.


Ami talán ehhez a részhez tartozik:

**ESP SPIFFS fájlrendszerén tárolt html fájlokat is használja a webszerverünk**

Webszerveren beállítjuk, hogy keressen az esp fájlrendszerén is oldalakat így elég csak ide feltenni a html (és egyéb js, css, stb) fájljainkat és már használhatjuk is. 

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

Ezt ezzel a sorral tehetjük meg. Itt még azt állítjuk be hogy ha az esp ip címét írjuk csak a böngészőbe akkor melyik fájl legyen az amit alapértelmezetten használ. (setDefaultFile("index.html"))

Ezek után már könnyedén fel tudjuk tenni az offline megszerkesztett oldalainkat az esp-re.




 


