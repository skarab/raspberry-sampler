# raspberry-sampler
Sampler for the Raspberry 3 

Key concept is to have a box full of knobs to live-edit samples, linked to other instruments using midi. 

Note this is WIP, the design of controllers may change, the box isn't designed yet. 

soundcloud sample: https://soundcloud.com/stroodge/rasperry-sampler-filters

youtube sample: https://www.youtube.com/watch?v=87chFwOwE38


-----------------------------------

Manual 


![alt text](https://raw.githubusercontent.com/skarab/raspberry-sampler/master/schema.png)
 
(0) display current edited value 
 
Bank : 

(1) select bank, basically a folder full of wavs in the main samples/ directory 

(2) lights up if bank is loaded 

(3) load or unload the bank 

(4) save the bank (all parameters & midi settings) 
 

Sample : 

(5) select sample, a 16bits 44100Hz WAV file inside the bank folder 

(6) switch mode : 
   
   0. one shot   : just play it til the end (default) 
   1. loop       : one press to start the loop, one press to stop it 
   2. key        : stop as soon the key is released 
   3. key loop   : loop til the key is released 
   4. instru key : use only midi device and pitch follow the notes 
   5. instru OS  : play notes as one shot samples 
   6. ins. legato: play instru legato 

(7) midi attach/detach : 
   - tap it one time, then the next received midi key is attached 
     note: a sample has a single attachment 
     If you switch another control before a note is received, it just cancel the tap 
   - tap it two time to detach it 

(8) play 
 
(9) select parameters to assign to the (10) knobs : 
 
    0.  Volume      Pan            Stereo Amplifier  Pitch, semi tone  Pitch, fine tune  Legato
    1.  BitCrusher  Distortion     Overdrive         Formant filter    Noise amount      Noise pinky
    2.  EQ Low      EQ Medium      EQ High           LP Moog CutOff    HP CutOff         Resonance
    3.  Loop Start  LStart Ftune   LStop Ftune       Loop Stop         Loop Delay        Loop Delay Env
    4.  Start       Start FTune    Env Attack        Env Release       Stop FTune        Stop
 
 
-----------------------------------
 
Hardware parts : 
 - Raspberry 3 
 - SD card 
 - 7-segment LED HT16K33 Backpack 
 - x9 rotary encoder (Electronics AB 9302540010), remove the guts for the parameters knobs (smoother) 
 - 1 led 
 - 5 buttons 
 - cables, proto board, cobbler 
 

