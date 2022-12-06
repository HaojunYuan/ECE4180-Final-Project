class SerialCommunication {
  private:
    bool open;

  public:
    SerialCommunication() {
      open = false;
    }

    bool isOpen(){
      return open;
    }

    void start(){
      Serial.begin(SERIAL_BAUD_RATE);
      open = true;
    }

    void output(char* data){
      Serial.print(data);
      Serial.flush();
    }

    bool readData(char* input){
      byte size = Serial.readBytesUntil('\n', input, 100);
      input[size] = NULL;
      return input != NULL && strlen(input) > 0;
    }
};
