const byte Encoder = 2;
const byte trigger = 7;
const byte echo = 6;
const byte VCC = 13;
boolean notificacao = false;
int timeout = 3000;

int obstaculos = 0;


int contador = 0; 
int valorPercorrer = 0;
double distanciaSeguranca = 10;


int splitString(char* frase);


void setup() {
  pinMode(Encoder, INPUT);
  attachInterrupt(0, calculaPulso, RISING);
   Serial.begin(9600);
   
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  digitalWrite(trigger, LOW);
  
   
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
      char frase[30];
      teste.toCharArray(frase, sizeof(frase)); 
      if(strstr(frase, ultra)){
        distanciaSeguranca = splitString(frase);
        Serial.print("Ultrassom: ");
        Serial.println(distanciaSeguranca);
     }

    else{
      if(strstr(frase, dist)){
        valorPercorrer = splitString(frase);
        String saida = "Valor Percorrer " + String(valorPercorrer);
        Serial.println(saida);
        
        }
      else{
        Serial.println("Comando invalido");
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
  digitalWrite(trigger, HIGH);
  delayMicroseconds(11);
  digitalWrite(trigger, LOW);
  
  uint32_t pulso = pulseIn(echo, HIGH, timeout);
  
  if(pulso == 0){
    pulso = 10000;
  }
  
  double distancia = 0.01715 * pulso;
  
  if(distancia < distanciaSeguranca){  
    obstaculos++;
    
    if(obstaculos > 3){
       Serial.print("Obstaculo ");
      Serial.println(distancia);
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