#include <stdio.h>
#include "pico/stdlib.h"
#include "joy_button.h"
#include "matriz_leds.h"
#include "buzzer.h"

#define NUM_ALVOS 15

const uint8_t pos_alvo[30]={64,20,100,30,50,50,78,30,18,22,70,37,37,23,59,32,103,50,115,47,92,46,26,19,18,20,48,28,81,41};

//Configurações básicas do I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

ssd1306_t ssd;
volatile bool cor=true;


int64_t turn_off_callback(alarm_id_t id, void *user_data);
bool verifica_acerto();
bool apaga=false;


int main()
{
    stdio_init_all();
    joystick_setup();

    i2c_init(I2C_PORT,400*1000);     //Inicialização do I2C
    init_i2c_pins(I2C_SDA, I2C_SCL); //Configuração dos pinos do I2C

    init_display(&ssd, ENDERECO,I2C_PORT); //Configuração do display

    setup_led_matrix();

    init_led(VERDE);
    init_led(VERMELHO);

    gpio_put(VERDE,1);
    gpio_put(VERMELHO,0);

    uint slice_buzzer=buzzer_init(BUZZER_A_PIN); //Configuração do buzzer

    button_init(BUTTON_B);
    button_init(BUTTON_SW);
    
    gpio_set_irq_enabled_with_callback(BUTTON_A,GPIO_IRQ_EDGE_FALL,true,&button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B,GPIO_IRQ_EDGE_FALL,true,&button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_SW,GPIO_IRQ_EDGE_FALL,true,&button_callback);
    char pontuacao[4]={"0"};

    while (true) {
        
        ssd1306_fill(&ssd,!cor);
        ssd1306_rect(&ssd,9,3,122,55,cor,!cor);

        ssd1306_draw_string(&ssd,"Pontos: ",3,0);
        ssd1306_draw_string(&ssd,pontuacao,66,0);

        draw_target(&ssd,pos_alvo[pos_atual*2], pos_alvo[(pos_atual*2)+1]);

        joystick_pwm_display_control(&ssd); //Manipulação do LED e Display via joystick

        if (shoot)
        {
            if (verifica_acerto() && strike){
                desenha_frame(estados,0);
                buzzer_beep(BUZZER_A_PIN, slice_buzzer, 1500, 100);
            }else{
                desenha_frame(estados,1);
                buzzer_beep(BUZZER_A_PIN, slice_buzzer, 600, 300);
            }
            sprintf(pontuacao,"%d",pos_atual);
            add_alarm_in_ms(2000,turn_off_callback,NULL,false);
            shoot=false;
            strike=false;
        }
        if (apaga)
        {
            apaga_matriz();
            apaga=false;
        }
        
        sleep_ms(41);
    }
}

bool verifica_acerto(){
    printf("Pos X: %d\n",pos_x);
    printf("Pos Y: %d\n",pos_y);
    printf("Pos Alvo X: %d\n",pos_alvo[pos_atual*2]-6);
    printf("Pos Alvo Y: %d\n",pos_alvo[(pos_atual*2)+1]-6);
    if (dificil)
    {
        if (pos_x == pos_alvo[pos_atual*2] - 6 && pos_alvo[pos_atual*2+1] -6 == pos_y && strike)
        {
            printf("Acertou!\n\n");
            pos_atual++;
            if(pos_atual >= NUM_ALVOS){
                pos_atual=0;
            }
            return true;
        }
        printf("Errou!\n\n");
        pos_atual=0;
        
        dificil=!dificil;
        muda_estado(VERDE);
        muda_estado(VERMELHO);
        
        
        return false;
    }
    
    if ((pos_x >= pos_alvo[pos_atual*2] - 7 && pos_x <= pos_alvo[pos_atual*2] - 5) &&
        (pos_y >= pos_alvo[pos_atual*2+1] -7 && pos_y <= pos_alvo[pos_atual*2+1] - 5))
    {
        printf("Acertou!\n\n");
        pos_atual++;
        if(pos_atual >= NUM_ALVOS){
            pos_atual=0;
            if (!dificil)
            {
                dificil=true;
                muda_estado(VERDE);
                muda_estado(VERMELHO);
                add_alarm_in_ms(4000,&game_timer,NULL,false);
            }
            
        }
        return true;
    }
    printf("Errou!\n\n");
    pos_atual=0;
    return false;
}

int64_t turn_off_callback(alarm_id_t id, void *user_data){
    apaga=true;
    return 0;
  }
