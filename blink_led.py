import time


def blink_led(interval):
    print("blink_led() called")
    try:
        with open('/proc/lll-gpio' , 'w', encoding='utf-8') as file:
            while(true):
                print("blinking led...")
                file.write("21,1")
                time.sleep(interval)
                file.write("21,0")
    except Exception as e:
        print("Error: {0}".format(e))

if __name__ == "__main__":
    print("blink script started")
    blink_led(0.5)
