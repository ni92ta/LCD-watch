# LCD-watch
The LCD-based clock is designed from scratch. The clock shows the current time with seconds. There is an alarm clock and date display.
The software is written in C in the MpLab development environment. 
The circuit diagram is shown in the figure below:
![image](https://user-images.githubusercontent.com/91215296/182890300-d24ab3b1-aefc-43da-a5db-648a6b27b949.png)
The clock is based on the PIC16F628A. This microcontroller processes the button presses, reads the time information, prepares and sends the data for displaying on the LCD. The HT1621B chip is the LCD driver. This watch uses a 1/3 multiplex LCD.
The printed circuit board (shown in the figure below) was designed in Altium Designer CAD. It consists of two parts that are connected to each other by wires.
![image](https://user-images.githubusercontent.com/91215296/168636123-9e992db8-8a6d-4999-8ea8-6adf94ad18ac.png)
