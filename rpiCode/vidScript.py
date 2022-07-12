import asyncio
import websockets
from picamera import PiCamera
from io import BytesIO
import cv2
import time


LOCAL_ADDRESS = "localhost/machine"
REMOTE_ADDRESS = "44.206.189.200/machine"

HEADERS = {"x-api-key": "zQXx6dS25g1osj74gXAL51nAmdAOBANL2gAvR3O8"}

def mainFunc():
    asyncio.run(connect())

async def connect():
    async with websockets.connect("ws://" + REMOTE_ADDRESS, extra_headers=HEADERS) as ws:
        await streamCamera(ws)

async def streamCamera():
    buffer = BytesIO()
    camera = PiCamera(resolution=(640,480))
    for foo in camera.capture_continuous(buffer,format='jpeg'):
        buffer.truncate()
        buffer.seek(0)
        bufferJ = cv2.imread(buffer)
        cv2.imshow("Current buffer",bufferJ)
        time.sleep(1)
        cv2.destroyWindow("Current buffer")
    camera.close()

if __name__=="__main__":
    mainFunc()

