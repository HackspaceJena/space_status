# status bot

import json
import os
import time
import urllib.request, json
import datetime
import tweepy
import random

def internet_on(url):
    try:
        urllib.request.urlopen(url)
        return True
    except urllib.error.URLError as e:
        print(e.reason)
        return False


def textselect(min, max):
    number = random.randint(0, 1)

    if number == 0:
        return 0
    else:
        return random.randint(min, max)



while True:

    # starting by configuring the bot
    file_configure_path = "config.json"
    with open(file_configure_path, 'r') as f:
        data_config = json.load(f)

    path = data_config["path"]

    CONSUMER_KEY = data_config["twitter_api"]["consumer_key"]
    CONSUMER_SECRET = data_config["twitter_api"]["consumer_secret"]
    ACCESS_KEY = data_config["twitter_api"]["token"]
    ACCESS_SECRET = data_config["twitter_api"]["token_secret"]
    print(CONSUMER_KEY)

    auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
    auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)
    api = tweepy.API(auth)


    if internet_on("http://twitter.com") == True:

        # finding out the status of the space...
        api_url = data_config["status_api"]["url"]
        with urllib.request.urlopen(api_url) as url:
            data_space = json.loads(url.read().decode())
            #print(data_space["state"]["open"])


        file_path = path + "space.json"
        if os.path.exists(file_path) == True:
            # Reading data back
            with open(file_path, 'r') as f:
                data_space_b4 = json.load(f)
                #print(data_space_b4["state"]["open"])


            if data_space["state"]["open"] != data_space_b4["state"]["open"]:
                print("status changed.")

                # starting by configuring the bot
                file_status_path = path + "status.json"
                with open(file_status_path, 'r') as f:
                    data_status = json.load(f)


                # finding out wich state it change and sending out the text
                if data_space["state"]["open"] == False:
                    print("closing at", time.time(), ",last change happend at", datetime.datetime.fromtimestamp(
                        int(data_space["state"]["lastchange"]) ).strftime('%Y-%m-%d %H:%M:%S'))

                    number = len(data_status["closing_text"])
                    number = textselect(0, number - 1)

                    text = data_status["closing_text"][number]["text"]
                    print("Text:", text)
                    api.update_status(text)
                    # same text as before cannot be posted!

                else:
                    print("opening at", time.time(), ",last change happend at", datetime.datetime.fromtimestamp(
                        int(data_space["state"]["lastchange"]) ).strftime('%Y-%m-%d %H:%M:%S'))

                    number = len(data_status["opening_text"])
                    number = textselect(0, number - 1)

                    text = data_status["opening_text"][number]["text"]
                    print("Text:", text)
                    api.update_status(text)


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