<template>
  <div id="app">
    <div class="status">
      <game-status 
        v-bind:points="state == null ? null : state.points"
        v-bind:stack="state == null ? null : state.stack"
        v-bind:status="state == null ? null : state.status"
        v-on:deal="deal()"/>
    </div>
    <div v-if="state" class="board">
      <div class="left">
        <stack v-bind:stack="state.stack" v-bind:briscola="state.briscola" />
      </div>
      <div class="right">
        <div>
          <played-cards v-bind:played-cards="state.playedCards" />
        </div>
      </div>
      <div class="bottom">
        <div class="bottom-part" style="display: none">
          <cards-in-hand v-bind:cards-in-hand="state.cardsInHand[0]" />
        </div>
        <div class="bottom-part">
          <cards-in-hand v-bind:cards-in-hand="state.cardsInHand[1]" v-bind:status="state.status" v-on:play="play($event)"/>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import GameStatus from './components/GameStatus.vue';
import Stack from './components/Stack.vue';
import PlayedCards from './components/PlayedCards.vue';
import CardsInHand from './components/CardsInHand.vue';

export default {
  name: 'App',
  data () {
    return {
      state: null
    }
  },
  props: [
    'baseUrl'
  ],
  methods: {
    deal () {
      axios.post(this.baseUrl + '/deal')
      .then(result => {
        this.state = result.data;
        if (result.data.status === 'aiMove') {
          this.play();
        }
      }, error => {
        console.error(error);
      });
    },
    play (cardIdx) {
      if (cardIdx != null) {
        const card = this.state.cardsInHand[1][cardIdx];
        this.state.cardsInHand[1][cardIdx] = -1;
        this.state.playedCards[1] = card;
      }

      axios.post(this.baseUrl + '/play', 
        this.state, 
        {
          headers: {
            'Content-Type': 'application/json'
          }
        })
      .then(result => {
        this.state = result.data;
        if (result.data.status === 'aiMove') {
          setTimeout(() => { this.play(); }, 500);
        }
        if (result.data.status === 'endOfTurn') {
          setTimeout(() => { this.play(); }, 2500);
        }
      }, error => {
        console.error(error);
      });
    }
  },
  components: {
    GameStatus,
    Stack,
    PlayedCards,
    CardsInHand
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  padding: 0 20px;
}
div.status {
  padding: 20px 0;
  border-bottom: 1px solid white;
}
div.board {
  padding-top: 20px;
  display: grid;
  grid-template-columns: repeat(2, 1fr);
}
div.board div.left {
  grid-column: 1;
  grid-row: 1;
}
div.board div.right {
  grid-column: 2;
  grid-row: 1;
}
div.board div.bottom {
  padding-top: 90px;
  grid-column: 1 / span 2;
  grid-row: 2;
}
div.board div.bottom .bottom-part:last-child {
  margin-top: 10px;
}
</style>
