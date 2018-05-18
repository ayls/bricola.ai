<template>
    <div class="card" v-bind:class="['card__' + cardsInHand[cardIdx]]" v-on:click="play()"></div>
</template>

<script>
export default {
  name: 'Card',
  props: [
    'cardsInHand',
    'cardIdx'
  ],
  methods: {
    play () {
      this.$emit('play', this.cardIdx);
    }
  }  
}
</script>

<style scoped>
div.card {
  cursor: pointer;
}
</style>