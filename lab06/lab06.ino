/*~~~~~~~~~~~~~~~~~~~~~~~~~SETTINGS~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <SPI.h>
#include <SD.h>

File archivo; 
String file;
const char* string1;
int bandera;
/*~~~~~~~~~~~~~~~~~~~~~~~~~SETUP~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void setup() {

  Serial.begin(9600);
  while(!Serial);
  SPI.setModule(0);
    
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PA_3, OUTPUT);

   if (!SD.begin(PA_3)) 
   {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~LOOP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void loop() 
{
  //imprime el menu
  Serial.print("\nMenu\n");
  Serial.print("1.Directorio\n2.Abrir\n3.Crear\n"); 
  
  while(!Serial.available()){ //espera a que se integrese una opcion
  }
  
  int selection = Serial.parseInt(); //recibe el dato de entrada (acepta solo enteros)

  switch(selection)
  {
      case 1: //lectura de directorio
      {   
          Serial.print("\n\n");
          archivo = SD.open("/"); //abre el directorio principal
          printDirectory(archivo, 0);
          break;
      }
                  
      case 2: //abrir archivo
      {   
          Serial.print("\nEscriba el nombre de su archivo:\n");   //muestra el mensaje en la pantalla   
          Escritura();
          
          Serial.print("Abriendo...");   //imprime un texto para mostrar que archivo se esta abriendo
          Serial.print(file); 
          Serial.print("\n");
           
          string1 = file.c_str(); //convierte el dato de la variable a un const string
          abrir_Img(string1); 
  
          break;
      }
      
      case 3: //crear archivo
      { 
          Serial.print("\n\n");
          crear("Per.txt"); 
          break;
      }
      
      default:
      {
          Serial.print("opcion no valida"); //si se ingresa en el menu una opcion diferente, imprime un mensaje de error
          break;
      }
  }

  while(Serial.available()) //espera al ingreso de datos
  {
    Serial.read();
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Funciones~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Escritura(void) //Recibe el nombre del archivo a abrir
{
  file.remove(0); //limpia la variable que almacena el dato
  bandera = 0;    //limpia la bandera de conteo de ciclos
  while(1)
  { delay(100);
    while(!Serial.available());
    
    char caracter = Serial.read();  //el caracter recibe el dato actual dentro del buffer
    if (caracter == '$' && bandera > 0) //cuando 
    {      
      file += ".txt";  //le agrega la extension al archivo
      return; //salir
    }
    else if(bandera > 0)
    { 
      file+=caracter;
    }
    bandera++;  //incrementa una vuelta
    
  }
}

void printDirectory(File dir, int numTabs){
  while(true) {
     
     File entry =  dir.openNextFile();
     if (!entry) {
      
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
  return;
}

void abrir_Img(const char *name) // funcion para abrir el archivo (nombre archivo)
{
 
  archivo = SD.open(name);
  if (archivo) {

    // read from the file until there's nothing else in it:
    while (archivo.available()) //mientras archivo tenga datos que mandar
    {
      Serial.write(archivo.read()); //cierra el archivo
    }

    archivo.close();
  } else {
    Serial.println("error opening the current file"); //si no encuentra el archivo, imprime un mensaje de error
  }
  return;
}

void crear(const char *NNombre)
{  
 
   File entry = SD.open(NNombre, FILE_WRITE); //abre el archivo en modo escritura
   if(entry) //si existe
   {  Serial.print("Ingrese su imagen (finalice con &):  \n");
      while(1)
      {
        while(!Serial.available()); //espera a que se le ingrese un dato
        char caracter = Serial.read();  //captura los datos del buffer
        if(caracter == '&') //cuando encuentra el caracter & termina el proceso
        {
          
          entry.close(); //cierra el archivo
          Serial.println("Guardado con exito\n"); //imprime mensaje de guardado
          return; //sale de la funcion
        }
        else
        {
          entry.print(caracter); //escribe en el archivo
        }
      }
   }
   else //si no existe el archivo lo crea
   {
    Serial.println("\nCreando el archivo...\n");
   }
   
}
