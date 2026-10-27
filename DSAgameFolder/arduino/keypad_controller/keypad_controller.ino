const int ROWS = 4;
const int COLS = 4;

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT);
  }

  for (int j = 0; j < COLS; j++) {
    pinMode(colPins[j], INPUT_PULLUP);
  }
}

void loop() {
  for (int r = 0; r < ROWS; r++) {

    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], LOW);

    for (int c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        delay(50); 
        while (digitalRead(colPins[c]) == LOW);
        
        Serial.write(keys[r][c]);
      }
    }

    pinMode(rowPins[r], INPUT);
  }
}