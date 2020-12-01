from . import state
import numpy as np

def determineFinalSixPlay(cardsInHand, f_Player, f_Card, b_Card):
    ai_CardsInHand = list(filter(lambda x: x != -1, cardsInHand[str(state.aiPlayerIndex)]))
    human_CardsInHand = list(filter(lambda x: x != -1, cardsInHand[str(state.humanPlayerIndex)]))

    playedCard = None

    if (len(ai_CardsInHand) == 1):
        playedCard = ai_CardsInHand[0]
    elif (f_Player == state.aiPlayerIndex):
        _, playedCard = __playFinalSixSteps(f_Player, ai_CardsInHand, human_CardsInHand, -1, b_Card)
    else:
        _, playedCard = __playFinalSixSteps(f_Player, human_CardsInHand, ai_CardsInHand, f_Card, b_Card)

    playedCardIdx = cardsInHand[str(state.aiPlayerIndex)].index(playedCard)

    return playedCardIdx

def __playFinalSixSteps(f_Player, f_CardsInHand, s_CardsInHand, f_Card, b_Card):
    playedCard = None
    totalWonPoints = 0
    maxWonPoints = 0

    f_CardsInHandToTry = f_CardsInHand
    if (f_Card != -1):
        f_CardsInHandToTry = [f_Card]

    for s_CardToTry in s_CardsInHand:
        _, _, s_CardValue = state.getCardInfo(s_CardToTry)
        for f_CardToTry in f_CardsInHandToTry:
            _, _, f_CardValue = state.getCardInfo(f_CardToTry)

            w_Player = state.determineCollectingPlayer(f_Player, f_CardToTry, s_CardToTry, b_Card)     

            f_NewCardsInHand = __removePlayedCard(f_CardsInHand, f_CardToTry)
            s_NewCardsInHand = __removePlayedCard(s_CardsInHand, s_CardToTry)   

            if (w_Player == state.aiPlayerIndex):
                totalWonPoints += f_CardValue + s_CardValue

            wonPoints, _ = __playFinalSixSteps(
                w_Player, 
                f_NewCardsInHand if w_Player == f_Player else s_NewCardsInHand, 
                s_NewCardsInHand if w_Player == f_Player else f_NewCardsInHand, 
                -1,
                b_Card)
            totalWonPoints += wonPoints

            if (totalWonPoints >= maxWonPoints):
                maxWonPoints = totalWonPoints
                playedCard = f_CardToTry if f_Player == state.aiPlayerIndex else s_CardToTry

            totalWonPoints = 0

    return maxWonPoints, playedCard

def __removePlayedCard(cardsInHand, card):
    newCardsInHand = np.array(cardsInHand)
    newCardsInHand[newCardsInHand == card] = -1

    return list(filter(lambda x: x != -1, newCardsInHand))        