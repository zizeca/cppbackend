import requests

addr = "http://127.0.0.1:8080"
req = ["/api/v1/maps", "/api/v1/maps/map1"]
count = 0
for i in req:
  count +=1
  print("______test", count, "______")
  r = requests.get(addr + i)
  print("request =", addr + i)
  print("Satus code =", r.status_code)
  for k in r.headers:
    print(k, r.headers[k] , sep=' "', end='"\n' )
  #print("body=", r.json())  
  print(r.text)
