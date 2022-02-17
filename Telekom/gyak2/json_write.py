import json

data = {
    "date":"2021.09.13",
    "system":"Windows",
    "pings":["google.hu","blabla"],
    }

with open('ping.json', 'w') as f:
    json.dump(data,f)