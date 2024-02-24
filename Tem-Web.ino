#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid = "ERIS8269"; // Cambia esto por el nombre de tu red WiFi
const char* password = "rover2024"; // Cambia esto por la contraseña de tu red WiFi

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

void handleRoot() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String page = "<html><head><meta http-equiv=\"refresh\" content=\"2\"></head><body>"; // Actualiza cada 5 segundos
  page += "<h1>Lecturas del sensor DHT11</h1>";
  page += "<p id=\"humidity\">Humedad: ";
  page += humidity;
  page += "%</p>";
  page += "<p id=\"temperature\">Temperatura: ";
  page += temperature;
  page += "°C</p>";
  page += "<script>function getData() {var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {var data = JSON.parse(this.responseText);document.getElementById('humidity').innerHTML = 'Humedad: ' + data.humidity + '%';document.getElementById('temperature').innerHTML = 'Temperatura: ' + data.temperature + '°C';}};xhttp.open('GET', '/data', true);xhttp.send();}setInterval(getData, 5000);</script>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String jsonData = "{\"humidity\": " + String(humidity) + ", \"temperature\": " + String(temperature) + "}";
  server.send(200, "application/json", jsonData);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando a la red WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conexión WiFi establecida");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  // Configurar las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
}
