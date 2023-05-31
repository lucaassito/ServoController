#include "servo.h"
#include <pthread.h>

#define SIGNAL_HIGH_DURATION_MIN 1000000    // Temps minimum à l'état haut (1 ms en nanosecondes)
#define SIGNAL_HIGH_DURATION_MAX 2000000    // Temps maximum à l'état haut  (2 ms en nanosecondes)

struct Servo_t
{  
    int gpio_pin;       // Le numéro de la broche GPIO liée à la commande du Servo
    int position;       // La position souhaitée du servomoteur
    pthread_t thread;   // Le thread utiliser pour positionner le servomoteur
};

/**
 * Configure la broche GPIO en sortie
 * @param line  La ligne permettant d'intéragir avec la broche GPIO
 * @return
 */
int configure_gpio_output(struct gpiod_line *line);

/**
 * Envoie un signal de durée spécifique sur la broche GPIO
 * @param line      La ligne sur laquelle envoyer le signal
 * @param duration  La durée du signal
 */
void send_gpio_signal(struct gpiod_line *line, unsigned long duration);

/**
 * Transforme une position (en degrés) en un temps correspondant à l'état haut de la commande pour atteindre la posittion
 * @param position  La position souhaitée
 * @return la durée du signal pour atteindre la position donnée
 */
unsigned long position_to_duration(double position);

Servo *Servo_init(int gpio_pin, int position){

    Servo* this;
    this = malloc(sizeof(Servo));
    this->gpio_pin = gpio_pin;
    this->position = position;
    return this;
}

void Servo_set_position(Servo* this, int position){

    this->position = position;
    return NULL;
}

void *Servo_thread_fct(void *arg) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;

    Servo *this = (Servo *)arg;
    int gpio_pin = this->gpio_pin;
    int position = this->position;
    int ret;

    // Ouvrir la puce GPIO par défaut
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        perror("Impossible d'ouvrir la puce GPIO");
        return NULL;
    }

    // Obtenir la ligne GPIO correspondant à la broche spécifiée
    line = gpiod_chip_get_line(chip, gpio_pin);
    if (!line) {
        perror("Impossible d'obtenir la ligne GPIO");
        gpiod_chip_close(chip);
        return NULL;
    }

    // Configurer la broche GPIO en sortie
    ret = configure_gpio_output(line);
    if (ret < 0) {
        gpiod_line_release(line);
        gpiod_chip_close(chip);
        return NULL;
    }

    // Envoyer des signaux de contrôle du servo-moteur (exemple)
    while(true) {
        unsigned long duration = position_to_duration(this->position);
        send_gpio_signal(line, duration);
    }
}

void Servo_launch(Servo* this){
    if (pthread_create(&this->thread, NULL, Servo_thread_fct, (void*)this) != 0) {
        fprintf(stderr, "erreur pthread_create\n");
        exit(-1);
    }
}

void Servo_free(Servo *this){
    free(this);
}

int configure_gpio_output(struct gpiod_line *line) {
    int ret = gpiod_line_request_output(line, "servo-motor", 0);
    if (ret < 0) {
        perror("Impossible de configurer la broche en sortie");
        return ret;
    }
    return 0;
}

void send_gpio_signal(struct gpiod_line *line, unsigned long duration) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = duration;
    gpiod_line_set_value(line, 1);
    nanosleep(&delay, NULL);
    gpiod_line_set_value(line, 0);
    nanosleep(&delay, NULL);
}

unsigned long position_to_duration(double position) {
    // Limiter la position entre 0 et 360 degrés
    if (position < 0.0) {
        position = 0.0;
    } else if (position > 180.0) {
        position = 180.0;
    }

    // Convertir la position en durée de signal
    double signal_range = SIGNAL_HIGH_DURATION_MAX - SIGNAL_HIGH_DURATION_MIN;
    double position_ratio = position / 180.0;
    unsigned long duration = SIGNAL_HIGH_DURATION_MIN + (unsigned long)(signal_range * position_ratio);

    return duration;
}



