

#include "Thread.h"
#include "ThreadController.h"

const byte Encoder = 2;
int contador = 0;
int valorPercorrer = 1000;
int distanciaUltrassom = 30;

int splitString(char* frase);
int timeout = 500;

const byte trigger = 5;
const byte echo = 6;
const byte trigger2 = 10;
const byte echo2 = 9;
byte atualTrigger = 5;
byte atualEcho = 6;

int bateria = A0;
int tensaoMotor1 = A8;
int tensaoMotor2 = A9; 
double constanteMotor = 0.004882813;




Thread ultrassomThread;
Thread serialThread;
Thread bateriaThread;
Thread tensaoMotorThread;

int tempoTensao = 1000;
int tempoUltrassom = 500;
int tempoSerial = 500;
int tempoBateria = 1000;

ThreadController controllerUltrassom;
ThreadController controllerSerial;

void setup() {
    pinMode(Encoder, INPUT);
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    pinMode(11, OUTPUT);
    digitalWrite(11, HIGH);
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
    pinMode(5, OUTPUT);
    pinMode(6, INPUT);
    pinMode(10, OUTPUT);
    pinMode(9, INPUT);


    attachInterrupt(0, calculaPulso, RISING);
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Iniciando encoder");
    

    ultrassomThread.setInterval(tempoUltrassom);
    ultrassomThread.onRun(ultrassom);
    
    bateriaThread.setInterval(tempoBateria);
    bateriaThread.onRun(leBateria);
  
  
    serialThread.setInterval(tempoSerial);
    serialThread.onRun(serial);

  tensaoMotorThread.setInterval(tempoTensao);
  tensaoMotorThread.onRun(tensaoMotor);

   // controllerUltrassom.add(&ultrassomThread);
    controllerSerial.add(&tensaoMotorThread);
    controllerSerial.add(&serialThread);
    controllerSerial.add(&bateriaThread);


}


int analisaSerial(char* frase) {

    char dist[10] = "Distancia";
    char ultra[10] = "Ultrassom";
    char sent[10] = "Sentido";
    char movendo[10] = "Movimento";


    if(strstr(frase, dist))
        return 2;
    if(strstr(frase, ultra))
        return 1;
    if(strstr(frase, sent))
        return 3;
    if(strstr(frase, movendo))
        return 4;


    return 0;

}


int distanciaultrassom = 0;
void serial() {

    if(Serial.available()> 0) {
        String teste = Serial.readString();
        if(teste == "Encoder\n") {
            Serial.print("Encode ");
            Serial.println(contador);
        }
        else {

            if(teste == "Zerar\n") {
                Serial.println("Zerando contador");
                contador = 0;
            }
            else {
                char frase[15];
                teste.toCharArray(frase, sizeof(frase));
                int opcao = analisaSerial(frase);
                int valor = splitString(frase);
                switch(opcao) {
                case 1:
                    distanciaultrassom = valor;
                    Serial.print("Ultrassom :");
                    Serial.println(valor);
                    break;
                case 2:
                    valorPercorrer = valor;
                    Serial.println("Valor Percorrer ");
                    Serial.println(valor);
                    break;
                case 3:
//                    if(valor == 1) {
//                        Serial.println("\t\t\t 1");
//                        atualEcho = echo;
//                        atualTrigger = trigger;
//                    } else {
//                        Serial.println("\t\t\t 0");
//                        atualEcho = echo2;
//                        atualTrigger = trigger2;
//                    }
                    break;
                case 4:
                    Serial.print("Motor está ");
                    if(valor == 1) {
                        controllerUltrassom.enabled = true;
                        Serial.println("Movendo");
                    } else {
                        controllerUltrassom.enabled = false;
                        Serial.println("Parado");
                    }
                    break;
                default:
                    Serial.println("Comando invalido");
                    break;
                }
            }
        }
    }
}

void leBateria(){
  int valor = analogRead(bateria) ;
  double valorFinal = 0.0049 * valor * (3.020202);
    
  Serial.print("Bateria ");
  Serial.println(valorFinal);

}
void tensaoMotor(){
   int tensao1 = analogRead(tensaoMotor1);
    double saida= 0;
    Serial.print("Tensao Motor1 ");
    saida = constanteMotor*tensao1;
   
    Serial.println(saida);

    int tensao2 = analogRead(tensaoMotor2);
    Serial.print("Tensao Motor2 ");
    saida = constanteMotor*tensao2;
    
    Serial.println(saida);
    
}
    void loop() {
  
        controllerSerial.run();
        controllerUltrassom.run();
    }

    int splitString(char* frase) {
        char * saida = strtok(frase, " ");
        char * auxiliar = strtok(NULL, " ");
        String converte = auxiliar;
        return converte.toInt();
    }


    void ultrassom() {

        digitalWrite(atualTrigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(atualTrigger, LOW);

        uint32_t pulse_time = pulseIn(atualEcho, LOW);
        double distance = 0.01715 * pulse_time;

        if(distance < distanciaUltrassom) {
            Serial.println("Obstaculo");
        }

    }

    void calculaPulso() {
        contador++;
        if(contador == valorPercorrer) {
            contador =0;
            Serial.println("Deslocou o valor desejado");
        }
    }
