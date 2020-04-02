"""
The following code is written for DHT11 (Digital Humidity & Temperature).
These sensors are low cost digital sensors with capacitive humidity 
sensors and thermistors to measure the surrounding air.

Note that the :
Temperature is returned in degree Celsius.
Humidity is percentage of relative humidity. 

Also this script takes reading from the sensor and uploads the same to ThingsSpeak Cloud.
ThingSpeak is an IoT analytics platform service that allows you to aggregate, visualize, 
and analyze live data streams in the cloud. We can use it to send data to ThingSpeak from
your devices, create instant visualization of live data, and send alerts.
"""
import dht
import ussl as ssl
import machine

try:
    import usocket as socket
except:
    import socket

# a template of HTTP request to ThingSpeak to post temperature and humidity
THINGSPEAK_POST_TEMPLATE = """
POST /update HTTP/1.1
Host: api.thingspeak.com
Connection: close
X-THINGSPEAKAPIKEY: %s
Content-Type: application/x-www-form-urlencoded
Content-Length: %d
%s
"""

DHT22_PIN = 12
API_THINGSPEAK_HOST = 'api.thingspeak.com'
API_THINGSPEAK_PORT = 443
THINGSPEAK_WRITE_KEY = '...' # write your key here

# timings in seconds
MESUREMENT_INTERVAL = 100 

def mesure_temperature_and_humidity():
    d = dht.DHT11(machine.Pin(DHT22_PIN))
    d.measure()
    t = d.temperature()
    h = d.humidity()
    print('temperature = %.2f' % t)
    print('humidity    = %.2f' % h)

    global THINGSPEAK_WRITE_KEY
    if not THINGSPEAK_WRITE_KEY:
        print('not ThingSpeak key specified, skip sending data')
        return

    print('send data to ThingSpeak')
    s = socket.socket()
    ai = socket.getaddrinfo(API_THINGSPEAK_HOST, API_THINGSPEAK_PORT)
    addr = ai[0][-1]
    s.connect(addr)
    s = ssl.wrap_socket(s)
    data = 'field1=%.2f&field2=%.2f' % (t, h)
    http_data = THINGSPEAK_POST_TEMPLATE % (THINGSPEAK_WRITE_KEY, len(data), data)
    s.write(http_data.encode())
    s.close()

while True:
    current_time = time.time()
    if current_time - last_mesurement_time > MESUREMENT_INTERVAL:
        mesure_temperature_and_humidity()
        last_mesurement_time = current_time
    time.sleep(DELAY)
