#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5]; 
int packet_sizes[5];
int set_packets = 0;


//Sorterings algoritme som er basert på bubble sort metode.
void sortering(uint8_t *arr[5],  int n) {

//Lager en nested loop.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
//x og y blir tildelt verdier. Disse verdiene er hentet fra sekvens pakken som vi har fått.
        int x = get_bits(arr[j], 14, 18);
        int y = get_bits(arr[j + 1], 14, 18);

//Verdiene til x og y blir sammenlignet og evt. byttet om på visst den laveste verdien står på feil plass.
            if (y < x) {
                uint8_t *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                int tempo = packet_sizes [j];
                packet_sizes[j] = packet_sizes[j + 1];
                packet_sizes[j + 1] = tempo;
      }
    }
  }
}
//Slutt på sorterings programmet.

//Begynnelse på main programmet. 
int main() {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
    for(int i = 0; i < 5; i++) {
        packets[i] = NULL;
    }
    //Programmet går i en loop så lenge den har data å lese ifra stdin.
    while(!feof(stdin)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 16, 32) + 1;
        data_read += fread(data + 6, 1, data_length, stdin);

//Begynner nå å lese og lagre data som vi får.
        packets[set_packets] = malloc(data_read);
        memcpy(packets[set_packets], data, data_read);
        packet_sizes[set_packets] = data_read;
        set_packets += 1;
        if(set_packets == 5){
//Starter sorterings programmet.
            sortering(packets, 5);
            for(int i = 0; i < 5; i++) {
                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                fflush(stdout);
//Når en pakke er blitt laget så får programmet beskjed om den er klar til å få en ny pakke den kan sortere.
                packets[i] = NULL;
            }
            set_packets = 0;
        }        
    }
}