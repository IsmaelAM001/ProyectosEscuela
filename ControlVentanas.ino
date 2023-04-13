//Prueba del sensor electromagentico para la ventana:
//Ismael Aparicio

//Librerias utilizadas:
#include <WiFi.h>
#include "FirebaseESP32.h"
#include "time.h"

//----------------WIFI configuracion-------------------------------
const char* ssid     = "TP-LINK_4106";
const char* password = "54281264";
//------------CONEXION CON LA BASE DE DATOS DESEADA---------------------------
#define FIREBASE_HOST "https://cisterna-tinaco-iot-default-rtdb.firebaseio.com/" //Link de Manu
#define FIREBASE_AUTH "fucx9kG1YkhDPdbI22iULkC3EbsT1zEnlsz1msMm" //*****************
//Firebase Data object--------------------------------------------------------
FirebaseData firebaseData;
//Pines
int pSM1 = 17; //Pin Sensor Magnetico //Funciona con el 39 (VN)
int pSM2 = 18; //Pin de SM / Ventana 2
int pbzz = 4; //Pin de buzzer


//Variables para ontrolar el buzzer:
int control; // Activar buzzer cuando se abra
int control1;

bool cbzz = true;//Variable para controlar el buzzer.
bool cbzz1 = true;//Variable para controlar el buzzer.
int contconexion = 0;
bool verificar = false;
bool verificar1 = false;
//Si deseamos una ruta especifica:
String nodo = "/ventanas";
String nodo1 = "/1";
String nodo2 = "/2";


void setup() {
  Serial.begin(115200);
//---------------------------------pines a ocupar-----------------
  pinMode(pSM1, INPUT_PULLUP);
  pinMode(pSM2, INPUT_PULLUP);
  pinMode(pbzz, OUTPUT);
  Serial.println("Listo Setup");
  ActBzz();
// -----------------------Conexión WIFI--------------------------
  WiFi.begin(ssid, password);
  //Cuenta hasta 50 si no se puede conectar lo cancela
  while (WiFi.status() != WL_CONNECTED and contconexion <50) { 
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion <50) { 
      Serial.println("");
      Serial.println("WiFi conectado");
  }
  else { 
      Serial.println("");
      Serial.println("Error de conexion");
  }
//---------------------------------------------------BASE DE DATOS
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  //VENTANA 2----------------------------------------------------------------
  //----------Sensor magnetico para la ventana1------------
  control = digitalRead(pSM1);
  Serial.println(control);
  if(control == HIGH){
    Serial.println("La ventana 1 esta abierta");
    //FireBase:
    Firebase.setBool(firebaseData, nodo + nodo1 + "/AbiertoCerrado" , true);
    if(cbzz==true){
      cbzz = false;
      ActBzz();
    }
    delay(1000);
  }
  else{
    Serial.println("La ventana 1 esta cerrada");
    //A continuación se mandara el dato a la base de datos:
    //FIRE BASE:
    Firebase.setBool(firebaseData, nodo + nodo1 + "/AbiertoCerrado", false);
    delay(1000);
    if(cbzz==false){
      cbzz = true;
      for (int i=0; i<2; i++){
        ActBzz();
      }
    }
  }
  //VENTANA 2----------------------------------------------------------------
  control1 = digitalRead(pSM2);
  Serial.println(control1);
  if(control1 == HIGH){
    Serial.println("La ventana 2 esta abierta");
    //FireBase:
    Firebase.setBool(firebaseData, nodo + nodo2 + "/AbiertoCerrado" , true);
    if(cbzz1==true){
      cbzz1 = false;
      ActBzz();
    }
    delay(1000);
  }
  else{
    Serial.println("La ventana 2 esta cerrada");
    //A continuación se mandara el dato a la base de datos:
    //FIRE BASE:
    Firebase.setBool(firebaseData, nodo + nodo2 + "/AbiertoCerrado", false);
    delay(1000);
    if(cbzz1==false){
      cbzz1 = true;
      for (int i=0; i<2; i++){
        ActBzz();
      }
    }
  }

  
  Firebase.getInt(firebaseData, nodo + nodo1 + "/Notificar");
  verificar = firebaseData.intData();
  delay(500);
  if (verificar == 1){
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    Firebase.setBool(firebaseData, nodo + nodo1 + "/Notificar", false);
  }


  //VENTANA 2
  Firebase.getBool(firebaseData, nodo + nodo2 + "/Notificar");
  verificar1 = firebaseData.intData();
  delay(500);
  if (verificar1 == 1){
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    ActBzz();
    Firebase.setBool(firebaseData, nodo + nodo2 + "/Notificar", false);
  }
}


void ActBzz(){   //En esta funciona se activa el buzzer.
  digitalWrite(pbzz, HIGH);
  Serial.print("SONIDO");
  delay(250);
  digitalWrite(pbzz, LOW);
  Serial.println("SONIDO!!");
  delay(250);
}
