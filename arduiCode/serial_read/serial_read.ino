/* This program checks the serial ports to see if there is 
    any data currently in the buffer, if there is then it will read the data, and print
    its unformatted output. Note that any strings received through the serial ports we be printed
    as raw bytes.
    
    .readline() can be replaced with the function ".readString()" to automatically format it to a string */
void setup(){
    Serial.begin(9600);
}

void loop (){
    if (Serial.available() > 0){
        byte data = 0;    
        Serial.readline(&data,8);
        if(data != 0){
          Serial.println("Received: ");
          Serial.println(data);
        }
    }

}
