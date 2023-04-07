//----------------------------------------Load libraries
#include <esp_now.h>
#include <WiFi.h>
//----------------------------------------

uint8_t broadcastAddress[] = {0xD4,0xD4,0xDA,0x5B,0x27,0xDC}; //   78:21:84:7D:87:F8 --> REPLACE WITH THE MAC Address of your receiver.   array for mac 

//----------------------------------------Variables to accommodate the data to be sent.
int IR1_send;
int IR2_send;
int T_count_send;
int traffic_send;
//----------------------------------------

//----------------------------------------Define variables to store incoming readings
int IR1_rec;
int IR2_rec;
int T_count_rec;
int traffic_rec;
//----------------------------------------

String success; //--> Variable to store if sending data was successful

//----------------------------------------Structure example to send data
// Must match with ESP32 paired.
typedef struct struct_message {
    int ir1;
    int ir2;
    int T_count;
    int traffic;
    } struct_message;

struct_message send_Data; // Create a struct_message to send data.

struct_message receive_Data; // Create a struct_message to receive data.
//----------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
  //Serial.println(">>>>>");
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receive_Data, incomingData, sizeof(receive_Data));
  //Serial.println();
  //Serial.println("<<<<< Receive Data:");
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  IR1_rec = receive_Data.ir1;
  IR2_rec = receive_Data.ir2;
  T_count_rec = receive_Data.T_count; 
  traffic_rec = receive_Data.traffic;
  Serial.println("Receive Data: ");
  Serial.println(IR1_rec);
  Serial.println(IR2_rec);
  Serial.println(T_count_rec);
  Serial.println(traffic_rec);
  Serial.println("..................");
  //Serial.println("<<<<<");
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
  pinMode(23,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station

  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------
  
  //----------------------------------------Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  //----------------------------------------
  
  //----------------------------------------Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //----------------------------------------
  
  //----------------------------------------Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  //----------------------------------------
  
  esp_now_register_recv_cb(OnDataRecv); //--> Register for a callback function that will be called when data is received
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void loop() {
  //----------------------------------------Set values to send
  T_count_send =45;
  IR2_send = 44;
  IR1_send = 35;
  traffic_send = 16;
  send_Data.ir1 = IR1_send;
  send_Data.ir2 = IR2_send;
  send_Data.T_count = T_count_send;
  send_Data.traffic = traffic_send;

//  Serial.println(IR1_rec);
//  Serial.println(IR2_rec);
//  Serial.println(T_count_rec);
//  Serial.println(traffic_rec);
//  Serial.println("..................");

//  Serial.println(ir1);
//  Serial.println(ir2);
//  Serial.println(T_count);
//  Serial.println(traffic);
//  Serial.println("yyyyyyyyyyyyyyyyyyy");
 
  //----------------------------------------
  
  //Serial.println();
  //Serial.print(">>>>> ");
  //Serial.println("Send data");

  //----------------------------------------Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
   
  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }
  //----------------------------------------
  
  delay(1000);
//Serial.print("1000000");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<