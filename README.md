# VRS - Slnečník

#### Cieľ projektu je pomocou servo motora a viacsegmentového svetelného senzora vyčítať intenzitu okolitého svetla, a otáčať svetelný senzor servo motorom tak, aby ju svetlo zakryl zo správnej strany. Pomocou STM32F303K8 mikrokontrolera sa budú spracovať dáta zo senzora. Servo motor bude senzor otáčať podľa dát na správnú stranu.

### Servo system

We set the counter period 9999 for the PWMM, that way we can change the pulse from 0 to 9999. 0 being 0% and 9999 being 100% pulse width.
With the help of HAL_TIM_SET_COMPARE() macro, we can set the pulse on while the prgoram is running.

For the servo motor we needed 50 Hz signal
The clock frequency = 8 000 000 Hz
Prescalor = 16
Frequency = 8 000 000 / 16 = 500 000 Hz
Then we can devide again by Counter period which is 10 000
The frequency is 500 000 / 10 000 = 50 Hz
The full pulse with time 1/50 seconds which is 20 milli seconds, We had no exact datasheet about the Futaba S3151 servo motor, but we assumed, that the servo motor we need to change the pulse from 0.5 ms to 2.5 ms.
The Futaba S3151 servo motor can rotate 120°, after working with the servo motor, we picked the pulse value: ~400 for 0° and the pulse value: ~1050 for 120°.
