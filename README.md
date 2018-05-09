# Briscola.ai

A Brisola game using NN to play all but the last 3 moves where an algorithm is used.

It is a rewrite of the game I wrote ages ago that used rules/tree trained with C5. 

The original game is available here: http://ayls.org/briscola/index.html

## Prerequisites

- Python 3.6
- pipenv installed
- Node 8 or greater
- vue CLI installed globally

## Running API

Run:
```
pipenv shell
pipenv install
```  

Change to to api folder and execute:
```
python app.py
```

## Running Web

Change to to web folder and execute:
```
npm install
```

Then change to src folder and execute:
```
vue serve
```