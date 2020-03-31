"""
The following code is written for DHT11 (Digital Humidity & Temperature).
These sensors are low cost digital sensors with capacitive humidity 
sensors and thermistors to measure the surrounding air.

Note that the :
Temperature is returned in degree Celsius.
Humidity is percentage of relative humidity. 
"""

import dht
import machine

#Replace dht.DHT11 by dht.DHT22 for DHT22 sensor
d = dht.DHT11(machine.Pin(4))

#For measuring values
Measure = d.measure()
Temperature = d.temperature()
Humidity = d.humidity()

