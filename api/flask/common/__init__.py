from . import game

g: game.Game = None

def initGame() -> game.Game:
  global g
  if g is None:
    g = game.Game()

  return g