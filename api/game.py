import numpy as np
import math
import random
import ai

aiPlayerIndex = 0
humanPlayerIndex = 1

def deal():
    cards = np.linspace(0, 39, 40)
    random.shuffle(cards)

    firstPlayerIndex = random.randint(0, 1),

    return {
        'cardsInHand': {
            0:  list(map(int, cards[0:3])),
            1:  list(map(int, cards[3:6]))
        },
        'playedCards': {
            0: -1,
            1: -1
        },
        'points': {
            0: 0,
            1: 0
        },
        'briscola': int(cards[6]),
        'stack': list(map(int, (np.append(cards[7:],[cards[6]])))),
        'firstPlayerIndex': firstPlayerIndex,
        'status': 'yourMove' if firstPlayerIndex == humanPlayerIndex else 'aiMove'
    }

def play(body, model):
    if bothPlayed(body):
        body = endTurn(body)
    elif useNN(body):
        body = nnPredict(body, model)
    else:
        body = playFinalSix(body)

    return body

def bothPlayed(body):
    return body['playedCards'][str(aiPlayerIndex)] != -1 and body['playedCards'][str(humanPlayerIndex)] != -1

def endTurn(body):
    f_Player = body['firstPlayerIndex']
    s_Player = 1 - f_Player
    f_Card = body['playedCards'][str(f_Player)]
    s_Card = body['playedCards'][str(s_Player)]
    b_Card = body['briscola']
    w_player = determineCollectingPlayer(f_Player, f_Card, s_Card, b_Card)        
    body = collect(body, w_player)   

    return body

def collect(body, w_Player):
    l_Player = 1 - w_Player
    w_Card, _, w_CardValue = getCardInfo(body['playedCards'][str(w_Player)])
    l_Card, _, l_CardValue = getCardInfo(body['playedCards'][str(l_Player)])

    body = addPoints(body, w_Player, w_CardValue, l_CardValue)
    body = dealCards(body, w_Player, w_Card, l_Player, l_Card)
    body = setNextPlayStatus(body, w_Player)

    return body

def addPoints(body, w_Player, w_CardValue, l_CardValue):
    body['points'][str(w_Player)] += w_CardValue + l_CardValue

    return body    

def dealCards(body, w_Player, w_Card, l_Player, l_Card):
    w_NextCard = -1
    l_NextCard = -1
    stack = body['stack']
    if len(stack) > 0:
        w_NextCard = stack[0]
        l_NextCard = stack[1]
        stack = stack[2:]
        body['stack'] = stack

    body = dealPlayerCard(body, w_Player, w_Card, w_NextCard)
    body = dealPlayerCard(body, l_Player, l_Card, l_NextCard)

    return body

def setNextPlayStatus(body, player):
    if (len(body['cardsInHand'][str(aiPlayerIndex)]) > 0 or len(body['cardsInHand'][str(humanPlayerIndex)]) > 0):
        body['firstPlayerIndex'] = player
        body['status'] = 'yourMove' if player == humanPlayerIndex else 'aiMove'
    else:
        body['status'] = 'gameOver'

    return body

def dealPlayerCard(body, player, card, nextCard):
    newCardsInHand = np.array(body['cardsInHand'][str(player)])
    newCardsInHand[newCardsInHand == card] = nextCard
    
    body['cardsInHand'][str(player)] = list(map(int, newCardsInHand))
    body['playedCards'][str(player)] = -1

    return body

def useNN(body):
    return len(body['stack']) > 0    

def nnPredict(body, model):
    X = ai.getFeatures(body)
    Y_pred = np.argmax(model.predict(X), axis=1)
    playedCard = body['cardsInHand'][str(aiPlayerIndex)][int(Y_pred[0])]
    body['playedCards'][str(aiPlayerIndex)] = playedCard
    body['status'] = 'yourMove' if body['firstPlayerIndex'] == aiPlayerIndex else 'endOfTurn'

    return body

def playFinalSix(body):
    f_Player = body['firstPlayerIndex']
    f_Card = body['playedCards'][str(f_Player)]
    b_Card = body['briscola']
    ai_CardsInHand = list(filter(lambda x: x != -1, body['cardsInHand'][str(aiPlayerIndex)]))
    human_CardsInHand = list(filter(lambda x: x != -1, body['cardsInHand'][str(humanPlayerIndex)]))

    playedCard = determineFinalSixPlay(ai_CardsInHand, human_CardsInHand, f_Player, f_Card, b_Card)
    body['playedCards'][str(aiPlayerIndex)] = playedCard
    body['status'] = 'yourMove' if body['firstPlayerIndex'] == aiPlayerIndex else 'endOfTurn'

    return body
    
def determineFinalSixPlay(ai_CardsInHand, human_CardsInHand, f_Player, f_Card, b_Card):
    playedCard = None

    if (f_Player == aiPlayerIndex):
        _, playedCard = playFinalSixSteps(f_Player, ai_CardsInHand, human_CardsInHand, -1, b_Card)
    else:
        _, playedCard = playFinalSixSteps(f_Player, human_CardsInHand, ai_CardsInHand, f_Card, b_Card)

    return playedCard

def playFinalSixSteps(f_Player, f_CardsInHand, s_CardsInHand, f_Card, b_Card):
    playedCard = None
    totalWonPoints = 0
    maxWonPoints = 0

    f_CardsInHandToTry = f_CardsInHand
    if (f_Card != -1):
        f_CardsInHandToTry = [f_Card]

    for s_CardToTry in s_CardsInHand:
        _, _, s_CardValue = getCardInfo(s_CardToTry)
        for f_CardToTry in f_CardsInHandToTry:
            _, _, f_CardValue = getCardInfo(f_CardToTry)

            w_Player = determineCollectingPlayer(f_Player, f_CardToTry, s_CardToTry, b_Card)     

            f_NewCardsInHand = removePlayedCard(f_CardsInHand, f_CardToTry)
            s_NewCardsInHand = removePlayedCard(s_CardsInHand, s_CardToTry)   

            if (w_Player == aiPlayerIndex):
                totalWonPoints += f_CardValue + s_CardValue

            wonPoints, _ = playFinalSixSteps(
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

def removePlayedCard(cardsInHand, card):
    newCardsInHand = np.array(cardsInHand)
    newCardsInHand[newCardsInHand == card] = -1

    return list(filter(lambda x: x != -1, newCardsInHand))

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