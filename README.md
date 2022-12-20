# VRS - Slnečník

Cieľ projektu je pomocou servo motora a viacsegmentového svetelného senzora vyčítať intenzitu okolitého svetla, a otáčať svetelný senzor servo motorom tak, aby ho svetlo zakrylo zo správnej strany. Pomocou STM32F303K8 mikrokontrolera sa budú spracovať dáta zo senzora. Servo motor bude senzor otáčať podľa dát na správnú stranu.

Na začiatku systém spraví tzv. mapovanie, senzor s pomocou motorčeka prebehne všetky uhly v rozmedzí 0°-180° s krokom 5°, zapamätá si, ktorá pozícia mala najväčšiu intezitu svetla a nastaví sa do nej.

Ďalej systém už len každých 15 sekúnd kontroluje danú pozíciu, pozíciu o 5° menej a pozíciu o 5° viac a nastaví sa na uhol, ktorý má najväčšiu intenzitu.

## Light sensor

We used multispectral sensor **VD6283TX**. </br>
It could get information from different kinds of light, *RED, VISIBLE, BLUE, GREEN, IR, CLEAR*, we only needed information from visible channel.

## SERVO 

We set the counter period 9999 for the PWMM, that way we can change the pulse from 0 to 9999. 0 being 0% and 9999 being 100% pulse width.
With the help of HAL_TIM_SET_COMPARE() macro, we can set the pulse on while the prgoram is running.

For the servo motor we needed 50 Hz signal </br>
The clock frequency = 8 000 000 Hz </br>
Prescalor = 16 </br>
Frequency = 8 000 000 / 16 = 500 000 Hz </br>
Then we can devide again by Counter period which is 10 000 </br>
The frequency is 500 000 / 10 000 = 50 Hz </br>
The full pulse with time 1/50 seconds which is 20 milli seconds, We had no exact datasheet about the SG90 servo motor, but we assumed, that the servo motor we need to change the pulse from 1 ms to 2 ms.
The SG90 servo motor can rotate 180°, after working with the servo motor, we picked the pulse value: ~180 for 0° and the pulse value: ~1180 for 180°.
