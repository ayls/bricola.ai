import logging

import azure.functions as func
import json
from ..common import *

def main(req: func.HttpRequest) -> func.HttpResponse: 
  g = initGame()
  deal = g.deal()

  return json.dumps(deal)
