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
const int ECHO = 11; // actualizado

const int BZR = 9;
const int LED = 13;

long duracion;
int distancia;
const int umbral = 15; // cm
const int giro = 450;
const int t_beep = 200;

void setup() {
  // Pines motores
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Pines sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(BZR, OUTPUT);
  pinMode(LED, OUTPUT);

  // Activar STBY
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
    Serial.println("Obstáculo detectado: Detener y girar");
    detenerMotores();
    beep();
    beep();
    delay(500);
    girarDerecha(150);
    delay(giro);  // tiempo de giro ajustable
    detenerMotores();
    delay(300);
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
  long duracion = pulseIn(ECHO, HIGH, 20000); // timeout de 20 ms
  if (duracion == 0) return 999; // sin respuesta
  return duracion * 0.034 / 2;
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

void beep(){
  digitalWrite(BZR, HIGH);
  delay(t_beep);
  digitalWrite(BZR, LOW);
  delay(t_beep);
}
