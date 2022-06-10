#r/bin/env python

import asyncio
import websockets

async def hello():
    async with websockets.connect("wss://cvyykl1zo6.execute-api.us-east-1.amazonaws.com/prod",extra_headers="x-api-key: {zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8}") as websocket:
        #await websocket.send("x-api-key: {zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8}")
        await websocket.send("Heyyyy Matt")
        response = await websocket.recv()
        print(response)
asyncio.run(hello())
