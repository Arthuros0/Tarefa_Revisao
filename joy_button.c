#include "joy_button.h"
#include "pico/bootrom.h"

volatile absolute_time_t debounce_button;
volatile bool shoot=false;
volatile bool dificil=false; 
volatile bool strike=false;

uint8_t pos_x;
uint8_t pos_y;

uint8_t pos_atual=0;
uint8_t pos_anterior=0;


void joystick_setup(){
  adc_init();
  adc_gpio_init(Y_JOY);
  adc_gpio_init(X_JOY);
}

void joystick_pwm_display_control(ssd1306_t *ssd){

  adc_select_input(Y_CHANNEL);  //Seleção do canal 
  uint16_t valor_adc=adc_read();  //Leitura do valor analogico de Y
  //printf("VRY: %d\n",valor_adc); //Debug do Joystick

  pos_y=(4095-valor_adc)/64; //Corrige inversão do eixo y do joystick;
  //Difine limite superior e inferior do quadrador para o eixo y do display
  if(pos_y > 55){
    pos_y=55;
  }else if(pos_y < 10){
    pos_y=10;
  }

  adc_select_input(X_CHANNEL);
  valor_adc=adc_read();
  //printf("VRX: %d\n", valor_adc);

  pos_x=valor_adc/32; //Calcula pos no eixo x no display

  //Difine limite esquerdo e direito do quadrado para o eixo x do display
  if(pos_x > 118){
    pos_x=118;
  }else if (pos_x < 10){
    pos_x=10;
  }
  
  draw_point(ssd,pos_x,pos_y,true); //Desenha quadrado
  ssd1306_send_data(ssd);
}

void button_init(uint8_t pin){
  gpio_init(pin);
  gpio_set_dir(pin,GPIO_IN);
  gpio_pull_up(pin);
}

void button_callback(uint pin, uint32_t events){
  if (time_reached(debounce_button))
  {
    if (pin == BUTTON_B)
    {
      shoot=true;
      strike=true;
    }else if (pin == BUTTON_SW)
    {
      dificil=!dificil;
      muda_estado(VERDE);
      muda_estado(VERMELHO);
      if (dificil)
      {
        add_alarm_in_ms(4000,&game_timer,NULL,false);
      }
      
    }
    debounce_button=delayed_by_ms(get_absolute_time(),200);
  }
  
}

int64_t game_timer(alarm_id_t id, void *user_data){
  if (dificil)
  {
    if (pos_atual!=pos_anterior)
    {
      pos_anterior=pos_atual;
      return 4000000;
    }
    shoot=true;
    strike=false;
    return 0;
  }
  return 0;
  
}