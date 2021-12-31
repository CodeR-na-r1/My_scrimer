import requests, json

acces_token  = "034d63490d6c4b113e9292fb39d7ce0f2cc4059c05d7fcfd2cc6110a5b325f36f4276bd31d54ace845654"
url_1 = "https://api.vk.com/method/status.get?"
url_2 = "https://api.vk.com/method/status.set?"

params_1 = {
    'access_token' : acces_token,
    'user_id' :  '160845091',
    'v' : '5.131'
}

params_2 = {
    'access_token' : acces_token,
    'text' :  '2',
    'v' : '5.131'
}

request = requests.post(url_1, params=params_1)
response = request.json() # Распаковка ответа

res = 0
volume = 100

try:
    res = str(response['response']['text'])
    #print(res)
except:
    file_out = open("answ.data", 'w')
    file_out.write("0 0")
    file_out.close()
    SystemExit(0)

if (len(res.split(' ')) > 1):
    volume = res.split(' ')[1]
res = res.split(' ')[0]

if (int(res) == 1):
    request = requests.post(url_2, params=params_2)

file_out = open("answ.data", 'w')
file_out.write(str(res) + ' ' + str(volume))
file_out.close()
SystemExit(int(res))
