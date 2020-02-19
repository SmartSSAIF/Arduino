const byte Encoder = 2;
int contador = 0; 
int valorPercorrer = 0;
int distanciaUltrassom = 30;
double distanciaSeguranca = 30;
int obstaculos = 0;
int splitString(char* frase);
int timeout = 3000;
boolean notificacao = false;
const byte trigger = 6;
const byte echo = 5;
const byte trigger2 = 10;
const byte echo2 = 9;
byte atualTrigger = 5;
byte atualEcho = 6;
int vcc1 = 7;
int vcc2 = 11;
int gnd1 = 4;
int gnd2 = 8;

void setup() {
  pinMode(Encoder, INPUT);
  
  pinMode(vcc1, OUTPUT);
  digitalWrite(vcc1, HIGH);
  pinMode(gnd1, OUTPUT);
  digitalWrite(gnd1, LOW);
  pinMode(vcc2, OUTPUT);
  digitalWrite(vcc2, HIGH);
  pinMode(gnd2, OUTPUT);
  digitalWrite(gnd2, LOW);

  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(echo, INPUT);
  pinMode(trigger2, OUTPUT);
  digitalWrite(trigger2, LOW);
  pinMode(echo2, INPUT);
  
  attachInterrupt(0, calculaPulso, RISING);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Iniciando encoder");

}

void loop() {

  ultrassom();
  
  if(Serial.available()> 0){
   String teste = Serial.readString();
   if(teste == "Encoder\n"){
    String saida = "Encoder " + String(contador);
    Serial.println(saida);
  }
  else{

    if(teste == "Zerar\n"){
      Serial.println("Zerando contador");
      contador = 0;
    }
    else{
      char dist[10] = "Distancia";
      char ultra[10] = "Ultrassom";
      char sent[10] = "Sentido";
      char frase[30];
      teste.toCharArray(frase, sizeof(frase)); 
      if(strstr(frase, ultra)){
        distanciaUltrassom = splitString(frase);
        Serial.print("Ultrassom: ");
        Serial.println(distanciaUltrassom);
      }

      else{
        if(strstr(frase, dist)){
          valorPercorrer = splitString(frase);
          String saida = "Valor Percorrer " + String(valorPercorrer);
          Serial.println(saida);

        }
        else{
          if(strstr(frase, sent)){
            Serial.println("\t\t Sentido");
            char frase1[30];
            teste.toCharArray(frase1, sizeof(frase1)); 
            int valor = splitString(frase1);
            if(valor == 1){
              Serial.println("\t\t\t 1");
              atualEcho = echo;
              atualTrigger = trigger;
            }else{
              Serial.println("\t\t\t 0");
              atualEcho = echo2;
              atualTrigger = trigger2;
            }
            Serial.println(teste);

          }else{
            Serial.println("Comando invalido");

          }
        }
      }
    }
  }
}
}

int splitString(char* frase){
  char * saida = strtok(frase, " ");
  char * auxiliar = strtok(NULL, " ");
  String converte = auxiliar;
  return converte.toInt();
}


void ultrassom(){

  digitalWrite(atualTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(atualTrigger, LOW);

  uint32_t pulse_time = pulseIn(atualEcho, HIGH, timeout);
  double distance = 0.01715 * pulse_time;


  if(distance < distanciaSeguranca && distance > 0){  
    obstaculos++;
    
    if(obstaculos > 3){
     Serial.print("Obstaculo ");
     Serial.println(distance);
     notificacao = true;
   } 
 }else{
  obstaculos = 0;
  if(notificacao){
    notificacao = false;
    Serial.println("Sem obstaculo");
  }
}
}
void calculaPulso(){
 contador++;
 if(contador == valorPercorrer){
  contador =0;
  Serial.println("Deslocou o valor desejado");
}
}
