#define minvalue 0
#define maxvalue 44.75

char text;

const String kb_lower[][1] = {
  "a","b","c","d","e","f","g","h","i","j",
  "k","l","m","n","o","p","q","r","s","t",
  "u","v","w","x","y","z","1","2","3","4",
  "5","6","7","8","9","0",",",".","-","=",
  "/","[","]","BS"," "};
const String kb_upper[][1] = {
  "A","B","C","D","E","F","G","H","I","J",
  "K","L","M","N","O","P","Q","R","S","T",
  "U","V","W","X","Y","Z","!","@","#","$",
  "%","^","&","*","(",")","<",">","_","+",
  "|","{","}","BS"," "};

class Rotary {
  private:
    float s=0;
    
    float checkrotary(int rotary1, int rotary2){
      static byte old=0;
      byte new1 = digitalRead(rotary1);
      byte new2 = digitalRead(rotary2);
      byte new_var = (new1*2)|(new2^new1);
      byte d = old-new_var;
      old=new_var;
      switch(d){
        case 1: return 0.25;
        case 3: return -0.25;
        case 255: return -0.25;
        case 253: return 0.25;
        default: return 0;
        }
    }

  public:
    int getRotary() {
      float d = checkrotary(6, 7);
      s=s+d;

      if (s>maxvalue){s = minvalue;}
      if (s<minvalue){s = maxvalue;}

      return int(s);
    }

    String getRotaryLetters(String letter_list){
      int turn_value = getRotary();
      String kbl_value = kb_lower[turn_value][0];
      String kbu_value = kb_upper[turn_value][0];
      if (letter_list == "Lower") {
        return kbl_value;
      }
      else if (letter_list == "Upper") {
        return kbu_value;
      }
      else if (letter_list == "Both") {
        return kbl_value + " - " + kbu_value;
      }
    }
};

Rotary rotary_encoder;

class Text {
  private:
    String text = "";

    void appendToText(String appending_text) {
      if (appending_text == "BS") {
        String old_text = text;
        int old_text_length = old_text.length();
        text = old_text.substring(0, old_text_length - 1);
      }
      else {
        text += appending_text;
      }
      Serial.print("Text: '");
      Serial.print(text);
      Serial.println("'");
    }
    
  public:
    String getText() {
      return text;
    }

    void updateButton() {
      if (digitalRead(5) == 0){
        do {} while (digitalRead(5) == 0);
        delay(50);
        int start_time = millis();
        int end_time = start_time + 100;
        bool double_click = false;
        do {
          if (digitalRead(5) == 0){
            double_click = true;
          }
        } while (millis() < end_time);

        if (double_click) {
          appendToText(kb_upper[rotary_encoder.getRotary()][0]);
          do {} while (digitalRead(5) == 0);
        }
        else{
          appendToText(kb_lower[rotary_encoder.getRotary()][0]);
        }
      }
    }
};

Text note;

void setup() {
  // put your setup code here, to run once:
  //Open the serial port
  Serial.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
}

String oldRotaryLetters;

void loop() {
  String newRotaryLetters = rotary_encoder.getRotaryLetters("Both");
  if (oldRotaryLetters != newRotaryLetters){
    Serial.println(newRotaryLetters);
    oldRotaryLetters = newRotaryLetters;
  }
  
  note.updateButton();
}
