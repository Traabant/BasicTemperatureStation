# BasicTemperatureStation
Small tempreture sensor, bulid with DEMOS D1 mini and MPB280. It connects to wifi, reads temp from sensor and POST data to my own website

## RUNTIME
Program is run i 5 minut loop. Before it checks the temp, IT checks if the sensor and Wife are still concected, if not reset the chip. 

## Data format
API exectes data in format 
{'temperature': ['22.6'], 'pressure': ['859'], 'room': ['bedroom']}
