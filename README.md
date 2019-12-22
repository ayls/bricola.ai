# Briscola.ai

A Brisola game using NN to play all but the last 3 moves where an algorithm is used.

It is a rewrite of the game I wrote ages ago that used rules/tree trained with C5. 

The original game is available here: http://ayls.org/briscola/index.html

## Prerequisites

- Python 3.6
- Node 8 or greater

## Running API

Change to api folder and build the docker image:
```
docker build -t briscola-ai-api .
```

Then launch the container:
```
docker run -dit -p 5000:5000 briscola-ai-api
```

## Running Web

Change to to web folder and execute:
```
npm install
```

Then execute:
```
npm run serve
```