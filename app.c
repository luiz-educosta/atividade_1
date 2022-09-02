#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 1
#define TASK3_PRIORITY 1

#define BLACK "\033[30m" /* Black */
#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */
#define DISABLE_CURSOR() printf("\e[?25l")
#define ENABLE_CURSOR() printf("\e[?25h")

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

typedef struct
{
    int pos;
    char *color;
    int period_ms;
} st_led_param_t;

st_led_param_t green = {
    6,
    GREEN,
    250};
st_led_param_t red = {
    13,
    RED,
    100};

TaskHandle_t ledGreenTask_hdlr, ledRedTask_hdlr, ledfunctionTask_hdlr;

#include <termios.h>

static void prvTask_getChar(void *pvParameters)
{
    char key;
    int n;

    /* I need to change  the keyboard behavior to
    enable nonblock getchar */
    struct termios initial_settings,
        new_settings;

    tcgetattr(0, &initial_settings);

    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 1;

    tcsetattr(0, TCSANOW, &new_settings);
    /* End of keyboard configuration */
    for (;;)
    {
        int stop = 0;
        key = getchar();
        
        
        switch (key)
        {

        case '+':
            // Voltar ao funcionamento normal quando pressionado o “+”!
            //colocao taskresume para o led verde
            
            //stop = 1;
            break;

        case '*':
            // Caso o usuário pressione a tecla “*”, o LED vermelho deverá ficar aceso e o LED verde deve parar de piscar (apagado);
            //coloca o contrario do task resume para parar a task e o led
            //stop = 1;
            break;

        case '0':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '1':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '2':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '3':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;
        
        case '4':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '5':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '6':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '7':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '8':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case '9':
            xTaskNotify(ledfunctionTask_hdlr, 3, eSetValueWithOverwrite);
            break;

        case 'k':
            stop = 1;
            break;
      
        }
        
        if (stop)
        {
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    tcsetattr(0, TCSANOW, &initial_settings);
    ENABLE_CURSOR();
    exit(0);
    vTaskDelete(NULL);
}

static void prvTask_led(void *pvParameters)
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {

        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);

    }

    vTaskDelete(NULL);
}

static void prvTask_ledfunction(void *pvParameters)
{
    uint32_t notificationValue;
    uint32_t stop_var_aux = 0; //variável auxiliar 

    for (;;)
    {
        if (xTaskNotifyWait(
                ULONG_MAX,
                ULONG_MAX,
                &notificationValue,
                portMAX_DELAY))
        {
            gotoxy(2, 4);
            if (notificationValue == 1)
            {
                // Caso o usuário pressione a tecla “*”, o LED vermelho deverá ficar aceso e o LED verde deve parar de piscar (apagado);
                stop_var_aux = 0;
            }
            else if (notificationValue == 2)
            {
                // Voltar ao funcionamento normal quando pressionado o “+”!

            }
            else 
            {
                // O LED vermelho deverá piscar (acender por 100ms e apagar) a cada vez que o usuário digitar um número;
                gotoxy(red.pos, 2);
                printf("%s⬤", red.color);
                fflush(stdout);
                vTaskDelay(red.period_ms / portTICK_PERIOD_MS);

                gotoxy(red.pos, 2);
                printf("%s ", BLACK);
                fflush(stdout);
            }
        }
    }
    vTaskDelete(NULL);
}


void app_run(void)
{

    clear();
    DISABLE_CURSOR();
    printf(
        "╔═════════════════╗\n"
        "║                 ║\n"
        "╚═════════════════╝\n");

    
    xTaskCreate(prvTask_led, "LED_green", configMINIMAL_STACK_SIZE, &green, TASK1_PRIORITY, NULL);
    //xTaskCreate(prvTask_getChar, "Get_key", configMINIMAL_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);
    xTaskCreate(prvTask_getChar, "Get_key", configMINIMAL_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks      to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for (;;)
    {
    }
}