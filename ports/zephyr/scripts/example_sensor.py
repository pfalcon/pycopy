import utime
import zsensor
s = zsensor.Sensor("HTS221")

while True:
    s.measure()
    print("Temp:", s.get_float(zsensor.TEMP))
    utime.sleep(1)
