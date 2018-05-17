import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

export default function makeStore() {
    return new Vuex.Store({
        state: {
            gameState: null
        },
        mutations: {
            set(state, newState) {
                state.gameState = newState;
            },
            setPlayedCard(state, cardIdx) {
                const card = state.gameState.cardsInHand[1][cardIdx];
                state.gameState.cardsInHand[1][cardIdx] = -1;
                state.gameState.playedCards[1] = card;
            }
        }
    })
}

