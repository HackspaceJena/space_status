# status bot

import json
import os
import time
import urllib.request, json
import datetime

def internet_on(url):
    try:
        urllib.request.urlopen(url)
        return True
    except urllib.error.URLError as e:
        print(e.reason)
        return False


while True:

    # starting by configuring the bot
    file_configure_path = "config.json"
    with open(file_configure_path, 'r') as f:
        data_config = json.load(f)

    if internet_on("http://twitter.com") == True:

        # finding out the status of the space...
        api_url = data_config["status_api"]["url"]
        with urllib.request.urlopen(api_url) as url:
            data_space = json.loads(url.read().decode())
            #print(data_space["state"]["open"])



        file_path = "space.json"
        if os.path.exists(file_path) == True:
            # Reading data back
            with open(file_path, 'r') as f:
                data_space_b4 = json.load(f)
                #print(data_space_b4["state"]["open"])

            if data_space["state"]["open"] != data_space_b4["state"]["open"]:
                print("status changed.")

                # starting by configuring the bot
                file_status_path = "status.json"
                with open(file_status_path, 'r') as f:
                    data_status = json.load(f)


                # finding out wich state it change and sending out the text
                if data_space["state"]["open"] == False:
                    print("closing at", time.time(), ",last change happend at", datetime.datetime.fromtimestamp(
                        int(data_space["state"]["lastchange"]) ).strftime('%Y-%m-%d %H:%M:%S'))

                    print("Text:", data_status["closing_text"][0]["text"])

                else:
                    print("opening at", time.time(), ",last change happend at", datetime.datetime.fromtimestamp(
                        int(data_space["state"]["lastchange"]) ).strftime('%Y-%m-%d %H:%M:%S'))

                    print("Text:", data_status["opening_text"][0]["text"])


            else:
                print("status is still the same at", time.time(), datetime.datetime.fromtimestamp(
                    int(data_space["state"]["lastchange"]) ).strftime('%Y-%m-%d %H:%M:%S'))

        else:
            print("welcome to your initial round")


        # saving the current space status just too compare them in the next round
        with open(file_path, 'w') as outfile:
            json.dump(data_space, outfile, indent=4)

    else:
        print(time.time(), "internet is off")

    # how often shall we check for status?
    time.sleep(data_config["status_api"]["checking_period"])