<template>
  <div v-if="$store.state.gameState" class="cards-in-hand">
    <div class="card" v-bind:class="['card__' + $store.state.gameState.cardsInHand[handIdx][0]]" v-on:click="play(0)"></div>
    <div class="card" v-bind:class="['card__' + $store.state.gameState.cardsInHand[handIdx][1]]" v-on:click="play(1)"></div>
    <div class="card" v-bind:class="['card__' + $store.state.gameState.cardsInHand[handIdx][2]]" v-on:click="play(2)"></div>
  </div>
</template>

<script>
export default {
  name: 'CardsInHand',
  props: {
    handIdx: {
      type: Number
    }
  },
  methods: {
    play (cardIdx) {
      const card = this.$store.state.gameState.cardsInHand[cardIdx];
      const playedCard = this.$store.state.gameState.playedCards[this.handIdx];
      if (this.$store.state.gameState.status !== 'yourMove' || card === -1 || playedCard !== -1) {
        return;
      }

      this.$emit('play', cardIdx)
    }
  }  
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
div.cards-in-hand div:first-child {
  margin-right: 10px;
}
div.cards-in-hand div:last-child {
  margin-left: 10px;
}
div.card {
  cursor: pointer;
}
</style>
