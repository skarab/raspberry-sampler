# raspberry-sampler
Sampler based on Raspberry Pi 3 B

Key concept is to have a box full of knobs to live-edit samples, linked to other instruments using midi.
Note this is WIP, the design of controllers may change, the box isn't designed yet.

-----------------------------------

Features

- stereo & polyphonic, up to 12 voices
- 44100Hz 16bits realtime playback
- jack output
- usb midi input, supporting multiple devices
- up to X midi attachments
- support up to X banks containing X samples
- one shot, loop and instrument play modes
- Per-sample stereo amplifier, distortion  & bitcrusher filters
- Global equalizer, low pass moog, high pass, noise, overdrive, distortion & formant filters
- 8 Knobs, 4 buttons, 1 play button, 1 parameters switch, 3 leds
- sampler software and samples on an usb key, allowing easy updates

-----------------------------------

Links

http://www.kstorm.org

https://www.youtube.com/watch?v=qFAxQr971z0

https://www.youtube.com/watch?v=87chFwOwE38

-----------------------------------

Manual


![alt text](https://raw.githubusercontent.com/skarab/raspberry-sampler/master/schema.png)

(0) display current edited value

Bank :

(1) select bank, basically a folder full of wavs in the main samples/ directory

    The bank 0 is the global bank : first sample contains the global parameters, main volume, filters, ... next samples are the current used samples (attached to midi), note this list is updated when switching bank so you can detach/re-attach used samples.

(2) lights up if bank is loaded

(3) load or unload the bank - if on global bank : stop all samples

(4) save the bank (all parameters & midi settings) - if on global bank : detach all samples


Sample :

(5) select sample, a 16bits 44100Hz WAV file inside the bank folder

(6) switch mode :

   0. one shot   : just play it til the end (default)
   1. loop       : one press to start the loop, one press to stop it
   2. instrument : use whole midi device, pitch linked to notes

(7) midi attach/detach :
   - tap it one time, then the next received midi key is attached.
     Note: a sample has a single attachment.
     If you switch another control before a note is received, it just cancel the tap.
   - tap it two time to detach it

    The green LED displays if the sample is attached.

(8) play the sample, the red LED displays if the sample is playing

(9) select parameters to assign to the (10) knobs :

 Global params:

    0.  Volume        Pan            Stereo          EQ Low          EQ Medium         EQ High
    1.  LP CutOff     LP Resonance   HP CutOff       HP Resonance    --                --
    2.  Noise         Noise Texture  Motor           Motor Texture   Motor Speed       Noise Mixer

 Sample params:

    0.  Volume        Pan            Stereo          Formant         Distortion        BitCrusher
    1.  Start         Env Attack     Env Release     Stop            Pitch semi-tone   Pitch fine-tune
    2.  LoopStart     Loop Attack    Loop Release    LoopStop        Silence           Legato


-----------------------------------

Hardware parts :
 - Raspberry 3 B
 - SD card
 - 7-segment LED HT16K33 Backpack
 - x8 rotary encoder (Electronics AB 9302540010), remove the guts for the parameters knobs (smoother)
 - 1 switch - 3 positions
 - 3 led
 - 4 buttons
 - 1 play button
 - cables, proto board, cobbler


