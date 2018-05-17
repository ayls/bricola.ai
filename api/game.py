import numpy as np
import math
import random
import ai
import finalSix
import common

class Game:

    def __init__(self):
        self.ai = ai.AI()

    def deal(self):
        cards = np.linspace(0, 39, 40)
        random.shuffle(cards)

        firstPlayerIndex = random.randint(0, 1)

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
            'briscola': cards[6],
            'stack': list(map(int, (np.append(cards[7:],[cards[6]])))),
            'firstPlayerIndex': firstPlayerIndex,
            'status': 'yourMove' if firstPlayerIndex == common.humanPlayerIndex else 'aiMove'
        }

    def play(self, body):
        if self.__isEndOfTurn(body):
            body = self.__endTurn(body)
        elif self.__haveBothPlayed(body):
            body = self.__setEndOfTurn(body)
        elif self.__useNN(body):
            body = self.__nnPredict(body)
        else:
            body = self.__playFinalSix(body)

        return body

    def __isEndOfTurn(self, body):
        return body['status'] == 'endOfTurn'

    def __setEndOfTurn(self, body):
        body['status'] = 'endOfTurn'
        return body      

    def __haveBothPlayed(self, body):
        return body['playedCards'][str(common.aiPlayerIndex)] != -1 and body['playedCards'][str(common.humanPlayerIndex)] != -1

    def __endTurn(self, body):
        f_Player = body['firstPlayerIndex']
        s_Player = 1 - f_Player
        f_Card = body['playedCards'][str(f_Player)]
        s_Card = body['playedCards'][str(s_Player)]
        b_Card = body['briscola']
        w_Player = common.determineCollectingPlayer(f_Player, f_Card, s_Card, b_Card)        
        l_Player = 1 - w_Player
        w_Card, _, w_CardValue = common.getCardInfo(body['playedCards'][str(w_Player)])
        l_Card, _, l_CardValue = common.getCardInfo(body['playedCards'][str(l_Player)])

        body = self.__addPoints(body, w_Player, w_CardValue, l_CardValue)
        body = self.__dealCards(body, w_Player, w_Card, l_Player, l_Card)
        body = self.__setNextPlayStatus(body, w_Player)

        return body

    def __addPoints(self, body, w_Player, w_CardValue, l_CardValue):
        body['points'][str(w_Player)] += w_CardValue + l_CardValue

        return body    

    def __dealCards(self, body, w_Player, w_Card, l_Player, l_Card):
        w_NextCard = -1
        l_NextCard = -1
        stack = body['stack']
        if len(stack) > 0:
            w_NextCard = stack[0]
            l_NextCard = stack[1]
            stack = stack[2:]
            body['stack'] = stack

        body = self.__dealPlayerCard(body, w_Player, w_Card, w_NextCard)
        body = self.__dealPlayerCard(body, l_Player, l_Card, l_NextCard)

        return body

    def __setNextPlayStatus(self, body, player):
        ai_CardsInHand = list(filter(lambda x: x != -1, body['cardsInHand'][str(common.aiPlayerIndex)]))
        ai_points = body['points'][str(common.aiPlayerIndex)]
        human_CardsInHand = list(filter(lambda x: x != -1, body['cardsInHand'][str(common.humanPlayerIndex)]))
        human_points = body['points'][str(common.humanPlayerIndex)]
        if (len(ai_CardsInHand) > 0 or len(human_CardsInHand) > 0):
            body['firstPlayerIndex'] = player
            body['status'] = 'yourMove' if player == common.humanPlayerIndex else 'aiMove'        
        elif (ai_points > human_points):
            body['status'] = 'aiWon'
        elif (human_points > ai_points):
            body['status'] = 'youWon'
        else:
            body['status'] = 'tie'

        return body

    def __dealPlayerCard(self, body, player, card, nextCard):
        newCardsInHand = np.array(body['cardsInHand'][str(player)])
        newCardsInHand[newCardsInHand == -1] = nextCard
        
        body['cardsInHand'][str(player)] = list(map(int, newCardsInHand))
        body['playedCards'][str(player)] = -1

        return body

    def __useNN(self, body):
        return len(body['stack']) > 0    

    def __nnPredict(self, body):
        card1 = body['cardsInHand'][str(common.aiPlayerIndex)][0]
        card2 = body['cardsInHand'][str(common.aiPlayerIndex)][1]
        card3 = body['cardsInHand'][str(common.aiPlayerIndex)][2]
        oppCard = body['playedCards'][str(common.humanPlayerIndex)]
        briscola = body['briscola']
        playedCardIdx = self.ai.nnPredict(card1, card2, card3, oppCard, briscola)
        playedCard = body['cardsInHand'][str(common.aiPlayerIndex)][playedCardIdx]
        body['cardsInHand'][str(common.aiPlayerIndex)][playedCardIdx] = -1
        body['playedCards'][str(common.aiPlayerIndex)] = playedCard
        body['status'] = 'yourMove' if body['firstPlayerIndex'] == common.aiPlayerIndex else 'endOfTurn'

        return body

    def __playFinalSix(self, body):
        f_Player = body['firstPlayerIndex']
        f_Card = body['playedCards'][str(f_Player)]
        b_Card = body['briscola']
        cardsInHand = body['cardsInHand']

        playedCardIdx = finalSix.determineFinalSixPlay(cardsInHand, f_Player, f_Card, b_Card)
        playedCard = body['cardsInHand'][str(common.aiPlayerIndex)][playedCardIdx]
        body['cardsInHand'][str(common.aiPlayerIndex)][playedCardIdx] = -1
        body['playedCards'][str(common.aiPlayerIndex)] = playedCard
        body['status'] = 'yourMove' if body['firstPlayerIndex'] == common.aiPlayerIndex else 'endOfTurn'

        return body