import csv
import math

def toDeckIndex(card, hasK, hasB, hasS, hasD):
    if card == '-1':
        return int(card)

    cardType = card[:1]
    if (cardType == 'a'):
        if not hasK:
            cardType = 'k'
        if not hasB:
            cardType = 'b'            
        if not hasS:
            cardType = 's'
        if not hasD:
            cardType = 'd'      

    deckIndex = None
    cardIndex = int(card[1:])
    if cardType == 'k':
         deckIndex = 0 + cardIndex - 1
    if cardType == 'b':
         deckIndex = 10 + cardIndex - 1    
    if cardType == 's':
         deckIndex = 20 + cardIndex - 1
    if cardType == 'd':
         deckIndex = 30 + cardIndex - 1        

    return deckIndex

def addCalculatedFeatures(row, idx):
    card = int(row[idx])
    if (card == -1):
        row = row[:idx + 1] + ["-1", "-1", "-1", "-1", "-1"] + row[idx + 1:]
    else:
        type = str(math.floor(card / 10))
        value = str(11 if (card % 10 == 0) else (10 if (card % 10 == 2) else (2 if (card % 10 == 7) else (3 if (card % 10 == 8) else (4 if (card % 10 == 9) else 0)))))
        hasBigValue = str(1 if (value == "11" or value == "10") else 0)
        hasSmallValue = str(1 if (value == "4" or value == "3" or value == "2") else 0)
        hasValue = str(1 if (hasBigValue == "1" or hasSmallValue == "1") else 0) 
        row = row[:idx + 1] + [type] + [value] + [hasValue] + [hasBigValue] + [hasSmallValue] + row[idx + 1:]

    return row

# add card value, type, hasBigValue, hasSmallValue, hasValue for three cards in hand and the briscola
def processRows(rows, hasK, hasB, hasS, hasD):
    with open("./data/preprocessed/briscola_2.data", "a") as output:
        for row in rows:
            row[1] = str(toDeckIndex(row[1], hasK, hasB, hasS, hasD))
            row[2] = str(toDeckIndex(row[2], hasK, hasB, hasS, hasD))
            row[3] = str(toDeckIndex(row[3], hasK, hasB, hasS, hasD))
            row[4] = str(toDeckIndex(row[4], hasK, hasB, hasS, hasD))
            row[8] = str(toDeckIndex(row[8], hasK, hasB, hasS, hasD))

            newRow = addCalculatedFeatures(row, 1)
            newRow = addCalculatedFeatures(newRow, 7)
            newRow = addCalculatedFeatures(newRow, 13)
            newRow = addCalculatedFeatures(newRow, 19)
            newRow = addCalculatedFeatures(newRow, 28)

            output.write(",".join(newRow) + "\n")

sliceHasK = False
sliceHasB = False
sliceHasS = False
sliceHasD = False
currentBriscola = None
sliceRows = []

for row in csv.reader(open("./data/preprocessed/briscola_0.data")):

    if not currentBriscola is None and row[5] == '40':
        processRows(sliceRows, sliceHasK, sliceHasB, sliceHasS, sliceHasD)
        sliceHasK = False
        sliceHasB = False
        sliceHasS = False
        sliceHasD = False        
        currentBriscola = None
        sliceRows = []

    currentBriscola = row[8]
    sliceRows.append(row)
    card1 = row[1][:1]
    card2 = row[2][:1]
    card3 = row[3][:1]

    sliceHasK = sliceHasK or card1 == 'k' or card2 == 'k' or card3 == 'k'
    sliceHasB = sliceHasB or card1 == 'b' or card2 == 'b' or card3 == 'b'
    sliceHasS = sliceHasS or card1 == 's' or card2 == 's' or card3 == 's'
    sliceHasD = sliceHasD or card1 == 'd' or card2 == 'd' or card3 == 'd'

# process final game
processRows(sliceRows, sliceHasK, sliceHasB, sliceHasS, sliceHasD)