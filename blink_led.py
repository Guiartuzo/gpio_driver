import time

def blink_led(state): 
    try: 
        with open('/sys/devices/platform/mygpio/led' , 'w', encoding='utf-8') as file: 
            file.write("{0}".format(state)) 
    except Exception as e: 
        print("Error: {0}".format(e)) 
 
if __name__ == "__main__":
    print("blink script started")
    while True:
        blink_led(0)
        time.sleep(0.5)
        blink_led(1)
        time.sleep(0.5)
