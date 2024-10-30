# morsekey
Decode Morse Code tapped into an Arduino with a straight (telegraph) key.  Details of the hardware
and more are documented at https://egb13.net/morse-code-decoder - except that site is currently down.
But here is [a video demonstration](https://youtu.be/MwUTLFZVcHY?si=3VG576jMB5DCvc94) of it in use.

Read dits and dahs on an Arduino pin connected to a telegraph key, decode the Morse Code, and display
the text on an LCD 16x2 character display.

Speed of expected input is read from a single digit wheel that outputs its current value on 4 pins
that are read as buttons.  TODO: Come up with a way to dynamically adjust the speed just like you do
when listening by ear.

The AdaFruit LiquidCrystal library is used to interface to the display using i2C and only 4 pins.

This code works but is rough.  TODO: Rewrite to use interrupts on the key input for better timing.
