/*  ATIVIDADE EMBARCATECH - TAREFA AULA SÍNCRONA 05/02
    Aluno: Devid Henrique Pereira dos Santos
    Matrícula: TIC370100380   
*/

#include <stdio.h>        // Biblioteca padrão da linguagem C
#include "pico/stdlib.h"  // Biblioteca padrão pico SDK
#include "hardware/pwm.h" // Biblioteca para controlar o hardware de PWM

/*  Calculando o valor do WRAP para uma frequência PWm de 50 Hz:
    WRAP = (125MHZ / (255*50)) -1 = 9803 

    Posição em 0°
    Tp1 = 0.12*9803   = 1176 
    
    Posição em 90°
    Tp2 = 0,0735*9803 = 720
    
    Posição em 180°
    Tp3 = 0.025*9803  = 242
*/

#define SERVO_PWM 12                // Define a pinagem do servomotor 
const uint16_t WRAP_PERIOD = 9803;  // Valor máximo do contador - WRAP
const uint16_t WRAP_0 = 1176;       // Duty cycle para 0°
const uint16_t WRAP_90 = 720;       // Duty cycle para 90°
const uint16_t WRAP_180 = 245;      // Duty cycle para 190°

const float PWM_DIVISER = 255.0;    // Divisor máximo do clock para o PWM
const uint16_t SERVO_STEP = 49;     // Passo de incremento/decremento para o duty cycle do LED
uint16_t led_level = 245;           // Nível inicial do PWM (duty cycle)
static int level;                   // O nível do PWM
bool POSITION = true;               // Verifica a posição 

// Função para configurar o módulo PWM
void pwm_setup()
{
    gpio_set_function(SERVO_PWM, GPIO_FUNC_PWM);    // Habilita o pino GPIO como PWM
    uint slice = pwm_gpio_to_slice_num(SERVO_PWM);  // Obter o canal PWM da GPIO
    pwm_set_clkdiv(slice, PWM_DIVISER);             // Define o divisor de clock do PWM
    pwm_set_wrap(slice, WRAP_PERIOD);               // Define o valor de wrap
    
    //Primeiro ciclo do servomotor para 3 posições
    pwm_set_gpio_level(SERVO_PWM, WRAP_0); // Define o duty cycle para posicionar 0°
    sleep_ms(5000); // Espera 5s
    pwm_set_gpio_level(SERVO_PWM, WRAP_90); // Define o duty cycle para posicionar 90°
    sleep_ms(5000); // Espera 5s
    pwm_set_gpio_level(SERVO_PWM, WRAP_180); // Define o duty cyle para posicionar 180°
    sleep_ms(5000); // Espera 5s
    
    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
}

int main()
{
    stdio_init_all();
    pwm_setup();

    while (true) {
        if (POSITION) {
            if (level < 1176) { // Começa em 0° e vai até 180°
                level += SERVO_STEP; 
                if (level > 1176) level = 1176; // Garante que não ultrapasse
            } else {
                POSITION = false; // Alcançou o máximo, começa a decrementar
            }
        } else {
            if (level > 245) { // Começa em 180° e vai até 0°
                level -= SERVO_STEP; 
                if (level < 245) level = 245; // Garante que não ultrapasse o mínimo
            } else {
                POSITION = true; // Alcançou o mínimo, começa a incrementar
            }
        }

        pwm_set_gpio_level(SERVO_PWM, level);
        sleep_ms(200); // Adiciona um pequeno atraso para suavizar o movimento
    }
}