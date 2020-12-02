# Briscola.ai

A Brisola game using NN to play all but the last 3 moves where an algorithm is used.

It is a rewrite of the game I wrote ages ago that used rules/tree trained with C5. 

The original game is available here: http://ayls.org/briscola/index.html

## Prerequisites

- Python 3.6
- Node 8 or greater

## Running Flask version of the API

Change to api/flask folder and build the docker image:
```
docker build -t briscola-ai-api .
```

Then launch the container:
```
docker run -dit -p 5000:5000 briscola-ai-api
```

## Running Azure functions version of the API

Ensure you have Azure Function tools installed (https://docs.microsoft.com/en-us/azure/azure-functions/functions-run-local?tabs=windows%2Ccsharp%2Cbash#v2)

Change to api/azure_functions and run:
```
func host start --cors *
```
## Running Web

To run against Flask version of API simply change to to web folder and execute:
```
npm install
```

Then execute:
```
npm run serve
```

To target Azure functions API go to index.html and modify the API's base url.