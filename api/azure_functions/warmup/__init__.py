import logging
import azure.functions as func
from ..common import *

def main(mytimer: func.TimerRequest) -> None:
  initGame()  
  logging.info('API warmed up')
