/* THIS PROGRAM OWNED AND BELONGS TO M DHIFA ALFITRA */ 

HardwareSerial SerialAT(2);
String inputString2, msg;
int dataA, dataB, dataC, dataD, dataE;

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600,SERIAL_8N1,16,17);
}

void loop() {
  parsingdata();
  // serialprintdata();
}

void parsingdata() {
  static uint32_t dataTime = 0;
  if (millis() <= dataTime)
    dataTime = millis();

    if (millis() - dataTime >= 100) {
       if (SerialAT.available() > 0) {
        char input = (SerialAT.read());
        inputString2 += input;
        Serial.print(input);
        
        if (input == 'G') {  //melakukan penentuan urutan abjad spliter
          int a = inputString2.indexOf('A');  //urutan karakter "A" pada inputString2
          int b = inputString2.indexOf('B');  //urutan karakter "B" pada inputString2
          int c = inputString2.indexOf('C');  //urutan karakter "C" pada inputString2
          int d = inputString2.indexOf('D');  //urutan karakter "D" pada inputString2
          int e = inputString2.indexOf('E');  //urutan karakter "E" pada inputString2
          int f = inputString2.indexOf('F');  //urutan karakter "F" pada inputString2
          int g = inputString2.indexOf('G');  //urutan karakter "G" pada inputString2

          //jumlah data didapat dari pengirim
          int fcs = inputString2.substring(f+1,g).toInt();  //antara i dan g     

          //jumlah data didapat dari hitung sendiri
          int jml_data =  inputString2.substring(a,f+1).length(); //dihitung dari a sampai i
      
          //jika jumlah data dari pengirim samadengan jumlah data hitung sendiri
          if (fcs == jml_data){   //memulai update masing-masing variable
            msg       = "LENGKAP";
            dataA     = inputString2.substring(a+1,b).toFloat();  //roll antara a dan b
            dataB     = inputString2.substring(b+1,c).toFloat();  //pitch antara b dan c
            dataC     = inputString2.substring(c+1,d).toFloat();  //yaw antara c dan d
            dataD     = inputString2.substring(d+1,e).toFloat();  //kanan antara d dan e
            dataE     = inputString2.substring(e+1,f).toFloat();  //kiri antara e dan f       
          }
          else{
            msg = "TIDAK LENGKAP";
          }
          inputString2="";  //kosongkan kembali inputString2 
        }
      }
    dataTime = millis();
    }
}

// void serialprintdata() {
//   static uint32_t dataTime = 0;
//   if (millis() <= dataTime)
//     dataTime = millis();

//     if (millis() - dataTime >= 1000) {
//       Serial.print("DATA 1 = ");
//       Serial.print(dataA);
//       Serial.print("\n");
//       Serial.print("DATA 2 = ");
//       Serial.print(dataB);
//       Serial.print("\n");
//       Serial.print("DATA 3 = ");
//       Serial.print(dataC);
//       Serial.print("\n");
//       Serial.print("DATA 4 = ");
//       Serial.print(dataD);
//       Serial.print("\n");
//       Serial.print("DATA 5 = ");
//       Serial.print(dataE);
//       Serial.print("\n\n");

//       dataTime = millis();
//     }
// }