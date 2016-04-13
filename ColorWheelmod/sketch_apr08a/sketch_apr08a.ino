  
int j=0;
byte SendRX[7] = {'1','2','3','4','5','6','7'};    // test
byte ControlRX[10];

void setup() 
{
//Serial.begin(9600);    // not needed
}

void Send_To_Controller(unsigned char *buf,int len)
{
  for (int i=0;i<len;i++)
    Serial.println(buf[i]);
}

// Receives an array from the Controller
//=================================
void Receive_From_Controller(unsigned char *buf,int len)
{
   if (Serial.available() > 0)        // if information available
    {
     for (int i=0;i<len;i++)
      buf[i] = Serial.read();           // enter into receive array
    }
}

void loop() 
{
if (j==0)
  { 
   Serial.begin(9600,3,2);
    if(Serial)
      {
      Send_To_Controller(SendRX,7);
      Receive_From_Controller(ControlRX,7);
      }
    Serial.end();
    Serial.begin(9600);
    if (Serial)
      {
      for (int i=0;i<7;i++)
       Serial.println(ControlRX[i]);
      }
    Serial.end();  
  }      
j++;        //run once
}
