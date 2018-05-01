import common
import numpy as np

def determineFinalSixPlay(ai_CardsInHand, human_CardsInHand, f_Player, f_Card, b_Card):
    playedCard = None

    if (f_Player == common.aiPlayerIndex):
        _, playedCard = __playFinalSixSteps(f_Player, ai_CardsInHand, human_CardsInHand, -1, b_Card)
    else:
        _, playedCard = __playFinalSixSteps(f_Player, human_CardsInHand, ai_CardsInHand, f_Card, b_Card)

    return playedCard

def __playFinalSixSteps(f_Player, f_CardsInHand, s_CardsInHand, f_Card, b_Card):
    playedCard = None
    totalWonPoints = 0
    maxWonPoints = 0

    f_CardsInHandToTry = f_CardsInHand
    if (f_Card != -1):
        f_CardsInHandToTry = [f_Card]

    for s_CardToTry in s_CardsInHand:
        _, _, s_CardValue = common.getCardInfo(s_CardToTry)
        for f_CardToTry in f_CardsInHandToTry:
            _, _, f_CardValue = common.getCardInfo(f_CardToTry)

            w_Player = common.determineCollectingPlayer(f_Player, f_CardToTry, s_CardToTry, b_Card)     

            f_NewCardsInHand = __removePlayedCard(f_CardsInHand, f_CardToTry)
            s_NewCardsInHand = __removePlayedCard(s_CardsInHand, s_CardToTry)   

            if (w_Player == common.aiPlayerIndex):
                totalWonPoints += f_CardValue + s_CardValue

            wonPoints, _ = __playFinalSixSteps(
                w_Player, 
                f_NewCardsInHand if w_Player == f_Player else s_NewCardsInHand, 
                s_NewCardsInHand if w_Player == f_Player else f_NewCardsInHand, 
                -1,
                b_Card)
            totalWonPoints += wonPoints

            if (totalWonPoints > maxWonPoints):
                maxWonPoints = totalWonPoints
                playedCard = f_CardToTry if w_Player == f_Player else s_CardToTry

            totalWonPoints = 0

    return maxWonPoints, playedCard

def __removePlayedCard(cardsInHand, card):
    newCardsInHand = np.array(cardsInHand)
    newCardsInHand[newCardsInHand == card] = -1

    return list(filter(lambda x: x != -1, newCardsInHand))        