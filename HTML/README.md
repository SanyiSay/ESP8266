Offline módon megnyitott html oldal nem minden esetben kapja meg az esp válaszát. ESP webszerverének a válasz fejlécébe be kell illeszteni a ("Access-Control-Allow-Origin", "*") sort. Biztonsági okokból letiltja a választ a böngésző. pld  chrome.   Erre példa és az online/offline használatra az [AsyncWebserver1](https://github.com/SanyiSay/ESP8266/tree/master/AsyncWebserver) mintánában található.

# offline_ajax.html
Egyszerű offline html oldal.  ESP felé küld AJAX-al adatot, és a választ kiírja az oldalra.
>**Offline módban az esp elérési útvonalának formája:**
>
>var ServerURL = "http://192.168.x.xx/oldal?adat=1&dadat2=2&adat3=3"

----------

# alap_on_off_ajax.html
Ez a fájl futhat esp-n tárolva (online) vagy a kliensen (pc, mobil, stb) tárolva (offline). Felismeri  honnan indították el. 

----------
