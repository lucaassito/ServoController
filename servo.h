#include <gpiod.h>
#include <stdio.h>

typedef struct Servo_t Servo;

/**
 * Initialise un Servo
 * @param gpio_pin  Le pin GPIO utilisé par le SERVO
 * @param position  La position surlaquelle on veut positionner le servomoteur
 * @return une instance de Servo
 */
Servo *Servo_init(int gpio_pin, int position);

/**
 * Etablie la connexion avec le pin GPIO et positionne le servo à la position souhaitée
 * @param arg   L'instance de Servo à utiliser
 * @return
 */
void *Servo_thread_fct(void *arg);

/**
 * Crée un thread et lance la fonction liée au thread du Servo
 * @param this
 */
void Servo_launch(Servo* this);

/**
 * Libère la mémoire du Servo
 * @param this L'instance de Servo à libérer
 */
void Servo_free(Servo *this);

/**
 * Modifie la position du servo
 * @param this L'instance du servo
 * @param position La nouvelle position
 */
void Servo_set_position(Servo *this, int position);