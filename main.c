#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "servo.h"

int main() {

    int servo_amount, gpio;
    
    printf("Combien de Servo voulez vous contrôler ?\n");
    scanf("%d", &servo_amount);

    Servo *servo_list[servo_amount];

    for(int i = 0; i < servo_amount; i++){

        printf("Pin du Servo %d : ", i + 1);
        scanf("%d", &gpio);

        servo_list[i] = Servo_init(gpio, 0);
        Servo_launch(servo_list[i]);
    }

    int end = 0;

    cls();

    do{

        char input[10];
        printf("Entrez 'X,Y' pour changer la position d'un moteur (X le numéro du servo, Y la nouvelle position)\nEntrez 'q' pour quitter\n");
        scanf("%s", input);
        

        if (strcmp(input, "q") == 0) {
            end = 1;
        }else{

            char* dash = strchr(input, ',');
            if (dash == NULL) {
                cls();
                printf("Entrée invalide\n");
                }else{
                    *dash = '\0';
                    int servo_id = atoi(input);
                    int position = atoi(dash + 1);
                    if(servo_id > 0 && servo_id <= servo_amount && position >=0 && position <=180){
                        Servo_set_position(servo_list[servo_id - 1], position);
                        cls();
                        printf("la position du servo %d vaut maintenant %d\n", servo_id, position);
                    }else{
                        cls();
                        printf("servo_amount : %d\n", servo_amount);
                        printf("veuillez entrer un numéro de servo compris entre 1 et %d, et une position comprise entre 0 et 180\n", servo_amount);
                    }
                }
        }
    }while(!end);
    
    for(int j = 0; j < servo_amount; j++){
        Servo_free(servo_list[j]);
    }
    cls();
    printf("Fin du programme\n");
    return 0;
}

void cls(){
printf("\033c");
}