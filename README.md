# BrushHero
The world's first dental hygiene based port of Guitar Hero 3: Legends of Rock™


## Inspiration
PickHacks 2021 is all about entertainment, so we asked ourselves "How can we take an ordinary task and make it exciting?" The answer, or course, is simple. Make it into a game! 

With that in mind, we made what every sensible person has ever wanted... 

*BrushHero — the world's first dental hygiene based version of Guitar Hero 3!*

## What it does
BrushHero brings entertainment to cleaning your teeth by combining the routine motions of brushing your teeth with the challenge of rhythm gaming. 

To get started, connect the toothbrush to your bluetooth compatible device. Then, use our script and you've got a functional game controller!

The controls are simple! Press the button on the toothbrush corresponding to notes in the game and start brushing! The toothbrush is motion controlled, make a brushing motion and you'll play the note in game.

## How we built it
The brush controller was built by hollowing out an electric toothbrush and using it to house as much of the electronics as we could fit. In the brush we used:
 - Arduino Nano
 - Custom button mounting bracket that we 3D printed
   - Models created in Fusion 360 
 - Accelerometer (Model: GY-521 MPU-6050)
 - Bluetooth Receiver/Transmitter (Model ZS-040 HC-06)

The computer that connects to the toothbrush uses a Python script to communicate with the brush and translate the data into keyboard inputs. The major libraries used when developing this part of the project include:
  - `pybluez`, for bluetooth functionality
  - `numpy`, for calculations to correct corrupted bluetooth data
  - `pandas` and `matplotlib`, for analyzing data to fine-tune parameters used by the toothbrush

The actual gameplay is done on the host computer by playing Clone Hero, a free Guitar Hero clone with large community backing it. Technically, the brush could be used for any game, but we figured most people are familiar with Guitar Hero. 

We decided not to build our own game alongside the controller because we wouldn't have enough time to do both.

## Challenges we ran into (and how we solved them!)

##### Accelerometers are wacky
*Problem*

It is *not* easy to say when someone has definitely started and finished a single motion of brushing, especially with a $2 accelerometer. This problem manifested in a variety of forms.
 1. The data is noisy — We don't care how steady you think your hand is, the data doesn't lie!
 2. Hardware inconsistency — We found our accelerometer isn't quite perfect. Depending on the direction we moved the brush, the overall magnitude wasn't always consistent. The data is lying!
 3. Changing orientations — The toothbrush can be moved freely, and is moved in different ways when it is used.
 4. Gravity! — Accelerometers show values for gravity. Combined with a changing orientation it is difficult to tell how much of an acceleration along an axis is from motion and how much is from gravity.
 
*Our Solution*

Our first step to solving these problems came in carefully mounting our accelerometer so brushing-like motions alligned with the more sensitive axes of the accelerometer. This helped remove some noise and our signals became much more clear!

However, more was needed to properly detect a brushing motion. Our first attempt was to get the magnitude of the acceleration, but the aforementioned gravitational field stopped us in our tracks. We got around this calculating the derivitive of the acceleration (called the *jerk*) which eliminated the constant force of gravity. Then, we looked at the jerk along different axes to detect different types of brushing motions. 

##### Toothbrushes are small
*Problem*
It's pretty self explanatory. Lots of hardware, little space.

*Solution*
Make space when we can. Be creative when we can't.

##### We had the wrong bluetooth module!
*Problem*
The bluetooth module we had access to was not capable of acting as an HID device, so we couldn't have the toothbrush act directly as a gamepad.

*Solution*
We made a seperate Python script on the host computer that read data serially from the toothbrush and converts the data into keystrokes.

##### 3D Printing
*Problem*
3D printing can take a long time! We went through several iterations of the mounting bracket for the buttons, each taking about 2 hours to print.

*Solution*
We had prototypes running using breadboards in the meantime so we could continue testing software while things printed.

## Accomplishments that we're proud of

We finished a software and hardware hack! 

This was a new style of project for both of us, we had never worked on hardware during a hackathon before so the pacing was much different. Also, due to the small form of the brush there were several design contraints that we had to overcome. There's room for improvement, but we're happy with the design.

## What we learned
There was a lot to learn from this project. We learned about bluetooth specs, finicky hardware and so much more! It's been a great learning experience and we definitely know more than we did just a few days ago.

## What's next for BrushHero
Mass production and available in stores near you.

