<template>
  <div v-if="$store.state.gameState" class="cards-in-hand">
    <card v-bind:cardsInHand="this.$store.state.gameState.cardsInHand[handIdx]" v-bind:cardIdx="0" v-on:play="play($event)"></card>
    <card v-bind:cardsInHand="this.$store.state.gameState.cardsInHand[handIdx]" v-bind:cardIdx="1" v-on:play="play($event)"></card>
    <card v-bind:cardsInHand="this.$store.state.gameState.cardsInHand[handIdx]" v-bind:cardIdx="2" v-on:play="play($event)"></card>
  </div>
</template>

<script>
import Card from "./Card.vue";

export default {
  name: 'CardsInHand',
  props: {
    handIdx: {
      type: Number,
    }
  },
  methods: {
    play (cardIdx) {
      const card = this.$store.state.gameState.cardsInHand[this.handIdx][cardIdx];
      const playedCard = this.$store.state.gameState.playedCards[this.handIdx];
      if (this.$store.state.gameState.status !== 'yourMove' || card === -1 || playedCard !== -1) {
        return;
      }

      if (cardIdx != null) {
        this.$store.commit('setPlayedCard', { handIdx: this.handIdx, cardIdx });
      }

      this._play();
    },
    _play() {
      this.$apiClient
        .play(this.$store.state.gameState)
        .then(result => {
          this.$store.commit('set', result);
          if (result.status === "aiMove") {
            setTimeout(() => {
              this._play();
            }, 500);
          } else if (result.status === "endOfTurn") {
            setTimeout(() => {
              this._play();
            }, 1500);
          }
        })
        .catch(error => {
          console.log(error);
        });
    }
  },
  components: {
    Card
  }
}
</script>

<style scoped>
div.cards-in-hand div:first-child {
  margin-right: 10px;
}
div.cards-in-hand div:last-child {
  margin-left: 10px;
}
/* div.card {
  cursor: pointer;
} */
</style>
