

#include "Thread.h"
#include "ThreadController.h"

const byte Encoder = 2;
int contador = 0;
int valorPercorrer = 0;
int distanciaUltrassom = 30;

int splitString(char* frase);
int timeout = 500;

const byte trigger = 5;
const byte echo = 6;
const byte trigger2 = 10;
const byte echo2 = 9;
byte atualTrigger = 5;
byte atualEcho = 6;



Thread ultrassomThread;
Thread serialThread;


int tempoUltrassom = 500;
int tempoSerial = 500;


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

    serialThread.setInterval(tempoSerial);
    serialThread.onRun(serial);

    controllerUltrassom.add(&ultrassomThread);
    controllerSerial.add(&serialThread);


}

void serial() {

    if(Serial.available()> 0) {
        String teste = Serial.readString();
        if(teste == "Encoder\n") {
            String saida = "Encoder " + String(contador);
            Serial.println(saida);
        }
        else {

            if(teste == "Zerar\n") {
                Serial.println("Zerando contador");
                contador = 0;
            }
            else {
                char dist[10] = "Distancia";
                char ultra[10] = "Ultrassom";
                char sent[10] = "Sentido";
                char movendo[10] = "Movimento";
                char frase[30];
                teste.toCharArray(frase, sizeof(frase));
                if(strstr(frase, ultra)) {
                    distanciaUltrassom = splitString(frase);
                    Serial.print("Ultrassom: ");
                    Serial.println(distanciaUltrassom);
                }

                else {
                    if(strstr(frase, dist)) {
                        valorPercorrer = splitString(frase);
                        String saida = "Valor Percorrer " + String(valorPercorrer);
                        Serial.println(saida);

                    }
                    else {
                        if(strstr(frase, sent)) {
                            Serial.println("\t\t Sentido");
                            char frase1[30];
                            teste.toCharArray(frase1, sizeof(frase1));
                            int valor = splitString(frase1);
                            if(valor == 1) {
                                Serial.println("\t\t\t 1");
                                atualEcho = echo;
                                atualTrigger = trigger;
                            } else {
                                Serial.println("\t\t\t 0");
                                atualEcho = echo2;
                                atualTrigger = trigger2;
                            }
                            Serial.println(teste);

                        } else {
                            if(strstr(frase, movendo)) {
                                char move[10];
                                teste.toCharArray(move, sizeof(move));
                                int value = splitString(move);
                                Serial.print("Motor está ");
                                if(value == 1) {
                                    controllerUltrassom.enabled = true;
                                    Serial.println("Movendo");
                                } else {
                                    controllerUltrassom.enabled = false;
                                    Serial.println("Parado");
                                }
                            }
                            else {
                                Serial.println("Comando invalido");
                            }
                        }
                    }
                }
            }
        }
    }
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
