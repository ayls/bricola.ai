import logging

import azure.functions as func
import json
from ..common import game

g = game.Game()

# def initializeGame():
#   # if g is None:
#   g = Game()

def main(req: func.HttpRequest) -> func.HttpResponse:
  # initializeGame()
  
  deal = g.deal()

  return json.dumps(deal)
