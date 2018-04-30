
# Create a realtime audio waveform based on the pitch and volume with Arduino

## Requirements
- Frequency to voltage converter such as Math Encounters Blog: http://mathscinotes.com/2014/03/a-simple-frequency-to-voltage-converter/
- WS2812B LED strip: https://goo.gl/YoFYP5
- Arduino: https://goo.gl/Ah6GGG

## How it works

Every 1 ms, the frequency(pitch) of the audio is read into the arduino, and the volume is read from the amplified audio signal.
The pitch is then mapped to rgb colors, and the volume is mapped to the brightness. The combination of these values give us unique rgb
values for a single LED. Since we have an array of LEDs we can shift all of them to the right before we update the first LED.
This creates a "memory" effect to display a real time portion of the audio waveform. 

## Note
This code assumes you have a frequency to voltage converter, which operates between 1 - 3 volts during regular music frequencies (roughly 
100 - 600hz). Take note to define the correct data pins if you try to build and use this code for your own builds.

### Watch the video
The entire process was filmed here: https://youtu.be/lU1GVVU9gLU
