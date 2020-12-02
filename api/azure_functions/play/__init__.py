import logging
import azure.functions as func
import json
from ..common import *

def main(req: func.HttpRequest) -> func.HttpResponse:        
  g = initGame()  
  body = req.get_json()
  response = g.play(body)

  return json.dumps(response)
