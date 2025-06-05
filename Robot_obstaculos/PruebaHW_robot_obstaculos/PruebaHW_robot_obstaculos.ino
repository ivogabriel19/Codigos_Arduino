// Pines del driver TB6612FNG
const int INA1 = 4;
const int INA2 = 2;
const int PWMA = 3;
const int INB1 = 7;
const int INB2 = 6;
const int PWMB = 5;
const int STBY = 8;

// Pines del sensor HC-SR04
const int TRIG = 12;
const int ECHO = 11;
const int LED = 13;

long duracion;
int distancia;

unsigned long ultimoCambio = 0;
int estadoMovimiento = 0;

void setup() {
  // Pines de motores
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Pines del sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  // Activar STBY
  digitalWrite(STBY, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Leer distancia
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duracion = pulseIn(ECHO, HIGH);
  distancia = duracion * 0.034 / 2;
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  digitalWrite(LED,(distancia<10));

  // Cambiar movimiento cada 5 segundos
  if (millis() - ultimoCambio >= 5000) {
    detenerMotores();
    delay(1000); // Pausa entre acciones
    switch (estadoMovimiento) {
      case 0:
        Serial.println("Avanzar");
        avanzar(150);
        break;
      case 1:
        Serial.println("Retroceder");
        retroceder(150);
        break;
      case 2:
        Serial.println("Giro Izquierda");
        girarIzquierda(150);
        break;
      case 3:
        Serial.println("Giro Derecha");
        girarDerecha(150);
        break;
    }
    estadoMovimiento = (estadoMovimiento + 1) % 4;
    ultimoCambio = millis();
  }

  delay(100); // Pequeño delay para lectura estable
}

// Funciones de movimiento
void avanzar(int velocidad) {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  analogWrite(PWMB, velocidad);
}

void retroceder(int velocidad) {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  analogWrite(PWMA, velocidad);

  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  analogWrite(PWMB, velocidad);
}

void girarIzquierda(int velocidad) {
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
  analogWrite(PWMB, velocidad);
}

void girarDerecha(int velocidad) {
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  analogWrite(PWMA, velocidad);

  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
  analogWrite(PWMB, velocidad);
}

void detenerMotores() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}
