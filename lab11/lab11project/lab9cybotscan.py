
#import serial # If using UART option (i.e., using a physical cable), then make sure serial library has been installed, and uncomment this line.
import time # Time library
#Socket library: https://realpython.com/python-sockets/
#See: Background, Socket API Overview, and TCP Sockets
import socket
import numpy as np
import matplotlib.pyplot as plt
import os
#cybot = serial.Serial('COM100', 115200) # UART (Make sure you are using the correct COM port and Baud rate!!)
HOST = "192.168.1.1" # The server's hostname or IP address
PORT = 288 # The port used by the server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Create a socket object
cybot_socket.connect((HOST, PORT)) # Connect to the socket (Note: Server must first be running)
cybot = cybot_socket.makefile("rbw", buffering=0) # makefile creates a file object out of a socket: https://pythontic.com/modules/socket/makefile
#send_message = "Hello
send_message = input("Enter a message:") + '' # 2) Have user enter text
cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server
angle_degrees = []
angle_radians = []
distance = []
print("Sent to server: " + send_message)
while send_message != 'quit':
#print("wait for server reply")
rx_message = cybot.readline() # Wait for a message, readline expects message to end with ""
#print("Got a message from server: " + rx_message.decode() + "") # Convert message from bytes to String (i.e., decode)
#send_message = input("Enter a message (enter quit to exit):") + '' # Enter next message to send to server
print("WROTE")
cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server
for i in range(91):
data = (cybot.readline()).split() # Split line into columns (by default delineates columns by whitespace)
angle_degrees.append(float(data[0])) # Column 0 holds the angle at which distance was measured
distance.append(float(data[1])/100)
angle_radians = np.deg2rad(angle_degrees)
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}) # One subplot of type polar
ax.plot(angle_radians, distance, color='r', linewidth=4.0) # Plot distance verse angle (in radians), using red, line width 4.0
ax.set_xlabel('Distance (m)', fontsize = 14.0) # Label x axis
ax.set_ylabel('Angle (degrees)', fontsize = 14.0) # Label y axis
ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
ax.tick_params(axis='both', which='major', labelsize=14) # set font size of tick labels
ax.set_rmax(2.5) # Saturate distance at 2.5 meters
ax.set_rticks([0.5, 1, 1.5, 2, 2.5]) # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
ax.set_rlabel_position(-22.5) # Adjust location of the radial labels
ax.set_thetamax(180) # Saturate angle to 180 degrees
ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
# Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
ax.grid(True) # Show grid lines
# Create title for plot (font size = 14pt, y & pad controls title vertical location)
ax.set_title("Mock-up Polar Plot of CyBot Sensor Scan from 0 to 180 Degrees", size=14, y=1.0, pad=-24)
plt.show() # Display plot
send_message = ' '

print("Client exiting, and closing file descriptor, and/or network socket")
time.sleep(2) # Sleep for 2 seconds
cybot.close() # Close file object associated with the socket or UART
cybot_socket.close() # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
