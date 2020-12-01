import math

aiPlayerIndex = 0
humanPlayerIndex = 1

def determineCollectingPlayer(f_Player, f_Card, s_Card, b_Card):
    w_player = None

    s_Player = 1 - f_Player
    _, f_CardType, f_CardValue = getCardInfo(f_Card)
    _, s_CardType, s_CardValue = getCardInfo(s_Card)
    _, b_CardType, _ = getCardInfo(b_Card) 
    if (f_CardType == b_CardType and s_CardType != f_CardType):
        w_player = f_Player
    elif (s_CardType == b_CardType and f_CardType != s_CardType):
        w_player = s_Player
    elif (s_CardType != f_CardType):
        w_player = f_Player
    elif (f_CardValue > s_CardValue):
        w_player = f_Player
    elif (s_CardValue > f_CardValue):
        w_player = s_Player 
    elif (f_Card > s_Card):
        w_player = f_Player 
    else:           
        w_player = s_Player  

    return w_player

def getCardInfo(card):
    type = math.floor(card / 10)
    value = (11 if (card % 10 == 0) else (10 if (card % 10 == 2) else (2 if (card % 10 == 7) else (3 if (card % 10 == 8) else (4 if (card % 10 == 9) else 0)))))

    return card, type, value