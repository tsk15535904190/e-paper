import configparser
import requests
import json

def get_weather_info():
    config = configparser.ConfigParser()
    config.read('./config.ini',encoding='utf-8')
    key = config.get('weather', 'weather_key')
    api = config.get('weather', 'weather_api')
    api = api.replace("weather_key", key, 1)

    response = requests.get(api)
    if response.status_code == 200:
        data = response.json()
        results  = data.get("results", [])

        location_info = results[0].get("location", {})['name']
        today_daily_forecasts = results[0].get("daily", [])[0]
        tomorrow_daily_forecasts = results[0].get("daily", [])[1]
        ret_info = {'location':location_info, 'today_daily_forecasts': today_daily_forecasts,'tomorrow_daily_forecasts':tomorrow_daily_forecasts}
        return ret_info
    else :
        return 0

def get_word_of_day():
    config = configparser.ConfigParser()
    config.read('./config.ini',encoding='utf-8')
    api = config.get('word of day', 'word_api')
    response = requests.get(api)
    if response.status_code == 200:
        data = response.json()
        ret_info = {'payload':data['hitokoto'],'from':data['from']}
        return ret_info
    else:
        return 0
