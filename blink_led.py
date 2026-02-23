import time

def blink_led(pin, state): 
    try: 
        with open('/proc/lll-gpio' , 'w', encoding='utf-8') as file: 
            file.write("{0},{1}".format(pin,state)) 
    except Exception as e: 
        print("Error: {0}".format(e)) 
 
if __name__ == "__main__":
    print("blink script started")
    while True:
        blink_led(21, 0)
        time.sleep(0.5)
        blink_led(21, 1)
        time.sleep(0.5)
