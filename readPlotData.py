import serial
import matplotlib.pyplot as plt
import time

# Set up Serial
ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

# Lists to store data
timestamps = []
left_sensor = []
right_sensor = []
left_motor_speed = []
right_motor_speed = []

start_time = time.time()
trial_duration = 10

while time.time() - start_time < trial_duration:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        data = line.split(",")
        if len(data) == 5:
            timestamps.append(int(data[0]))
            left_sensor.append(int(data[1]))
            right_sensor.append(int(data[2]))
            left_motor_speed.append(int(data[3]))
            right_motor_speed.append(int(data[4]))

# Close the Serial port
ser.close()

# Plotting
plt.figure(figsize=(12, 6))

# Plot sensor data
plt.subplot(2, 1, 1)
plt.plot(timestamps, left_sensor, label='Left Sensor', color='blue')
plt.plot(timestamps, right_sensor, label='Right Sensor', color='orange')
plt.title('IR Sensor Data')
plt.xlabel('Time (ms)')
plt.ylabel('Sensor Value')
plt.legend()
plt.grid()

# Plot motor speeds
plt.subplot(2, 1, 2)
plt.plot(timestamps, left_motor_speed, label='Left Motor Speed', color='green')
plt.plot(timestamps, right_motor_speed, label='Right Motor Speed', color='red')
plt.title('Motor Speeds')
plt.xlabel('Time (ms)')
plt.ylabel('Motor Speed')
plt.legend()
plt.grid()

plt.tight_layout()
plt.savefig('output_plot.png')
plt.close()