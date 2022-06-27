#r/bin/env python

from websocket import create_connection


ws = create_connection("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod",header={'x-api-key':'zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8'})
ws.send("Heyyyy Matt")
response = ws.recv()
print(response)
ws.close()
