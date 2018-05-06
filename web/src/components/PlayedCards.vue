<template>
  <div v-if="playedCards" class="played-cards">
    <div class="card" v-bind:class="['card__' + playedCards[0]]"></div>
    <div class="card" v-bind:class="['card__' + playedCards[1]]"></div>
  </div>
</template>

<script>
export default {
  name: "PlayedCards",
  props: [
    'playedCards'
  ]
}
</script>

<style scoped>
div.played-cards div:first-child {
  margin-right: 10px;
}
</style>