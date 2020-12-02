import logging

import azure.functions as func
import json
from ..common import game

g = game.Game()

def main(req: func.HttpRequest) -> func.HttpResponse:        
    body = req.get_json()

    response = g.play(body)

    return json.dumps(response)
