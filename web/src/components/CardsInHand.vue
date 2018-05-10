<template>
  <div v-if="cardsInHand" class="cards-in-hand">
    <div class="card" v-bind:class="['card__' + cardsInHand[0]]" v-on:click="play(cardsInHand[0])"></div>
    <div class="card" v-bind:class="['card__' + cardsInHand[1]]" v-on:click="play(cardsInHand[1])"></div>
    <div class="card" v-bind:class="['card__' + cardsInHand[2]]" v-on:click="play(cardsInHand[2])"></div>
  </div>
</template>

<script>
export default {
  name: 'CardsInHand',
  props: {
    cardsInHand: {
      type: Array
    },
    status: {
      type: String
    }
  },
  methods: {
    play (card) {
      if (this.status !== 'yourMove' || card === -1) {
        return;
      }

      this.$emit('play', card)
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
</style>
