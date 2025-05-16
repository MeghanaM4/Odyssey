# Odyssey

> a project for [apex.hackclub.com](https://apex.hackclub.com) by Meghana Madiraju and help from Kieran Klukas

I have been in love with space for as long as I can remember, so when I heard Hack Club was letting teenagers send whatever they want 113,000 feet in the air I knew I had to do it. We get $100 per person to make an invention that'll go onto a high altitude balloon (HAB), and if you spend 30 hours on it you'll get to go to Boston for a hackathon where everyone will launch their payloads at the same time and make cool stuff.

## Quick Overview (what we're sending up)
- Sensor data from SMD components on PCB (stored on a microSD card)
- PCB powered by AA batteries that can survive -45C
- Internal camera that can see outside through a polycarbonate window on the wall
- 7-segment display that shows the current altitude, which the int. camera can also see
- Bouncy balls so we can test if the low pressure affected their bouncification index (technical term)

## Collected data
- Pressure
- Magnetic field (3-axis)
- Internal temperature of payload
- Gyroscopic data (3-axis)

## Component Stack
| Component | Purpose |
| --------- | ------- |
| Seeed Studio XIAO RP2040 | Main microcontroller |
| MS560702BA03-50 | Barometer |
| MPU-6050 | Gyroscope/accelerometer |
| ALS31313KLEATR-1000 | Magnetometer |
| Mini Body Cam | Internal view of payload |      

View the complete BOM [here](https://docs.google.com/spreadsheets/d/1y77u3fouo9j1fYMRJqijLE8DN1v_GkXtAQhd4oCJCtc/edit?usp=sharing)          
<br>
## PCB
<img src="/imgs/odysseyPCBview.png" alt="Odyssey PCB CAD rendering" style="height: 300px; width:500px;"/>             
<br>

## Timeline
| Phase | Status (%)|
| ------ | ------ |
| Research + Planning | 95% |
| Designing PCB and electronics layout | 90% | 
| Building payload | 0% |
| Coding firmware | 1% |
| Final tests and edits | 0% |
| Launch | 0% |

## Design Process
My first sketch of Odyssey looked like this:
<br>
<img src="/imgs/odysseyv1drawing.png" alt="Odyssey v1 drawing" style="height: 300px; width:300px;"/>
<br>
The main PCB (the green square artfully drawn in the center, complete with blue copper traces) with the XIAO and SMDs would process and collect data. The 4 digit 7-segment display displays the altitude of the payload (math that would be calculated from the barometer), and would sit in front of an internal camera (the purple box) so the altitude could be captured in the video feed. The internal camera would also look outside via the polycarbonate window that I am going to steal from my robotics team's shop (sorry Mr. S).      
The camera on top was originally meant to capture the sky, but then I found out all the payloads would be aligned on top of eachother candy-necklace style, so I scrapped that pretty quick.

The second sketch had some improvements:
<br>
<img src="/imgs/odysseyv2drawing.png" alt="Odyssey v2 drawing" style="height: 300px; width:300px;"/>
<br>

I thought that we would need 3 PCBs: one for the main sensors, one for the LCD screen and 7-seg display, and one for the pyranometer. What's overkill? This obviously was taken out after I thought about a bit more (see: overkill) and after I saw the customs and duties taxes. The yellow stuff you see on the walls is my artistic interpretation of spray insulation.      

I really wanted our data to be space-focused, so getting a pyranometer to measure cosmic ray data was at the top of my list. Unfortunately, the Apogee Instrument component I found was a massive pain to work with, so I decided to just scrap it. If I get the basic functionality of everything else working, then I'll look into aurora borealis or radiation detection. The challenge is that those have to be on the outside of the payload, or at least shielded solely by polycarb or some other low-z window material.

I also drew a sketch of what I wanted the internal view to be:
<br>
<img src="/imgs/odysseyintview1.png" alt="Odyssey internal camera view v1" style="height: 250px; width:500px;"/>
<br>

Here's an updated internal view with a smaller box and an updated layout:
<br>
<img src="/imgs/odysseyintview2.png" alt="Odyssey internal camera view v2" style="height: 375px; width:500px;"/>
<br>

## Big thank you to
-My teachers for putting up with my late work      
-My parents for letting me work on this instead of school       
-Kieran for answering all my questions        
