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

// Otros pines
const int BZR = 9;
const int LED = 13;

// Variables
long duracion;
int distancia;
const int umbral = 15;     // cm
const int giro = 450;      // ms
const int t_beep = 200;

void setup() {
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BZR, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(STBY, HIGH);
  Serial.begin(9600);

  beep();
}

void loop() {
  distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  digitalWrite(LED, (distancia < umbral));

  if (distancia < umbral) {
    Serial.println("Obstáculo detectado");

    detenerMotores();

    // Seniales
    beep();
    beep();
    luces();

    distancia = medirDistancia();
    Serial.print("Nueva distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (distancia >= umbral) {
      Serial.println("Obstáculo se fue. Avanzar.");
      avanzar(150);
    } else {
      Serial.println("Sigue bloqueado. Evaluando lados...");
      int izquierda = medirLadoIzquierdo();
      int derecha = medirLadoDerecho();

      Serial.print("Izquierda: ");
      Serial.print(izquierda);
      Serial.print(" cm, Derecha: ");
      Serial.print(derecha);
      Serial.println(" cm");

      if (izquierda > derecha) {
        Serial.println("Girar a la izquierda");
        girarIzquierda(150);
      } else {
        Serial.println("Girar a la derecha");
        girarDerecha(150);
      }

      delay(giro);  // tiempo de desvío
      avanzar(150);
    }
  } else {
    avanzar(150);
  }

  delay(100);
}

int medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracion = pulseIn(ECHO, HIGH, 20000); // timeout
  if (duracion == 0) return 999; // sin respuesta
  return duracion * 0.034 / 2;
}

int medirLadoIzquierdo() {
  girarIzquierda(150);
  delay(giro);
  detenerMotores();
  delay(300);
  int d = medirDistancia();
  girarDerecha(150);
  delay(giro);
  detenerMotores();
  delay(300);
  return d;
}

int medirLadoDerecho() {
  girarDerecha(150);
  delay(giro);
  detenerMotores();
  delay(300);
  int d = medirDistancia();
  girarIzquierda(150);
  delay(giro);
  detenerMotores();
  delay(300);
  return d;
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

void beep() {
  analogWrite(BZR, 255);
  delay(t_beep/2);
  analogWrite(BZR, 120);
  delay(t_beep/2);
  digitalWrite(BZR, LOW);
}

void luces(){
  for(int i=0; i<=20 ; i++){ //total= 2 segundos
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
}
