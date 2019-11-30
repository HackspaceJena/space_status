import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


import json
import os
import time
import urllib.request, json
import datetime
import tweepy
import random


def textselect(min, max):
    number = random.randint(0, 1)

    if number == 0:
        return 0
    else:
        return random.randint(min, max)


path = os.path.abspath(__file__)
dir_path = os.path.dirname(path)
print("dir path is", dir_path)

# starting by configuring the bot
file_configure_path = dir_path + os.sep + "config.json"
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


state = 1
state_b4 = 1


while True:
    if GPIO.input(12) == GPIO.HIGH:
        #print("button was pushed:")
        
        state = 1
        
    
    if GPIO.input(12) == GPIO.LOW:
        #print("button was not pushed:")
        state = 0
    
    
    time_now = time.time()
    
    file_status_path = dir_path + os.sep + "status.json"
    #print(file_status_path)
        
    
    if state == 1 and state_b4 == 0:
        
        if os.path.exists(file_status_path) == True:
            with open(file_status_path, 'r') as f:
                    data_status = json.load(f)
        
        number = len(data_status["opening_text"])
        number = textselect(0, number - 1)

        text = data_status["opening_text"][number]["text"] + " [OPEN:" + datetime.datetime.fromtimestamp(
        int( time_now )).strftime('%Y-%m-%d %H:%M:%S') + "]"
                    
        print("Text:", text)
        
        #print("opened")
        #text = "#Krautspace was opened. testmode (if you think I am wrong, report to me this bug/observation!) " + datetime.datetime.fromtimestamp(
        #        int( time_now )).strftime('%Y-%m-%d %H:%M:%S') + ""
                    
        #print("Text:", text)
        try:
            api.update_status(text)
            # same text as before cannot be posted!
        except:
            print(time_now, "did not tweet closing status")
    
    if state == 0 and state_b4 == 1:
        #print("closed")
        
        if os.path.exists(file_status_path) == True:
            with open(file_status_path, 'r') as f:
                    data_status = json.load(f)
        
        number = len(data_status["closing_text"])
        number = textselect(0, number - 1)

        text = data_status["closing_text"][number]["text"] + " [CLOSED:" + datetime.datetime.fromtimestamp(
        int( time_now )).strftime('%Y-%m-%d %H:%M:%S') + "]"
                    
        print("Text:", text)
        
        
        #text = "#Krautspace was closed. testmode (if you think I am wrong, report to me this bug/observation!) " + datetime.datetime.fromtimestamp(
        #                int( time_now )).strftime('%Y-%m-%d %H:%M:%S') + ""
                    
        #print("Text:", text)
        try:
            api.update_status(text)
            # same text as before cannot be posted!
        except:
            print(time_now, "did not tweet closing status")
    
    state_b4 = state
    
    time.sleep(10)