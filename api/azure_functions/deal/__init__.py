import logging

import azure.functions as func
import json
from ..common import game

g = game.Game()

def main(req: func.HttpRequest) -> func.HttpResponse: 
  deal = g.deal()

  return json.dumps(deal)
