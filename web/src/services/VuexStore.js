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
            setPlayedCard(state, { handIdx, cardIdx }) {
                const card = state.gameState.cardsInHand[handIdx][cardIdx];
                Vue.set(state.gameState.cardsInHand[handIdx], cardIdx, -1);
                Vue.set(state.gameState.playedCards, handIdx, card);
            }
        }
    })
}

