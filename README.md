# Trafic-Datalogger
Datalogger for trafic control programmed in C for DSPIC 33FJ256GP710  Microchip microcontroller 

This is the final project from "Computer Architecture" course in National University of Patagonia San Juan Bosco

The full project description and the code comments are in Spanish, for so was required by the course teachers

The program is used to control trafic in a single lane of the road, using two piezoelectrics sensors perpendicular to the flow of cars and spaced 30 cm apart. When the sensors detect the presence of a vehicle the program stores the time of the day, te amount of axles and the speed of the vehicle. If the speed is higher than 60km/h, a camera is activated and a photo of the vehicle plate is taken to later issue a fine. The microcontroller is conected to a computer that can send comands to the program to request information abot the ammount of vehicles that passed through the sensors in a certain hour, the ammount of vehicles of more than 3 axles or to delete the whole register of vehicles. Both the command and the information returned are transmitted using a communication protocol which is further described in the "Full Project Description" file.
